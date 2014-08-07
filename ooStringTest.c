/*
 * ooStringTest.c
 *
 *  Created on: Aug 1, 2014
 *      Author: fcuri
 */

#include <stdio.h>
#include "ooString.h"
#include "ooCollection.h"

int main() {
	ooString *This;
	ooString *is;
	ooString *aTest;
	ooString *all;
	ooString *fmt;
	ooCollection *col;

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

	col = ooNew(ooCollection, col);

	col->Add(col, This);
	col->Add(col, is);
	col->Add(col, aTest);

	oCollIterator *i = col->GetIterator(col);
	ooString *elem;
	ooIterForEach(i,  col) {
		elem = i->GetCurrent(i);
		printf("Collection string member: %s\n", elem->st);
	}
	ooDeleteFree(i); //remember to destroy iterator.

	ooDeleteFree(This);
	ooDeleteFree(is);
	ooDeleteFree(aTest);
	ooDeleteFree(all);
	ooDeleteFree(fmt);
	ooDeleteFree(col);
	ooDeleteFree(col);

	return(0);
}
