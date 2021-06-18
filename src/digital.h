#ifndef DIGITAL_H
# define DIGITAL_H

# include "common.h"
# include "progmem.h"

char digitalRead(u8 pin);
void digitalWrite(u8 pin, u8 val);

#endif
