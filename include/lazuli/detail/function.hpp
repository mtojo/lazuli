#ifndef LAZULI_DETAIL_FUNCTION_HPP
#define LAZULI_DETAIL_FUNCTION_HPP

#include <lazuli/detail/scope.hpp>
#include <squirrel.h>

namespace lazuli { namespace detail {

class function_registration
  : public registration
{

public:
  function_registration(const ::SQChar* name, ::SQFUNCTION func,
    ::SQInteger nparamscheck, const ::SQChar* typemask) throw()
    : name(name), func(func), nparamscheck(nparamscheck), typemask(typemask)
  {
  }

  ~function_registration() throw()
  {
  }

  void register_(::HSQUIRRELVM hvm) const throw()
  {
    ::sq_pushstring(hvm, this->name, -1);
    ::sq_newclosure(hvm, this->func, 0);
    ::sq_setparamscheck(hvm, this->nparamscheck, this->typemask);
    ::sq_setnativeclosurename(hvm, -1, this->name);
    ::sq_newslot(hvm, -3, SQFalse);
  }

private:
  const ::SQChar* name;
  ::SQFUNCTION func;
  ::SQInteger nparamscheck;
  const ::SQChar* typemask;

};

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_FUNCTION_HPP
