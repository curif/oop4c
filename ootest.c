/*
 * ootest.c
 *
 *  Created on: Jun 30, 2014
 *      Author: fcuri
 */

#include <stdio.h>
#include <stdlib.h>

#include "ootest.h"

ooPropertyGetD(int, x) {
	return this->x;
}
ooPropertySetD(int, x) {
	this->x = x;
	return;
}
ooPropertyGetD(int, y) {
	return this->y;
}
ooPropertySetD(int, y) {
	this->y = y;
	return ;
}
int ooMethodD(sum) {
	return this->x + this->y;
}

ooCtorD(int x, int y) {

	ooTypeableD();

	this->x = x;
	this->y = y;
	ooMethodInitD(sum);

	//initialize setters & getters
	ooPropertyInitD(x);
	ooPropertyInitD(y);

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

ooPropertyGet(punto3d, int, x) {
	return ooBase()->Getx(ooBase());
}
ooPropertySet(punto3d, int, x) {
	ooBase()->Setx(ooBase(), x);
	return ;
}
ooPropertyGet(punto3d, int, y) {
	return ooBase()->Gety(ooBase());
}
ooPropertySet(punto3d, int, y) {
	ooBase()->Sety(ooBase(), y);
	return ;
}
ooPropertyGet(punto3d, int, z) {
	return this->z;
}
ooPropertySet(punto3d, int, z) {
	this->z = z;
	return;
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

	//Initialize Properties
  ooPropertyInit(punto3d, x);
  ooPropertyInit(punto3d, y);
  ooPropertyInit(punto3d, z);

	//Override parent method with its own.
	ooMethodInitOverride(punto3d, sum);

	//initialization
	this->Setz(this, z);

	return;
}
ooDtor(punto3d) {
	return;
}

//Collection & iterator --------------------------------------
/**
 * Add a punto to collection.
 */
void ooMethod(puntoColl, Add, punto *p) {
	this->count++;
	if (!this->arr || this->count > this->len) {
		this->len+=10;
		this->arr = realloc(this->arr, this->len * sizeof(punto));
	}
	this->arr[this->count-1] = p;
}
/**
 * Add a punto from collection
 */
void ooMethod(puntoColl, Remove, punto *p) {
	int f, g, found=-1;
	if (!this->count) {
		return;
	}
	for (f=0; f<this->count; f++) {
		if (p == this->arr[f]) {
			found = f;
			break;
		}
	}
	if (found>=0) {
		for (g = f+1; g < this->count; g++) {
			this->arr[g-1] = this->arr[g];
		}
		this->arr[this->count-1]=NULL;
		this->count--;
	}
	return;
}
/**
 * Get an iterator.
 */
ooPropertyGet(puntoColl, ooObj, Iterator) {
	puntoIter *i;
	i = ooNew(puntoIter, i, this);
	return i;
}
/***
 * Count elements
 */
ooPropertyGet(puntoColl, int, Count) {
	return this->count;
}
/**
 * Get a punto from collection.
 */
punto *ooMethod(puntoColl, Index, int idx) {
	if (idx > this->count - 1) {
		return NULL;
	}
	return this->arr[idx];
}
/**
 * Collection constructor
 */
ooCtor(puntoColl) {
	ooAgregator(puntoColl);
}
/**
 * Collection destructor
 */
ooDtor(puntoColl) {
	if (this->arr) {
		free(this->arr);
	}
	return;
}

//Iterator -----------------

ooPropertyGet(puntoIter, ooBoolean, HasNext) {
	if (!this->coll) {
		return ooFalse;
	}
	return this->idxCurrent < this->coll->GetCount(this->coll);
}
ooPropertyGet(puntoIter, punto *, Next) {
	if (!this->GetHasNext(this)) {
		return NULL;
	}
	this->idxCurrent++;
	return this->GetCurrent(this);
}
ooPropertyGet(puntoIter, punto *, Current) {
	return this->coll->Index(this->coll, this->idxCurrent);
}

ooCtor(puntoIter, puntoColl *coll) {
	ooIterator(puntoIter);
	if (ooIsAgregable(coll)) {
		this->coll = coll;
	}
}

ooDtor(puntoIter) {
	return;
}

int main() {
	punto3d *p;
	punto3d pCopied;
	punto3d *pCloned;
	punto *pu;
	int f;
	puntoColl *pc;
	puntoIter *i;

	printf("----------------------------------------------\n");
	printf("oop4c test example\n");
	printf("----------------------------------------------\n");

	p = ooNew(punto3d, p, 10, 10, 30);
	printf("p: %i+%i+%i = sum:%i\n", p->base.x, p->base.y, p->z, p->base.sum((punto*)p));

	p->Setx(p,20);
	p->Sety(p,50);
	p->Setz(p,100);
	printf("%i+%i+%i = sum:%i\n", p->Getx(p), p->Gety(p), p->Getz(p), p->base.sum((punto*)p));

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

	pCloned->Setz(pCloned,1000);
	printf("cloned p: %i+%i+%i = sum:%i\n", pCloned->Getx(pCloned), pCloned->Gety(pCloned),
			pCloned->Getz(pCloned), pCloned->base.sum((punto*)pCloned));

	// copy the cloned object over pCopied object.
	if (ooSize(pCloned) != sizeof(punto3d)) {
		printf("ERROR sizeof punto3d\n");
		return EXIT_FAILURE;
	}

	ooInit(punto3d, &pCopied, 0,0,0);
	pCloned->copy(pCloned, &pCopied);
	printf("OK; pCopied\n");
	printf("pCopied: %i+%i+%i = sum:%i\n", pCopied.Getx(&pCopied), pCopied.Gety(&pCopied), pCopied.Getz(&pCopied), pCopied.base.sum((punto*)&pCopied));

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
	l = NULL;
	//add 10 obj to list.
	for (f=0; f<10; f++) {
		l2 = ooNew(punto3d, l2, f+1, 0, 0);
		ooListPush(l, l2);
	}
	printf("for each...\n");
	ooListForEach(l, iter) {
		printf("%i in list %s %s \n", iter->Getx(iter), ooListIsFirst(iter)? "FIRST":"", ooListIsLast(iter)? "LAST":"");
		if (!ooListIsFirst(iter)) {
			printf("   prev:%i\n", iter->_prev->Getx(iter->_prev));
		}
		if (!ooListIsLast(iter)) {
			printf("   next:%i\n", iter->_next->Getx(iter->_next));
		}
	}
	printf("Search first\n");
	ooListFirst(l, iter);
	if (!iter) {
		printf("ERROR ooListFirst\n");
		return EXIT_FAILURE;
	}
	printf("%i first found\n", iter->Getx(iter));

	printf("Search last\n");
	ooListLast(l, iter);
	if (!iter) {
		printf("ERROR ooListLast\n");
		return EXIT_FAILURE;
	}
	printf("%i last found\n", iter->Getx(iter));

	printf("Remove 7\n");
	ooListForEach(l, iter) {
		if (iter->Getx(iter) == 7) {
			printf("7 found, remove\n");
			ooListRemove(iter);
			ooDeleteFree(iter);
			break;
		}
	}
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->Getx(iter));
	}
	printf("Remove last\n");
	ooListLast(l, iter);
	ooListRemove(iter);
	ooDeleteFree(iter);
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->Getx(iter));
	}

	printf("Remove first\n");
	ooListFirst(l, iter);
	if (!iter) {
		printf("ERROR ooListFirst\n");
		return EXIT_FAILURE;
	}
	printf("%i is first \n", iter->Getx(iter));
	l = ooListNext(iter); //save the new first.
	ooListRemove(iter);
	printf("Destroy first\n");
	ooDeleteFree(iter);
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->Getx(iter));
	}

	printf("Pop\n");
	ooListPop(l, iter);
	printf("%i poped\n", iter->Getx(iter));
	printf("List after Pop\n");
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->Getx(iter));
	}
	printf("Remove and destroy all\n");
	while (!ooListIsEmpty(l)) {
		ooListPop(l, iter);
		printf("%i removed\n", iter->Getx(iter));
		ooDeleteFree(iter);
	}
	printf("remove first: %i \n", l->Getx(l));
	ooDeleteFree(l);

	//Destruction
	ooDeleteFree(p);
	ooDeleteFree(pCloned);
	ooDelete(&pCopied);
	printf("Objects destroyed\n");

	//TEST collections and iterators
	printf("TEST collections \n");
	pc = ooNew(puntoColl, pc); //Create collection
	//load 20 items
	for (f=0; f<20; f++) {
		pu = ooNew(punto, pu, f, 0);
		pc->Add(pc, pu);
		printf("Add %i to collection, count:%i\n", f, pc->GetCount(pc));
	}

	//iterate collection
	printf("Iteration\n");
	ooIterForEach(i,  pc) {
		pu = i->GetCurrent(i);
		printf("Member %i ok\n", pu->Getx(pu));
	}
	ooDeleteFree(i); //remember to destroy iterator.

	//Delete all members
	printf("Delete collection:\n");
	while (pc->GetCount(pc)) {
		pu = pc->Index(pc, 0); //Obtain object in position 0.
		printf("Delete member:%i\n", pu->Getx(pu));
		pc->Remove(pc, pu); //Remove from the collection
		ooDeleteFree(pu); //Destroy object
	}
	printf("Actual collection members:%i\n", pc->GetCount(pc));
	ooDeleteFree(pc);

	return EXIT_SUCCESS;
}
