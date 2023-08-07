#ifndef __DEV_H__
#define __DEV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void uart1_config(void);
void uart1_dmarx_done_isr(void);
void uart1_dmarx_part_done_isr(void);
void uart1_dmatx_done_isr(void);

void uart1_tx_poll(void);
uint16_t uart1_read(uint8_t *buf, uint16_t size);
uint16_t uart1_write(const uint8_t *buf, uint16_t size);
void print_uart1_tx_rx(void);

#ifdef __cplusplus
}
#endif

#endif
