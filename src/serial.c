#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "common.h"
#include "serial.h"
#include "digital.h"
#include "pins.h"
#include "buffer.h"
#include "time.h"

void _IRQ_UART_TX(void) __attribute__ ((signal,used, externally_visible));
void _IRQ_UART_TX(void)
{
}

void _IRQ_UART_RX(void) __attribute__ ((signal,used, externally_visible));
void _IRQ_UART_RX(void)
{
	if (!buf_push(&serial.rx, *serial.UDR)) {
		serial.rx.overflow |= 1;
		return;
	}
	serial.rx.flag |= 1;
}

void _IRQ_UART_UDRE(void) __attribute__ ((signal,used, externally_visible));
void _IRQ_UART_UDRE(void)
{
	uv8 tx;
	if (!buf_pop(&serial.tx, &tx))
		return;
	*serial.UDR = tx;
}

struct serial serial = {
	.UBRRH = &(*(uv8 *)(_UBRRH)),
	.UBRRL = &(*(uv8 *)(_UBRRL)),
	.UCSRA = &(*(uv8 *)(_UCSRA)),
	.UCSRB = &(*(uv8 *)(_UCSRB)),
	.UCSRC = &(*(uv8 *)(_UCSRC)),
	.UDR   = &(*(uv8 *)(_UDR))
};

void begin(unsigned long baud)
{
	buf_clean(&serial.rx);
	buf_clean(&serial.tx);

#if defined(SERIAL0)
#elif defined(SERIAL1)
	pinMode(18, 1);
	digitalWrite(18, 1);
	pinMode(19, 0);
	digitalWrite(19, 1);
#endif

	/* Set baud rate */
	u16 baud_setting = (16000000U/8)/(baud-1);
	*serial.UBRRH = (u8) baud_setting >> 8;
	*serial.UBRRL = (u8) baud_setting;

	*serial.UCSRA = (uv8) 1 << 1;

	// UCSRnC – USART Control and Status Register n C
	// UCSZ0{0,1} and clear UCSZ02 to enable 8bit
	*serial.UCSRC = 1 << 1 | 1 << 2;

	// UCSRnB – USART Control and Status Register n B
	// 7 RXCIEn: RX Complete Interrupt Enable n
	// 4 RXENn: Receiver Enable n
	// 3 TXENn: Transmitter Enable n
	// 5 UDRIEn: USART Data Register Empty Interrupt Enable n
	*serial.UCSRB |= (1 << 4) | (1 << 3) | (1 << 7) | 1 << 5;

	sei_mem();
}


void end(void)
{
	flush();

	// rolling back UCSRnB from begin
	*(uv8 *)(((u16) &(serial.UCSRB))) &=
		~(1 << 4 | 1 << 3 | 1 << 7 | 1 << 5);

	buf_clean(&serial.rx);
	buf_clean(&serial.tx);
}


int available(void)
{
	return mod(serial.rx.in - serial.rx.out, 64);
}

int peek(u8 *data)
{
	return buf_peek(&serial.rx, data);
}

int read(u8 *data)
{
	return buf_pop(&serial.rx, data);
}

int readBytes(u8 *data, u16 sz)
{
	u8 rx;
	u8 pos = 0;
	i16 timeout = UART_TIMEOUT_MS;

	while (timeout > 0 && pos < sz)
	{
		if (!read(&rx)) {
			delay(UART_DELAY_MS);
			timeout -= UART_DELAY_MS;
			continue;
		}
		data[pos++] = rx;
	}

	return pos == sz;
}

int availableForWrite(void)
{
	return buf_can_push(&serial.tx);
}

void flush(void)
{
	while (!buf_empty(&serial.tx)) {
		while (!((*serial.UCSRA) & (1 << 5))){}
		_IRQ_UART_UDRE();
	}
}

size_t write(u8 c)
{
/* this ifndef is here just for testing purposes. */
#ifndef TEST_UART_UDRE
	/* if the buffer is empty, and the tx buffer is ready to receive
	 * we can just push it through */
	if (buf_empty(&serial.tx) && (*serial.UCSRA) & (1 << 5)) {
		*serial.UDR = c;
		return 1;
	}
#endif

	/* buffer full, let's make this nonblocking */
	if (!buf_sz(&serial.tx))
		return 0;

	return buf_push(&serial.tx, c);
}

int uprintf(char *fmt, ...)
{
	int n;
	char buf[512];
	va_list ap;

	va_start(ap, fmt);
	n = vsnprintf(buf, 512, fmt, ap);
	va_end(ap);

	if (n < 0)
		return n;

	for (int i = 0; i < 512 && buf[i] != 0; i++)
		write(buf[i]);
	write('\n');

	return n;
}


int uprintf_debug(char *fmt, ...)
#ifndef DEBUG
{
	(void)fmt;
	return 0;
}
#else
__attribute__((alias("uprintf")));
#endif

int parseInt(u16 *data)
{
	u8 rx;
	u8 pos = 0;
	i16 timeout = UART_TIMEOUT_MS;

	// max int we can store is 65535
	char buf[5];

	while (timeout > 0)
	{
		uprintf_debug("Waiting for an input.");

		if (!read(&rx)) {
			uprintf_debug("Did not receive an input.\n");
			goto wait;
		}

		uprintf_debug("Received an input.\n");

		if (!isdigit(rx)) {
			uprintf_debug("Input is not a digit.\n");
			if (pos > 0) {
				uprintf_debug("Already got a digit,"
					      "breaking.\n");
				break;
			}
			goto wait;
		}
		
		uprintf_debug("Digit: %c\n.", rx);

		buf[pos++] = rx;
		continue;
wait:
		delay(UART_DELAY_MS);
		timeout -= UART_DELAY_MS;
	}

	if (pos == 0) {
		uprintf_debug("Timed out.\n");
		return 0;
	}

	*data = atoi(buf);
	uprintf_debug("Digit: %d.\n", *data);
	return 1;
}
