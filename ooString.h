/*
 * oostring.h
 *
 *  Created on: Aug 1, 2014
 *      Author: fcuri
 */

#ifndef OOSTRING_H_
#define OOSTRING_H_

#include <string.h>

#include "ooSimple.h"

#define ooStringC(_st) (char*)(_st)->st

ooClass(ooString)
char *st;
int len;

ooImpTypeable(ooString);

ooPropertyGetDeclare(ooString, ooBoolean, IsEmpty);
ooPropertyGetDeclare(ooString, int, Len);

ooString *ooMethodDeclare(ooString, Cat, char *st);
ooString *ooMethodDeclare(ooString, Copy, char *st);
ooBoolean ooMethodDeclare(ooString, Equal, char *st);

ooClassEnd(ooString)

ooCtor(ooString, char *st);
ooDtor(ooString);

#endif /* OOSTRING_H_ */
