/*
 * ooCollection.h
 *
 *  Created on: Aug 7, 2014
 *      Author: fcuri
 */

#ifndef OOCOLLECTION_H_
#define OOCOLLECTION_H_

#include "ooSimple.h"

ooExcepUse();

/**
 * Generic collection storage
 */
#undef _ooClass
#define _ooClass ooCollection
ooClassD
	ooImpAgregatorD(void);
	void **arr;
	int count;
	int len;
ooClassEndD

ooCtorD();
ooDtorD();

/**
 * Iterator over ooCollection
 */
ooClass(oCollIterator)

	ooImpIterator(oCollIterator, void);  //Next, HasNext

	ooCollection *coll;
	int idxCurrent;

ooClassEnd(oCollIterator)

ooCtor(oCollIterator, ooCollection *coll);
ooDtor(oCollIterator);

#endif /* OOCOLLECTION_H_ */
