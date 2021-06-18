#include "pins.h"
#include "analog.h"
#include "digital.h"

// TODO use the bit operation functions
//
u16 analogRead(u8 pin)
{
        // the MUX5 bit of ADCSRB selects whether we're reading from channels
        // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	(*(uv8 *)(0x7B)) = ((*(uv8 *)(0x7B)) & ~(1 << 3)) | (((pin >> 3) & 0x01) << 3);

        // set the analog reference (high two bits of ADMUX) and select the
        // channel (low 4 bits).
	// this also sets ADLAR (left-adjust result) to 0 (the default).
	//
	// DEFAULT: the default analog reference of 5 volts (on 5V Arduino boards)
	(*(uv8 *)(0x7C)) = (1 << 6) | (pin & 0x07);
	
	// ADCSRA, ADSC
	((*(uv8 *)(((u16) &((*(uv8 *)(0x7A)))))) |= (1 << (6)));

	// while ADSC of ADCSRA is set
	while (((*(uv8 *)(((u16) &((*(uv8 *)(0x7A)))))) & (1 << (6))));

	// must read ADCL first -> locks ADCL and ADCH until ADCH is read
	// ADCL second read would cause results to be discarded
	// low = (*(uv8 *)(0x78));
	// high = (*(uv8 *)(0x79));
	return (*(uv8 *)(0x78)) | (*(uv8 *)(0x79)) << 8;
}

void analogWrite(u8 pin, int val)
{
	u16 mem;
	u8 timer;

	pinMode(pin, 0x1);

	if (val == 0) {
		digitalWrite(pin, 0x0);
		return;
	}

	if (val == 255) {
		digitalWrite(pin, 0x1);
		return;
	}

	mem = (u16)(digital_pin_to_timer_PGM + pin);
	lpm_z(timer, mem);

	#define CONN_PWM_TO_PIN_ON_TIMER_CHANNEL(addr, shift) \
			*(uv8 *)((u16) &(*((uv8 *)addr))) |= 1 << shift
	#define CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_A(addr) \
			CONN_PWM_TO_PIN_ON_TIMER_CHANNEL(addr, 7)
	#define CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_B(addr) \
			CONN_PWM_TO_PIN_ON_TIMER_CHANNEL(addr, 5)
	#define CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_C(addr) \
			CONN_PWM_TO_PIN_ON_TIMER_CHANNEL(addr, 3)
	#define SET_PWM_DUTY(addr) \
		*(uv8 *)(addr) = val

	// TIMER0
	switch(timer) {
		// TIMER0
		// 0x24 + 0x20
		// pwm channel A 0x27 + 0x20
		// pwm channel B 0x28 + 0x20
		// these values are referred to as TCCR0A, COM0A1, and OCR0A
		case 1:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_A(0x24 + 0x20);
			SET_PWM_DUTY(0x27 + 0x20);
			break;
		case 2:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_B(0x24 + 0x20);
			SET_PWM_DUTY(0x28 + 0x20);
			break;

		// TIMER1
		// 0x80
		// pwm channel A 0x88
		// pwm channel B 0x8a
		// pwm channel C 0x8c
		case 3:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_A(0x80);
			SET_PWM_DUTY(0x88);
			break;
		case 4:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_B(0x80);
			SET_PWM_DUTY(0x8a);
			break;
		case 5:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_C(0x80);
			SET_PWM_DUTY(0x8c);
			break;

		// TIMER2
		// 0xb0
		// pwm channel A 0xb3
		// pwm channel B 0xb4
		case 7:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_A(0xb0);
			SET_PWM_DUTY(0xb3);
			break;
		case 8:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_B(0xb0);
			SET_PWM_DUTY(0xb4);
			break;

		// TIMER3
		// 0x90
		// pwm channel A 0x98
		// pwm channel B 0x9a
		// pwm channel C 0x9c
		case 9:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_A(0x90);
			SET_PWM_DUTY(0x98);
			break;
		case 10:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_B(0x90);
			SET_PWM_DUTY(0x9a);
			break;
		case 11:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_C(0x90);
			SET_PWM_DUTY(0x9c);
			break;

		// TIMER4
		// 0xa0
		// pwm channel A 0xa8
		// pwm channel B 0xaa
		// pwm channel C 0xac
		case 12:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_A(0xa0);
			*(uv8 *)(((u16) &((*(uv8 *)(0xa0))))) &= ~ (1 << (6));
			SET_PWM_DUTY(0xa8);
			break;
		case 13:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_B(0xa0);
			SET_PWM_DUTY(0xaa);
			break;
		case 14:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_C(0xa0);
			SET_PWM_DUTY(0xac);
			break;


		// TIMER 5
		// 0x120
		// pwm channel A 0x128
		// pwm channel B 0x12a
		// pwm channel C 0x12c
		case 16:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_A(0x120);
			SET_PWM_DUTY(0x128);
			break;
		case 17:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_B(0x120);
			SET_PWM_DUTY(0x12a);
			break;
		case 18:
			CONN_PWM_TO_PIN_ON_TIMER__CHANNEL_C(0x120);
			SET_PWM_DUTY(0x12c);
			break;

		// NOT ON TIMER
		case 0:
		default:
			if (val < 128) {
				digitalWrite(pin, 0x0);
				return;
			}
			digitalWrite(pin, 0x1);
			break;
	}
}
