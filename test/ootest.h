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
#define _ooClass punto

//punto definition
ooClassD

//implements...
ooImpTypeableD();
ooImpStringableD();

//properties (define Get & Set - puntoGetx, puntoSetx
int x;
int y;
ooPropertyDeclareD(int, x);
ooPropertyDeclareD(int, y);

//methods
int ooMethodDeclareD(sum);

ooClassEndD

//punto3d definition, subclass of punto
ooClassH(punto3d, punto)

//implements....
ooImpClonable(punto3d);
ooImpTypeable(punto3d);
ooImpCopiable(punto3d);
ooImpComparable(punto3d);
ooImpListable(punto3d);
ooImpStringable(punto3d);

//properties
int z;
//getters & setters
ooPropertyDeclare(punto3d, int, x);
ooPropertyDeclare(punto3d, int, y);
ooPropertyDeclare(punto3d, int, z);

ooClassEnd(punto3d)

//collection of punto ---------------------------------------
ooClass(puntoColl)

ooImpAgregator(puntoColl, punto); //Add, Remove, Index, GetIterator

punto **arr;
int count;
int len;

ooClassEnd(puntoColl)

//iterator over puntoColl -----------------
ooClass(puntoIter)

ooImpIterator(puntoIter, punto);  //Next, HasNext

puntoColl *coll;
int idxCurrent;

ooClassEnd(puntoIter)

ooCtor(puntoColl);
ooDtor(puntoColl);
ooCtor(puntoIter, puntoColl *coll);
ooDtor(puntoIter);

#endif /* OOTEST_H_ */
