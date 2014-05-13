CFLAGS += -Wall -g -I..
TESTS = 10-basic.t

%.t: %.c ../tap.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

check: $(TESTS)
	PATH=.:$$PATH prove $(TESTS)

tests: $(TESTS)

all: $(TESTS)

clean:
	$(RM) $(TESTS)
	$(RM) tmp_*

.PHONY: all clean check tests
