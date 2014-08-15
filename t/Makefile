CFLAGS += -Wall -Wextra -g -I..

TESTS = 01-sanity.t 10-basic.t

01-sanity.t: CFLAGS += -std=c99 -pedantic -Werror

%.t: %.c ../tap.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

check: tests
	PATH=.:$$PATH prove $(TESTS)

tests: $(TESTS)

all: tests

clean:
	$(RM) $(TESTS)
	$(RM) tmp_*

.PHONY: all clean check tests
