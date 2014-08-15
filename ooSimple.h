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
#include <stdlib.h> //malloc, free
#include <setjmp.h> //error handling
#include <stdio.h> //stderr

//http://stackoverflow.com/questions/1489932/c-preprocessor-and-concatenation
//#define REFERENCE(x) x
#define __concat(_x,_y) _x##_y
#define __evaluator(_x,_y)  __concat(_x,_y)
#define __asString(_x) #_x


#define ooObj void*
#define ooBoolean char
#define ooFalse 0
#define ooTrue 1

//working with _ooClass when defined, used in postfixed "D" functions

#define __namenew(_name) __evaluator(_name, New)
#define __namedestruct(_name) __evaluator(_name, Destructor)
#define __namemethod(_name) __evaluator(_ooClass, _name)

//Base object
#define __ooObjectBase(_class) \
	unsigned int _cap; \
	void (*destroy)(_class *this);

#define __ooClass(_namestruct, _class) \
	typedef struct _namestruct _class; \
	struct _namestruct {

#define __ooClassH(_namestruct, _class, _base) typedef struct _namestruct _class; \
	struct _namestruct {\
		_base base;

//--------------------------------------------

#define ooClass(_class) __ooClass(_class, _class)
#define ooClassH(_class, _base) __ooClassH(_class, _class, _base)
//Working with the _ooClass definition (D as postfix in every function)
#define ooClassD __ooClass(_ooClass, _ooClass)
#define ooClassDH(_base) __ooClassH(_ooClass, _ooClass, _base)

#define ooClassEnd(_class) \
		__ooObjectBase(_class)\
	};
#define ooClassEndD ooClassEnd(_ooClass)

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
#define ooMethodInit(_class, _name) this->_name = &_ooMethodName(_class, _name) //assign a method (during constructor execution)
#define ooMethod(_class, _name, _params...) _ooMethodName(_class, _name)(_class *this, ##_params) //declare a method

#define ooMethodDeclareD(_name, _params...) (*_name)(_ooClass *this, ##_params)
#define ooMethodInitD(_name) this->_name = &_ooMethodNameD(_name)
#define ooMethodD(_name, _params...) _ooMethodNameD(_name)(_ooClass *this, ##_params)

//To change parent's methods
#define ooMethodInitOverride(_class, _name) this->base._name = &_ooMethodName(_class, _name) //override parent's method during constructor
#define ooMethodOverride(_class, _classBase, _name, _params...) _ooMethodName(_class, _name)(_classBase *base, ##_params) //declare an overrided method
#define ooMethodInitOverrideD(_name) this->base._name = &_ooMethodNameD(_name)
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
#define ooDeleteStatic(_obj) if ((_obj).destroy) { (_obj).destroy((&_obj)); } //object destruction call static object
#define ooDeleteFree(_obj) ooDelete(_obj); free(_obj); _obj=NULL //object destruction call and free mem

//setters and getters properties -----------------------------------
#define __namePropSetDecl(_varName) __evaluator(Set, _varName)
#define __namePropGetDecl(_varName) __evaluator(Get, _varName)
#define __namePropSet(_class, _varName) __evaluator(_class, __evaluator(Set, _varName))
#define __namePropGet(_class, _varName) __evaluator(_class, __evaluator(Get, _varName))
//During class declaration
#define ooPropertyGetDeclare(_class, _type, _name) _type ooMethodDeclare(_class, __namePropGetDecl(_name))
#define ooPropertySetDeclare(_class, _type, _name) void ooMethodDeclare(_class, __namePropSetDecl(_name), _type _name)
#define ooPropertyDeclare(_class, _type, _name)  \
	ooPropertyGetDeclare(_class, _type, _name);\
	ooPropertySetDeclare(_class, _type, _name)
#define ooPropertyGetDeclareD(_type, _name)  ooPropertyGetDeclare(_ooClass, _type, _name)
#define ooPropertySetDeclareD(_type, _name)  ooPropertySetDeclare(_ooClass, _type, _name)
#define ooPropertyDeclareD(_type, _name)  ooPropertyDeclare(_ooClass, _type, _name)

//During class initialization
#define ooPropertySetInit(_class, _name) ooMethodInit(_class, __namePropSetDecl(_name))
#define ooPropertyGetInit(_class, _name) ooMethodInit(_class, __namePropGetDecl(_name))
#define ooPropertyInit(_class, _name) \
	ooPropertySetInit(_class, _name);\
	ooPropertyGetInit(_class, _name)
#define ooPropertyInitD(_name) ooPropertyInit(_ooClass, _name)
#define ooPropertySetInitD(_name) ooPropertySetInit(_ooClass, _name)
#define ooPropertyGetInitD(_name) ooPropertyGetInit(_ooClass, _name)

#define ooPropertySetInitOverride(_class, _name) ooMethodInitOverride(_class, __namePropSetDecl(_name))
#define ooPropertyGetInitOverride(_class, _name) ooMethodInitOverride(_class, __namePropGetDecl(_name))
#define ooPropertyInitOverride(_class, _name) \
	ooPropertySetInitOverride(_class, _name);\
	ooPropertyGetInitOverride(_class, _name)
#define ooPropertyInitOverrideD(_name) ooPropertyInitOverride(_ooClass, _name)


#define ooPropertyGet(_class, _type, _name)  \
		_type ooMethod(_class, __namePropGetDecl(_name))
#define ooPropertySet(_class, _type, _name)  \
		void ooMethod(_class, __namePropSetDecl(_name), _type _name)

#define ooPropertyGetD(_type, _name)  ooPropertyGet(_ooClass, _type, _name)
#define ooPropertySetD(_type, _name)  ooPropertySet(_ooClass, _type, _name)

#define ooPropertyGetOverride(_class, _classBase, _type, _name) \
	_type ooMethodOverride(_class, _classBase, __namePropGetDecl(_name))
#define ooPropertyGetOverrideD(_classBase, _type, _name) \
	ooPropertyGetOverride(_ooClass, _classBase, _type, _name)
#define ooPropertySetOverride(_class, _classBase, _type, _name)  \
	void ooMethodOverride(_class, _classBase, __namePropSetDecl(_name), _type _name)
#define ooPropertySetOverrideD(_classBase, _type, _name)  \
	ooPropertySetOverride(_ooClass, _classBase, _type, _name)



//-----------
//Implements methods and properties
//Clonable, Typeable, Comparable, Copyable and Listable.
//Clonable: object can make copies from itself using the clone method.
//Copiable: object con copy its properties over other object of same class (copy method)
//Comparable: two objects of same class can compare (compare method)
//Typeable: a program can know the class of a object.
//Listable: a object can contain a list of other objects of same class.
//Iterable: a iterator object
//Stringable: Implements ToString method
//Serializable: Implements the Serializable pattern

//Use the ooImpXXX macros during class declaration (ooImpXXXD as well)
//Use the ooXXXX macros during constructor execution.

//object implements...
#define __ooCapClonable 1
#define __ooCapTypeable (1<<1)
#define __ooCapComparable (1<<2)
#define __ooCapCopyable (1<<3)
#define __ooCapListable (1<<4)
#define __ooCapIterable (1<<5)
#define __ooCapAgregable (1<<6)
#define __ooCapStringable (1<<7)
#define __ooCapSerializable (1<<8)

//is...
#define ooIsClonable(_obj) (((_obj)->_cap & __ooCapClonable) && (_obj)->clone)
#define ooIsTypeable(_obj) ((_obj)->_cap & __ooCapTypeable)
#define ooIsComparable(_obj) (((_obj)->_cap & __ooCapComparable) && (_obj)->compare)
#define ooIsCopyable(_obj) (((_obj)->_cap & __ooCapCopyable) && (_obj)->copy)
#define ooIsListable(_obj) (((_obj)->_cap & __ooCapListable))
#define ooIsIterable(_obj) (((_obj)->_cap & __ooCapIterable))
#define ooIsAgregable(_obj) (((_obj)->_cap & __ooCapAgregable))
#define ooIsSerializable(_obj) (((_obj)->_cap & __ooCapSerializable))

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
#define ooTypeable(_class) this->_cap |= __ooCapTypeable; this->_type = __asString(_class)
#define ooTypeableD() ooTypeable(_ooClass)
#define ooType(_obj) (_obj)->_type //object type, return class name
//compare objects class name
#define ooTypeOf(_obj, _class) ((_obj)->_cap & __ooCapTypeable? strcmp(ooType(_obj), __asString(_class))==0 : 0)

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
#define ooListNext(_obj) ((_obj)->_next)
#define ooListPrev(_obj) ((_obj)->_prev)
#define ooListIsFirst(_obj) (ooListPrev(_obj) == NULL)
#define ooListIsLast(_obj) (ooListNext(_obj) == NULL)
#define ooListIsEmpty(_obj) (!(_obj) || ((_obj)->_next == NULL && (_obj)->_prev == NULL))
#define ooListForEach(_obj, _iter) for (_iter = (_obj); _iter; _iter = ooListNext(_iter))
#define ooListFirst(_obj, _iter) _iter = (_obj); while (!ooListIsFirst(_iter)) {_iter = ooListPrev(_iter);}
#define ooListLast(_obj, _iter) _iter = (_obj); while (!ooListIsLast(_iter)) {_iter = ooListNext(_iter);}

//iterator
#define ooImpIterator(_class, _classAgregator) \
		ooPropertyGetDeclare(_class, _classAgregator *, Next);\
		ooPropertyGetDeclare(_class, _classAgregator *, Current);\
		ooPropertyGetDeclare(_class, ooBoolean, HasNext);
#define ooImpIteratorD() ooImpIterator(_ooClass)
#define ooIterator(_class) this->_cap |= __ooCapIterable;\
	ooPropertyGetInit(_class, Next);\
	ooPropertyGetInit(_class, HasNext);\
	ooPropertyGetInit(_class, Current)
#define ooIteratorD() ooIterator(_ooClass)
#define ooIterForEach(_iter, _coll) \
	for (_iter = (_coll)->GetIterator((_coll)); (_iter)->GetHasNext((_iter)); (_iter)->GetNext(_iter))

//Agregable (collections)
#define ooImpAgregator(_class, _classAgregatorFrom) \
		ooPropertyGetDeclare(_class, ooObj, Iterator);\
		ooPropertyGetDeclare(_class, int, Count);\
		_class *ooMethodDeclare(_class, Add, _classAgregatorFrom *obj);\
		ooBoolean ooMethodDeclare(_class, Remove, _classAgregatorFrom *obj);\
		_classAgregatorFrom *ooMethodDeclare(_class, Index, int idx);
#define ooImpAgregatorD(_classAgregatorFrom) ooImpAgregator(_ooClass, _classAgregatorFrom)
#define ooAgregator(_class) this->_cap |= __ooCapAgregable;\
		ooPropertyGetInit(_class, Iterator);\
		ooPropertyGetInit(_class, Count);\
		ooMethodInit(_class, Add);\
		ooMethodInit(_class, Remove);\
		ooMethodInit(_class, Index)
#define ooAgregatorD() ooAgregator(_ooClass)

//Stringable
#define ooImpStringable(_class) char *ooMethodDeclare(_class, ToString, char*buf, int lenBut)
#define ooImpStringableD() ooImpStringable(_ooClass)
#define ooStringable(_class) this->_cap |= __ooCapStringable;\
	ooMethodInit(_class, ToString)
#define ooStringableD() ooStringable(_ooClass)

#define ooImpSerializable(_class) \
	char *ooMethodDeclare(_class, Serialize, long  *lenBuf);\
	ooBoolean ooMethodDeclare(_class, Deserialize, char *buf, long lenBuf);
#define ooImpSerializableD() ooImpSerializable(_ooClass)
#define ooSerializable(_class) this->_cap |= __ooCapSerializable;\
	ooMethodInit(_class, Serialize);\
	ooMethodInit(_class, Deserialize);
#define ooSerializableD() ooSerializable(_ooClass)


// ERROR HANDLING ----------------------------------------

/**
 * One jump buf environment for the stack.
 */
struct _ooExcepEnvStack {
	jmp_buf env;
	struct _ooExcepEnvStack *next;
};

/**
 * An error.
 */
typedef struct ooExcepError {
	int no; //setjmp error number
	const char *id; //error ID
	const char *function; //function caller
	const char *file; //file (program.c)
	int line; //line number in the file
	const char *desc; //A user description error.
} ooExcepError;

//reentrant versions: postfix "R"


/**
 * Exception environment
 */
struct ooExcepEnv {
	struct _ooExcepEnvStack *stack; //list of jumps, can be null.
	ooExcepError lastError; //last error raised
	void (*unhandled)(ooExcepError *e); //unhandled manager function (can be null)
};

#define __ooErrName(_id)  __evaluator(__ooErrDecl, _id)

#define __ooExcepEnvSize() sizeof(struct ooExcepEnv)
#define __ooExcepEnvStackSize() sizeof(struct _ooExcepEnvStack)
//obtains the first environment from the stack (or the env from init for unhandled call).
//#define __ooSetjmpEnv(_env) ((_env)->stack? (_env)->stack->env : (_env)->env)
#define __ooSetjmpEnv(_env) ((_env)->stack->env)

//push a new environment to the stack
#define __ooTmpStackName()  __evaluator(_st_, __LINE__)
#define __ooPushExcep(_env) \
		static struct _ooExcepEnvStack __ooTmpStackName(); \
		__ooTmpStackName().next = (_env)->stack;\
		(_env)->stack = &__ooTmpStackName();
#define __ooPopExcep(_env) \
			(_env)->stack = (_env)->stack->next
#define __Jmp(_env, _error) longjmp(__ooSetjmpEnv(_env), _error.no)

#define __ooExcepInitR(_env, _unhandledfunc)  \
		static struct _ooExcepEnvStack __ooTmpStackName(); \
		__ooTmpStackName().next = NULL; \
		memset(_env, 0, __ooExcepEnvSize()); \
		(_env)->stack = &__ooTmpStackName();\
		(_env)->unhandled = _unhandledfunc;\
		if (setjmp(__ooTmpStackName().env)!=0) { \
			if ((_env)->unhandled) {\
				(_env)->unhandled(&(_env)->lastError);\
			}\
			return ((_env)->lastError.no);\
		}


#define ooExcepDefineR(_env) struct ooExcepEnv *_env
#define ooExcepUseR(_env) extern ooExcepDefineR(_env)

#define __ooRaiseR(_env, _error) \
		memcpy(&(_env)->lastError, &_error, sizeof(struct ooExcepError));\
		(_env)->lastError.file = __FILE__; \
		(_env)->lastError.line = __LINE__; \
		(_env)->lastError.function = __func__; \
		__Jmp(_env, (_env)->lastError)

//show a message to stderr before raise.
#define __ooRaiseMsgR(_env, _error, _msg, _rest...) \
		fprintf(stderr, _msg, ##_rest); __ooRaiseR(_env, _error)

#define ooRaiseMsgR(_env, _errname, _msg, _rest...) __ooRaiseMsgR(_env, __ooErrName(_errname), _msg,  ##_rest)
#define ooRaiseMsgIfR(_env, _cond, _errname, _msg, _rest...) if (_cond) {__ooRaiseMsgR(_env, __ooErrName(_errname), _msg,  ##_rest);}
#define ooRaiseMsgAnotherR(_env, _error, _msg, _rest...) __ooPopExcep(_env); __ooRaiseMsgR(_env, _error, _msg,  ##_rest)

#define ooRaiseR(_env, _errname) __ooRaiseR(_env, __ooErrName(_errname))
#define ooRaiseIfR(_env, _cond, _errname) if (_cond) {__ooRaiseR(_env, __ooErrName(_errname));}
#define ooRaiseAnotherR(_env, _error) __ooPopExcep(_env); __ooRaiseR(_env, _error)

//#define ooRaiseThrowR(_env) ooRaiseAnotherR(_env, (_env)->lastError)
#define ooRaiseThrowR(_env) __ooPopExcep(_env); __Jmp(_env, (_env)->lastError)

#define ooTryR(_env) { __ooPushExcep(_env);  \
	int _ooerrno = setjmp((_env)->stack->env); \
	char _oocatch = 0;\
	if (!_ooerrno)
#define ooTryEndR(_env)  \
	__ooPopExcep(_env); \
	if (!_oocatch && _ooerrno) {\
		__Jmp(_env, (_env)->lastError);\
	}\
	}
//#define ooTryEndR(_env)  __ooPopExcep(_env); _ooerrno=0;}
#define ooCatchR(_errname) if (_ooerrno == __ooErrName(_errname).no? _oocatch=1 : 0)
#define ooCatchAnyR() if (_ooerrno? _oocatch=1 : 0)
#define ooExcepInitR(_env, _unhandledfunc) _env = malloc(__ooExcepEnvSize()); __ooExcepInitR(_env)
#define ooLastErrorR(_env) ((_env)->lastError)

//Non reentrant functions (using ooExcep struct):

//Define error manager in the main program:
#define ooExcepDefine() struct ooExcepEnv ooExcep
//Use the error manager in others programs
#define ooExcepUse() extern ooExcepDefine()
//Initialize the error manager (usually in main())
#define ooExcepInit(_unhandledfunc) __ooExcepInitR(&ooExcep, _unhandledfunc)

//Try / Catch
#define ooTry ooTryR(&ooExcep)
#define ooTryEnd  ooTryEndR(&ooExcep)
#define ooCatch(_err) ooCatchR(_err)
#define ooCatchAny() ooCatchAnyR()
//Last error number raiced
#define ooErrNo() (_ooerrno)
//Last &ooExcepError
#define ooLastError() ooLastErrorR(&ooExcep)

#define ooRaiseMsg(_errname, _msg, _rest...) ooRaiseMsgR(&ooExcep, _errname, _msg,  ##_rest)
#define ooRaiseMsgIf(_cond, _errname, _msg, _rest...)  ooRaiseMsgIfR(&ooExcep, _cond, _errname, _msg,  ##_rest)
#define ooRaiseMsgAnother(_errname, _msg, _rest...) ooRaiseMsgAnotherR(&ooExcep, __ooErrName(_errname), _msg,  ##_rest)

#define ooRaise(_errname) ooRaiseR(&ooExcep, _errname)
#define ooRaiseIf(_cond, _errname)  ooRaiseIfR(&ooExcep, _cond, _errname)
//Raise a different error inside the try/catch block to the function caller.
#define ooRaiseAnother(_errname) ooRaiseAnotherR(&ooExcep, __ooErrName(_errname))

//Raise the same error inside the try/catch block to the function caller.
#define ooRaiseThrow() ooRaiseThrowR(&ooExcep)

//Declare an error
#define ooExcepDeclareError(_err, _no, _desc) static const ooExcepError __ooErrName(_err) = { _no, #_err, "Undefined", "Undefined", 0, _desc}

//Common error declarations:
ooExcepDeclareError(Malloc, -1, "Out of Memory");
ooExcepDeclareError(InvIndex, -2, "Invalid Index");

#endif /* OOSIMPLE_H_ */
