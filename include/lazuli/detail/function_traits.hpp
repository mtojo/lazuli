#ifndef LAZULI_DETAIL_FUNCTION_TRAITS_HPP
#define LAZULI_DETAIL_FUNCTION_TRAITS_HPP

#include <lazuli/config.hpp>
#include <lazuli/detail/object.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <squirrel.h>

namespace lazuli { namespace detail {

template <class F, F> struct function_traits;

#define __ARG(z, i, d) class BOOST_PP_CAT(d, i)
#define __TYP(z, i, d) object<BOOST_PP_CAT(d, i)>::typemask
#define __VAR(z, i, d) object<BOOST_PP_CAT(d, i)>(hvm, BOOST_PP_ADD(i, 2)).value
#define BOOST_PP_LOCAL_MACRO(i)                                               \
template <class R BOOST_PP_ENUM_TRAILING(i, __ARG, A),                        \
  R (*F)(BOOST_PP_ENUM_PARAMS(i, A))>                                         \
struct function_traits<R (*)(BOOST_PP_ENUM_PARAMS(i, A)), F>                  \
{                                                                             \
  BOOST_STATIC_CONSTANT(::SQInteger, arity = BOOST_PP_ADD(i, 1));             \
  static const ::SQChar* typemask() throw()                                   \
  {                                                                           \
    static const ::SQChar t[] = { _SC('.')                                    \
      BOOST_PP_ENUM_TRAILING(i, __TYP, A), 0 };                               \
    return t;                                                                 \
  }                                                                           \
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()                          \
  {                                                                           \
    R r = (*F)(BOOST_PP_ENUM(i, __VAR, A));                                   \
    object<R>::push(hvm, r);                                                  \
    return 1;                                                                 \
  }                                                                           \
};                                                                            \
template <BOOST_PP_ENUM(i, __ARG, A) BOOST_PP_COMMA_IF(i)                     \
  void (*F)(BOOST_PP_ENUM_PARAMS(i, A))>                                      \
struct function_traits<void (*)(BOOST_PP_ENUM_PARAMS(i, A)), F>               \
{                                                                             \
  BOOST_STATIC_CONSTANT(::SQInteger, arity = BOOST_PP_ADD(i, 1));             \
  static const ::SQChar* typemask() throw()                                   \
  {                                                                           \
    static const ::SQChar t[] = { _SC('.')                                    \
      BOOST_PP_ENUM_TRAILING(i, __TYP, A), 0 };                               \
    return t;                                                                 \
  }                                                                           \
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()                          \
  {                                                                           \
    (*F)(BOOST_PP_ENUM(i, __VAR, A));                                         \
    return 0;                                                                 \
  }                                                                           \
};                                                                            \
template <class X, class R BOOST_PP_ENUM_TRAILING(i, __ARG, A),               \
  R (X::*F)(BOOST_PP_ENUM_PARAMS(i, A))>                                      \
struct function_traits<R (X::*)(BOOST_PP_ENUM_PARAMS(i, A)), F>               \
{                                                                             \
  BOOST_STATIC_CONSTANT(::SQInteger, arity = BOOST_PP_ADD(i, 1));             \
  static const ::SQChar* typemask() throw()                                   \
  {                                                                           \
    static const ::SQChar t[] = { _SC('x')                                    \
      BOOST_PP_ENUM_TRAILING(i, __TYP, A), 0 };                               \
    return t;                                                                 \
  }                                                                           \
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()                          \
  {                                                                           \
    X* x = 0;                                                                 \
    ::sq_getinstanceup(hvm, 1, reinterpret_cast< ::SQUserPointer*>(&x), 0);   \
    R r = (x->*F)(BOOST_PP_ENUM(i, __VAR, A));                                \
    object<R>::push(hvm, r);                                                  \
    return 1;                                                                 \
  }                                                                           \
};                                                                            \
template <class X, class R BOOST_PP_ENUM_TRAILING(i, __ARG, A),               \
  R (X::*F)(BOOST_PP_ENUM_PARAMS(i, A)) const>                                \
struct function_traits<R (X::*)(BOOST_PP_ENUM_PARAMS(i, A)) const, F>         \
{                                                                             \
  BOOST_STATIC_CONSTANT(::SQInteger, arity = BOOST_PP_ADD(i, 1));             \
  static const ::SQChar* typemask() throw()                                   \
  {                                                                           \
    static const ::SQChar t[] = { _SC('x')                                    \
      BOOST_PP_ENUM_TRAILING(i, __TYP, A), 0 };                               \
    return t;                                                                 \
  }                                                                           \
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()                          \
  {                                                                           \
    X* x = 0;                                                                 \
    ::sq_getinstanceup(hvm, 1, reinterpret_cast< ::SQUserPointer*>(&x), 0);   \
    R r = (x->*F)(BOOST_PP_ENUM(i, __VAR, A));                                \
    object<R>::push(hvm, r);                                                  \
    return 1;                                                                 \
  }                                                                           \
};                                                                            \
template <class X BOOST_PP_ENUM_TRAILING(i, __ARG, A),                        \
  void (X::*F)(BOOST_PP_ENUM_PARAMS(i, A))>                                   \
struct function_traits<void (X::*)(BOOST_PP_ENUM_PARAMS(i, A)), F>            \
{                                                                             \
  BOOST_STATIC_CONSTANT(::SQInteger, arity = BOOST_PP_ADD(i, 1));             \
  static const ::SQChar* typemask() throw()                                   \
  {                                                                           \
    static const ::SQChar t[] = { _SC('x')                                    \
      BOOST_PP_ENUM_TRAILING(i, __TYP, A), 0 };                               \
    return t;                                                                 \
  }                                                                           \
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()                          \
  {                                                                           \
    X* x = 0;                                                                 \
    ::sq_getinstanceup(hvm, 1, reinterpret_cast< ::SQUserPointer*>(&x), 0);   \
    (x->*F)(BOOST_PP_ENUM(i, __VAR, A));                                      \
    return 0;                                                                 \
  }                                                                           \
};                                                                            \
template <class X BOOST_PP_ENUM_TRAILING(i, __ARG, A),                        \
  void (X::*F)(BOOST_PP_ENUM_PARAMS(i, A)) const>                             \
struct function_traits<void (X::*)(BOOST_PP_ENUM_PARAMS(i, A)) const, F>      \
{                                                                             \
  BOOST_STATIC_CONSTANT(::SQInteger, arity = BOOST_PP_ADD(i, 1));             \
  static const ::SQChar* typemask() throw()                                   \
  {                                                                           \
    static const ::SQChar t[] = { _SC('x')                                    \
      BOOST_PP_ENUM_TRAILING(i, __TYP, A), 0 };                               \
    return t;                                                                 \
  }                                                                           \
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()                          \
  {                                                                           \
    X* x = 0;                                                                 \
    ::sq_getinstanceup(hvm, 1, reinterpret_cast< ::SQUserPointer*>(&x), 0);   \
    (x->*F)(BOOST_PP_ENUM(i, __VAR, A));                                      \
    return 0;                                                                 \
  }                                                                           \
};
#define BOOST_PP_LOCAL_LIMITS (0, LAZULI_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef __VAR
#undef __TYP
#undef __ARG

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_FUNCTION_TRAITS_HPP
