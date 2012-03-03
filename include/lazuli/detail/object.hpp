#ifndef LAZULI_DETAIL_OBJECT_HPP
#define LAZULI_DETAIL_OBJECT_HPP

#include <lazuli/detail/releasehook.hpp>
#include <string>
#include <squirrel.h>

namespace lazuli { namespace detail {

namespace {

template <class X>
static inline void copy(::HSQUIRRELVM hvm, const void* value) throw()
{
  X* instance = new X(*static_cast<const X*>(value));
  ::sq_setinstanceup(hvm, 1, reinterpret_cast< ::SQUserPointer>(instance));
  ::sq_setreleasehook(hvm, 1, releasehook<X>);
}

} // namespace unnamed

template <class T>
struct object
{
  static const ::SQChar typemask = _SC('y');
  T value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    T* ptr = 0;
    ::sq_getinstanceup(hvm, idx, reinterpret_cast< ::SQUserPointer*>(&ptr), 0);
    this->value = *ptr;
  }
  static void push(::HSQUIRRELVM hvm, T value) throw()
  {
    copy<T>(hvm, &value);
  }
};

template <class T>
struct object<T&>
{
  static const ::SQChar typemask = _SC('y');
  T value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    T* ptr = 0;
    ::sq_getinstanceup(hvm, idx, reinterpret_cast< ::SQUserPointer*>(&ptr), 0);
    this->value = *ptr;
  }
  static void push(::HSQUIRRELVM hvm, T value) throw()
  {
    copy<T>(hvm, &value);
  }
};

template <class T>
struct object<const T&>
{
  static const ::SQChar typemask = _SC('y');
  T value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    T* ptr = 0;
    ::sq_getinstanceup(hvm, idx, reinterpret_cast< ::SQUserPointer*>(&ptr), 0);
    this->value = *ptr;
  }
  static void push(::HSQUIRRELVM hvm, T value) throw()
  {
    copy<T>(hvm, &value);
  }
};

template <class T>
struct object<T*>
{
  static const ::SQChar typemask = _SC('y');
  T* value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    ::sq_getinstanceup(hvm, idx, reinterpret_cast< ::SQUserPointer*>(&this->value), 0);
  }
  static void push(::HSQUIRRELVM hvm, T* value) throw()
  {
    ::sq_setinstanceup(hvm, -1, value);
  }
};

template <class T>
struct object<const T*>
{
  static const ::SQChar typemask = _SC('y');
  T* value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    ::sq_getinstanceup(hvm, idx, reinterpret_cast< ::SQUserPointer*>(&this->value), 0);
  }
  static void push(::HSQUIRRELVM hvm, T* value) throw()
  {
    ::sq_setinstanceup(hvm, -1, value);
  }
};

#define __SCRIPT_VAR_INTEGER(T) \
template <> \
struct object<T> \
{ \
  static const ::SQChar typemask = _SC('i'); \
  T value; \
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw() \
  { \
    ::SQInteger i; \
    ::sq_getinteger(hvm, idx, &i); \
    this->value = static_cast<T>(i); \
  } \
  static inline void push(::HSQUIRRELVM hvm, T value) throw() \
  { \
    ::sq_pushinteger(hvm, static_cast< ::SQInteger>(value)); \
  } \
};
__SCRIPT_VAR_INTEGER(signed char);
__SCRIPT_VAR_INTEGER(unsigned char);
__SCRIPT_VAR_INTEGER(signed short);
__SCRIPT_VAR_INTEGER(unsigned short);
__SCRIPT_VAR_INTEGER(signed int);
__SCRIPT_VAR_INTEGER(unsigned int);
__SCRIPT_VAR_INTEGER(signed long);
__SCRIPT_VAR_INTEGER(unsigned long);
#ifdef _MSC_VER
__SCRIPT_VAR_INTEGER(signed __int8);
__SCRIPT_VAR_INTEGER(unsigned __int8);
__SCRIPT_VAR_INTEGER(signed __int16);
__SCRIPT_VAR_INTEGER(unsigned __int16);
__SCRIPT_VAR_INTEGER(signed __int32);
__SCRIPT_VAR_INTEGER(unsigned __int32);
#endif
#if (defined(_WIN64) || defined(_LP64))
# ifdef _MSC_VER
__SCRIPT_VAR_INTEGER(signed __int64);
__SCRIPT_VAR_INTEGER(unsigned __int64);
# else
__SCRIPT_VAR_INTEGER(signed long long);
__SCRIPT_VAR_INTEGER(unsigned long long);
# endif
#endif
#undef __SCRIPT_VAR_INTEGER

