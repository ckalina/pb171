#include "pins.h"
#include "progmem.h"

void pinMode(u8 pin, u8 mode)
{
	u16 mem;
	u8 bit, port, oldSREG;
	volatile u8 *reg, *out;

	mem = (u16)(digital_pin_to_bit_mask_PGM + pin);
	lpm_z(bit, mem);

	mem = ((u16)(digital_pin_to_port_PGM + pin));
	lpm_z(port, mem);

	if (port == 0)
		return;

	mem = (u16)(port_to_mode_PGM + port);
	lpm_zpz(reg, mem);

	mem = (u16)(port_to_output_PGM + port);
	lpm_zpz(out, mem);

	oldSREG = SREG_READ();
	cli_mem();
	switch (mode) {
	case 0x0:
		*reg &= ~bit;
		*out &= ~bit;
		break;
	case 0x2:
		*reg &= ~bit;
		*out |= bit;
		break;
	default:
		*reg |= bit;
		break;
	}
	SREG_WRITE(oldSREG);
}
