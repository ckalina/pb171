#define TEST_EEPROM_READONLY

#pragma desc "Attempt to read EEPROM-stored data. Please write it first using a different test."

#include "test.h"
#include "test_eeprom.h"

TEST(test_eeprom_ro)
{
	test_eeprom();
}
