SOURCES := $(wildcard *.c)
HEADERS := $(wildcard *.h)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
TESTS := $(patsubst %.c,%.test,$(SOURCES))

CC := gcc
CFLAGS := -std=c99 -g -Os -I../src/

.PHONY: help all clean

help:
	@echo
	@echo Available tests:
	@echo $(TESTS) | tr ' ' '\n' | sed 's/^/\t/g'
	@echo
	@echo make all		run every test
	@echo make TEST_NAME	run TEST_NAME test
	@echo

all: $(TESTS)
	@echo $(TESTS) | tr ' ' '\n' | xargs -I% sh -c ' \
		echo -n "Running test % ... "; \
		./% && echo OK || echo FAILED'

clean:
	rm -f $(OBJECTS) $(TESTS)

%.test: %.c
	@FILES=($$(grep -Po '#pragma dependency \K.*' $^ | sed 's#^#../src/#') $^); \
	$(CC) $(CFLAGS) -o $@ $${FILES[@]}
	@if [ "$(MAKECMDGOALS)" != "all" ]; then \
		echo -n "Running test $@ ... "; \
		./$@ && echo OK || echo FAILED;\
	fi

