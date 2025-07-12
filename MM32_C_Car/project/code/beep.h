#ifndef __BEEP_H__
#define __BEEP_H__
#include "zf_common_headfile.h"

#define BEEP                (D7 )
extern uint8 beep_flag;
void beep_init(void);
void beep_ms(uint16 time);
void beep_on(void);
void beep_off(void);

#endif
