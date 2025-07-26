#include "protocols/protocol_spacecan.h"
#include <string.h>
#include <stdio.h>

static uint8_t buffer[16];
static uint8_t bytes_in_buffer = 0;
static bool waiting_for_frame2 = false;

void spacecan_assembler_handle(const can_frame_t* frame) {
    if (frame->id == 0x200 && frame->data[0] == 0x10) {
        // This is FRAME 1
        memcpy(buffer, frame->data, 8);
        bytes_in_buffer = 8;
        waiting_for_frame2 = true;
        printf("[SPACECAN_ASSEMBLER] Stored FRAME 1.\n");
        return;
    }

    if (frame->id == 0x201 && frame->data[0] == 0x11 && waiting_for_frame2) {
        // This is FRAME 2
        buffer[8] = frame->data[1]; // SF
        buffer[9] = frame->data[2]; // CR
        buffer[10] = frame->data[3]; // checksum

        can_frame_t assembled;
        assembled.id = 0x200;
        assembled.dlc = 11;
        memcpy(assembled.data, buffer, 11);

        printf("[SPACECAN_ASSEMBLER] Assembled complete SPACECAN command.\n");

        spacecan_parse_frame(&assembled);

        waiting_for_frame2 = false;
    }
}