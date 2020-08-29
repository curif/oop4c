oop4c
=====
A header only oop toolkit for C, with easy common implementation patterns and exception handling.

### Description
Add Object Oriented Programming to a C program using just one include.
This is a structure based, all preprocesor macro (ab)use implementation and without vtables solution.

#### Characteristics

 * One include, fewer dependencies, no libraries, no objects.
 * Exception handling: try/catch, etc.
 * Basic pattern implementation: permits to add methods to handle the most commond patterns.

### Basic pattern implementation
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

### Use
Add ```#include "ooSimple.h"``` to your C program and you are ready.

#### Example: class ooString

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
  // Initialize Typeable
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

#### As alternative you can use a shorter form, just declare the _ooClass macro, add a "D" in every function macro:

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
ooClassD

  char *st;
  int len;

  ooImpTypeableD();
  ooPropertyGetDeclareD(ooBoolean, IsEmpty);
  ooPropertyGetDeclareD(int, Len);
  ooString *ooMethodDeclareD(Cat, char *st);
  ooString *ooMethodDeclareD(Copy, char *st);
  ooBoolean ooMethodDeclareD(Equal, char *st);

ooClassEndD

//Declare constructor and destuctor.
ooCtor(ooString, char *st);
ooDtor(ooString);

#endif /* OOSTRING_H_ */
```
### Exception handling example
```

#include <stdlib.h>
#include <stdio.h>

#include "ooSimple.h"

ooExcepDefine(); //define the exception handling environment.

//Declare exceptions:
ooExcepDeclareError(DivByZero, -1, "Zero Division");

//A function to print error messages:
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
void unhandled(struct ooExcepError* e) {
	printf("Unhandled ERROR: \n");
	printError(e);
	return;
}

//This function will raise a DivByZero when b is zero.
int makediv(int a, int b) {
  ooRaiseIf(!b, DivByZero);
  return(a/b);
}

int main() {
  int a,b;
  
  //Initialize the exception environment.
  ooExcepInit(&unhandled);
  
  //Try/catch
  ooTry {
    a=1;
    b=1;
    printf("a/b:%i\n", makediv(a,b));
    b=0;
    printf("a/b:%i\n", makediv(a,b)); //error here.
    printf("END\n"); // <--- never happen
  }
  ooCatch(DivByZero) {
    //Catch DivByZero
    printf("ERROR b can't be zero\n");
  }
  ooCatchAny() {
    //catch any error (DivByZero too):
    printError(&ooLastError());
  }
  ooTryEnd
  
  printf("end\n");
}

```
Example Result:
```
a/b:1
ERROR b can't be zero
ERROR-------------
   Number: -1 id:DivByZero 
   Program: ooErrorTest.c
   Function: makediv
   Line: 79
   Description: Zero Division
```

### References
* [book] (http://www.cs.rit.edu/~ats/books/ooc.pdf)
* [stackoverflow: Can you write object oriented code in C?] (http://stackoverflow.com/questions/415452/object-orientation-in-c)
* [stackoverflow: Object-Orientation in C] (http://stackoverflow.com/questions/415452/object-orientation-in-c)
* [Object Oriented Programming in C (Laurent Deniau)] (http://ldeniau.web.cern.ch/ldeniau/html/oopc.html)
* [Iterator pattern] (http://www.codeproject.com/Articles/362986/Understanding-and-Implementing-the-Iterator-Patter)
