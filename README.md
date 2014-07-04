oop4c
=====
A header only oop toolkit for C.

##Description
Point 1: Before start... did you try C++?, why don't?. Continue if you have a coherent response, if not please analize your needs.

With this header any C program can work with a sort of Object Oriented Programming using just one include.
Structure based, all preprocesor macro implementation and without vtables (if you don't know what a vtable is, please read the point 1). If you really need vtables, may be you need C++.

#Example: class punto, class punto3d subclass punto:
punto.h:
```
 #include "ooSimple.h"

 //class definition
 ooClass(punto)
 int x;
 int y;
 int ooMethodDeclare(punto, sum); //sum method declaration
 ooClassEnd
```
punto.c 
```
 #include "punto.h"
 
 //define method "sum"
 int ooMethod(punto, sum) {
   return this->x + this->y;
 }
 //punto constructor
 ooCtor(punto, int x, int y) {
   this->x = x;
   this->y = y;
   //Asign the sum method
   ooMethodSet(sum);
 }
 //punto destructor
 ooDtor(punto) {
   return;
 }
```

punto3d.h
```
 #include "ooSimple.h"
 #include "punto.h"

 //declare class punto3d subclass of punto
 ooClassH(punto3d, punto)
 int z;
 int ooMethodDeclare(punto3d, getx);
 int ooMethodDeclare(punto3d, gety);
 int ooMethodDeclare(punto3d, getz);
 void ooMethodDeclare(punto3d, setx, int px);
 void ooMethodDeclare(punto3d, sety, int py);
 void ooMethodDeclare(punto3d, setz, int pz);
 void ooMethodDeclare(punto3d, setxy, int px, int py);
 ooClassEnd
```

punto3d.c
```
	#include "punto3d.h"
  
 //------------------------------
 //method sum in subclass punto3d overrides method sum in parent class punto
 int ooMethodOverride(punto3d, punto, sum) {
   ooThisDeclare(punto3d); //declare this when a method override the same parent method.
	 return this->z + base->x + base->y;
 }
 int ooMethod(punto3d, getx) {
   return ooBase()->x ;
 }
 int ooMethod(punto3d, gety) {
	 return ooBase()->y ;
 }
 int ooMethod(punto3d, getz) {
   return this->z ;
 }
 void ooMethod(punto3d, setx, int x) {
   ooBase()->x = x ;
 }
 void ooMethod(punto3d, sety, int y) {
   ooBase()->y = y ;
 }
 void ooMethod(punto3d, setz, int z) {
   this->z = z ;
 }
 void ooMethod(punto3d, setxy, int x, int y) {
   ooBase()->y = y ;
 }
 
 //punto3d constructor
 ooCtor(punto3d, int x, int y, int z) {

	//initialize parent
	ooInit(punto, ooBase(), x, y);

	//methods
	//ooMethodSet(punto3d, sum); //don't needed
	ooMethodSet(punto3d, getx);
	ooMethodSet(punto3d, gety);
	ooMethodSet(punto3d, getz);
	ooMethodSet(punto3d, setx);
	ooMethodSet(punto3d, sety);
	ooMethodSet(punto3d, setz);

	//Override parent method with is own.
	ooMethodSetOverride(punto3d, sum);

	//properties initialization
	this->z = z;

	return;
 }
 //punto3d destructor.
 ooDtor(punto3d) {
 }
```
main.c
```
 int main() {
   punto3d *p;
   punto3d p2;
   punto3d p3;
   punto *pu;
	
   p = ooNew(punto3d, p, 10, 10, 30);
   ooInit(punto3d, &p2, 100, 100, 300);
 
   printf("p: %i+%i+%i = sum:%i\n", p->base.x, p->base.y, p->z, p->base.sum((punto*)p));

   printf("p2: %i+%i+%i = sum:%i\n", p2.base.x, p2.base.y, p2.z, p2.base.sum((punto*)&p2));
  
   p->setx(p,20);
   p->sety(p,50);
   p->setz(p,100);
   printf("%i+%i+%i = sum:%i\n", p->getx(p), p->gety(p), p->getz(p), p->base.sum((punto*)p)); 

   p2.setx(&p2,200);
   p2.sety(&p2,500);
   p2.setz(&p2,1000);
   printf("p2: %i+%i+%i = sum:%i\n", p2.getx(&p2), p2.gety(&p2), p2.getz(&p2), p2.base.sum((punto*)&p2));
  
   //Clone a object
 	 ooClone(&p3, p);
   p2.setx(&p3,250);
   printf("p3: %i+%i+%i = sum:%i\n", p3.getx(&p3), p3.gety(&p3), p3.getz(&p3), p3.base.sum((punto*)&p3)); 

   //Polymorphism, cast a punto3d object as class punto
   pu = (punto*)p;
   printf("pu: %i+%i = sum:%i\n", pu->x, pu->y, pu->sum(pu));
   if (pu->sum(pu) != 170) {
     printf("ERROR\n");
   }
 
   //delete objects.
   ooDeleteFree(p);
   ooDelete(&p2);
   ooDelete(&p3);
 }
```

As alternative you can use a shorter form, simply declare the _ooClass macro, add a "D" in every function macro:

punto.h:
``` 
 #include "ooSimple.h"
 
 #undef _ooClass
 #define _ooClass punto

 //class definition
 ooClassD
 int x;
 int y;
 int ooMethodDeclareD(sum);
 ooClassEnd
```
punto3d.h
```
 #include "ooSimple.h"
 #include "punto.h"

 #undef _ooClass
 #define _ooClass punto3d
 
 //declare class punto3d subclass of punto
 ooClassDH(punto)
 
 int z;
 
 int ooMethodDeclareD(getx);
 int ooMethodDeclareD(gety);
 int ooMethodDeclareD(getz);
 void ooMethodDeclareD(setx, int px);
 void ooMethodDeclareD(sety, int py);
 void ooMethodDeclareD(setz, int pz);
 void ooMethodDeclareD(setxy, int px, int py);

 ooClassEnd
```

