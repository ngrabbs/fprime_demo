#include "peripheral_rfm9x.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "config.h"
#include "debug.h"
#include <stdio.h>

void spi_cs_select(uint cs_pin) {
    gpio_put(cs_pin, 0);
}

void spi_cs_deselect(uint cs_pin) {
    gpio_put(cs_pin, 1);
}

void spi_write_register(uint8_t addr, uint8_t value) {
    uint8_t tx[2];
    tx[0] = addr | 0x80; // MSB = 1 → write
    tx[1] = value;
    spi_cs_select(pin_config.cs_lora);
    DEBUG_DEBUG("[RADIO DEBUG] SPI WRITE tx: %02X %02X\n", tx[0], tx[1]);
    spi_write_blocking(pin_config.spi_bus0, tx, 2);
    spi_cs_deselect(pin_config.cs_lora);
}

uint8_t spi_read_register(uint8_t addr) {
    uint8_t tx[2] = { addr & 0x7F, 0x00 };
    uint8_t rx[2] = { 0 };
    spi_cs_select(pin_config.cs_lora);
    spi_write_read_blocking(pin_config.spi_bus0, tx, rx, 2);
    spi_cs_deselect(pin_config.cs_lora);

    DEBUG_DEBUG("[DEBUG] SPI READ tx: %02X %02X | rx: %02X %02X\n", tx[0], tx[1], rx[0], rx[1]);

    return rx[1];
}

bool rfm9x_init(void) {
    DEBUG_INFO("[RFM9x] init\n");
    radio_config_t current_config = get_active_radio_config();

    gpio_init(pin_config.cs_lora);
    gpio_set_dir(pin_config.cs_lora, GPIO_OUT);
    gpio_put(pin_config.cs_lora, 1);
    DEBUG_INFO("[RFM9x] pin set done\n");

    uint8_t version = spi_read_register(0x42);
    DEBUG_INFO("[RFM9x] RegVersion = 0x%02X %d\n", version, version);

    if (version == 0x12) {
        DEBUG_INFO("[RFM9x] SPI OK!\n");
    } else {
        DEBUG_INFO("[RFM9x] SPI FAILURE - check wiring or power!\n");
        return false;
    }

    spi_write_register(0x01, 0x08);
    sleep_ms(10);
    spi_write_register(0x01, 0x88);
    spi_write_register(0x01, 0x80);

    spi_write_register(0x0E, 0x00);
    spi_write_register(0x0F, 0x00);
    spi_write_register(0x01, 0x81);

    // Set frequency
    rfm9x_set_frequency(current_config.rf_freq);

    // Set preamble length = 8
    spi_write_register(0x20, 0x00);
    spi_write_register(0x21, 0x08);

    // Set bandwidth
    rfm9x_set_bandwidth(current_config.band_width);

    // Set coding rate
    rfm9x_set_coding_rate(current_config.code_rate);

    // Set spreading factor
    rfm9x_set_spreading_factor(current_config.lora_sf);

    // Enable CRC
    uint8_t modem_cfg_2 = spi_read_register(0x1E);
    modem_cfg_2 |= 0x04;
    spi_write_register(0x1E, modem_cfg_2);

    // Disable AGC
    spi_write_register(0x26, 0x00);

    // Set transmit power
    rfm9x_set_power(current_config.tx_power);

    // Additional errata/low-level settings
    spi_write_register(0x4D, 0x84);
    spi_write_register(0x4D, 0x87);

    DEBUG_INFO("[RFM9x] basic registers configured\n");
    DEBUG_INFO("[RFM9x] end init()\n");
    return true;
}

bool rfm9x_set_frequency(uint32_t freq_hz) {
    DEBUG_INFO("[RFM9x] Set frequency: %u Hz\n", freq_hz);

    uint32_t frf = (uint32_t)((double)freq_hz / 61.035);
    spi_write_register(0x06, (frf >> 16) & 0xFF); // RegFrMsb
    spi_write_register(0x07, (frf >> 8) & 0xFF);  // RegFrMid
    spi_write_register(0x08, frf & 0xFF);         // RegFrLsb

    return true;
}

bool rfm9x_set_power(uint8_t power_dbm) {
    DEBUG_INFO("[RFM9x] Set power: %u dBm\n", power_dbm);

    uint8_t pa_config = 0x80; // PA_BOOST
    if (power_dbm > 17) {
        pa_config |= 0x0F;
    } else {
        pa_config |= (power_dbm - 2);
    }
    spi_write_register(0x09, pa_config);
    return true;
}

bool rfm9x_set_modulation(uint8_t modulation) {
    DEBUG_INFO("[RFM9x] Set modulation: 0x%02X\n", modulation);

    // RFM9x only supports LoRa in this use-case
    // So we simply enforce LoRa mode
    spi_write_register(0x01, 0x81); // LoRa + standby
    return true;
}

