#ifndef TEST_EEPROM_STRUCT_H
# define TEST_EEPROM_STRUCT_H
# include "test.h"

void test_eeprom_struct()
{
	u8 err = 0;

	struct {
		u8 foo;
		u8 bar;
	} in = { .foo = 5, .bar = 10 }, out;

	/* Added just to avoid error: unused variable 'in' 
	 * in READONLY test */
	(void) in;

# ifndef TEST_EEPROM_READONLY
	if (!eeprom_put(0, &in, sizeof(in))) {
		signal_led(3);
		err = 1;
		goto uart;
	}
# endif

	if (!eeprom_get(0, &out, sizeof(out))) {
		signal_led(3);
		err = 2;
		goto uart;
	}
	digitalWrite(13, out.foo == 1 && out.bar == 2);
uart:
	begin(9600);
	uprintf("Error Flag: %u\n", err);
	uprintf("Got: %u %u\n", out.foo, out.bar);
}
#endif
