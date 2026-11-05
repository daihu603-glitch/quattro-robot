#ifndef QUATTRO_VOICE_IIC_H
#define QUATTRO_VOICE_IIC_H

#include <stdint.h>

// BSP transport: owns Wire; call only from Arduino's loop task.
// A new nonzero ID is delivered once, and zero rearms an identical command.
void voice_iic_begin();
bool voice_iic_poll(uint8_t &command_id);

#endif
