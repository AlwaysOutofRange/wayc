#include "wayc/core.h"
#include "wayc/buffer.h"
#include "wayc/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

wayc_wayland_t *wayc_connect(void) {
  wayc_state_t *wayc_state = malloc(sizeof(wayc_state_t));

  char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
  if (xdg_runtime_dir == NULL) {
    fprintf(stderr, "XDG_RUNTIME_DIR is not set!\n");
    return NULL;
  }

  size_t xdg_runtime_dir_len = strlen(xdg_runtime_dir);

  struct sockaddr_un addr = {.sun_family = AF_UNIX};
  wayc_assert(xdg_runtime_dir_len < sizeof(addr.sun_path));

  size_t socket_path_len = 0;

  memcpy(addr.sun_path, xdg_runtime_dir, xdg_runtime_dir_len);
  socket_path_len += xdg_runtime_dir_len;

  addr.sun_path[socket_path_len++] = '/';

  char *wayland_display = getenv("WAYLAND_DISPLAY");
  if (wayland_display == NULL) {
    char *wayland_display_default = "wayland-0";
    size_t wayland_display_default_len = strlen(wayland_display_default);

    memcpy(addr.sun_path + socket_path_len, wayland_display_default,
           wayland_display_default_len);
  } else {
    size_t wayland_display_len = strlen(wayland_display);

    memcpy(addr.sun_path + socket_path_len, wayland_display,
           wayland_display_len);
  }

  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  wayc_assert(socket_fd != -1);

  int ret = connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
  wayc_assert(ret != -1);

  wayc_state->socket_fd = socket_fd;

  wayc_wayland_t *wayc = malloc(sizeof(wayc_wayland_t));
  wayc->state = wayc_state;

  return wayc;
}

void wayc_write(wayc_wayland_t *wayc, wayc_buffer_t *buffer) {
  size_t ret =
      send(wayc->state->socket_fd, buffer->data, buffer->size, MSG_DONTWAIT);
  wayc_assert(ret == buffer->size);
}

wayc_buffer_t *wayc_read(wayc_wayland_t *wayc) {
  wayc_buffer_t *buffer = wayc_buffer_create(4096);
  size_t ret = read(wayc->state->socket_fd, buffer->data, buffer->capacity);
  wayc_assert((int)ret != -1);
  buffer->size = ret;
  printf("Read %zu bytes\n", ret);

  return buffer;
}

void wayc_disconnect(wayc_wayland_t *wayc) {
  close(wayc->state->socket_fd);
  free(wayc->state);
  free(wayc);
}
