
/*
 * ooSimple.h
 *
 *  Created on: Jun 30, 2014
 *      Author: fcuri
 *
 *      oop4c object oriented toolkit for C in a simple included head.
 *
 */

#ifndef OOSIMPLE_H_
#define OOSIMPLE_H_

#include <string.h> //memset

//http://stackoverflow.com/questions/1489932/c-preprocessor-and-concatenation
#define REFERENCE(x) x
#define PASTER(x,y) x ## y
#define EVALUATOR(x,y)  PASTER(x,y)

//working with _ooClass when defined, used in postfixed "D" functions

#define __namestruct(_name) EVALUATOR(oo,_name)
#define __namenew(_name) EVALUATOR(_name, New)
#define __namedestruct(_name) EVALUATOR(_name, Destructor)
#define __namemethod(_name) EVALUATOR(_ooClass, _name)

#define __ooClass(_namestruct, _class) typedef struct _namestruct _class; \
	struct _namestruct {\
		int size; \
		void (*destroy)(_class *this);

#define __ooClassH(_namestruct, _class, _base) typedef struct _namestruct _class; \
	struct _namestruct {\
		_base base; \
		int size; \
		void (*destroy)(_class *this);

#define ooClass(_class) __ooClass(__namestruct(_class), _class)
#define ooClassH(_class, _base) __ooClassH(__namestruct(_class), _class, _base)

//Working with the _ooClass definition (D as postfix in every function)
#define ooClassD __ooClass(__namestruct(_ooClass), _ooClass)
#define ooClassDH(_base) __ooClassH(__namestruct(_ooClass), _ooClass, _base)

#define ooClassEnd };

//initialization
#define __ooInit(_class, _obj, _nameDestruct, _nameNew, _params...) \
		memset((_obj), 0, sizeof(_class)); \
		(_obj)->size = sizeof(_class); \
		(_obj)->destroy = &__namedestruct(_class);

#define ooInit(_class, _obj, _params...) \
		__ooInit(_class, _obj, __namedestruct(_class), __namenew(_class), _params);\
		__namenew(_class)(_obj, ##_params);
#define ooInitD(_obj, _params...)\
		__ooInit(_ooClass, _obj, __namedestruct(_ooClass), __namenew(_ooClass), _params);\
		__namenew(_ooClass)(_obj, ##_params);

#define ooNew(_class, _obj, _params...) \
	(_class *)malloc(sizeof(_class));\
	__ooInit(_class, _obj, __namedestruct(_class), __namenew(_class), _params);\
	__namenew(_class)(_obj, ##_params);
#define ooNewD(_obj, _params...) \
		(_ooClass *)malloc(sizeof(_ooClass));\
		__ooInit(_ooClass, _obj, __namedestruct(_ooClass), __namenew(_ooClass), _params);\
		__namenew(_ooClass)(_obj, ##_params);

//Declaraciones de constructor y destructor.
#define ooCtor(_class, _params...) void __namenew(_class)(_class *this, ##_params) //declarar el constructor
#define ooCtorD(_params...) void __namenew(_ooClass)(_ooClass *this, ##_params) //declarar el constructor
#define ooDtor(_class) void __namedestruct(_class)(_class *this) //declarar el destructor
#define ooDtorD() void __namedestruct(_ooClass)(_ooClass *this) //declarar el destructor

#define ooMethodName(_class, _name) _class##_name
#define ooMethodNameD(_name) __namemethod(_name)

//metodos (se ejecutan dentro del consutructor y en la declaracion)
#define ooMethodDeclare(_class, _name, _params...) (*_name)(_class *this,##_params) //declara un methodo en la definicion de la clase
#define ooMethodSet(_class, _name) this->_name = &ooMethodName(_class, _name) //asigna el metodo (durante el new del objeto)
#define ooMethod(_class, _name, _params...) ooMethodName(_class, _name)(_class *this, ##_params) //declara el metodo en el programa.

#define ooMethodDeclareD(_name, _params...) (*_name)(_ooClass *this, ##_params) //declara un methodo en la definicion de la clase
#define ooMethodSetD(_name) this->_name = &ooMethodNameD(_name) //asigna el metodo (durante el new del objeto)
#define ooMethodD(_name, _params...) ooMethodNameD(_name)(_ooClass *this, ##_params) //declara el metodo en el programa.

//To change parent's methods
//asigna el metodo (durante el new del objeto)
#define ooMethodSetOverride(_class, _name) this->base._name = &ooMethodName(_class, _name)
#define ooMethodOverride(_class, _classBase, _name, _params...) ooMethodName(_class, _name)(_classBase *base, ##_params) //declara el metodo en el programa.

#define ooMethodSetOverrideD(_name) this->base._name = &ooMethodNameD(_name)
#define ooMethodOverrideD(_classBase, _name, _params...) ooMethodNameD(_name)(_classBase *base, ##_params) //declara el metodo en el programa.

//declare 'this' in overrided method.
#define ooThisDeclare(_class) _class *this = (_class *)base
#define ooThisDeclareD() _ooClass *this = (_ooClass *)base

#define ooBase() (&this->base) //referencia al objeto heredado.
//declare 'base' as replacement for ooBase() in a method
#define ooBaseDeclare(_classBase) _classBase *base = (_classBase *)&this->base

//Messages
#define ooCall(_obj, _name, _params...) (_obj)->_name((_obj), ##params)
#define ooCallS(_obj, _name, _params...) (_obj)._name((_obj), ##params)
#define ooCallSuper(_obj, _name, _params...) (_obj)->base._name((_obj), ##params)
#define ooCallSuperS(_obj, _name, _params...) (_obj)->base._name((_obj), ##params)

//Destruccion
#define ooDelete(_obj) if ((_obj) && (_obj)->destroy) { (_obj)->destroy((_obj)); } //borrar un objeto.
#define ooDeleteFree(_obj) ooDelete(_obj); free(_obj); _obj=NULL

//clone
#define ooClone(_objd, _obj) memcpy(_objd, _obj, _obj->size);
#endif /* OOSIMPLE_H_ */
