CC ?= gcc

default: all

all: ootest

ootest: ootest.c ootest.h ../ooSimple.h
	$(CC) -o $@ $<

clean:
	rm ootest

.PHONY: all clean default
