#include "test.h"

#pragma desc "Test delay microseconds by flashing LED."

TEST(test_delayMicroseconds)
{
	pinMode(13, 1);

	int c = 0;

	while (1) {
		c = (c + 1) % 2;

		for(int i = 0; i < 1024; i++)
			delayMicroseconds(1024);

		digitalWrite(13, c);
	}
}
