#ifndef LAZULI_CONSTANT_HPP
#define LAZULI_CONSTANT_HPP

#include <lazuli/detail/constant.hpp>
#include <lazuli/detail/scope.hpp>
#include <string>
#include <squirrel.h>

namespace lazuli {

template <class T>
static inline detail::scope
const_(const ::std::basic_string< ::SQChar>& name, T value) throw()
{
  return detail::scope(new detail::const_registration<T>(
    reinterpret_cast<const ::SQChar*>(name.c_str()), value));
}

} // namespace lazuli

#endif // LAZULI_CONSTANT_HPP
