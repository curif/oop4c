/*
 * ooSimple.h
 *
 *  Created on: Jun 30, 2014
 *      Author: fcuri
 *
 *      oop4c object oriented toolkit for C in a single included head.
 *
 */

#ifndef OOSIMPLE_H_
#define OOSIMPLE_H_

#include <string.h> //memset, strcmp, memcpy

//http://stackoverflow.com/questions/1489932/c-preprocessor-and-concatenation
//#define REFERENCE(x) x
#define PASTER(x,y) x ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define ASSTRING(x) #x

//working with _ooClass when defined, used in postfixed "D" functions
#define __namenew(_name) EVALUATOR(_name, New)
#define __namedestruct(_name) EVALUATOR(_name, Destructor)
#define __namemethod(_name) EVALUATOR(_ooClass, _name)

//Base object
#define __ooObjectBase(_class) unsigned int _cap; \
	void (*destroy)(_class *this);


#define __ooClass(_namestruct, _class) typedef struct _namestruct _class; \
	struct _namestruct {\
		__ooObjectBase(_namestruct)

#define __ooClassH(_namestruct, _class, _base) typedef struct _namestruct _class; \
	struct _namestruct {\
		_base base; \
		__ooObjectBase(_namestruct)


#define ooClass(_class) __ooClass(_class, _class)
#define ooClassH(_class, _base) __ooClassH(_class, _class, _base)
//Working with the _ooClass definition (D as postfix in every function)
#define ooClassD __ooClass(_ooClass, _ooClass)
#define ooClassDH(_base) __ooClassH(_ooClass, _ooClass, _base)

#define ooClassEnd };

//initialization
#define __ooInit(_class, _obj, _nameDestruct, _nameNew, _params...) \
		memset((_obj), 0, sizeof(_class)); \
		(_obj)->destroy = &__namedestruct(_class);

#define ooInit(_class, _obj, _params...) \
		__ooInit(_class, _obj, __namedestruct(_class), __namenew(_class), _params);\
		__namenew(_class)(_obj, ##_params);
#define ooInitD(_obj, _params...)\
		__ooInit(_ooClass, _obj, __namedestruct(_ooClass), __namenew(_ooClass), _params);\
		__namenew(_ooClass)(_obj, ##_params);

#define ooNew(_class, _obj, _params...) \
	(_class *)malloc(sizeof(_class));\
	if (_obj) { __ooInit(_class, _obj, __namedestruct(_class), __namenew(_class), _params);\
	__namenew(_class)(_obj, ##_params);}

#define ooNewD(_obj, _params...) \
		(_ooClass *)malloc(sizeof(_ooClass));\
		if (_obj) {	__ooInit(_ooClass, _obj, __namedestruct(_ooClass), __namenew(_ooClass), _params);\
		__namenew(_ooClass)(_obj, ##_params);}

//Declaraciones de constructor y destructor.
#define ooCtor(_class, _params...) void __namenew(_class)(_class *this, ##_params) //declare constructor
#define ooCtorD(_params...) void __namenew(_ooClass)(_ooClass *this, ##_params) //declare constructor
#define ooDtor(_class) void __namedestruct(_class)(_class *this) //declare destructor
#define ooDtorD() void __namedestruct(_ooClass)(_ooClass *this) //declare destructor

#define _ooMethodName(_class, _name) _class##_name
#define _ooMethodNameD(_name) __namemethod(_name)

//methods
#define ooMethodDeclare(_class, _name, _params...) (*_name)(_class *this,##_params) //declare a method in class definition
#define ooMethodSet(_class, _name) this->_name = &_ooMethodName(_class, _name) //assign a method (during constructor execution)
#define ooMethod(_class, _name, _params...) _ooMethodName(_class, _name)(_class *this, ##_params) //declare a method

#define ooMethodDeclareD(_name, _params...) (*_name)(_ooClass *this, ##_params)
#define ooMethodSetD(_name) this->_name = &_ooMethodNameD(_name)
#define ooMethodD(_name, _params...) _ooMethodNameD(_name)(_ooClass *this, ##_params)

//To change parent's methods
#define ooMethodSetOverride(_class, _name) this->base._name = &_ooMethodName(_class, _name) //override parent's method during constructor
#define ooMethodOverride(_class, _classBase, _name, _params...) _ooMethodName(_class, _name)(_classBase *base, ##_params) //declare an overrided method
#define ooMethodSetOverrideD(_name) this->base._name = &_ooMethodNameD(_name)
#define ooMethodOverrideD(_classBase, _name, _params...) _ooMethodNameD(_name)(_classBase *base, ##_params)

//declare 'this' in overrided method.
#define ooThisDeclare(_class) _class *this = (_class *)base
#define ooThisDeclareD() _ooClass *this = (_ooClass *)base

#define ooBase() (&this->base) //reference to parent
//declare 'base' as replacement for ooBase() in a method
#define ooBaseDeclare(_classBase) _classBase *base = (_classBase *)&this->base

//Messages
#define ooCall(_obj, _name, _params...) (_obj)->_name((_obj), ##params)
#define ooCallS(_obj, _name, _params...) (_obj)._name((_obj), ##params)
#define ooCallSuper(_obj, _name, _params...) (_obj)->base._name((_obj), ##params)
#define ooCallSuperS(_obj, _name, _params...) (_obj)->base._name((_obj), ##params)

//Destruccion
#define ooDelete(_obj) if ((_obj) && (_obj)->destroy) { (_obj)->destroy((_obj)); } //object destruction call
#define ooDeleteFree(_obj) ooDelete(_obj); free(_obj); _obj=NULL //object destruction call and free mem

//-----------
//Implements methods and properties
//Clonable, Typeable, Comparable, Copyable and Listable.
//Clonable: object can make copies from itself using the clone method.
//Copiable: object con copy its properties over other object of same class (copy method)
//Comparable: two objects of same class can compare (compare method)
//Typeable: a program can know the class of a object.
//Listable: a object can contain a list of other objects of same class.
//Use the ooImpXXX macros during class declaration (ooImpXXXD as well)
//Use the ooXXXX macros during constructor execution.

//object implements...
#define __ooCapClonable 1
#define __ooCapTypeable (1<<1)
#define __ooCapComparable (1<<2)
#define __ooCapCopyable (1<<3)
#define __ooCapListable (1<<4)

#define ooIsClonable(_obj) (((_obj)->_cap & __ooCapClonable) && (_obj)->clone)
#define ooIsTypeable(_obj) ((_obj)->_cap & __ooCapTypeable)
#define ooIsComparable(_obj) (((_obj)->_cap & __ooCapComparable) && (_obj)->compare)
#define ooIsCopyable(_obj) (((_obj)->_cap & __ooCapCopyable) && (_obj)->copy)
#define ooIsListable(_obj) (((_obj)->_cap & __ooCapListable))

//implements comparable, add method compare (must return 1 if the objects are equals)
#define ooImpComparable(_class) int ooMethodDeclare(_class, compare, _class *compareto)
#define ooImpComparableD() ooImpComparable(_ooClass)
#define ooComparable(_class, _method) this->_cap |= __ooCapComparable; this->compare = _ooMethodName(_class, _method)
#define ooComparableD(_method) ooComparable(_ooClass, _method)
//shortcut:
#define ooCompare(_obj, _obj2) (ooIsComparable(_obj)? (_obj)->compare((_ojb2)) : 0)

//implements clonable, add method clone and property _size
#define ooImpClonable(_class) int _size; _class *ooMethodDeclare(_class, clone)
#define ooImpClonableD() ooImpClonable(_ooClass)
#define ooClonable(_class, _method) this->_cap |= __ooCapClonable; this->_size = sizeof(_class); this->clone = _ooMethodName(_class, _method)
#define ooClonableD(_method) ooClonable(_ooClass, _method)
//alternative to obj->clone(obj2)
#define ooSize(_obj) (ooIsClonable(_obj) ? (_obj)->_size : 0)
#define ooClone(_objd, _obj) if (ooIsClonable(_obj)) {memcpy(_objd, _obj, _obj->_size)}

//implements typeable, add property _type
#define ooImpTypeable(_class) const char *_type
#define ooImpTypeableD() ooImpTypeable(_ooClass)
#define ooTypeable(_class) this->_cap |= __ooCapTypeable; this->_type = ASSTRING(_class)
#define ooTypeableD() ooTypeable(_ooClass)
#define ooType(_obj) (_obj)->_type //object type, return class name
//compare objects class name
#define ooTypeOf(_obj, _class) ((_obj)->_cap & __ooCapTypeable? strcmp(ooType(_obj), ASSTRING(_class))==0 : 0)

//implements typeable, add method copy
#define ooImpCopiable(_class) void ooMethodDeclare(_class, copy, _class *tocopy)
#define ooImpCopiableD() ooImpCopiable(_ooClass)
#define ooCopiable(_class, _method) this->_cap |= __ooCapCopyable; this->copy = _ooMethodName(_class, _method)
#define ooCopiableD(_method) ooCopiable(_ooClass, _method)

//lists
#define ooImpListable(_class) _class *_prev, *_next
#define ooImpListableD() ooImpListable(_ooClass)
#define ooListable(_class) this->_cap |= __ooCapListable
#define ooListableD() ooListable(_ooClass)
#define ooListPush(_objFirst, _objToAdd) \
	if (!_objFirst) {\
			_objFirst =_objToAdd;\
		}\
		else {\
			_objToAdd->_next = _objFirst;\
			_objFirst->_prev = _objToAdd;\
			_objFirst =_objToAdd;\
		}
#define ooListPop(_objFirst, _objResult) \
		_objResult = _objFirst; _objFirst = _objFirst->_next; ooListRemove(_objResult)

#define ooListAdd(_objWhere, _objToAdd) \
		if (!_objWhere) {\
			_objWhere =_objToAdd;\
		}\
		else {\
			if (_objWhere->_next) {\
				(_objWhere)->_next->_prev = _objToAdd;\
			} \
			(_objToAdd)->_next = (_objWhere)->_next;\
			(_objToAdd)->_prev = (_objWhere);\
			(_objWhere)->_next = _objToAdd;\
		}
#define ooListRemove(_obj) \
	if ((_obj)->_next) { \
		(_obj)->_next->_prev = (_obj)->_prev;\
	}\
	if ((_obj)->_prev) { \
		(_obj)->_prev->_next = (_obj)->_next;\
	}
#define ooListIsFirst(_obj) ((_obj)->_prev == NULL)
#define ooListIsLast(_obj) ((_obj)->_next == NULL)
#define ooListIsEmpty(_obj) ((_obj) && (_obj)->_next == NULL && (_obj)->_prev == NULL)
#define ooListNext(_obj) ((_obj)->_next)
#define ooListPrev(_obj) ((_obj)->_prev)
#define ooListForEach(_obj, _iter) for (_iter = (_obj); _iter; _iter = (_iter)->_next)
#define ooListFirst(_obj, _iter) _iter = (_obj); while (!ooListIsFirst(_iter)) {_iter = (_iter)->_prev;}
#define ooListLast(_obj, _iter) _iter = (_obj); while (!ooListIsLast(_iter)) {_iter = (_iter)->_next;}

#endif /* OOSIMPLE_H_ */
