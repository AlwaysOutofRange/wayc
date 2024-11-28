#pragma once

#include "wayc/buffer.h"

typedef struct wayc_state {
  int socket_fd;
} wayc_state_t;

typedef struct wayc_wayland {
  wayc_state_t *state;
} wayc_wayland_t;

wayc_wayland_t *wayc_connect(void);
void wayc_write(wayc_wayland_t *wayc, wayc_buffer_t *buffer);
wayc_buffer_t *wayc_read(wayc_wayland_t *wayc);
void wayc_disconnect(wayc_wayland_t *wayc);
