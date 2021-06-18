#include "common.h"
#include "time.h"

/*
 * IMPORTANT.
 *
 * WE ASSUME THAT THE CPU RUNS ON 16 MHz.
 * ONLY THIS FREQUENCY IS IMPLEMENTED AND IT IS
 * IMPLICITLY ASSUMED.
 *
 * RATIONALE: IT IS MUCH EASIER TO ARGUE ABOUT THE
 *            DURATION OF INSTRUCTIONS/OPERATATIONS
 *            WITH A FIXED FREQUENCY.
 */

/** TODO not sure if I can reasonably use the uint32_t for these
 * to check
 */
volatile unsigned long timer0_ms = 0;
volatile unsigned long timer0_overflows = 0;
static u16 timer0_skew = 0;

// Timer/Counter0 Overflow
void __vector_23(void) __attribute__ ((signal, used, externally_visible));
void __vector_23(void)
{
	u16 overflow;
	unsigned long ms;

	overflow = timer0_skew;
	ms = timer0_ms + 1;

	/*
	 * The CPU frequency is, 16MHz. This makes it 16 clocks per microsecond.
	 * 
	 * Timer/Counter0 is 8 bit: 256 values, making overflow period 16us.
	 * init sets prescaler/64, making 1024us overflow period, or roughly
	 * each ms.
	 * 
	 * This makes a counter++ each 4us. Each second will then incurr an
	 * error of 6.
	 *
	 * 1024us ... overflow
	 * so this function is called each 1024us
	 *
	 */

	if (overflow >= 1000) {
		// we might lose some resolution here, but only a bit
		overflow = 0;
		ms++;
	} else {
		overflow += 24;
	}

	timer0_skew = overflow;
	timer0_ms = ms;

	if (ms < timer0_ms)
		timer0_overflows++;
}

void delay(unsigned long ms)
{
	volatile unsigned long start, of;

	start = timer0_ms;
	of = timer0_overflows;

	while (1) {
		if (timer0_overflows > of) {
			start = timer0_ms;
			of = timer0_overflows;
			ms -= ((unsigned long)-1) - start;
		}
		if (*(&timer0_ms) - start > ms)
			break;
	}
}

void delayMicroseconds(u16 us)
{
	/* the following comments are measurements are
	 * taken from arduino docs;
	 * i dont have an oscilloscope or a similar device, so
	 * hopefully these measurements are ok
	 */

	/* fn call overhead is 14 cycles which is about 1us */

	/* t0 = now() */
	if (us <= 1) {
		/* t - t0 = 4 cycles */
		return;
	} else {
		/* t - t0 = 3 cycles */
	}

	/* we use 4 cycle busy wait, i.e., 1/4ms, so mul by 4 */
	us = us << 2;

	/* 19 cycles were spent above, remove them this in itself
	 * takes 2 cycles */
	us -= 5;

	__asm__ __volatile__ (
		/* 2 cycles */
		"1: sbiw %0,1" "\n\t"

		/* 2 cycles */
		"brne 1b"

		: "=w" (us)
		: "0" (us)
	);

	/* return from fn takes 4 cycles */
}


unsigned long millis()
{
	u8 oldSREG;
	unsigned long m;

	oldSREG = SREG_READ();
	cli();
	m = timer0_ms;
	SREG_WRITE(oldSREG);

	return m;
}

unsigned long micros() {
	u8 oldSREG;
	u8 counter;
	unsigned long overflow;

	cli_mem();
	oldSREG = SREG_READ();
	overflow = timer0_overflows;

	// TCNT0
	counter = (*(volatile uint8_t *)((0x26) + 0x20));

	// Timer/Counter 0 Interrupt Flag Register overflow flag
	if (((*(volatile uint8_t *)(0x15+0x20)) & 1) && (counter < 255))
		overflow++;
	SREG_WRITE(oldSREG);

	return ((overflow << 8) + counter) * 4;
}
