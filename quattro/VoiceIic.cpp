#include "VoiceIic.h"
#include "VoiceConfig.h"
#include <Arduino.h>
#include <Wire.h>

namespace {
struct voice_state_t {
  bool bus_ready;
  bool online;
  uint8_t previous_id;
  uint32_t started_ms;
  uint32_t last_poll_ms;
  uint32_t last_probe_ms;
};

static voice_state_t state = {};

static bool read_result(uint8_t &id) {
  Wire.beginTransmission(voice_config::address);
  if (Wire.write(voice_config::result_register) != 1U) {
    Wire.endTransmission(true);
    return false;
  }
  if (Wire.endTransmission(true) != 0U) {
    return false;
  }
  const uint8_t received = Wire.requestFrom(voice_config::address, uint8_t(1));
  if (received != 1U || Wire.available() < 1) {
    return false;
  }
  const int32_t value = Wire.read();
  if (value < 0) {
    return false;
  }
  id = static_cast<uint8_t>(value);
  return true;
}
}

void voice_iic_begin() {
  state = {};
  state.started_ms = millis();
  if (!voice_config::enabled) {
    return;
  }
  state.bus_ready = Wire.begin(voice_config::sda_pin, voice_config::scl_pin,
                               voice_config::clock_hz);
  if (!state.bus_ready) {
    Serial.println("[Voice] IIC initialization failed; WiFi control remains available.");
    return;
  }
  Wire.setTimeOut(voice_config::timeout_ms);
  Serial.println("[Voice] IIC enabled: SDA=21 SCL=22 address=0x2B; waiting for module.");
}

bool voice_iic_poll(uint8_t &command_id) {
  command_id = 0;
  if (!state.bus_ready) {
    return false;
  }
  const uint32_t now_ms = millis();
  if (uint32_t(now_ms - state.started_ms) < voice_config::startup_ms) {
    return false;
  }
  const uint32_t interval_ms = state.online ? voice_config::poll_ms : voice_config::retry_ms;
  const uint32_t previous_ms = state.online ? state.last_poll_ms : state.last_probe_ms;
  if (uint32_t(now_ms - previous_ms) < interval_ms) {
    return false;
  }
  state.last_poll_ms = now_ms;
  state.last_probe_ms = now_ms;

  uint8_t id = 0;
  if (!read_result(id)) {
    if (state.online) {
      Serial.println("[Voice] Module offline; retrying every second.");
    }
    state.online = false;
    return false;
  }
  if (!state.online) {
    state.online = true;
    state.previous_id = id; // Discard a stale result at startup/reconnection.
    Serial.println("[Voice] Module connected. Wake it, then say a command.");
    return false;
  }
  const uint8_t previous_id = state.previous_id;
  state.previous_id = id;
  if (id == 0U || id == previous_id) {
    return false;
  }
  command_id = id;
  return true;
}
