/*
 * ootest.h
 *
 *  Created on: Jul 1, 2014
 *      Author: curif
 */

#ifndef OOTEST_H_
#define OOTEST_H_

#include "ooSimple.h"

#undef _ooClass
#define _ooClass punto

//punto definition
ooClassD

//implements...
ooImpTypeableD();

//properties
int x;
int y;

//methods
int ooMethodDeclareD(sum);

ooClassEnd

//punto3d definition, subclass of punto
ooClassH(punto3d, punto)

//implements....
ooImpClonable(punto3d);
ooImpTypeable(punto3d);
ooImpCopiable(punto3d);
ooImpComparable(punto3d);
ooImpListable(punto3d);

//properties
int z;

//methods
int ooMethodDeclare(punto3d, getx);
int ooMethodDeclare(punto3d, gety);
int ooMethodDeclare(punto3d, getz);
void ooMethodDeclare(punto3d, setx, int px);
void ooMethodDeclare(punto3d, sety, int py);
void ooMethodDeclare(punto3d, setz, int pz);
void ooMethodDeclare(punto3d, setxy, int px, int py);

ooClassEnd



#endif /* OOTEST_H_ */
