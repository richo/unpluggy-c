unpluggy: unpluggy.c
	clang -o $@ $< -framework CoreFoundation -framework IOKit
