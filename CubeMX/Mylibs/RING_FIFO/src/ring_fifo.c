#include "ring_fifo.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int8_t ring_push(RING_FIFO *ring, void *element) {
  uint8_t *pbuf = NULL;

  if (ring->size >= ring->capacity) {
    if (ring->cover) {
      ring->head = (ring->head + 1) % ring->capacity;
      ring->size -= 1;
    } else {
      return -1;
    }
  }

  pbuf = (uint8_t *)ring->buffer + ring->tail * ring->element_size;
  memcpy(pbuf, element, ring->element_size);
  ring->tail = (ring->tail + 1) % ring->capacity;

  ring->size += 1;

  return 0;
}

int8_t ring_pop(RING_FIFO *ring, void *element) {
  uint8_t *pbuf = NULL;

  if (ring->size == 0) {
    return -1;
  }

  pbuf = (uint8_t *)ring->buffer + ring->head * ring->element_size;
  memcpy(element, pbuf, ring->element_size);
  ring->head = (ring->head + 1) % ring->capacity;

  ring->size -= 1;

  return 0;
}

void ring_reset(RING_FIFO *ring) {
  ring->head = 0;
  ring->tail = 0;
  ring->size = 0;
}

int8_t ring_is_empty(RING_FIFO *ring) {
  return (ring->size == 0);
}

int8_t ring_is_full(RING_FIFO *ring) {
  return (ring->size >= ring->capacity);
}

int16_t ring_size(RING_FIFO *ring) {
  return ring->size;
}

void print_ring(RING_FIFO *ring) {
  printf("========== ring ==========\n");
  printf("cover: %hu\n", ring->cover);
  printf("element_size: %hu\n", ring->element_size);
  printf("capacity: %hu\n", ring->capacity);
  printf("head: %hd\n", ring->head);
  printf("tail: %hd\n", ring->tail);
  printf("size: %hd\n", ring->size);
  printf("--------------------------\n");
}