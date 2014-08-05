/*
 * oostring.c
 *
 *  Created on: Aug 1, 2014
 *      Author: fcuri
 */


#include "ooString.h"

ooPropertyGet(ooString, int, Len) {
	return this->len;
}
ooPropertyGet(ooString, ooBoolean, IsEmpty) {
	return this->len==0? ooTrue: ooFalse;
}
ooString *ooMethod(ooString, Cat, char *st) {
	if (st) {
		int l = strlen(st);
		if (l) {
			void *new = realloc(this->st, this->len + l + 1);
			if (new) {
				this->st = new;
				strcpy(this->st + this->len, st);
				this->len += l;
			}
		}
	}
	return this;
}

ooBoolean ooMethod(ooString, Equal, char *st) {
	return(strcmp(this->st, st) == 0 ? ooTrue: ooFalse);
}

ooString *ooMethod(ooString, Copy, char *st) {
	if (st) {
		int l = strlen(st);
		if (l) {
			char *new = (char*) malloc(l + 1);
			if (new) {
				if (this->st) {
					free(this->st);
				}
				this->st = new;
				strcpy(this->st, st);
				this->len = l;
			}
		}
	}
	return this;
}

ooCtor(ooString, char *st) {

	ooMethodInit(ooString, Copy);
	ooMethodInit(ooString, Cat);
	ooMethodInit(ooString, Equal);
	ooPropertyGetInit(ooString, IsEmpty);
	ooPropertyGetInit(ooString, Len);
	ooTypeable(ooString);

	this->Copy(this, st);

	return;
}

ooDtor(ooString) {
	if (this->st) {
		free(this->st);
	}
	return;
}
