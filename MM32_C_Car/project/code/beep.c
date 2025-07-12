#include "zf_common_headfile.h"
#include "beep.h"

uint8 beep_flag = 0;

void beep_init(void){
	gpio_init(BEEP, GPO, GPIO_LOW, GPO_PUSH_PULL);
}

void beep_ms(uint16 time){
	gpio_set_level(BEEP, GPIO_HIGH);                                            // BEEP Ïì
	system_delay_ms(time);
	gpio_set_level(BEEP, GPIO_LOW);                                             // BEEP Í£

}

void beep_on(void){
	gpio_set_level(BEEP, GPIO_HIGH);                                            // BEEP Ïì

}

void beep_off(void){
	gpio_set_level(BEEP, GPIO_LOW);                                             // BEEP Í£

}
