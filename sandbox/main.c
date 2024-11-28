#include "wayc/buffer.h"
#include "wayc/core.h"
#include "wayc/message.h"
#include <stdio.h>

int main() {
  wayc_wayland_t *wayc = wayc_connect();

  wayc_buffer_t *msg = wayc_buffer_create(128);

  wayc_write_u32(msg, 1);  // object id
  wayc_write_u16(msg, 1);  // opcode
  wayc_write_u16(msg, 12); // length
  wayc_write_u32(msg, 2);  // new_id

  wayc_write(wayc, msg);
  wayc_buffer_free(msg);

  wayc_buffer_t *res = wayc_read(wayc);

  while (res->size > 8) {
    wayc_message_t *message = wayc_message_from_bytes(res);
    if (!message) {
      break;
    }
    uint32_t name = wayc_read_u32(message->data);
    char *value = wayc_read_string(message->data);
    uint32_t version = wayc_read_u32(message->data);

    printf("Name: %d, Interface: %s, Version: %d\n", name, value, version);
    wayc_message_free(message);
  }

  wayc_disconnect(wayc);
  wayc_buffer_free(res);

  return 0;
}
