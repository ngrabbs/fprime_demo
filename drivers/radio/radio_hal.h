#ifndef RADIO_HAL_H
#define RADIO_HAL_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

bool radio_hal_init(void);
bool radio_hal_transmit(const uint8_t *data, uint8_t length);

#endif
