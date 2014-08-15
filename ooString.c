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
			if (!new) {
				ooRaise(Malloc);
			}
			this->st = new;
			strcpy(this->st + this->len, st);
			this->len += l;
		}
	}
	return(this);
}

ooString *ooMethod(ooString, CatBuffer, char *buf, int l) {
	if (buf && l) {
		void *new = realloc(this->st, this->len + l + 1);
		if (new) {
			this->st = new;
			memcpy(this->st + this->len, buf, l);
			this->len += l;
			this->st[this->len]=0;
		}
	}
	return(this);
}

ooString *ooMethod(ooString, Clean) {
	if (this->st) {
		this->st[0]=0;
		this->len=0;
	}
	return(this);
}

ooString *ooMethod(ooString, Catf, const char *format, ...) {
	char *buf = malloc(1024);
	if (buf) {
	  va_list va;

	  va_start(va, format);
	  vsnprintf(buf, 1024, format, va);
	  buf[1023]=0;
	  va_end(va);

	  this->Cat(this, buf);

	  free(buf);
	}
	return(this);
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

int ooMethod(ooString, compare, ooString *with) {
	return(strcmp(this->st, with->st));
}

ooCtor(ooString, char *st) {

	ooTypeable(ooString);
	ooComparable(ooString, compare);

	ooMethodInit(ooString, Copy);
	ooMethodInit(ooString, Cat);
	ooMethodInit(ooString, Catf);
	ooMethodInit(ooString, CatBuffer);
	ooMethodInit(ooString, Equal);
	ooMethodInit(ooString, Clean);
	ooPropertyGetInit(ooString, IsEmpty);
	ooPropertyGetInit(ooString, Len);

	this->Copy(this, st);

	return;
}

ooDtor(ooString) {
	if (this->st) {
		free(this->st);
	}
	return;
}
