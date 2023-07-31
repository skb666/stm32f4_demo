#ifndef __RING_FIFO_H__
#define __RING_FIFO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

typedef struct {
  void *buffer;
  const uint16_t capacity;
  const uint16_t element_size;
  const uint16_t cover;
  int16_t head;
  int16_t tail;
  int16_t size;
} RING_FIFO;

#define ring_def(Type, BufName, Size, Cover) \
  Type __##BufName##_data[Size];             \
  RING_FIFO BufName = {                      \
      .buffer = __##BufName##_data,          \
      .capacity = Size,                      \
      .element_size = sizeof(Type),          \
      .cover = Cover,                        \
      .head = 0,                             \
      .tail = 0,                             \
      .size = 0,                             \
  }

int8_t ring_push(RING_FIFO *ring, void *element);
int8_t ring_pop(RING_FIFO *ring, void *element);
void ring_reset(RING_FIFO *ring);
int8_t ring_is_empty(RING_FIFO *ring);
int8_t ring_is_full(RING_FIFO *ring);
int16_t ring_size(RING_FIFO *ring);
void print_ring(RING_FIFO *ring);

#ifdef __cplusplus
}
#endif

#endif
