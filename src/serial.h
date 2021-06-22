#include "common.h"
#include "buffer.h"

#ifndef UART_TIMEOUT_MS
# define UART_TIMEOUT_MS 10000U
#endif

#ifndef UART_DELAY_MS
# define UART_DELAY_MS 1000U
#endif

#define SERIAL1

#if defined(SERIAL0)
#error "Not tested."
#define _UBRRH 0xC5
#define _UBRRL 0xC4
#define _UCSRA 0xC0
#define _UCSRB 0XC1
#define _UCSRC 0xC2
#define _UDR   0XC6
#define _IRQ_UART_RX __vector_25
#define _IRQ_UART_UDRE __vector_26
#elif defined(SERIAL1)
#define _UBRRH 0xCD
#define _UBRRL 0xCC
#define _UCSRA 0xC8
#define _UCSRB 0XC9
#define _UCSRC 0xCA
#define _UDR   0XCE
#define _IRQ_UART_RX __vector_36
#define _IRQ_UART_UDRE __vector_37
#define _IRQ_UART_TX __vector_38
#endif

__attribute__((packed))
struct serial {
	uv8 * UBRRH;
	uv8 * UBRRL;
	uv8 * UCSRA;
	uv8 * UCSRB;
	uv8 * UCSRC;
	uv8 * UDR;

	struct buf rx;
	struct buf tx;
};

/**
 * IMPORTANT
 *
 * At the moment only single serial connection is used.
 * There is nothing (except the defines above) stopping us
 * from using different/multiple serials, I think/wish :).
 *
 * We would only need to make each serial function take
 * an index to which serial the given fn corresponds to.
 */

extern struct serial serial;

void begin(unsigned long baud);
void end(void);
int available(void);
int peek(u8 *data);
int read(u8 *data);
int availableForWrite(void);
void flush(void);
size_t write(u8);
int uprintf(char *fmt, ...);
int uprintf_debug(char *fmt, ...);
void empty_irq(void);
int parseInt(u16 *data);
int readBytes(u8 *data, u16 sz);

static inline int print_int(int i)
{
	return uprintf("%d", i);
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                                  \
  (byte & 0x80 ? '1' : '0'),                                                  \
  (byte & 0x40 ? '1' : '0'),                                                  \
  (byte & 0x20 ? '1' : '0'),                                                  \
  (byte & 0x10 ? '1' : '0'),                                                  \
  (byte & 0x08 ? '1' : '0'),                                                  \
  (byte & 0x04 ? '1' : '0'),                                                  \
  (byte & 0x02 ? '1' : '0'),                                                  \
  (byte & 0x01 ? '1' : '0') 

static inline int print_int_base(int i, int base)
{
	switch (base) {
	case 2:
		return uprintf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(i));
	case 8:
		return uprintf("%o", i);
	case 10:
		return uprintf("%d", i);
	case 16:
		return uprintf("%x", i);
	}
	return 0;
}

static inline int print_char(char c)
{
	return uprintf("%c", c);
}

static inline int print_string(char * str)
{
	return uprintf("%s", str);
}
