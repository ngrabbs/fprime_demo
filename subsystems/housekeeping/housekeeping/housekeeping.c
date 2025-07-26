// housekeeping.c - Provides basic system telemetry for CubeSat

#include "subsystems/housekeeping/housekeeping.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

// Placeholder counters for radio stats
static uint8_t radio_tx_count = 0;
static uint8_t radio_rx_count = 0;

void housekeeping_init(void) {
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); // Temp sensor is on ADC4
}

// Call this when you successfully transmit
void hk_increment_tx_count(void) {
    radio_tx_count++;
}

// Call this when you receive a radio frame
void hk_increment_rx_count(void) {
    radio_rx_count++;
}

static float read_internal_temp_c() {
    uint16_t raw = adc_read();
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = raw * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

hk_telemetry_t collect_housekeeping_data(void) {
    hk_telemetry_t data;

    data.temperature_c = read_internal_temp_c();
    data.uptime_ms = to_ms_since_boot(get_absolute_time());
    data.heap_free_bytes = xPortGetFreeHeapSize();
    data.task_count = uxTaskGetNumberOfTasks();
    data.radio_tx_count = radio_tx_count;
    data.radio_rx_count = radio_rx_count;

    return data;
}
