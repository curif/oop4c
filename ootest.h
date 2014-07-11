/*
 * ootest.h
 *
 *  Created on: Jul 1, 2014
 *      Author: fcuri
 */

#ifndef OOTEST_H_
#define OOTEST_H_

#include "config.h"
#include "rutinas.h"
#include "ooSimple.h"

#undef _ooClass
#define _ooClass punto

//punto definition
ooClassD
char st[1024];
int x;
int y;
int ooMethodDeclareD(suma);
ooClassEnd

ooClassH(punto3d, punto)
//propiedades
int z;
//methodos
int ooMethodDeclare(punto3d, getx);
int ooMethodDeclare(punto3d, gety);
int ooMethodDeclare(punto3d, getz);
void ooMethodDeclare(punto3d, setx, int px);
void ooMethodDeclare(punto3d, sety, int py);
void ooMethodDeclare(punto3d, setz, int pz);
void ooMethodDeclare(punto3d, setxy, int px, int py);
ooClassEnd

ooClass(another)
int x;
ooClassEnd

#endif /* OOTEST_H_ */
