#ifndef QUATTRO_VOICE_CONFIG_H
#define QUATTRO_VOICE_CONFIG_H

#include <stdint.h>

namespace voice_config {
static constexpr bool enabled = true;
static constexpr int32_t sda_pin = 21;
static constexpr int32_t scl_pin = 22;
static constexpr uint32_t clock_hz = 100000;
static constexpr uint16_t timeout_ms = 25;
static constexpr uint32_t startup_ms = 1500;
static constexpr uint32_t poll_ms = 50;
static constexpr uint32_t retry_ms = 1000;
static constexpr uint8_t address = 0x2B;
static constexpr uint8_t result_register = 0x64;
}

#endif
