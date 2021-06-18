#ifndef TEST_EEPROM_H
# define TEST_EEPROM_H

# include "test.h"

void test_eeprom()
{
	u8 ret;
# ifndef TEST_EEPROM_READONLY
	eeprom_write(2, 5);
# endif
	ret = eeprom_read(2);
	digitalWrite(13, ret == 5);
	begin(9600);
	uprintf("Got: %u\n", ret);
}
#endif
