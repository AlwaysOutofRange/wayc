#pragma once

#include "wayc/buffer.h"
#include <stdint.h>

typedef struct wayc_message_header {
  uint32_t object_id;
  uint16_t opcode;
  uint16_t length;
} wayc_message_header_t;

typedef struct wayc_message {
  wayc_message_header_t header;
  wayc_buffer_t *data;
} wayc_message_t;

wayc_message_t *wayc_message_from_bytes(wayc_buffer_t *buffer);
void wayc_message_free(wayc_message_t *message);
