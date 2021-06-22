#include "test.h"

#pragma desc "Test UART integer parser. Please open a serial console and provide an input."

TEST(test_uart_int)
{
	begin(9600);
	u16 rx;

	uprintf("Please enter an integer:\n");
	if (!parseInt(&rx)) {
		uprintf("Timed out.\n");
		return;
	}

	uprintf("Received int: %d\n", rx);
}
