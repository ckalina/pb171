#ifndef ANALOG_H
# define ANALOG_H

# include "common.h"
# include "progmem.h"

u16 analogRead(u8 pin);
void analogWrite(u8 pin, int val);

#endif
