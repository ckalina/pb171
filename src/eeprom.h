// EEARH (3-0) and EEARL (7-0) – The EEPROM Address Register
// Bits 11:0 – EEAR8:0: EEPROM Address
//
// The EEPROM Address Registers – EEARH and EEARL specify the
// EEPROM address in the 4Kbytes EEPROM space.
//
// The EEPROM data bytes are addressed linearly between 0
// and 4096.
//
// The initial value of EEAR is undefined.
// A proper value must be written before the EEPROM may be
// accessed.
//

// EEDR – The EEPROM Data Register
// 7:0 – EEDR7:0: EEPROM Data
//
// For the EEPROM write operation, the EEDR Register contains
// the data to be written to the EEPROM in the address given
// by the EEAR Register.
//
// For the EEPROM read operation, the EEDR contains the data
// read out fromthe EEPROM at the address given by EEAR.

// EECR – The EEPROM Control Register
// Bits 5, 4 – EEPM1 and EEPM0: EEPROM Programming Mode Bits
// The EEPROM Programming mode bit setting defines which
// programming action that will be triggered when writing EEPE.
//
// It is possible to program data in one atomic operation
// (erase the old value and program the new value) or to split
// the Erase and Write operations in two different operations.
//
// While EEPE is set, any write to EEPMn will be ignored.
// During reset, the EEPMn bits will be reset to 0b00 unless
// the EEPROM is busy programming.
//
// EEPM1 EEPM0	Time	Comment
// 0     0	3.4ms	Erase and Write in one operation (atomic)
// 0     1	1.8ms	Erase
// 1     0      1.8ms	Write
//
// Bit 3 – EERIE: EEPROM Ready Interrupt Enable
//
// Bit 2 – EEMPE: EEPROM Master Programming Enable
// The EEMPE bit determines whether setting EEPE to one causes
// the EEPROM to be written.
//
// When EEMPE is set, setting EEPE within four clock cycles will
// write data to the EEPROM at the selected address.
//
// If EEMPE is zero, set-ting EEPE will have no effect.
//
// When EEMPE has been written to one by software, hardware
// clears the bit to zero after four clock cycles.
//
// Bit 1 – EEPE: EEPROM Programming Enable
// 
// The EEPROM Write Enable Signal EEPE is the write strobe to
// the EEPROM.
//
// When address and data are correctly set up, the EEPE bit
// must be written to one to write the value into the EEPROM.
//
// The EEMPE bit must be written to one before a logical one
// is written to EEPE, otherwise no EEPROM write takes place.
//
// The following procedure should be followed when writing the
// EEPROM (the order of steps 3 and 4 is not essential):
//
// 1. Wait until EEPE becomes zero.
// 2. Wait until SPMEN in SPMCSR becomes zero.
//
// 3. Write new EEPROM address to EEAR (optional).
// 4. Write new EEPROM data to EEDR (optional).
//
// 5. Write a logical one to the EEMPE bit while writing
//    a zero to EEPE in EECR.
//
// 6. Within four clock cycles after setting EEMPE, write
//    a logical one to EEPE.
// 
// The EEPROM can not be programmed during a CPU write to the
// Flash memory.
// The software must check that the Flash programming is
// completed before initiating a new EEPROM write.
// 
// Caution: An interrupt between step 5 and step 6 will make
//          the write cycle fail, since the EEPROM Master
//          WriteEnable will time-out.
//
//          If an interrupt routine accessing the EEPROM is
//          interrupting another EEPROM access, the EEAR or
//          EEDR Register will be modified, causing the
//          interrupted EEPROM access to fail.
//
//          It is recommended to have the Global Interrupt Flag
//          cleared during all the steps to avoid these problems.
//
//          When the write access time has elapsed, the EEPE bit
//          is cleared by hardware.
//
//          The user software can poll this bit and wait for a
//          zero before writing the next byte.
//
//          When EEPE has been set, the CPU is halted for two
//          cycles before the next instruction is executed.
//
// Bit 0 – EERE: EEPROM Read Enable
//
// The EEPROM Read Enable Signal EERE is the read strobe to the
// EEPROM. When the correct address is set up inthe EEAR Register,
// the EERE bit must be written to a logic one to trigger the
// EEPROM read.
//
// The EEPROM read access takes one instruction, and the
// requested data is available immediately.
//
// When the EEPROM is read, the CPU is halted for four cycles
// before the next instruction is executed.

#ifndef EEPROM_H
# define EEPROM_H
# include "common.h"

//#define eeprom __attribute__((section(".eeprom")))
/*
// 1 if EEPROM is ready for a new read/write operation, 0 if not.
// bit_is_clear (EECR, EEPE)
#define eeprom_is_ready (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)((0x1F) + 0x20)))))) & (1 << (1))))

//#define EECR (*(volatile uint8_t *)((0x1F) + 0x20))
//#define EEPE 1
//#define EEWE 1
*/

# define EEARH &(*(uv8 *)(0x42))
# define EEARL &(*(uv8 *)(0x41))
# define EEDR &(*(uv8 *)(0x40))
# define SPMCSR &(*(uv8 *)(0x57))
# define EECR &(*(uv8 *)(0x3F))

# define EEMPE 2
# define EEPE 1
# define EERE 0
# define SPMEN 0

u8 eeprom_read(u16 addr);
void eeprom_write(u16 addr, u8 data);
u8 eeprom_put(u16 addr, void *data, size_t size);
u8 eeprom_get(u16 addr, void *storage, size_t size);

static inline void eeprom_address(uv16 addr, uv8 *lo, uv8 *hi)
{
	*lo = (uv8) addr;
	*hi = (addr >> 8) & 15;
}

static inline u8 eeprom_read_data(void)
{
	return *EEDR;
}

static inline void eeprom_write_data(u8 data)
{
	*EEDR = data;
}

static inline void eeprom_write_addr(u16 addr)
{
	uv8 lo, hi;
	eeprom_address(addr, &lo, &hi);
	*EEARH = hi;
	*EEARL = lo;
}


#endif
