#ifndef LAZULI_FUNCTION_HPP
#define LAZULI_FUNCTION_HPP

#include <lazuli/detail/function.hpp>
#include <lazuli/detail/function_traits.hpp>
#include <lazuli/detail/scope.hpp>
#include <string>
#include <squirrel.h>

namespace lazuli {

template <class F, F f>
static inline detail::scope
def(const ::std::basic_string< ::SQChar>& name) throw()
{
  return detail::scope(new detail::function_registration(name.c_str(),
    detail::function_traits<F, f>::impl,
    detail::function_traits<F, f>::arity,
    detail::function_traits<F, f>::typemask()));
}

} // namespace lazuli

#endif // LAZULI_FUNCTION_HPP