#define __SCRIPT_VAR_FLOAT(T) \
template <> \
struct object<T> \
{ \
  static const ::SQChar typemask = _SC('f'); \
  T value; \
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw() \
  { \
    ::SQFloat f; \
    ::sq_getfloat(hvm, idx, &f); \
    this->value = static_cast<T>(f); \
  } \
  static inline void push(::HSQUIRRELVM hvm, T value) throw() \
  { \
    ::sq_pushfloat(hvm, static_cast< ::SQFloat>(value)); \
  } \
};
__SCRIPT_VAR_FLOAT(float);
__SCRIPT_VAR_FLOAT(double);
#undef __SCRIPT_VAR_FLOAT

template <>
struct object<bool>
{
  static const ::SQChar typemask = _SC('b');
  bool value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    ::SQBool b;
    ::sq_tobool(hvm, idx, &b);
    this->value = (b != SQFalse);
  }
  static inline void push(::HSQUIRRELVM hvm, bool value) throw()
  {
    ::sq_pushbool(hvm, (value ? SQTrue : SQFalse));
  }
};

template <>
struct object< ::SQChar*>
{
  static const ::SQChar typemask = _SC('s');
  ::SQChar* value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    ::sq_tostring(hvm, idx);
    ::sq_getstring(hvm, -1, const_cast<const ::SQChar**>(&this->value));
    ::sq_pop(hvm, 1);
  }
  static inline void push(::HSQUIRRELVM hvm, ::SQChar* value) throw()
  {
    ::sq_pushstring(hvm, value, -1);
  }
};

template <>
struct object<const ::SQChar*>
{
  static const ::SQChar typemask = _SC('s');
  const ::SQChar* value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    ::sq_tostring(hvm, idx);
    ::sq_getstring(hvm, -1, &this->value);
    ::sq_pop(hvm, 1);
  }
  static inline void push(::HSQUIRRELVM hvm, const ::SQChar* value) throw()
  {
    ::sq_pushstring(hvm, value, -1);
  }
};

template <>
struct object< ::std::basic_string< ::SQChar> >
{
  static const ::SQChar typemask = _SC('s');
  ::std::basic_string< ::SQChar> value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    const ::SQChar* ret;
    ::sq_tostring(hvm, idx);
    ::sq_getstring(hvm, -1, &ret);
    this->value = ::std::basic_string< ::SQChar>(ret);
    ::sq_pop(hvm, 1);
  }
  static inline void push(::HSQUIRRELVM hvm,
    const ::std::basic_string< ::SQChar>& value) throw()
  {
    ::sq_pushstring(hvm, reinterpret_cast<const ::SQChar*>(value.c_str()), -1);
  }
};

template <>
struct object< ::std::basic_string< ::SQChar>&>
{
  static const ::SQChar typemask = _SC('s');
  ::std::basic_string< ::SQChar> value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    const ::SQChar* ret;
    ::sq_tostring(hvm, idx);
    ::sq_getstring(hvm, -1, &ret);
    this->value = ::std::basic_string< ::SQChar>(ret);
    ::sq_pop(hvm, 1);
  }
  static inline void push(::HSQUIRRELVM hvm,
    const ::std::basic_string< ::SQChar>& value) throw()
  {
    ::sq_pushstring(hvm, reinterpret_cast<const ::SQChar*>(value.c_str()), -1);
  }
};

template <>
struct object<const ::std::basic_string< ::SQChar>&>
{
  static const ::SQChar typemask = _SC('s');
  ::std::basic_string< ::SQChar> value;
  object(::HSQUIRRELVM hvm, ::SQInteger idx) throw()
  {
    const ::SQChar* ret;
    ::sq_tostring(hvm, idx);
    ::sq_getstring(hvm, -1, &ret);
    this->value = ::std::basic_string< ::SQChar>(ret);
    ::sq_pop(hvm, 1);
  }
  static inline void push(::HSQUIRRELVM hvm,
    const ::std::basic_string< ::SQChar>& value) throw()
  {
    ::sq_pushstring(hvm, reinterpret_cast<const ::SQChar*>(value.c_str()), -1);
  }
};

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_OBJECT_HPP
