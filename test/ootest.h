/*
 * ootest.h
 *
 *  Created on: Jul 1, 2014
 *      Author: curif
 */

#ifndef OOTEST_H_
#define OOTEST_H_

#include "../ooSimple.h"


#undef _ooClass
#define _ooClass point

// point definition
ooClassD

//implements...
ooImpTypeableD();
ooImpStringableD();

// properties (define Get & Set - pointGetx, pointSetx
int x;
int y;
ooPropertyDeclareD(int, x);
ooPropertyDeclareD(int, y);

//methods
int ooMethodDeclareD(sum);

ooClassEndD

// point3d definition, subclass of point
ooClassH(point3d, point)

//implements....
ooImpClonable(point3d);
ooImpTypeable(point3d);
ooImpCopiable(point3d);
ooImpComparable(point3d);
ooImpListable(point3d);
ooImpStringable(point3d);

//properties
int z;
//getters & setters
ooPropertyDeclare(point3d, int, x);
ooPropertyDeclare(point3d, int, y);
ooPropertyDeclare(point3d, int, z);

ooClassEnd(point3d)

// collection of points ---------------------------------------
ooClass(pointColl)

ooImpAgregator(pointColl, point); // Add, Remove, Index, GetIterator

point * * arr;
int count;
int len;

ooClassEnd(pointColl)

// iterator over pointColl -----------------
ooClass(pointIter)

ooImpIterator(pointIter, point);  // Next, HasNext

pointColl * coll;
int idxCurrent;

ooClassEnd(pointIter)

ooCtor(pointColl);
ooDtor(pointColl);
ooCtor(pointIter, pointColl * coll);
ooDtor(pointIter);

#endif /* OOTEST_H_ */
