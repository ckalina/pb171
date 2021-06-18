#define TEST_EEPROM_READONLY

#include "test.h"
#include "test_eeprom_struct.h"

#pragma desc "Attempt to read a structure from an EEPROM. Please store it in EEPROM first using a different test."

TEST(test_eeprom_struct_ro)
{
	test_eeprom_struct();
}
