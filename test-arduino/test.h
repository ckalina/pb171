#ifndef TEST_H
# define TEST_H

# include <stdint.h>

# include "test.h"

# include "pins.h"
# include "digital.h"
# include "time.h"
# include "serial.h"
# include "eeprom.h"
# include "init.h"

static inline void signal_led(int t)
{
	for (int i = 0; i < t; ++i) {
		digitalWrite(13, 1);
		delay(1024);
		digitalWrite(13, 0);
		delay(1024);
	}
}

# define TEST(name) \
	void name(void) __attribute__((alias("test"))); \
	void test(void)
#endif