bool rfm9x_set_bandwidth(uint32_t bw_reg) {
    DEBUG_INFO("[RFM9x] Set bandwidth reg: 0x%02X\n", bw_reg);

    uint8_t modem_cfg_1 = spi_read_register(0x1D);
    modem_cfg_1 &= 0x0F;          // Clear BW bits
    modem_cfg_1 |= (bw_reg << 4); // Set new BW bits
    spi_write_register(0x1D, modem_cfg_1);
    return true;
}

bool rfm9x_set_spreading_factor(uint8_t sf_reg) {
    DEBUG_INFO("[RFM9x] Set spreading factor reg: 0x%02X\n", sf_reg);

    uint8_t modem_cfg_2 = spi_read_register(0x1E);
    modem_cfg_2 &= 0x0F;
    modem_cfg_2 |= sf_reg;
    spi_write_register(0x1E, modem_cfg_2);
    return true;
}

bool rfm9x_set_coding_rate(uint8_t cr_reg) {
    DEBUG_INFO("[RFM9x] Set coding rate reg: 0x%02X\n", cr_reg);

    uint8_t modem_cfg_1 = spi_read_register(0x1D);
    modem_cfg_1 &= 0xF1; // Clear CR bits
    modem_cfg_1 |= (cr_reg << 1);
    spi_write_register(0x1D, modem_cfg_1);
    return true;
}

bool rfm9x_transmit(const uint8_t *data, uint8_t length) {
    DEBUG_INFO("[RFM9x] Transmitting (%d bytes)\n", length);

    // Put device into standby mode
    spi_write_register(0x01, 0x81);

    // Set payload length
    spi_write_register(0x22, length);

    // Read back to confirm payload length register
    uint8_t check_len = spi_read_register(0x22);
    DEBUG_INFO("[RFM9x] Confirmed RegPayloadLength = %u\n", check_len);

    // Explicitly reset FIFO pointer to base address (0x00)
    spi_write_register(0x0D, 0x00);
    DEBUG_INFO("[RFM9x] FIFO pointer reset to 0x00\n");

    // Write payload bytes into FIFO
    for (uint8_t i = 0; i < length; i++) {
        spi_write_register(0x00, data[i]);
    }

    // Set device into transmit mode
    spi_write_register(0x01, 0x83);

    // Wait for TX_DONE flag in RegIrqFlags (0x12)
    while (1) {
        uint8_t irq_flags = spi_read_register(0x12);
        if (irq_flags & 0x08) { // Bit 3 = TxDone
            break;
        }
    }

    // Clear all IRQ flags
    spi_write_register(0x12, 0xFF);

    DEBUG_INFO("[RFM9x] TX complete\n");
    return true;
}

void rfm9x_check_and_disable_whitening(void) {
    DEBUG_INFO("[RFM9x] Checking whitening and packet config registers...\n");

    // Read RegModemConfig1 (0x1D)
    uint8_t modem_cfg_1 = spi_read_register(0x1D);
    DEBUG_INFO("[DEBUG] RegModemConfig1 (0x1D): 0x%02X\n", modem_cfg_1);

    // Read RegModemConfig2 (0x1E)
    uint8_t modem_cfg_2 = spi_read_register(0x1E);
    DEBUG_INFO("[DEBUG] RegModemConfig2 (0x1E): 0x%02X\n", modem_cfg_2);

    // Read RegPacketConfig (0x1F) if present in your chip variant
    uint8_t packet_cfg = spi_read_register(0x1F);
    DEBUG_INFO("[DEBUG] RegPacketConfig (0x1F): 0x%02X\n", packet_cfg);

    // Check DCFree bits in RegPacketConfig for whitening (bits 5:4)
    uint8_t dc_free_bits = (packet_cfg & 0x30) >> 4;
    if (dc_free_bits != 0x02) {
        DEBUG_INFO("[RFM9x] Whitening is currently DISABLED. Enabling it now...\n");
        packet_cfg &= ~(0x30);         // Clear bits 5:4
        packet_cfg |= (0x02 << 4);     // Set DCFree = 10 (whitening enabled)
        spi_write_register(0x1F, packet_cfg);

        // Read back to confirm write
        uint8_t verify_cfg = spi_read_register(0x1F);
        DEBUG_INFO("[RFM9x] Whitening enabled. RegPacketConfig now reads back: 0x%02X\n", verify_cfg);
    } else {
        DEBUG_INFO("[RFM9x] Whitening already ENABLED. DCFree bits: 0x%02X\n", dc_free_bits);
    }
}
