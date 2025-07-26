// housekeeping.h - Collects system-level telemetry

#ifndef HOUSEKEEPING_H
#define HOUSEKEEPING_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    float temperature_c;
    uint32_t uptime_ms;
    size_t heap_free_bytes;
    uint8_t task_count;
    uint8_t radio_tx_count;
    uint8_t radio_rx_count;
} hk_telemetry_t;

void housekeeping_init(void);
hk_telemetry_t collect_housekeeping_data(void);

#endif // HOUSEKEEPING_H
