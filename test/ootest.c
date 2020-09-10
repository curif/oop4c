/*
 * ootest.c
 *
 *  Created on: Jun 30, 2014
 *      Author: fcuri
 */

#include <stdio.h>
#include <stdlib.h>

#include "ootest.h"

ooExcepDefine();


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
char *ooMethodD(ToString, char *buf, int len) {
	snprintf(buf, len, "%i, %i", this->x, this->y);
	return buf;
}
ooCtorD(int x, int y) {

	ooTypeableD();
	ooStringableD();

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

int ooMethodOverride(point3d, point, sum) {
	ooThisDeclare(point3d);
	//return this->z + ooBase()->sum(ooBase()); //not possible, recursion.
	return this->z + base->x + base->y;
}

ooPropertyGet(point3d, int, x) {
	return ooBase()->Getx(ooBase());
}
ooPropertySet(point3d, int, x) {
	ooBase()->Setx(ooBase(), x);
	return ;
}
ooPropertyGet(point3d, int, y) {
	return ooBase()->Gety(ooBase());
}
ooPropertySet(point3d, int, y) {
	ooBase()->Sety(ooBase(), y);
	return ;
}
ooPropertyGet(point3d, int, z) {
	return this->z;
}
ooPropertySet(point3d, int, z) {
	this->z = z;
	return;
}

// Clone a point3d object.
point3d * ooMethod(point3d, clone) {
	point3d * x = (point3d *) malloc(ooSize(this));
	memcpy(x,this,ooSize(this));
	return x;
}

//Copy without Z
void ooMethod(point3d, copyWithoutZ, point3d * to) {
	to->base.x = this->base.x;
	to->base.y = this->base.y;
	to->z = 0;
	return;
}
int ooMethod(point3d, compare, point3d * with) {
	return this->base.x == with->base.x && this->base.y == with->base.y && this->z == with->z;
}
char * ooMethod(point3d, ToString, char * buf, int len) {
	ooBaseDeclare(point);
	char buf2[20];
	snprintf(buf, len, "%s, %i", base->ToString(base, buf2, 20), this->z);
	return buf;
}
ooCtor(point3d, int x, int y, int z) {
	//parent init.
	ooInitD(ooBase(), x, y);

	//implementations:
	ooClonable(point3d, clone);
	ooTypeable(point3d);
	ooCopiable(point3d, copyWithoutZ);
	ooComparable(point3d, compare);
	ooListable(point3d);
	ooStringable(point3d);

	//Initialize Properties
	ooPropertyInit(point3d, x);
	ooPropertyInit(point3d, y);
	ooPropertyInit(point3d, z);

	//Override parent method with its own.
	ooMethodInitOverride(point3d, sum);

	//initialization
	this->Setz(this, z);

	return;
}
ooDtor(point3d) {
	return;
}

//Collection & iterator --------------------------------------
/**
 * Add a point to collection.
 */
pointColl * ooMethod(pointColl, Add, point * p) {
	if (!this->arr || this->count + 1 > this->len) {
		void * new = realloc(this->arr, (this->len + 10) * sizeof(point *));
		ooRaiseIf(!new, Malloc);
		this->len += 10;
		this->arr = new;
	}
	this->arr[this->count] = p;
	this->count++;
	return(this);
}
/**
 * Add a point from collection
 */
ooBoolean ooMethod(pointColl, Remove, point * p) {
	int f, g, found=-1;
	if (!this->count) {
		return(ooFalse);
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
	return(found>=0);
}

/**
 * Get an iterator.
 */
ooPropertyGet(pointColl, ooObj, Iterator) {
	pointIter * i;
	i = ooNew(pointIter, i, this);
	return i;
}
/***
 * Count elements
 */
ooPropertyGet(pointColl, int, Count) {
	return this->count;
}
/**
 * Get a point from collection.
 */
point * ooMethod(pointColl, Index, int idx) {
	if (idx > this->count - 1) {
		return NULL;
	}
	return this->arr[idx];
}
/**
 * Collection constructor
 */
ooCtor(pointColl) {
	ooAgregator(pointColl);
}
/**
 * Collection destructor
 */
ooDtor(pointColl) {
	if (this->arr) {
		free(this->arr);
	}
	return;
}

//Iterator -----------------

ooPropertyGet(pointIter, ooBoolean, HasNext) {
	if (!this->coll) {
		return ooFalse;
	}
	return this->idxCurrent < this->coll->GetCount(this->coll);
}
ooPropertyGet(pointIter, point *, Next) {
	if (!this->GetHasNext(this)) {
		return NULL;
	}
	this->idxCurrent++;
	return this->GetCurrent(this);
}
ooPropertyGet(pointIter, point *, Current) {
	return this->coll->Index(this->coll, this->idxCurrent);
}

ooCtor(pointIter, pointColl * coll) {
	ooIterator(pointIter);
	if (ooIsAgregable(coll)) {
		this->coll = coll;
	}
}

ooDtor(pointIter) {
	return;
}


void printError(struct ooExcepError *e) {
	printf("ERROR-------------\n");
	printf("   Number: %i id:%s \n", e->no, e->id);
	printf("   Program: %s\n", e->file);
	printf("   Function: %s\n", e->function);
	printf("   Line: %i\n", e->line);
	printf("   Description: %s\n", e->desc);
	return;
}


/**
 * This function will be called when
 * an unhandled exception raises.
 */
void unhandled(struct ooExcepError *e) {
	printf("Unhandled exception\n");
	printError(&ooLastError());
	return;
}

int main() {
	point3d * p;
	point3d pCopied;
	point3d * pCloned;
	point * pu;
	int f;
	pointColl * pc;
	pointIter * i;
	char st[50];

	ooExcepInit(&unhandled);

	printf("----------------------------------------------\n");
	printf("oop4c test example\n");
	printf("----------------------------------------------\n");

	p = ooNew(point3d, p, 10, 10, 30);
	printf("p: %i+%i+%i = sum:%i\n", p->base.x, p->base.y, p->z, p->base.sum((point *) p));

	p->Setx(p,20);
	p->Sety(p,50);
	p->Setz(p,100);
	printf("ToString: %s\n", p->ToString(p, st, 50));
	printf("%i+%i+%i = sum:%i\n", p->Getx(p), p->Gety(p), p->Getz(p), p->base.sum((point *) p));

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
			pCloned->Getz(pCloned), pCloned->base.sum((point *) pCloned));

	// copy the cloned object over pCopied object.
	if (ooSize(pCloned) != sizeof(point3d)) {
		printf("ERROR sizeof point3d\n");
		return EXIT_FAILURE;
	}

	ooInit(point3d, & pCopied, 0,0,0);
	pCloned->copy(pCloned, &pCopied);
	printf("OK; pCopied\n");
	printf("pCopied: %i+%i+%i = sum:%i\n", pCopied.Getx(&pCopied), pCopied.Gety(&pCopied), pCopied.Getz(&pCopied),
			pCopied.base.sum((point *) & pCopied));

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

	// Polymorphism, cast a point3d object as class point
	pu = (point *) p;
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
	if (!ooTypeOf(pu, point)) {
		printf("ERROR pu must be a point instance.\n");
		return EXIT_FAILURE;
	}
	if (!ooTypeOf(p, point3d)) {
		printf("ERROR p must be a point3d instance.\n");
		return EXIT_FAILURE;
	}
	printf("OK typeOf\n");

	//list test ---------------------------
	point3d * l, * l2;
	point3d * iter;
	l = NULL;
	//add 10 obj to list.
	for (f=0; f<10; f++) {
		l2 = ooNew(point3d, l2, f + 1, 0, 0);
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
	point3d * l_ = l;
	ooListPop(l, iter);
	printf("%i poped\n", iter->Getx(iter));
	printf("List after Pop\n");
	ooListForEach(l, iter) {
		printf("%i in list \n", iter->Getx(iter));
	}
	ooDeleteFree(l_);
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

	ooTry {
		//TEST collections and iterators
		printf("TEST collections \n");
		pc = ooNew(pointColl, pc); // Create collection
		//load 20 items
		for (f=0; f<20; f++) {
			pu = ooNew(point, pu, f, 0);
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
	}
	ooCatchAny() {
		printError(&ooLastError());
	}
	ooTryEnd;

	return EXIT_SUCCESS;
}
