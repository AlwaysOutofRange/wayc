#include "wayc/message.h"
#include "wayc/buffer.h"
#include "wayc/utils.h"
#include <stdlib.h>

wayc_message_t *wayc_message_from_bytes(wayc_buffer_t *buffer) {
  wayc_assert(buffer->size >= 8);
  wayc_message_t *message = malloc(sizeof(wayc_message_t));
  wayc_message_header_t header = {
      .object_id = wayc_read_u32(buffer),
      .opcode = wayc_read_u16(buffer),
      .length = wayc_read_u16(buffer),
  };

  message->header = header;
  size_t rest_bytes = message->header.length - 8;

  if (buffer->size < rest_bytes) {
    return NULL;
  }

  uint8_t *data = wayc_read_bytes(buffer, rest_bytes);
  message->data = wayc_buffer_from_bytes(data, rest_bytes);

  return message;
}

void wayc_message_free(wayc_message_t *message) {
  wayc_buffer_free(message->data);
  free(message);
}
