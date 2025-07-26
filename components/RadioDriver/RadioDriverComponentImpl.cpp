#include "RadioDriverComponentImpl.hpp"
#include "config.h"
#include "drivers/radio/radio_hal.h"
#include "subsystems/housekeeping/housekeeping.h"

#include <cstring>

using namespace SBandDemo;

RadioDriverComponentImpl::RadioDriverComponentImpl(const char* compName)
    : RadioDriverComponentBase(compName)
{
    // Constructor: nothing to do
}

void RadioDriverComponentImpl::init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance) {
    // Initialise the base class with the queue depth and instance number
    RadioDriverComponentBase::init(queueDepth, instance);
    // Load default configuration and initialise hardware
    config_load_defaults();
    radio_hal_init();
}

void RadioDriverComponentImpl::cmdIn_handler(NATIVE_INT_TYPE portNum, const SBandDemo::RadioMessage& msg) {
    (void) portNum;
    // Type 0: configure radio
    if (msg.type == 0) {
        radio_config_t cfg = get_active_radio_config();
        cfg.rf_freq       = msg.frequencyHz;
        cfg.tx_power      = msg.txPower;
        cfg.modulation    = msg.modulation;
        cfg.band_width    = msg.bandwidthHz;
        cfg.lora_sf       = msg.spreadingFactor;
        cfg.code_rate     = msg.codingRate;
        set_active_radio_config(&cfg);
        bool ok = radio_hal_init();
        // Send status out (1 for OK, 0 for failure)
        this->status_out(0, ok ? 1u : 0u);
    }
    // Type 1: transmit data
    else if (msg.type == 1) {
        // Build a RadioHead‑style header (0xFF 0xFF 0x00 0x00)
        uint8_t header[4] = {0xFF, 0xFF, 0x00, 0x00};
        uint8_t tx_buf[256];
        // Limit payload length to avoid overflow
        size_t dataLen = msg.payloadLength;
        if (dataLen > sizeof(tx_buf) - sizeof(header)) {
            dataLen = sizeof(tx_buf) - sizeof(header);
        }
        std::memcpy(tx_buf, header, sizeof(header));
        std::memcpy(tx_buf + sizeof(header), msg.payload, dataLen);
        size_t totalLen = dataLen + sizeof(header);
        radio_hal_transmit(tx_buf, static_cast<uint8_t>(totalLen));
        // Increment housekeeping counter for successful transmit
        hk_increment_tx_count();
        this->status_out(0, 1u);
    }
    // Unknown type: ignore or flag error
    else {
        this->status_out(0, 0u);
    }
}