HEX := $(NAME).hex
OUT := $(NAME).out
MAP := $(NAME).map

LIBSOURCES := $(wildcard ../src/*.c)
LIBOBJECTS := $(patsubst %.c,%.o,$(LIBSOURCES))

SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
TESTS := $(patsubst %.c,%.test,$(SOURCES))

HEXS := $(patsubst %.c,%.hex,$(SOURCES))
OUTS := $(patsubst %.c,%.out,$(SOURCES))
MAPS := $(patsubst %.c,%.map,$(SOURCES))

MCU := atmega2560
MCU_AVRDUDE := m2560
PARTNO := stk500v2
DEVICE ?= $(wildcard /dev/ttyACM*)
UART_DEV ?= $(wildcard /dev/ttyUSB*)
BAUDRATE := 115200
AVRDUDE_FLAGS := -F -V -D -v

CC := avr-gcc
OBJCOPY := avr-objcopy
SIZE := avr-size -A

AFLAGS := -mmcu=$(MCU)
CFLAGS := -Wall -Wextra -pedantic -Werror -Wno-unknown-pragmas -mmcu=$(MCU) -std=c99 -g -Os -DF_CPU=16000000UL  -I../src/ $(DEFINE)

help:
	@echo
	@echo Available tests:
	@echo $(TESTS) |  tr ' ' '\n' | sort | grep -v main.test | xargs -ITEST sh -c ' \
		printf "    %-30s\t" "TEST"; grep -Po "#pragma desc \"\K.*(?=\")" $$(echo TEST | sed 's/.test/.c/')\
	'
	@echo
	@echo You may run a test TEST_NAME.test using: make TEST_NAME.test.
	@echo
	@echo Using $(DEVICE) for device flashing. If you wish to select a different
	@echo device, please use DEVICE=/dev/something make YOUR_TARGET.
	@echo
	@echo Using $(UART_DEV) for device IO via UART. As above, use UART_DEV for override.
	@echo
	@echo For tests that use UART, it suffices to open up two terminal windows in this directory,
	@echo and issue
	@echo "   make uart"
	@echo before running the test in the other by
	@echo "   make TEST_NAME.test"
	@echo

uart:
	@echo Press CTRL-A and type :quit to exit screen.
	@echo Note: your screen might require setting up to use UNIX line endings only.
	@echo "      inclr onclr settings should fix that (or just change LF for CRLF in the source)."
	@echo Press ENTER to continue.
	@read
	@test -w $(UART_DEV) || { \
		echo "ERROR: You do not have the permissions to use $(UART_DEV)."; \
		exit 1; \
	}
	@screen /dev/ttyUSB* 9600



all: $(TESTS)

clean:
	rm -f $(HEXS) $(OUTS) $(MAPS) $(OBJECTS) $(LIBOBJECTS)

%.test: %.flash
	@echo -n

%.flash: %.hex
	test -w $(DEVICE) || { \
		echo "ERROR: You do not have the permissions to use $(DEVICE)."; \
		exit 1; \
	}
	avrdude $(AVRDUDE_FLAGS) -c $(PARTNO) -p $(MCU_AVRDUDE) -P $(DEVICE) -b $(BAUDRATE) -U flash:w:$^

%.hex: %.out
	$(OBJCOPY) -R .eeprom -O ihex $< $@

%.out: main.o %.o $(LIBOBJECTS)
	$(CC) $(CFLAGS) -o $@ -Wl,-Map,$(MAP) $^
	@echo
	@$(SIZE) $@
	@echo

%.o: %.S
	avr-as $(AFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.pp: %.c
	$(CC) $(CFLAGS) -E -o $@ $<

%.ppo: %.c
	$(CC) $(CFLAGS) -E $<
