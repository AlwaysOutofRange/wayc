#include "wayc/buffer.h"
#include "wayc/core.h"
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

  uint32_t object_id = wayc_read_u32(res);
  uint16_t opcode = wayc_read_u16(res);
  uint16_t length = wayc_read_u16(res);
  uint8_t *data = wayc_read_bytes(res, length - 8);
  wayc_buffer_t *interface = wayc_buffer_from_bytes(data, length - 8);
  uint32_t name = wayc_read_u32(interface);
  char *name_str = wayc_read_string(interface);
  uint32_t version = wayc_read_u32(interface);
  wayc_buffer_free(res);

  printf("Object id: %u\n", object_id);
  printf("Opcode: %u\n", opcode);
  printf("Length: %u\n", length);
  printf("Interface name: %u\n", name);
  printf("Interface name: %s\n", name_str);
  printf("Interface version: %u\n", version);

  wayc_disconnect(wayc);

  return 0;
}
