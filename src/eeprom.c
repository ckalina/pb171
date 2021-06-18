#include "common.h"
#include "eeprom.h"

static void _eeprom_write(u16 addr, u8 data)
{
	/* Wait for completion of previous write */
	while(*EECR & (1<<EEPE));

	// Wait until SPMEN in SPMCSR is zero
	//while(*SPMCSR & 1 < SPMEN);

	/* Set up address and Data Registers */
	eeprom_write_addr(addr);
	*EEDR = data;

	/* Write logical one to EEMPE */
	*EECR |= (1<<EEMPE);

	// AT MOST 4 cycles

	/* Start eeprom write by setting EEPE */
	*EECR |= (1<<EEPE);

	// When the write access time has elapsed, the EEPE bit
	// is cleared by hardware.
	while(*EECR & (1<<EEPE));
}

static u8 _eeprom_read(u16 addr)
{
	u8 data;

	cli();

	/* Wait for completion of previous write */
	while(*EECR & (1<<EEPE));

	/* Set up address register */
	eeprom_write_addr(addr);

	/* Start eeprom read by writing EERE */
	*EECR |= (1<<EERE);

	data = *EEDR;

	return data;
}

void eeprom_write(u16 addr, u8 data)
{
	cli();
	_eeprom_write(addr, data);
	sei();
}

u8 eeprom_read(u16 addr)
{
	u8 data;
	cli();
	data = _eeprom_read(addr);
	sei();
	return data;
}

u8 eeprom_update(u16 addr, u8 data)
{
	if (eeprom_read(addr) == data)
		return 1;
	eeprom_write(addr, data);
	return 0;
}

u8 eeprom_put(u16 addr, void *data, size_t size)
{
	u8 *bytedata = (u8*) data;

	// Check if there's enough space
	if (addr + 8*size > 4095)
		return 0;
	
	cli();
	for (size_t i = 0; i < size; ++i)
		_eeprom_write(addr + i, bytedata[i]);
	sei();

	return 1;
}

u8 eeprom_get(u16 addr, void *storage, size_t size)
{
	u8 *bytedata = (u8*)storage;

	cli();
	for (size_t i = 0; i < size; ++i)
		bytedata[i] = _eeprom_read(addr + i);

	sei();

	return 1;
}
