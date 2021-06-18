#include "test.h"

#pragma desc "Test delay with a flashing LED."

TEST(test_delay)
{
	pinMode(13, 1);

	int c = 0;

	while (1) {
		c = (c + 1) % 2;
		delay(1024);
		digitalWrite(13, c);
	}
}
