#ifndef QUATTRO_VOICE_COMMANDS_H
#define QUATTRO_VOICE_COMMANDS_H

#include <stdint.h>

// Factory V3 command IDs, not the spreadsheet's semantic-label/row numbers.
// Only supported robot functions are dispatched. Other IDs are diagnostic only.
static inline const char *voice_command_for_id(uint8_t id) {
  switch (id) {
    case 0x01: // Robot stop
    case 0x02: // Stop
    case 0x03: // Robot sleep: retain standing posture, keep servo power enabled.
    case 0x21: // Back to original: posture only, not navigation.
      return "home";
    case 0x04: return "forward";
    case 0x05: return "backward";
    case 0x06: return "turn_L";
    case 0x07: return "turn_R";
    default: return nullptr;
  }
}

#endif
