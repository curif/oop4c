/*
 * ootest.c
 *
 *  Created on: Jun 30, 2014
 *      Author: fcuri
 */

#include "ootest.h"

int ooMethodD(suma) {
	return this->x + this->y;
}
ooCtorD(int x, int y) {
	this->x = x;
	this->y = y;
	ooMethodSetD(suma);
}
ooDtorD() {
	//free(this);
}

int ooMethodOverride(punto3d, punto, suma) {
	ooThisDeclare(punto3d);
	//return this->z + ooBase()->suma(ooBase()); //not possible, recursion.
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
ooCtor(punto3d, int x, int y, int z) {

	//inicializa el parent (punto defined in _ooClass)
	ooInitD(ooBase(), x, y);

	//methods
	//ooMethodSet(punto3d, suma);
	ooMethodSet(punto3d, getx);
	ooMethodSet(punto3d, gety);
	ooMethodSet(punto3d, getz);
	ooMethodSet(punto3d, setx);
	ooMethodSet(punto3d, sety);
	ooMethodSet(punto3d, setz);

	//Override parent method with is own.
	ooMethodSetOverride(punto3d, suma);

	//initialization
	this->z = z;

	return;
}
ooDtor(punto3d) {
	//ooDelete(ooBase());
	//free(this);
}

int main() {
	punto3d *p;
	punto3d p2;
	punto3d p3;
	punto *pu;
	
	p = ooNew(punto3d, p, 10, 10, 30);
	ooInit(punto3d, &p2, 100, 100, 300);

	printf("p: %i+%i+%i = suma:%i\n", p->base.x, p->base.y, p->z, p->base.suma((punto*)p));

	printf("p2: %i+%i+%i = suma:%i\n", p2.base.x, p2.base.y, p2.z, p2.base.suma((punto*)&p2));

	p->setx(p,20);
	p->sety(p,50);
	p->setz(p,100);
	printf("%i+%i+%i = suma:%i\n", p->getx(p), p->gety(p), p->getz(p), p->base.suma((punto*)p));

	p2.setx(&p2,200);
	p2.sety(&p2,500);
	p2.setz(&p2,1000);
	printf("p2: %i+%i+%i = suma:%i\n", p2.getx(&p2), p2.gety(&p2), p2.getz(&p2), p2.base.suma((punto*)&p2));

	ooClone(&p3, p);
	p2.setx(&p3,250);
	printf("p3: %i+%i+%i = suma:%i\n", p3.getx(&p3), p3.gety(&p3), p3.getz(&p3), p3.base.suma((punto*)&p3));

	//polimorfismo, si bien pu es de tipo punto, en realidad es punto3d y su suma debe sumar x,y,z
	pu = (punto*)p;
	printf("pu: %i+%i = suma:%i\n", pu->x, pu->y, pu->suma(pu));
	if (pu->suma(pu) != 170) {
		printf("ERROR\n");
	}

	printf("Type of p=%s\n", ooType(p));
	printf("Type of pu=%s\n", ooType(pu));
	if (ooTypeOf(pu, punto)) {
		printf("Correct, pu is punto\n");
	}
	
	ooDeleteFree(p);
	ooDelete(&p2);
	ooDelete(&p3);

}
