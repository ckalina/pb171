#include "test.h"
#include "buffer.h"

#pragma dependency buffer.c

int main(void)
{
	struct buf buffer;

	TEST_ASSERT(__LINE__, buf_empty(&buffer));
	TEST_ASSERT(__LINE__, buf_can_push(&buffer));
	TEST_ASSERT(__LINE__, !buf_can_pop(&buffer));

	TEST_ASSERT(__LINE__, buf_push(&buffer, 1));
	TEST_ASSERT(__LINE__, buf_can_pop(&buffer));

	for (int i = 0; i < 62; ++i)
		TEST_ASSERT(__LINE__, buf_push(&buffer, 1));
	TEST_ASSERT(__LINE__, !buf_push(&buffer, 1));

	u8 data;
	TEST_ASSERT(__LINE__, buf_pop(&buffer, &data));
	TEST_ASSERT(__LINE__, data == 1);
	TEST_ASSERT(__LINE__, buf_push(&buffer, 1));

	for (int i = 0; i < 62; ++i) {
		TEST_ASSERT(__LINE__, buf_pop(&buffer, &data));
		TEST_ASSERT(__LINE__, data == 1);
	}
}
