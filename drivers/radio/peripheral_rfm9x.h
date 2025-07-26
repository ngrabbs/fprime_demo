#ifndef PERIPHERAL_RFM9X_H
#define PERIPHERAL_RFM9X_H

#include <stdint.h>
#include <stdbool.h>

bool rfm9x_init(void);
bool rfm9x_set_frequency(uint32_t freq_hz);
bool rfm9x_set_power(uint8_t power_dbm);
bool rfm9x_set_modulation(uint8_t modulation);
bool rfm9x_set_bandwidth(uint32_t bw_reg);
bool rfm9x_set_spreading_factor(uint8_t sf_reg);
bool rfm9x_set_coding_rate(uint8_t cr_reg);
bool rfm9x_transmit(const uint8_t *data, uint8_t length);
void rfm9x_check_and_disable_whitening(void);

#endif
