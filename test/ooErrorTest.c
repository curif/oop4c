/*
 * ooErrorTest.c
 *
 *  Created on: Aug 11, 2014
 *      Author: fcuri
 */


#include <stdlib.h>
#include <stdio.h>


#include "ooSimple.h"

//Define exception manager
ooExcepDefine();

//Define errors
ooExcepDeclareError(ERR1, -1, "ERROR 1");
ooExcepDeclareError(ERR2, -2, "ERROR 2");
ooExcepDeclareError(ERR3, -3, "ERROR 3");
ooExcepDeclareError(ERR4, -4, "ERROR 4");
ooExcepDeclareError(ERR4UNHANDLED, -10, "ERROR Not handled, raise this for non handled errors in test");
ooExcepDeclareError(DivByZero, -1, "Zero Division");


void printError(struct ooExcepError *e) {
	fprintf(stderr, "ERROR-------------\n");
	fprintf(stderr, "   Number: %i id:%s \n", e->no, e->id);
	fprintf(stderr, "   Program: %s\n", e->file);
	fprintf(stderr, "   Function: %s\n", e->function);
	fprintf(stderr, "   Line: %i\n", e->line);
	fprintf(stderr, "   Description: %s\n", e->desc);
	return;
}

/**
 * This function will be called when
 * an unhandled exception raises.
 */
void unhandled(struct ooExcepError *e) {
	printf("Unhandled exception\n");
	printError(e);
	return;
}

//Decide wich error raise here.
void fun2() {
	printf("fun2\n");
	//ooRaise(ERR3);
	//ooRaise(ERR2);
	ooRaiseMsg(ERR1, "Err1 generation in func: %s number:%i\n", "fun2", 1); //test stderr
	ooRaise(ERR4UNHANDLED);
	printf("fun2 no error\n");
}

void fun1() {
	ooTry {
		fun2();
	}
	ooCatch(ERR2) {
		printf("fun1, err2 but raise another (err4)\n");
		ooRaiseAnother(ERR4); //change error if error 2 to error 4
	}
	/*
	ooCatchAny() {
		printf("fun1: A non catched error, throw to caller function\n");
		ooRaiseThrow();
	}
	*/
	printf("Fun1 ends, unmanaged errors will be managed in main function\n");
	ooTryEnd;

	printf("fun1 ok\n");
}

//This function will raise a DivByZero when b is zero.
int makediv(int a, int b) {
	ooRaiseIf(!b, DivByZero);
	return(a / b);
}

int main() {
	int a, b;

	ooExcepInit(&unhandled);

	ooTry {
		printf("call fun1\n");
		fun1();
	}
	/*
	ooCatchAny() {
		printf("MAIN: TEST Catch any error to print:%i\n", ooErrNo());
		printError(&ooLastError());
	}
	*/
	ooCatch(ERR1) {
		printf("MAIN: Catch ERR1:\n");
		printError(&ooLastError());
	}
	ooCatch(ERR2) {
		printf("MAIN: Catch ERR2, never happens:\n");
		printError(&ooLastError());
	}
	ooCatch(ERR3) {
		printf("MAIN: Catch ERR3:\n");
		printError(&ooLastError());

		printf("MAIN: raise unhandled when ERR3\n");
		ooRaiseAnother(ERR4UNHANDLED);
	}
	printf("End Main try, will be raise unhandled if there are uncatched errors.\n");
	ooTryEnd;

	printf("main ok, try finalized.\n");


	// Try/catch
	ooTry {
		a = 1;
		b = 1;
		printf("a/b:%i\n", makediv(a, b));
		b = 0;
		printf("a/b:%i\n", makediv(a, b)); // error here.
		printf("END\n"); // <--- never happen
	}
	ooCatch(DivByZero) {
		// Catch DivByZero
		printf("ERROR b can't be zero\n");
	}
	ooCatchAny() {
		// catch any error (DivByZero too):
		printError(&ooLastError());
	}
	ooTryEnd


	printf("At last, force to Raise an unhandled exception\n");
	ooRaise(ERR4UNHANDLED);

	printf("Never success -----\n");
	return(EXIT_SUCCESS);
}
