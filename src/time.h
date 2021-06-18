#ifndef TIME_H
# define TIME_H
# include "common.h"

extern volatile unsigned long timer0_overflows;
extern volatile unsigned long timer0_ms;

void delayMicroseconds(u16 us);
void delay(unsigned long ms);
unsigned long millis();
unsigned long micros();

#endif
