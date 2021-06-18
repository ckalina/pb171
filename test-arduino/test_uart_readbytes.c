#include "test.h"

#pragma desc "Test UART readbytes. Please open a serial console and provide the input."

TEST(test_readbytes)
{
	begin(9600);

	char buf[10];
	if (!readBytes(buf, 10)) {
		uprintf("Timed out.\n");
		return;
	}

	uprintf("Bytes: %s\n", buf);
}
