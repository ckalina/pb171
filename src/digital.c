#include "digital.h"

char digitalRead(u8 pin)
{
	u16 mem, res;
	u8 bit, port;

	mem = (u16)(digital_pin_to_bit_mask_PGM + pin);
	lpm_z(bit, mem);

	mem = ((u16)(digital_pin_to_port_PGM + pin));
	lpm_z(port, mem);

	// not a pin
	if (port == 0)
		return 0x0;

	mem = (u16)(port_to_input_PGM + port);
	lpm_zpz(res, mem);

	return !!(*(volatile u8 *)(res) & bit);
}

void digitalWrite(u8 pin, u8 val)
{
	u16 mem;
	u8 bit, port, oldSREG;
	volatile u8 *out;

	mem = (u16)(digital_pin_to_bit_mask_PGM + pin);
	lpm_z(bit, mem);

	mem = ((u16)(digital_pin_to_port_PGM + pin));
	lpm_z(port, mem);

	// not a pin
	if (port == 0)
		return;

	mem = (u16)(port_to_output_PGM + port);
	lpm_zpz(out, mem);

	oldSREG = SREG_READ();
	cli_mem();
	*out = (val == 0) ? *out & ~bit : *out | bit;
	SREG_WRITE(oldSREG);
}
