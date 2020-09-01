CC ?= gcc

default: all

all: ooErrorTest ooStringTest ootest

ooCollection.o: ../ooCollection.c ../ooCollection.h ../ooSimple.h
	$(CC) -c -O0 -g -o $@ $<

ooString.o: ../ooString.c ../ooString.h ../ooSimple.h
	$(CC) -c -O0 -g -o $@ $<

ooErrorTest.o: ooErrorTest.c ../ooSimple.h
	$(CC) -c -O0 -g -o $@ $<

ooErrorTest: ooErrorTest.o
	$(CC) -o $@ $^

ooStringTest.o: ooStringTest.c ../ooString.h ../ooCollection.h ../ooSimple.h
	$(CC) -c -O0 -g -o $@ $<

ooStringTest: ooStringTest.o ooString.o ooCollection.o
	$(CC) -o $@ $^

ootest: ootest.c ootest.h ../ooSimple.h
	$(CC) -O0 -g -o $@ $<

clean:
	rm -f ooCollection.o ooString.o ooErrorTest ooErrorTest.o ooStringTest ooStringTest.o ootest

.PHONY: all clean default
