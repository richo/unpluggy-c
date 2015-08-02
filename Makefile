all: unpluggy lock

unpluggy: unpluggy.c locker.o
	clang -o $@ -framework CoreFoundation -framework IOKit $^

lock: lock.c locker.o
	clang -o $@ $^

%.o: %.c
	clang -o $@ -c $<

.PHONY: all
