// protocol_spacecan.h
#ifndef PROTOCOL_SPACECAN_H
#define PROTOCOL_SPACECAN_H

#include "subsystems/housekeeping/housekeeping.h"
#include "drivers/can/peripheral_mcp2515.h"
#include <stdint.h>
#include <stddef.h>

// SpaceCAN beacon type
#define SPACECAN_TYPE_BEACON 0x01

#define SPACECAN_TYPE_COMMAND  0x10

#define SPACECAN_CMD_RUN_TEST      0x01
#define SPACECAN_CMD_STOP_TX       0x02
#define SPACECAN_CMD_SEND_BEACON   0x03
#define SPACECAN_CMD_SEND_IMAGE    0x04
#define SPACECAN_CMD_SET_RADIO     0x05


// Output: fills `out` with bytes and returns how many were written
size_t protocol_spacecan_format_beacon(uint8_t *out, size_t max_len, const hk_telemetry_t *hk);
void spacecan_parse_frame(const can_frame_t* frame);

void send_spacecan_burst(void);

#endif
