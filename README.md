oop4c
=====
A header only oop toolkit for C, with easy common implementation patterns.


###Description
Add Object Oriented Programming to a C program using just one include.
This is a structure based, all preprocesor macro (ab)use implementation and without vtables solution.

####Basic pattern implementation
To add basic pattern functionality to a class:

 * Copyable: Make the class copiable
 * Typeable: Convert a class in comparable by his type.
 * Clonable: To make an object's duplicate.
 * Comparable: To compare two objects instance of the same class.
 * Listable: Add double list functionality to a class.
 * Agregable: Implement agregato pattern (collections).
 * Iterator: Implement iterator pattern.
 * Stringable: Implements ToString method
 * Serializable: Implements the Serializable pattern
 
Check ootest.c and ootest.h for more information. 
 

###Example: class ooString 

ooString.h:
```
#ifndef OOSTRING_H_
#define OOSTRING_H_

#include <string.h>

#include "ooSimple.h"

#define ooStringC(_st) (char*)(_st)->st

//Declare class ooString
ooClass(ooString)

char *st;
int len;

//Implements Type
ooImpTypeable(ooString);

//Declare properties
ooPropertyGetDeclare(ooString, ooBoolean, IsEmpty);
ooPropertyGetDeclare(ooString, int, Len);

//Declare Methods
ooString *ooMethodDeclare(ooString, Cat, char *st);
ooString *ooMethodDeclare(ooString, Copy, char *st);
ooBoolean ooMethodDeclare(ooString, Equal, char *st);

//End Class
ooClassEnd(ooString)

//Declare constructor and destuctor.
ooCtor(ooString, char *st);
ooDtor(ooString);

#endif /* OOSTRING_H_ */
```
ooString.c 
```
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

//Constructor
ooCtor(ooString, char *st) {

  //Initialize methods
  ooMethodInit(ooString, Copy);
  ooMethodInit(ooString, Cat);
  ooMethodInit(ooString, Equal);
  //Initialize properties
  ooPropertyGetInit(ooString, IsEmpty);
  ooPropertyGetInit(ooString, Len);
  //Initialize Typeable 
  ooTypeable(ooString);

  //copy the initial value
  this->Copy(this, st);
  
  return;
}

//Destructor
ooDtor(ooString) {
  if (this->st) {
    free(this->st);
  }
  return;
}

```

ooStringTest.c
```

#include <stdio.h>
#include "ooString.h"

int main() {
	ooString *This;
	ooString *is;
	ooString *aTest;
	ooString *all;

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

	ooDeleteFree(This);
	ooDeleteFree(is);
	ooDeleteFree(aTest);
	ooDeleteFree(all);

	return(0);
}

```

###As alternative you can use a shorter form, simply declare the _ooClass macro, add a "D" in every function macro:

ooString.h:
``` 
#ifndef OOSTRING_H_
#define OOSTRING_H_

#include <string.h>
#include "ooSimple.h"


#define ooStringC(_st) (char*)(_st)->st

//Declare class ooString
#undef _ooClass
#define _ooClass ooString
ooClassD()

char *st;
int len;

ooImpTypeableD();
ooPropertyGetDeclareD(ooBoolean, IsEmpty);
ooPropertyGetDeclareD(int, Len);
ooString *ooMethodDeclareD(Cat, char *st);
ooString *ooMethodDeclareD(Copy, char *st);
ooBoolean ooMethodDeclareD(Equal, char *st);

ooClassEndD()

//Declare constructor and destuctor.
ooCtor(ooString, char *st);
ooDtor(ooString);

#endif /* OOSTRING_H_ */
```

###References
* [book] (http://www.cs.rit.edu/~ats/books/ooc.pdf)
* [stackoverflow: Can you write object oriented code in C?] (http://stackoverflow.com/questions/415452/object-orientation-in-c)
* [stackoverflow: Object-Orientation in C] (http://stackoverflow.com/questions/415452/object-orientation-in-c)
* [Object Oriented Programming in C (Laurent Deniau)] (http://ldeniau.web.cern.ch/ldeniau/html/oopc.html)
* [Iterator pattern] (http://www.codeproject.com/Articles/362986/Understanding-and-Implementing-the-Iterator-Patter)
