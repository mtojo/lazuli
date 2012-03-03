#ifndef LAZULI_DETAIL_CONSTANT_HPP
#define LAZULI_DETAIL_CONSTANT_HPP

#include <lazuli/detail/object.hpp>
#include <lazuli/detail/scope.hpp>
#include <squirrel.h>

namespace lazuli { namespace detail {

template <class T>
class const_registration
  : public registration
{

public:
  const_registration(const ::SQChar* name, T value) throw()
    : name(name), value(value)
  {
  }

  ~const_registration() throw()
  {
  }

  void register_(::HSQUIRRELVM hvm) const throw()
  {
    ::sq_pushconsttable(hvm);
    ::sq_pushstring(hvm, this->name, -1);
    object<T>::push(hvm, this->value);
    ::sq_newslot(hvm, -3, SQFalse);
    ::sq_pop(hvm, 1);
  }

private:
  const ::SQChar* name;
  T value;

};

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_CONSTANT_HPP
