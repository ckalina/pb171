#include "test.h"

#pragma desc "Test UART readbytes. Please open a serial console and provide the input."

TEST(test_readbytes)
{
	u8 buf[10];

	begin(9600);

	uprintf("Please enter input:\n");
	if (!readBytes(buf, 10)) {
		uprintf("Timed out.\n");
		return;
	}

	uprintf("Bytes: %s\n", buf);
}
