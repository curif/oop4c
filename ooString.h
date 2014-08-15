/*
 * oostring.h
 *
 *  Created on: Aug 1, 2014
 *      Author: fcuri
 */

#ifndef OOSTRING_H_
#define OOSTRING_H_

#include <string.h>
#include <stdarg.h>
#include "ooSimple.h"

#define ooStringC(_st) (char*)(_st)->st

//Use error handling
ooExcepUse();

//REdefine the stringable implementation to use the ooString class.
#undef ooImpStringable
#define ooImpStringable(_class) ooString *ooMethodDeclare(_class, ToString, ooString *st)

ooClass(ooString)
  char *st;
  int len;

  ooImpTypeable(ooString);
  ooImpComparable(ooString);

  ooPropertyGetDeclare(ooString, ooBoolean, IsEmpty);
  ooPropertyGetDeclare(ooString, int, Len);

  ooString *ooMethodDeclare(ooString, Cat, char *st);
  ooString *ooMethodDeclare(ooString, Catf, const char *format, ...);
  ooString *ooMethodDeclare(ooString, CatBuffer, char *buf, int len);
  ooString *ooMethodDeclare(ooString, Copy, char *st);
  ooString *ooMethodDeclare(ooString, Clean);
  ooBoolean ooMethodDeclare(ooString, Equal, char *st);

ooClassEnd(ooString)

ooCtor(ooString, char *st);
ooDtor(ooString);

#endif /* OOSTRING_H_ */
