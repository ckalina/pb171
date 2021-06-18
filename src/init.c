#include "common.h"

void init(void)
{
	sei_mem();

	#define INDIRECT_OR(addr, val) \
		*(uv8 *)((u16) &(*(uv8 *)(addr))) |= val;

	// UCSR0B  USART0 Control and Status Register B 
	*(uv8 *)(0XC1) = 0;

	/** TIMER 0 **/

	// TCCR0A – Timer/Counter Control Register A
	// Bits 1:0 – WGM01:0: Waveform Generation Mode
	// Enable fast PWM.
	INDIRECT_OR(0x24 + 0x20, 1 << 1 | 1 << 0);

	// TCCR0B – Timer/Counter Control Register B
	// Bits 2:0 – CS02:0: Clock Select
	// clk I/O/64 (prescaler)
	INDIRECT_OR(0x25 + 0x20, 1 << 1 | 1 << 0);

	// TIMSK0 – Timer/Counter Interrupt Mask Register
	// When the TOIE0 bit is written to one, and the I-bit in the Status
	// Register is set, the Timer/Counter0 Overflow interrupt is enabled.
	// The corresponding interrupt is executed if an overflow in Timer/
	// Counter0 occurs, that is, when the TOV0 bit is set in the Timer/
	// Counter 0 Interrupt Flag Register – TIFR0.
	INDIRECT_OR(0x6e, 1 << 0);

	/** TIMER 1 **/
	
	// TCCR1A – Timer/Counter 1 Control Register A
	*(uv8 *)(0x81) = 0;

	// TCCR1A – Timer/Counter 1 Control Register A
	INDIRECT_OR(0x80, 1 << 0);

	// CCR1B – Timer/Counter 1 Control Register B
	INDIRECT_OR(0x81, 1 << 1 | 1 << 0);

	/** TIMER 2 **/

	// TCCR2A –Timer/Counter Control Register A
	INDIRECT_OR(0xb0, 1 << 0);

	// TCCR2B – Timer/Counter Control Register B
	INDIRECT_OR(0xb1, 1 << 2);

	/** TIMER 3 **/

	// TCCR3A – Timer/Counter 3 Control Register A
	INDIRECT_OR(0x90, 1 << 0);

	// CCR3B – Timer/Counter 3 Control Register B
	INDIRECT_OR(0x91, 1 << 1 | 1 << 0);

	/** TIMER 4 **/

	// TCCR4A – Timer/Counter 4 Control Register A
	INDIRECT_OR(0xa0, 1 << 0);

	// TCCR4B – Timer/Counter 4 Control Register B
	INDIRECT_OR(0xa1, 1 << 1 | 1 << 0);

	/** TIMER 5 **/

	// TCCR5A – Timer/Counter 5 Control Register A
	INDIRECT_OR(0x120, 1 << 0);

	// ADCSRA – ADC Control and Status Register A
	INDIRECT_OR(0x7a, 1 << 2 | 1 << 1 | 1 << 0 | 1 << 7);

	// TCCR5B – Timer/Counter 5 Control Register B
	INDIRECT_OR(0x121, 1 << 1 | 1 << 0);
}
