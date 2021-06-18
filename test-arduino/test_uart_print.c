#include "test.h"

#pragma desc "Test UART printing. Please open a serial console and observe the output."

TEST(test_print)
{
	begin(9600);

	print_int(5);
	print_int_base(15, 2);
	print_int_base(15, 8);
	print_int_base(15, 10);
	print_int_base(15, 16);
	print_char('N');
	print_string("Ahoj");
}
