#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct wayc_buffer {
  uint8_t *data;
  size_t pos;
  size_t size;
  size_t capacity;
} wayc_buffer_t;

wayc_buffer_t *wayc_buffer_create(size_t capacity);
wayc_buffer_t *wayc_buffer_from_bytes(const uint8_t *bytes, size_t len);
void wayc_buffer_free(wayc_buffer_t *buf);

void wayc_write_u32(wayc_buffer_t *buf, uint32_t value);
void wayc_write_u16(wayc_buffer_t *buf, uint16_t value);
void wayc_write_string(wayc_buffer_t *buf, const char *value);

uint32_t wayc_read_u32(wayc_buffer_t *buf);
uint16_t wayc_read_u16(wayc_buffer_t *buf);
char *wayc_read_string(wayc_buffer_t *buf);
uint8_t *wayc_read_bytes(wayc_buffer_t *buf, size_t len);
