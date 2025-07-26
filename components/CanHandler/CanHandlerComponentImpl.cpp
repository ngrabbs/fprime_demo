#include "CanHandlerComponentImpl.hpp"
#include "drivers/can/peripheral_mcp2515.h"
#include "protocols/protocol_spacecan.h"

#include <cstdio>
#include <cstring>

using namespace SBandDemo;

CanHandlerComponentImpl::CanHandlerComponentImpl(const char* compName)
    : CanHandlerComponentBase(compName)
    , waitingForSecondFrame(false)
{
    std::memset(&frameBuffer, 0, sizeof(frameBuffer));
}

void CanHandlerComponentImpl::init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance) {
    CanHandlerComponentBase::init(queueDepth, instance);
    // Initialise the CAN controller
    if (!can_init()) {
        printf("[CanHandler] MCP2515 initialisation failed\n");
    }
}

void CanHandlerComponentImpl::schedIn_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    (void) portNum;
    (void) context;
    can_frame_t frame;
    // Poll the CAN controller for a frame.  This mirrors the polling loop in
    // `task_can.c`【805012935155573†L26-L35】.
    if (can_receive(&frame)) {
        // Two‑frame command assembly.  SpaceCAN commands are sent in two CAN
        // frames with IDs 0x200 and 0x201.  The first frame carries the
        // command header and most parameters; the second carries the SF, CR
        // and checksum【740037136390698†L21-L29】.
        if (!this->waitingForSecondFrame) {
            if (frame.id == 0x200 && frame.data[0] == SPACECAN_TYPE_COMMAND) {
                this->frameBuffer = frame;
                this->waitingForSecondFrame = true;
            }
        } else {
            if (frame.id == 0x201 && frame.data[0] == 0x11) {
                // Assemble 11‑byte command
                uint8_t cmdBuf[11] = {0};
                std::memcpy(cmdBuf, this->frameBuffer.data, 8);
                // According to the original assembler: data[1] is SF, [2] is CR, [3] is checksum
                cmdBuf[8]  = frame.data[1];
                cmdBuf[9]  = frame.data[2];
                cmdBuf[10] = frame.data[3];

                // Parse fields
                uint8_t type  = cmdBuf[0];
                uint8_t cmd   = cmdBuf[1];
                uint8_t testId = cmdBuf[2];
                uint16_t freqX10 = (static_cast<uint16_t>(cmdBuf[3]) << 8) | cmdBuf[4];
                uint8_t power = cmdBuf[5];
                uint8_t modulation = cmdBuf[6];
                uint8_t bandwidth = cmdBuf[7];
                uint8_t sf = cmdBuf[8];
                uint8_t cr = cmdBuf[9];
                uint8_t checksum = cmdBuf[10];

                // Compute checksum of first 10 bytes
                uint8_t calc = 0;
                for (int i = 0; i < 10; ++i) {
                    calc += cmdBuf[i];
                }
                if (calc == checksum) {
                    // Convert fields into a RadioMessage and send to RadioDriver
                    RadioMessage cfgMsg;
                    cfgMsg.type = 0; // configure
                    cfgMsg.frequencyHz    = static_cast<uint32_t>(freqX10) * 100000UL;
                    cfgMsg.txPower        = power;
                    cfgMsg.modulation     = modulation;
                    cfgMsg.bandwidthHz    = bandwidth;
                    cfgMsg.spreadingFactor= sf;
                    cfgMsg.codingRate     = cr;
                    cfgMsg.payloadLength  = 0;
                    // Send configuration
                    this->radioOut_out(0, cfgMsg);

                    // Send a test payload as in the original code
                    RadioMessage dataMsg;
                    dataMsg.type = 1;
                    dataMsg.payloadLength = 5;
                    dataMsg.payload[0] = 0x42;
                    dataMsg.payload[1] = 0x61;
                    dataMsg.payload[2] = 0x64;
                    dataMsg.payload[3] = 0x5A;
                    dataMsg.payload[4] = testId;
                    this->radioOut_out(0, dataMsg);
                }
                // Reset assembly state
                this->waitingForSecondFrame = false;
            } else {
                // Unexpected frame – reset state
                this->waitingForSecondFrame = false;
            }
        }
    }
}