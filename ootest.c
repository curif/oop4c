/*
 * ootest.c
 *
 *  Created on: Jun 30, 2014
 *      Author: fcuri
 */

#include <stdio.h>
#include <stdlib.h>

#include "ootest.h"

int ooMethodD(sum) {
	return this->x + this->y;
}
ooCtorD(int x, int y) {

	ooTypeableD();

	this->x = x;
	this->y = y;
	ooMethodSetD(sum);

	return;
}
ooDtorD() {
	return;
}

int ooMethodOverride(punto3d, punto, sum) {
	ooThisDeclare(punto3d);
	//return this->z + ooBase()->sum(ooBase()); //not possible, recursion.
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
//Clone a punto3d object.
punto3d *ooMethod(punto3d, clone) {
	punto3d *x = (punto3d *)malloc(ooSize(this));
	memcpy(x,this,ooSize(this));
	return x;
}
//Copy without Z
void ooMethod(punto3d, copyWithoutZ, punto3d *to) {
	to->base.x = this->base.x;
	to->base.y = this->base.y;
	to->z = 0;
	return;
}
int ooMethod(punto3d, compare, punto3d *with) {
	return this->base.x == with->base.x && this->base.y == with->base.y && this->z == with->z;
}
ooCtor(punto3d, int x, int y, int z) {
	//parent init.
	ooInitD(ooBase(), x, y);

	//implementations:
	ooClonable(punto3d, clone);
	ooTypeable(punto3d);
	ooCopiable(punto3d, copyWithoutZ);
  ooComparable(punto3d, compare);
  ooListable(punto3d);

	//methods
	ooMethodSet(punto3d, getx);
	ooMethodSet(punto3d, gety);
	ooMethodSet(punto3d, getz);
	ooMethodSet(punto3d, setx);
	ooMethodSet(punto3d, sety);
	ooMethodSet(punto3d, setz);

	//Override parent method with its own.
	ooMethodSetOverride(punto3d, sum);

	//initialization
	this->z = z;

	return;
}
ooDtor(punto3d) {
	return;
}

int main() {
	punto3d *p;
	punto3d pCopied;
	punto3d *pCloned;
	punto *pu;

	printf("----------------------------------------------\n");
	printf("oop4c test example\n");
	printf("----------------------------------------------\n");

	p = ooNew(punto3d, p, 10, 10, 30);
	printf("p: %i+%i+%i = sum:%i\n", p->base.x, p->base.y, p->z, p->base.sum((punto*)p));

	p->setx(p,20);
	p->sety(p,50);
	p->setz(p,100);
	printf("%i+%i+%i = sum:%i\n", p->getx(p), p->gety(p), p->getz(p), p->base.sum((punto*)p));

	//Clone p
	if (!ooIsClonable(p)) {
		printf("ERROR, p must be clonable !\n");
		return EXIT_FAILURE;
	}
	pCloned = p->clone(p);
	if (!pCloned) {
		printf("ERROR clone");
		return EXIT_FAILURE;
	}
	printf("OK, pCloned\n");

	pCloned->setz(pCloned,1000);
	printf("cloned p: %i+%i+%i = sum:%i\n", pCloned->getx(pCloned), pCloned->gety(pCloned),
			pCloned->getz(pCloned), pCloned->base.sum((punto*)pCloned));

	// copy the cloned object over pCopied object.
	if (ooSize(pCloned) != sizeof(punto3d)) {
		printf("ERROR sizeof punto3d\n");
		return EXIT_FAILURE;
	}

	ooInit(punto3d, &pCopied, 0,0,0);
	pCloned->copy(pCloned, &pCopied);
	printf("OK; pCopied\n");
	printf("pCopied: %i+%i+%i = sum:%i\n", pCopied.getx(&pCopied), pCopied.gety(&pCopied), pCopied.getz(&pCopied), pCopied.base.sum((punto*)&pCopied));

	//Compare
	if (!ooIsComparable(p)) {
		printf("ERROR p must be comparable..\n");
		return EXIT_FAILURE;
	}
	if (p->compare(p, pCloned)) {
		printf("ERROR p and pCloned must not be equals.\n");
		return EXIT_FAILURE;
	}
	else {
		printf("OK p and pCloned are equals\n");
	}

	//Polymorphism, cast a punto3d object as class punto
	pu = (punto*)p;
	printf("pu: %i+%i = sum:%i\n", pu->x, pu->y, pu->sum(pu));
	if (pu->sum(pu) != 170) {
		printf("sum ERROR \n");
		return EXIT_FAILURE;
	}
	else {
		printf("OK sum\n");
	}

	//type
	if (!ooIsTypeable(p)) {
		printf("ERROR p must be not typeable! \n");
		return EXIT_FAILURE;
	}
	printf("Type of p=%s\n", ooType(p));
	printf("Type of pu=%s\n", ooType(pu));
	if (!ooTypeOf(pu, punto)) {
		printf("ERROR pu must by a punto instance.\n");
		return EXIT_FAILURE;
	}
	if (!ooTypeOf(p, punto3d)) {
		printf("ERROR p must by a punto3d instance.\n");
		return EXIT_FAILURE;
	}
	printf("OK typeOf\n");

	//list test ---------------------------
	punto3d *l, *l2;
	punto3d *iter;
	int f;
	l = NULL;
	//add 10 obj to list.
	for (f=0; f<10; f++) {
		l2 = ooNew(punto3d, l2, f+1, 0, 0);
		ooListPush(l, l2);
	}
	printf("for each...\n");
	ooListForEach(l, iter) {
		printf("%i in list %s %s \n", iter->getx(iter), ooListIsFirst(iter)? "FIRST":"", ooListIsLast(iter)? "LAST":"");
		if (!ooListIsFirst(iter)) {
			printf("   prev:%i\n", iter->_prev->getx(iter->_prev));
		}
		if (!ooListIsLast(iter)) {
			printf("   next:%i\n", iter->_next->getx(iter->_next));
		}
	}
	printf("Search first\n");
	ooListFirst(l, iter);
	if (!iter) {
		printf("ERROR ooListFirst\n");
		return EXIT_FAILURE;
	}
	printf("%i first found\n", iter->getx(iter));

	printf("Search last\n");
	ooListLast(l, iter);
	if (!iter) {
		printf("ERROR ooListLast\n");
		return EXIT_FAILURE;
	}
	printf("%i last found\n", iter->getx(iter));

	printf("Remove 7\n");
	ooListForEach(l, iter) {
		if (iter->getx(iter) == 7) {
			printf("7 found, remove\n");
			ooListRemove(iter);
			ooDeleteFree(iter);
			break;
		}
	}
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->getx(iter));
	}
	printf("Remove last\n");
	ooListLast(l, iter);
	ooListRemove(iter);
	ooDeleteFree(iter);
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->getx(iter));
	}

	printf("Remove first\n");
	ooListFirst(l, iter);
	if (!iter) {
		printf("ERROR ooListFirst\n");
		return EXIT_FAILURE;
	}
	printf("%i is first \n", iter->getx(iter));
	l = ooListNext(iter); //save the new first.
	ooListRemove(iter);
	printf("Destroy first\n");
	ooDeleteFree(iter);
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->getx(iter));
	}

	printf("Pop\n");
	ooListPop(l, iter);
	printf("%i poped\n", iter->getx(iter));
	printf("List after Pop\n");
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->getx(iter));
	}
	printf("Remove and destroy all\n");
	while (!ooListIsEmpty(l)) {
		ooListPop(l, iter);
		printf("%i removed\n", iter->getx(iter));
		ooDeleteFree(iter);
	}
	printf("remove first: %i \n", l->getx(l));
	ooDeleteFree(l);

	//Destruction
	ooDeleteFree(p);
	ooDeleteFree(pCloned);
	ooDelete(&pCopied);

	printf("Objects destroyed\n");

	return EXIT_SUCCESS;
}
