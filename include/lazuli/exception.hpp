#ifndef LAZULI_EXCEPTION_HPP
#define LAZULI_EXCEPTION_HPP

#include <exception>

namespace lazuli {

class exception
  : public ::std::exception
{
};

class compile_error
  : public exception
{
};

class runtime_error
  : public exception
{
};

} // namespace lazuli

#endif // LAZULI_EXCEPTION_HPP
