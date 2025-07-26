// spacecan_assembler.h
#pragma once
#include "drivers/can/peripheral_mcp2515.h"

void spacecan_assembler_handle(const can_frame_t* frame);