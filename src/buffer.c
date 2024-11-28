#include "wayc/buffer.h"
#include "wayc/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

wayc_buffer_t *wayc_buffer_create(size_t capacity) {
  wayc_buffer_t *buf = malloc(sizeof(wayc_buffer_t));
  buf->data = malloc(capacity);
  buf->size = 0;
  buf->pos = 0;
  buf->capacity = capacity;

  return buf;
}

wayc_buffer_t *wayc_buffer_from_bytes(const uint8_t *bytes, size_t len) {
  wayc_buffer_t *buf = wayc_buffer_create(len);
  memcpy(buf->data, bytes, len);
  buf->size = len;

  return buf;
}

void wayc_buffer_free(wayc_buffer_t *buf) {
  free(buf->data);
  free(buf);
}

void wayc_write_u32(wayc_buffer_t *buf, uint32_t value) {
  if (buf->size + 4 > buf->capacity) {
    buf->capacity *= 2;
    wayc_assert(buf->data != NULL);
    uint8_t *new_data = realloc(buf->data, buf->capacity);
    wayc_assert(new_data != NULL);
    buf->data = new_data;
  }

  memcpy(buf->data + buf->size, &value, 4);
  buf->size += 4;
}

void wayc_write_u16(wayc_buffer_t *buf, uint16_t value) {
  if (buf->size + 2 > buf->capacity) {
    buf->capacity *= 2;
    wayc_assert(buf->data != NULL);
    uint8_t *new_data = realloc(buf->data, buf->capacity);
    wayc_assert(new_data != NULL);
    buf->data = new_data;
  }

  memcpy(buf->data + buf->size, &value, 2);
  buf->size += 2;
}

void wayc_write_string(wayc_buffer_t *buf, const char *value) {
  size_t len = strlen(value);
  size_t padded_len = roundup_4(len + 1);

  if (buf->size + padded_len > buf->capacity) {
    while (buf->size + padded_len > buf->capacity) {
      buf->capacity *= 2;
    }

    wayc_assert(buf->data != NULL);
    uint8_t *new_data = realloc(buf->data, buf->capacity);
    wayc_assert(new_data != NULL);
    buf->data = new_data;
  }

  wayc_write_u32(buf, len);
  memcpy(buf->data + buf->size, value, len);
  buf->size += padded_len - 1;
}

uint32_t wayc_read_u32(wayc_buffer_t *buf) {
  if (buf->pos + 4 > buf->size) {
    return 0;
  }

  uint32_t value;
  memcpy(&value, buf->data + buf->pos, 4);
  buf->pos += 4;

  return value;
}

uint16_t wayc_read_u16(wayc_buffer_t *buf) {
  if (buf->pos + 2 > buf->size) {
    return 0;
  }

  uint16_t value;
  memcpy(&value, buf->data + buf->pos, 2);
  buf->pos += 2;

  return value;
}

char *wayc_read_string(wayc_buffer_t *buf) {
  if (buf->pos >= buf->size) {
    return NULL;
  }

  uint8_t len = wayc_read_u32(buf);

  if (buf->pos + len > buf->size) {
    return NULL;
  }

  char *value = malloc(len + 1);
  memcpy(value, buf->data + buf->pos, len);
  value[len] = '\0';
  buf->pos += roundup_4(len);

  return value;
}

uint8_t *wayc_read_bytes(wayc_buffer_t *buf, size_t len) {
  if (buf->pos + len > buf->size) {
    return NULL;
  }

  uint8_t *value = malloc(len);
  memcpy(value, buf->data + buf->pos, len);
  buf->pos += len;

  return value;
}
