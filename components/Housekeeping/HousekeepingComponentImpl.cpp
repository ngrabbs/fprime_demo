#include "HousekeepingComponentImpl.hpp"
#include "subsystems/housekeeping/housekeeping.h"
#include "protocols/protocol_spacecan.h"

#include <cstring>

using namespace SBandDemo;

HousekeepingComponentImpl::HousekeepingComponentImpl(const char* compName)
    : HousekeepingComponentBase(compName)
{
}

void HousekeepingComponentImpl::init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance) {
    HousekeepingComponentBase::init(queueDepth, instance);
    // Initialise housekeeping subsystem (ADC etc.)
    housekeeping_init();
}

void HousekeepingComponentImpl::schedIn_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    (void) portNum;
    (void) context;
    // Collect telemetry
    hk_telemetry_t data = collect_housekeeping_data();
    uint8_t buf[64];
    size_t len = protocol_spacecan_format_beacon(buf, sizeof(buf), &data);
    if (len > 0) {
        // Build a radio message
        RadioMessage msg;
        msg.type = 1;
        msg.payloadLength = static_cast<uint8_t>(len);
        std::memcpy(msg.payload, buf, len);
        // Send to radio
        this->radioOut_out(0, msg);
    }
}