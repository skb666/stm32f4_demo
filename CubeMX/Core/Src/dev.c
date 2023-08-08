#include "dev.h"

#include <stdio.h>

#include "main.h"
#include "ring_fifo.h"

#define UART1_TX_RING_SIZE 1024
#define UART1_RX_RING_SIZE 1024
#define UART1_DMATX_BUF_SIZE 256
#define UART1_DMARX_BUF_SIZE 256

typedef struct {
  volatile uint16_t status; /* 发送状态 */
  uint16_t last_dmarx_size; /* dma上一次接收数据大小 */
  uint32_t tx_count;
  uint32_t rx_count;
} uart_device_t;

static ring_def(uint8_t, uart1_tx_ring, UART1_TX_RING_SIZE, 1);
static ring_def(uint8_t, uart1_rx_ring, UART1_RX_RING_SIZE, 1);
static uint8_t uart1_dmatx_buf[UART1_DMATX_BUF_SIZE] __attribute__((section(".fast.dmatx")));
static uint8_t uart1_dmarx_buf[UART1_DMARX_BUF_SIZE] __attribute__((section(".fast.dmarx")));

static uart_device_t uart1_dev = {0};

void uart1_config(void) {
  /* USART1_RX DMA */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_2, LL_DMA_CHANNEL_4);
  LL_DMA_ConfigTransfer(DMA2, LL_DMA_STREAM_2,
      LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
          LL_DMA_PRIORITY_HIGH |
          LL_DMA_MODE_CIRCULAR |
          LL_DMA_PERIPH_NOINCREMENT |
          LL_DMA_MEMORY_INCREMENT |
          LL_DMA_PDATAALIGN_BYTE |
          LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_2,
      LL_USART_DMA_GetRegAddr(USART1),
      (uint32_t)uart1_dmarx_buf,
      LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_2));
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, UART1_DMARX_BUF_SIZE);

  /* USART1_TX DMA */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_7, LL_DMA_CHANNEL_4);
  LL_DMA_ConfigTransfer(DMA2, LL_DMA_STREAM_7,
      LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
          LL_DMA_PRIORITY_HIGH |
          LL_DMA_MODE_NORMAL |
          LL_DMA_PERIPH_NOINCREMENT |
          LL_DMA_MEMORY_INCREMENT |
          LL_DMA_PDATAALIGN_BYTE |
          LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_7,
      (uint32_t)uart1_dmatx_buf,
      LL_USART_DMA_GetRegAddr(USART1),
      LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_7));

  LL_DMA_ClearFlag_DME2(DMA2);
  LL_DMA_ClearFlag_DME7(DMA2);
  LL_DMA_ClearFlag_HT2(DMA2);
  LL_DMA_ClearFlag_TC2(DMA2);
  LL_DMA_ClearFlag_TC7(DMA2);
  LL_DMA_ClearFlag_TE2(DMA2);
  LL_DMA_ClearFlag_TE7(DMA2);

  LL_DMA_EnableIT_HT(DMA2, LL_DMA_STREAM_2);
  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_7);

  LL_USART_EnableDMAReq_RX(USART1);
  LL_USART_EnableDMAReq_TX(USART1);
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);

  LL_USART_EnableIT_IDLE(USART1);
}

/**
 * @brief  串口dma接收完成中断处理
 * @param
 * @retval
 */
void uart1_dmarx_done_isr(void) {
  uint16_t recv_size;

  recv_size = UART1_DMARX_BUF_SIZE - uart1_dev.last_dmarx_size;

  __disable_irq();
  ring_push_mult(&uart1_rx_ring, &uart1_dmarx_buf[uart1_dev.last_dmarx_size], recv_size);
  __enable_irq();

  uart1_dev.rx_count += recv_size;
  uart1_dev.last_dmarx_size = 0;
}

/**
 * @brief  串口dma接收部分数据中断处理
 * @param
 * @retval
 */
void uart1_dmarx_part_done_isr(void) {
  uint16_t recv_total_size;
  uint16_t recv_size;

  recv_total_size = UART1_DMARX_BUF_SIZE - LL_DMA_GetDataLength(DMA2, LL_DMA_STREAM_2);
  recv_size = recv_total_size - uart1_dev.last_dmarx_size;

  __disable_irq();
  ring_push_mult(&uart1_rx_ring, &uart1_dmarx_buf[uart1_dev.last_dmarx_size], recv_size);
  __enable_irq();

  uart1_dev.rx_count += recv_size;
  uart1_dev.last_dmarx_size = recv_total_size;
}

/**
 * @brief  串口dma发送完成中断处理
 * @param
 * @retval
 */
void uart1_dmatx_done_isr(void) {
  uart1_dev.status = 0; /* DMA发送空闲 */
}

void uart1_tx_poll(void) {
  uint16_t size = 0;

  if (uart1_dev.status) {
    return;
  }

  if (ring_is_empty(&uart1_tx_ring)) {
    return;
  }

  __disable_irq();
  size = ring_pop_mult(&uart1_tx_ring, &uart1_dmatx_buf, UART1_DMATX_BUF_SIZE);
  __enable_irq();

  uart1_dev.status = 1;
  uart1_dev.tx_count += size;

  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_7);
  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_7, size);
  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_7);
}

uint16_t uart1_read(uint8_t *buf, uint16_t size) {
  uint16_t ok = 0;

  if (buf == NULL) {
    return 0;
  }

  if (ring_is_empty(&uart1_rx_ring)) {
    return 0;
  }

  __disable_irq();
  ok = ring_pop_mult(&uart1_rx_ring, buf, size);
  __enable_irq();

  return ok;
}

uint16_t uart1_write(const uint8_t *buf, uint16_t size) {
  uint16_t ok = 0;

  if (buf == NULL) {
    return 0;
  }

  __disable_irq();
  ok = ring_push_mult(&uart1_tx_ring, buf, size);
  __enable_irq();

  return ok;
}

void print_uart1_tx_rx(void) {
  printf("uart1_tx: %u\n", uart1_dev.tx_count);
  printf("uart1_rx: %u\n", uart1_dev.rx_count);
}
