CC ?= gcc

default: all

all: ooStringTest ootest

ooCollection.o: ../ooCollection.c ../ooCollection.h
	$(CC) -c -o $@ $<

ooString.o: ../ooString.c ../ooString.h
	$(CC) -c -o $@ $<

ooStringTest.o: ooStringTest.c ../ooString.h ../ooCollection.h
	$(CC) -c -o $@ $<

ooStringTest: ooStringTest.o ooString.o ooCollection.o
	$(CC) -o $@ $^

ootest: ootest.c ootest.h ../ooSimple.h
	$(CC) -o $@ $<

clean:
	rm -f ooStringTest.o ooStringTest ootest

.PHONY: all clean default
