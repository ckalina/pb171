#include "test.h"

#pragma desc "Test UART RX. Please open a serial console and provide the input."

TEST(test_uart_rx)
{
	begin(9600);
	u8 rx;

watch_rx:
	if (buf_empty(&serial.rx))
		goto wait;
	if (!read(&rx))
		goto wait;

	if (availableForWrite())
		uprintf("Received: (%d remaining) %c\n", available(), rx);

	for (int i = 0; i < 3; i++) {
		if (!peek(&rx))
			break;
		uprintf("Peek: %c\n", rx);
	}
wait:
	delay(1000);
	goto watch_rx;
}
