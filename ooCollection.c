/*
 * ooCollection.c
 *
 *  Created on: Aug 7, 2014
 *      Author: fcuri
 */


#include "ooCollection.h"


//Get Iterator
ooPropertyGetD(ooObj, Iterator) {
	oCollIterator *i = ooNew(oCollIterator, i, this);
	ooRaiseIf(!i, Malloc);

	return(i);
}
//Add element
ooCollection *ooMethodD( Add, void *o) {
	if (!this->arr || this->count + 1 > this->len) {
		void *new = realloc(this->arr, (this->len+10) * sizeof(void*));
		ooRaiseIf(!new, Malloc);
		this->len += 10;
		this->arr = new;
	}
	this->arr[this->count] = o;
	this->count++;

	return(this);
}
//Remove element
ooBoolean ooMethodD(Remove, void *o) {
	int f, g, found=-1;
	if (!this->count) {
		return(ooFalse);
	}
	for (f=0; f<this->count; f++) {
		if (o == this->arr[f]) {
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
	return(found >= 0);
}
//Get Count
ooPropertyGetD( int, Count) {
	return(this->count);
}
//Get an element
void *ooMethodD(Index, int idx) {
	if (idx > this->count - 1) {
		return(NULL);
	}
	return(this->arr[idx]);
}
//Constructor
ooCtorD() {
	ooAgregatorD();
	return;
}
//Destructor
ooDtorD() {
	if (this->arr) {
		free(this->arr);
	}
	return;
}


//Iterator -----------------


//Has next element?
ooPropertyGet(oCollIterator, ooBoolean, HasNext) {
	if (!this->coll) {
		return(ooFalse);
	}
	return(this->idxCurrent < this->coll->GetCount(this->coll));
}
//Get Next Element
ooPropertyGet(oCollIterator, void *, Next) {
	if (!this->GetHasNext(this)) {
		return NULL;
	}
	this->idxCurrent++;
	return this->GetCurrent(this);
}
//Get current element
ooPropertyGet(oCollIterator, void *, Current) {
	return(this->coll->Index(this->coll, this->idxCurrent));
}

ooCtor(oCollIterator, ooCollection *coll) {
	ooIterator(oCollIterator);
	if (ooIsAgregable(coll)) {
		this->coll = coll;
	}
}

ooDtor(oCollIterator) {
	this = 0;
	return;
}
