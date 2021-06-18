#include "test.h"

#pragma desc "Test timer interrupt. LED will blink in a fixed interval."

TEST(test_interrupt_timing)
{
	pinMode(13, 1);

	int c = 0;

	unsigned long start = timer0_ms;
	int of = timer0_overflows;
	while (1) {
		if (timer0_overflows > of) {
			start = timer0_ms;
			of = timer0_overflows;
		}
		if (timer0_ms - start > 1024) {
			c = (c + 1) % 2;
			start = timer0_ms;
		}

		digitalWrite(13, c);
	}
}
