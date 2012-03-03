#ifndef LAZULI_DETAIL_STRING_HPP
#define LAZULI_DETAIL_STRING_HPP

#include <string>
#include <squirrel.h>

namespace lazuli { namespace detail {

namespace {

static inline ::SQInteger uppercase(::HSQUIRRELVM hvm) throw()
{
  ::std::basic_string< ::SQChar> r = object< ::std::basic_string< ::SQChar> >(hvm, 2).value;
  object< ::std::basic_string< ::SQChar> >::push(hvm, r);
  return 1;
}

static inline ::SQInteger lowercase(::HSQUIRRELVM hvm) throw()
{
  ::std::basic_string< ::SQChar> r = object< ::std::basic_string< ::SQChar> >(hvm, 2).value;
  object< ::std::basic_string< ::SQChar> >::push(hvm, r);
  return 1;
}

} // namespace unnamed

static inline void register_string_functions(::HSQUIRRELVM hvm) throw()
{
  ::sq_pushstring(hvm, _SC("uppercase"), -1);
  ::sq_newclosure(hvm, uppercase, 0);
  ::sq_setparamscheck(hvm, 2, _SC(".s"));
  ::sq_setnativeclosurename(hvm, -1, _SC("uppercase"));
  ::sq_newslot(hvm, -3, SQFalse);

  ::sq_pushstring(hvm, _SC("lowercase"), -1);
  ::sq_newclosure(hvm, lowercase, 0);
  ::sq_setparamscheck(hvm, 2, _SC(".s"));
  ::sq_setnativeclosurename(hvm, -1, _SC("lowercase"));
  ::sq_newslot(hvm, -3, SQFalse);
}

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_STRING_HPP
