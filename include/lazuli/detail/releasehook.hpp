#ifndef LAZULI_DETAIL_RELEASEHOOK_HPP
#define LAZULI_DETAIL_RELEASEHOOK_HPP

#include <squirrel.h>

namespace lazuli { namespace detail {

template <class X>
static inline ::SQInteger releasehook(::SQUserPointer p, ::SQInteger size) throw()
{
  X* instance = reinterpret_cast<X*>(p);
  delete instance;
  return 0;
}

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_RELEASEHOOK_HPP
