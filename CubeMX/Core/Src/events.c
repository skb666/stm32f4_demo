#include "events.h"

#include <stdio.h>

#include "main.h"
#include "ring_fifo.h"

static ring_def(EVENT, g_events, 100, 1);

int8_t event_put(EVENT *ev) {
  int8_t err = 0;

  __disable_irq();
  err = ring_push(&g_events, ev);
  __enable_irq();

  return err;
}

int8_t event_get(EVENT *ev) {
  int8_t err = 0;

  __disable_irq();
  err = ring_pop(&g_events, ev);
  __enable_irq();

  return err;
}

int16_t event_count() {
  return ring_size(&g_events);
}

int8_t event_empty() {
  return ring_is_empty(&g_events);
}

void tim_event_proc(EVENT *ev) {
  LL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

void key_event_proc(EVENT *ev) {
  switch (ev->sub_type) {
    case KE_PRESS: {
      printf("[%hhu]: PRESS\n", ev->id);
    } break;
    case KE_RELEASE: {
      printf("[%hhu]: RELEASE\n", ev->id);
    } break;
    case KE_LONG_PRESS: {
      printf("[%hhu]: LONG_PRESS\n", ev->id);
      if (ev->id == 0) {
        LL_GPIO_TogglePin(LCD_BL_GPIO_Port, LCD_BL_Pin);
      }
    } break;
    case KE_LONG_RELEASE: {
      printf("[%hhu]: LONG_RELEASE\n", ev->id);
    } break;
    default: break;
  }
}
