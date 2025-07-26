#include "radio_hal.h"
#include "drivers/radio/peripheral_lora1280.h"
#include "peripheral_rfm9x.h"
#include "config.h"

bool radio_hal_init() {
    if (sys_config.radio_type == RADIO_TYPE_SX1280) {
        return lora1280_init();
    } else if (sys_config.radio_type == RADIO_TYPE_RFM9X) {
        return rfm9x_init();
    }
    return false;
}

bool radio_hal_transmit(const uint8_t *data, uint8_t length) {
    if (sys_config.radio_type == RADIO_TYPE_SX1280) {
        return lora1280_transmit(data, length);
    } else if (sys_config.radio_type == RADIO_TYPE_RFM9X) {
        return rfm9x_transmit(data, length);
    }
    return false;
}