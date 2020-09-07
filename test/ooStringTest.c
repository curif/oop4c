/*
 * ooStringTest.c
 *
 *  Created on: Aug 1, 2014
 *      Author: fcuri
 */

#include <stdio.h>
#include "../ooString.h"
#include "../ooCollection.h"

ooExcepDefine();

void printError(struct ooExcepError *e) {
	printf("ERROR-------------\n");
	printf("   Number: %i id:%s \n", e->no, e->id);
	printf("   Program: %s\n", e->file);
	printf("   Function: %s\n", e->function);
	printf("   Line: %i\n", e->line);
	printf("   Description: %s\n", e->desc);
	return;
}

/**
 * This function will be called when
 * an unhandled exception raises.
 */
void unhandled(struct ooExcepError *e) {
	printf("Unhandled exception\n");
	printError(&ooLastError());
	e = 0;
	return;
}

int main() {
	ooString *This;
	ooString *is;
	ooString *aTest;
	ooString *all;
	ooString *fmt;
	ooCollection *col;

	ooExcepInit(&unhandled);

	ooTry {
		This = ooNew(ooString, This, "This ");
		is = ooNew(ooString, is, "is ");
		aTest = ooNew(ooString, aTest, NULL);
		aTest->Copy(aTest, "a ooString Test.");

		printf("%s%s%s\n", ooStringC(This), ooStringC(is), ooStringC(aTest));
		printf("Len: %i, %i, %i\n", This->GetLen(This), is->GetLen(is), aTest->GetLen(aTest));

		all = ooNew(ooString, all, NULL);
		printf("All: %s\n", ooStringC(all->Cat(all, ooStringC(This))->Cat(all, ooStringC(is))->Cat(all, ooStringC(aTest))));

		if (! all->Equal(all, "This is a ooString Test.")) {
			printf("ERROR Equal\n");
		}

		fmt = ooNew(ooString, fmt, NULL);
		fmt->Catf(fmt, "%s %s %s", ooStringC(This), ooStringC(is), ooStringC(aTest));
		printf("fmt result: %s\n", ooStringC(fmt));

	}
	ooCatchAny() {
		printError(&ooLastError());
	}
	ooTryEnd;

	ooTry {
		col = ooNew(ooCollection, col);

		col->Add(col, This)->Add(col, is)->Add(col, aTest);

		oCollIterator *i = col->GetIterator(col);
		oCollIterator * a = i;
		ooString *elem;
		ooIterForEach(i,  col) {
			elem = i->GetCurrent(i);
			printf("Collection string member: %s\n", elem->st);
		}
		ooDeleteFree(i); //remember to destroy iterator.
		ooDeleteFree(a); // remember to destroy the iterator.
	}
	ooCatchAny() {
		printError(&ooLastError());
	}
	ooTryEnd;

	ooDeleteFree(This);
	ooDeleteFree(is);
	ooDeleteFree(aTest);
	ooDeleteFree(all);
	ooDeleteFree(fmt);
	ooDeleteFree(col);
	ooDeleteFree(col);

	return(0);
}
