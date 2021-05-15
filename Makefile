CC ?= cc
CFLAGS ?= -O3

g15fun: g15fun.c hidraw_lib.c
	$(CC) -o $@ $(CFLAGS) $^

clean:
	rm -fv g15fun

.PHONY: clean
