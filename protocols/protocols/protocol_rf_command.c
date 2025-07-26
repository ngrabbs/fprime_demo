// protocol_rf_command.c
#include "protocol_rf_command.h"
#include <stdio.h>
#include <string.h>

void handle_rf_command(const char *payload, size_t len) {
    printf("[RF CMD] Received RF payload (%zu bytes): ", len);
    for (size_t i = 0; i < len; ++i) {
        printf("%c", payload[i]);
    }
    printf("\n");

    // TODO: parse and act on the payload contents
}
