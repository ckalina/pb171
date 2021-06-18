#include "test.h"

#pragma desc "Test UART UDRE. Please open a serial console and observe the output. DEFINE=-DTEST_UART_UDRE"
#pragma define TEST_UART_UDRE

TEST(test_uart_tx_udre)
{
	begin(9600);
	*serial.UDR = 'A';
	write('h');
	write('o');
	write('j');
}
