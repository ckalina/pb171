#include "test.h"

void test(void);

int main(void)
{
	init();

	pinMode(13, 1);
	digitalWrite(13, 1);
	delay(1024);
	digitalWrite(13, 0);
	delay(1024);

	test();
}
