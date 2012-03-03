#ifndef LAZULI_VM_HPP
#define LAZULI_VM_HPP

#include <lazuli/class.hpp>
#include <lazuli/constant.hpp>
#include <lazuli/function.hpp>

#include <lazuli/exception.hpp>
#include <lazuli/detail/table.hpp>
#include <lazuli/detail/utility.hpp>

#include <cstdarg>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#include <squirrel.h>
#include <sqstdblob.h>
#include <sqstdio.h>
#include <sqstdsystem.h>
#include <sqstdmath.h>  
#include <sqstdstring.h>

namespace lazuli {

class vm
  : private ::boost::noncopyable
{

public:
  typedef ::boost::function<void (::std::basic_string< ::SQChar>,
    ::std::basic_string< ::SQChar>, unsigned int, unsigned int)>
      compile_error_handler;
  typedef ::boost::function<void (::std::basic_string< ::SQChar>,
    ::std::basic_string< ::SQChar>, unsigned int)>
      runtime_error_handler;

public:
  vm() throw()
    : hvm(::sq_open(1024)), ceh(0), reh(0)
#ifdef SQUNICODE
    , os(&::std::wcout)
#else
    , os(&::std::cout)
#endif
  {
    this->initialize();
  }

  explicit vm(::HSQUIRRELVM hvm) throw()
    : hvm(hvm), ceh(0), reh(0)
#ifdef SQUNICODE
    , os(&::std::wcout)
#else
    , os(&::std::cout)
#endif
  {
    this->initialize();
  }

  ~vm() throw()
  {
    ::sq_close(this->hvm);
  }

  void register_stdlib() throw()
  {
    ::SQInteger top = ::sq_gettop(this->hvm);
    ::sqstd_register_bloblib(this->hvm);
    ::sqstd_register_iolib(this->hvm);
    ::sqstd_register_systemlib(this->hvm);
    ::sqstd_register_mathlib(this->hvm);
    ::sqstd_register_stringlib(this->hvm);
    ::sq_settop(this->hvm, top);
  }

  void register_compile_error_handler(compile_error_handler ceh) throw()
  {
    this->ceh = ceh;
  }

  void register_runtime_error_handler(runtime_error_handler reh) throw()
  {
    this->reh = reh;
  }

  void register_output_stream(::std::basic_ostream< ::SQChar>* os) throw()
  {
    this->os = os;
  }

  detail::global global() const throw()
  {
    return detail::global(this->hvm);
  }

  detail::table table(const ::std::basic_string< ::SQChar>& name) const throw()
  {
    return detail::table(this->hvm,
      reinterpret_cast<const ::SQChar*>(name.c_str()));
  }

  template <class X, class Constructor>
  detail::class_<X, Constructor>
  class_(const ::std::basic_string< ::SQChar>& name) const throw()
  {
    return detail::class_<X, Constructor>(this->hvm,
      reinterpret_cast<const ::SQChar*>(name.c_str()));
  }

  vm& run(const ::std::basic_string< ::SQChar>& str,
    const ::std::basic_string< ::SQChar>& symbol = _SC(""))
    throw(compile_error, runtime_error)
  {
    if (SQ_FAILED(::sq_compilebuffer(this->hvm,
      reinterpret_cast<const ::SQChar*>(str.c_str()),
      static_cast< ::SQInteger>(str.size() * sizeof(::SQChar)),
      reinterpret_cast<const ::SQChar*>(symbol.c_str()), SQTrue)))
    {
      throw compile_error();
    }
    ::sq_push(this->hvm, -2);
    if (SQ_FAILED(::sq_call(this->hvm, 1, SQFalse, SQTrue)))
    {
      ::sq_pop(this->hvm, 1);
      throw runtime_error();
    }
    ::sq_remove(this->hvm, -1);
    return *this;
  }

  vm& run(::std::basic_ifstream< ::SQChar>& ifs,
    const ::std::basic_string< ::SQChar>& symbol = _SC(""))
    throw(compile_error, runtime_error)
  {
    if (SQ_FAILED(::sq_compile(this->hvm, detail::ifstream_lexfeed,
      reinterpret_cast< ::SQUserPointer>(&ifs),
      reinterpret_cast<const ::SQChar*>(symbol.c_str()), SQTrue)))
    {
      throw compile_error();
    }
    ::sq_push(this->hvm, -2);
    if (SQ_FAILED(::sq_call(this->hvm, 1, SQFalse, SQTrue)))
    {
      ::sq_pop(this->hvm, 1);
      throw runtime_error();
    }
    ::sq_remove(this->hvm, -1);
    return *this;
  }

  ::std::basic_string< ::SQChar> last_error_string() const throw()
  {
    const ::SQChar* err;
    ::sq_getlasterror(this->hvm);
    if (::sq_gettype(this->hvm, -1) == ::OT_NULL)
    {
      return ::std::basic_string< ::SQChar>();
    }
    ::sq_tostring(this->hvm, -1);
    ::sq_getstring(this->hvm, -1, &err);
    return ::std::basic_string< ::SQChar>(err);
  }

  operator ::HSQUIRRELVM() const throw()
  {
    return this->hvm;
  }

private:
  void initialize() throw()
  {
    ::sq_setforeignptr(this->hvm, reinterpret_cast< ::SQUserPointer>(this));

    ::sq_setcompilererrorhandler(this->hvm, vm::compilererrorhandler);
    ::sq_newclosure(this->hvm, vm::errorhandler, 0);
    ::sq_seterrorhandler(this->hvm);

    ::sq_setprintfunc(this->hvm, vm::printfunc);

    ::sq_pushroottable(this->hvm);
  }

  static void compilererrorhandler(::HSQUIRRELVM hvm, const ::SQChar* desc,
    const ::SQChar* source, ::SQInteger line, ::SQInteger column) throw()
  {
    vm* v = reinterpret_cast<vm*>(::sq_getforeignptr(hvm));
    if ((v != 0) && (v->ceh != 0))
    {
      v->ceh(desc, source, static_cast<unsigned int>(line),
        static_cast<unsigned int>(column));
    }
  }

  static ::SQInteger errorhandler(::HSQUIRRELVM hvm) throw()
  {
    vm* v = reinterpret_cast<vm*>(::sq_getforeignptr(hvm));
    if ((v != 0) && (v->reh != 0))
    {
      const ::SQChar *desc = _SC("an error has occured");
      if (::sq_gettop(hvm) >= 1)
      {
        ::sq_getstring(hvm, 2, &desc);
      }
      ::SQStackInfos si;
      ::SQInteger level = 1;
      const ::SQChar* source = _SC("[unnamed source]");
      ::SQInteger line = 0;
      while (SQ_SUCCEEDED(::sq_stackinfos(hvm, level, &si)))
      {
        if (::std::basic_string< ::SQChar>(si.source).size() > 0)
        {
          source = si.source;
        }
        line = si.line;
        ++level;
      }
      v->reh(desc, source, static_cast<unsigned int>(line));
    }
    return 0;
  }

  static void printfunc(::HSQUIRRELVM hvm, const ::SQChar* s, ...) throw()
  {
    vm* v = reinterpret_cast<vm*>(::sq_getforeignptr(hvm));
    if (v != 0)
    {
      ::std::va_list vl;
      va_start(vl, s);
      int nsize = detail::format(0, 0, s, vl) + 1;
      ::boost::scoped_array< ::SQChar> buffer(new ::SQChar[nsize]);
      detail::format(buffer.get(), nsize, s, vl);
      *(v->os) << buffer.get() << ::std::endl;
      va_end(vl);
    }
  }

private:
  ::HSQUIRRELVM hvm;
  compile_error_handler ceh;
  runtime_error_handler reh;
  ::std::basic_ostream< ::SQChar>* os;

};

} // namespace lazuli

#endif // LAZULI_VM_HPP
