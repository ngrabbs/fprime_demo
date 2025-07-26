// protocol_spacecan.c
#include "protocols/protocol_spacecan.h"
#include "core/message_queue.h"
#include "freertos/task_radio.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void spacecan_parse_frame(const can_frame_t* frame) {
    if (frame->dlc < 1) {
        printf("SPACECAN: Frame too short.\n");
        return;
    }

    uint8_t type = frame->data[0];

    if (type == SPACECAN_TYPE_COMMAND) {
        if (frame->dlc < 11) {
            printf("SPACECAN: Command frame too short for extended RUN_TEST.\n");
            return;
        }

        uint8_t cmd = frame->data[1];
        uint8_t test_id = frame->data[2];
        uint16_t freq_x10 = (frame->data[3] << 8) | frame->data[4];
        uint16_t freq_mhz = freq_x10 / 10;
        uint8_t power = frame->data[5];
        uint8_t modulation = frame->data[6];
        uint8_t bandwidth = frame->data[7];
        uint8_t spreading_factor = frame->data[8];
        uint8_t coding_rate = frame->data[9];
        uint8_t checksum = frame->data[10];

        // Recalculate checksum
        uint8_t calc_csum = 0;
        for (int i = 0; i < 10; i++) {
            calc_csum += frame->data[i];
        }
        printf("Calculating checksum on RX side:\n");
        for (int i = 0; i < 10; i++) {
            printf("%02X ", frame->data[i]);
        }
        printf("\n");
        printf("Computed checksum: 0x%02X\n", calc_csum);

        if (calc_csum != checksum) {
            printf("SPACECAN: Checksum failed! Got 0x%02X, expected 0x%02X\n", checksum, calc_csum);
            return;
        }

        switch (cmd) {
            case SPACECAN_CMD_RUN_TEST:
                printf("SPACECAN: RUN TEST command\n");
                printf("  Test ID: %u\n", test_id);
                printf("  Frequency: %u MHz\n", freq_mhz);
                printf("  Power: %u dBm\n", power);
                printf("  Modulation: 0x%02X\n", modulation);
                printf("  Bandwidth reg: 0x%02X\n", bandwidth);
                printf("  Spreading Factor reg: 0x%02X\n", spreading_factor);
                printf("  Coding Rate reg: 0x%02X\n", coding_rate);

                // Prepare CONFIGURE message with new parameters
                radio_message_t cfg_msg = {
                    .type = RADIO_MSG_CONFIGURE,
                    .body.config.frequency_hz = freq_mhz * 1000000,
                    .body.config.power_dbm = power,
                    .body.config.modulation = modulation,
                    .body.config.bandwidth_hz = bandwidth,
                    .body.config.spreading_factor = spreading_factor,
                    .body.config.coding_rate = coding_rate
                };
                message_queue_send(&cfg_msg);

                // Prepare example SEND_DATA message
                radio_message_t data_msg = {
                    .type = RADIO_MSG_SEND_DATA,
                    .body.payload.length = 5
                };
                uint8_t test_payload[] = { 0x42, 0x61, 0x64, 0x5A, test_id };
                memcpy(data_msg.body.payload.data, test_payload, sizeof(test_payload));

                message_queue_send(&data_msg);

                break;

            case SPACECAN_CMD_STOP_TX:
                printf("SPACECAN: STOP TRANSMISSION\n");
                // Optionally send a STOP message or implement logic
                break;

            default:
                printf("SPACECAN: Unknown command 0x%02X\n", cmd);
                break;
        }
    } else {
        printf("SPACECAN: Unknown frame type 0x%02X\n", type);
    }
}

// Example binary beacon format (13 bytes):
// [0]   Type (1 byte)
// [1-2] Temperature (int16_t, 0.01°C units)
// [3-6] Uptime (uint32_t ms)
// [7-8] Free heap (uint16_t bytes / 32)
// [9]   Task count
// [10]  TX count
// [11]  RX count
// [12]  Reserved / checksum / flags

size_t protocol_spacecan_format_beacon(uint8_t *out, size_t max_len, const hk_telemetry_t *hk) {
    if (max_len < 13 || out == NULL || hk == NULL) return 0;

    memset(out, 0, 13);
    out[0] = SPACECAN_TYPE_BEACON;

    int16_t temp_encoded = (int16_t)(hk->temperature_c * 100);
    out[1] = (temp_encoded >> 8) & 0xFF;
    out[2] = temp_encoded & 0xFF;

    out[3] = (hk->uptime_ms >> 24) & 0xFF;
    out[4] = (hk->uptime_ms >> 16) & 0xFF;
    out[5] = (hk->uptime_ms >> 8) & 0xFF;
    out[6] = (hk->uptime_ms) & 0xFF;

    uint16_t heap_compact = (hk->heap_free_bytes / 32);
    out[7] = (heap_compact >> 8) & 0xFF;
    out[8] = heap_compact & 0xFF;

    out[9]  = hk->task_count;
    out[10] = hk->radio_tx_count;
    out[11] = hk->radio_rx_count;

    out[12] = 0x00; // Reserved / checksum placeholder

    return 13;
}

void send_spacecan_burst(void) {
    // TODO: replace with actual telemetry data later
    uint8_t fake_data[] = { 0x42, 0x61, 0x64, 0x5A, 0x00 };

    printf("[SPACECAN] Sending burst: ");
    for (int i = 0; i < sizeof(fake_data); ++i) {
        printf("%02X ", fake_data[i]);
    }
    printf("\n");

    // TODO: send over LoRa or CAN bus
}
