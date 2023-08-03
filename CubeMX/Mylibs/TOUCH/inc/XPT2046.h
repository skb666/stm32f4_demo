#ifndef __XPT2046_H__
#define __XPT2046_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint16_t x;
  uint16_t y;
} POINT;

typedef struct {
  // sizeof(long double) = 8
  long double An, Bn, Cn, Dn, En, Fn, Divider;
} TOUCH_PARAM;

extern volatile uint8_t touch_flag;
extern uint8_t cal_flag;
extern long double cal_p[6];

extern POINT ScreenSample[4];
extern POINT DisplaySample[4];
extern POINT display;
extern TOUCH_PARAM touch_param;

void Touch_Init(void);
int Touch_Calibrate(void);
int8_t Get_touch_point(POINT *displayPtr, POINT *screenPtr, TOUCH_PARAM *param);

POINT *Read_2046_2(void);
void Palette_Init(void);
void Palette_draw_point(uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif
