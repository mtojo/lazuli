#ifndef LAZULI_DETAIL_UTILITY_HPP
#define LAZULI_DETAIL_UTILITY_HPP

#include <cstdarg>
#include <fstream>

#ifdef SQUNICODE
# include <cwchar>
#else
# include <cstdio>
#endif

#include <squirrel.h>

namespace lazuli { namespace detail {

static inline int format(::SQChar* buffer, ::std::size_t count,
  const SQChar* format, ::std::va_list argptr) throw()
{
#ifdef SQUNICODE
# if defined(_MSC_VER) && (_MSC_VER >= 1400)
  return ::_vsnwprintf_s(buffer, count, _TRUNCATE, format, argptr);
# elif defined(WIN32)
  return ::_vsnwprintf(buffer, count, format, argptr);
# else
  return ::std::vswprintf(buffer, count, format, argptr);
# endif
#else
# if defined(_MSC_VER) && (_MSC_VER >= 1400)
  return ::vsnprintf_s(buffer, count, _TRUNCATE, format, argptr);
# else
  return ::std::vsnprintf(buffer, count, format, argptr);
# endif
#endif
}

static inline ::SQInteger ifstream_lexfeed(::SQUserPointer p) throw()
{
  ::std::basic_ifstream< ::SQChar>*
    ifs = reinterpret_cast< ::std::basic_ifstream< ::SQChar>*>(p);
  ::SQInteger c = static_cast< ::SQInteger>(ifs->get());
  if (ifs->eof())
  {
    c = 0;
  }
  return c;
}

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_UTILITY_HPP
