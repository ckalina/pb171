#include "test.h"
#include "bit.h"

int main(void)
{
	TEST_ASSERT(__LINE__, bit(0) == 1);
	TEST_ASSERT(__LINE__, bit(1) == 2);

	TEST_ASSERT(__LINE__, bitClear(7, 0) == 6);
	TEST_ASSERT(__LINE__, bitClear(7, 1) == 5);
	TEST_ASSERT(__LINE__, bitClear(7, 2) == 3);
	TEST_ASSERT(__LINE__, bitClear(7, 3) == 7);

	for (int i = 0, exp = 1; i < 8; ++i, exp *= 2) {
		for (int j = 0; j < 7; ++j) {
			TEST_ASSERT(__LINE__, bitSet(0, i) == exp | j);
			TEST_ASSERT(__LINE__, bitWrite(0, i, 1) == bitSet(0, i));
			TEST_ASSERT(__LINE__, bitWrite(0, i, 0) == bitClear(0, i));
		}
	}

	for (int lower = 0; lower < 255; ++lower)
	{
		for (int upper = 0; upper < 255; ++upper)
		{
			int val = lower | (upper << 8);
			TEST_ASSERT(__LINE__, lowByte(val) == lower);
			TEST_ASSERT(__LINE__, highByte(val) == upper);
		}
	}

	return 0;
}
