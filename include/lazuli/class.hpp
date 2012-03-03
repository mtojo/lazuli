#ifndef LAZULI_CLASS_HPP
#define LAZULI_CLASS_HPP

#include <lazuli/config.hpp>
#include <lazuli/detail/object.hpp>
#include <lazuli/detail/releasehook.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <squirrel.h>

namespace lazuli {

namespace {

struct none;

} // namespace unnamed

#define __ARG(z, i, d) BOOST_PP_CAT(d, i) = none
#define __PRN(z, i, d) d
#define __TYP(z, i, d) detail::object<BOOST_PP_CAT(d, i)>::typemask
#define __VAR(z, i, d) detail::object<BOOST_PP_CAT(d, i)>(hvm, BOOST_PP_ADD(i, 2)).value

template <BOOST_PP_ENUM(LAZULI_MAX_ARITY, __ARG, class A)>
struct constructor
{
  BOOST_STATIC_CONSTANT(::SQInteger, arity = LAZULI_MAX_ARITY + 1);
  static const ::SQChar* typemask() throw()
  {
    static const ::SQChar t[] = { _SC('.')
      BOOST_PP_ENUM_TRAILING(LAZULI_MAX_ARITY, __TYP, A), 0 };
    return t;
  }
  template <class X>
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()
  {
    X* instance = new X(BOOST_PP_ENUM(LAZULI_MAX_ARITY, __VAR, A));
    ::sq_setinstanceup(hvm, 1, reinterpret_cast< ::SQUserPointer>(instance));
    ::sq_setreleasehook(hvm, 1, detail::releasehook<X>);
    return 0;
  }
};

#define BOOST_PP_LOCAL_MACRO(i)                                               \
template <BOOST_PP_ENUM_PARAMS(i, class A)>                                   \
struct constructor<BOOST_PP_ENUM_PARAMS(i, A)                                 \
  BOOST_PP_ENUM_TRAILING(BOOST_PP_SUB(LAZULI_MAX_ARITY, i), __PRN, none)>     \
{                                                                             \
  BOOST_STATIC_CONSTANT(::SQInteger, arity = BOOST_PP_ADD(i, 1));             \
  static const ::SQChar* typemask() throw()                                   \
  {                                                                           \
    static const ::SQChar t[] = { _SC('.')                                    \
      BOOST_PP_ENUM_TRAILING(i, __TYP, A), 0 };                               \
    return t;                                                                 \
  }                                                                           \
  template <class X>                                                          \
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()                          \
  {                                                                           \
    X* instance = new X(BOOST_PP_ENUM(i, __VAR, A));                          \
    ::sq_setinstanceup(hvm, 1, reinterpret_cast< ::SQUserPointer>(instance)); \
    ::sq_setreleasehook(hvm, 1, detail::releasehook<X>);                      \
    return 0;                                                                 \
  }                                                                           \
};
#define BOOST_PP_LOCAL_LIMITS (1, LAZULI_MAX_ARITY - 1)
#include BOOST_PP_LOCAL_ITERATE()

template <>
struct constructor<BOOST_PP_ENUM(LAZULI_MAX_ARITY, __PRN, none)>
{
  BOOST_STATIC_CONSTANT(::SQInteger, arity = 1);
  static const ::SQChar* typemask() throw()
  {
    return _SC(".");
  }
  template <class X>
  static ::SQInteger impl(::HSQUIRRELVM hvm) throw()
  {
    X* instance = new X();
    ::sq_setinstanceup(hvm, 1, reinterpret_cast< ::SQUserPointer>(instance));
    ::sq_setreleasehook(hvm, 1, detail::releasehook<X>);
    return 0;
  }
};

#undef __VAR
#undef __TYP
#undef __PRN
#undef __ARG

} // namespace lazuli

#endif // LAZULI_CLASS_HPP
