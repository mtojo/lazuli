#ifndef LAZULI_DETAIL_TABLE_HPP
#define LAZULI_DETAIL_TABLE_HPP

#include <lazuli/detail/scope.hpp>
#include <squirrel.h>

namespace lazuli { namespace detail {

class table_registration
  : public registration
{

public:
  table_registration(const ::SQChar* name, ::HSQOBJECT* obj) throw()
    : name(name), obj(obj)
  {
  }

  ~table_registration() throw()
  {
  }

  void register_(::HSQUIRRELVM hvm) const throw()
  {
    ::sq_pushstring(hvm, this->name, -1);
    ::sq_pushobject(hvm, *this->obj);
    ::sq_newslot(hvm, -3, SQFalse);
  }

private:
  const ::SQChar* name;
  ::HSQOBJECT* obj;

};

class container
{

public:
  explicit container(::HSQUIRRELVM hvm) throw()
    : hvm(hvm)
  {
    ::sq_resetobject(&this->obj);
  }

  virtual ~container() throw()
  {
//    ::sq_release(this->hvm, &this->obj);
  }

protected:
  void bind(scope& s) const throw()
  {
    ::sq_pushobject(this->hvm, this->obj);
    s.register_(this->hvm);
    ::sq_pop(this->hvm, 1);
  }

  ::HSQUIRRELVM hvm;
  ::HSQOBJECT obj;

};

class global
  : public container
{

public:
  explicit global(::HSQUIRRELVM hvm) throw()
    : container(hvm)
  {
    ::sq_pushroottable(this->hvm);
    ::sq_getstackobj(this->hvm, -1, &this->obj);
    ::sq_addref(this->hvm, &this->obj);
    ::sq_pop(this->hvm, 1);
  }

  global& operator[](scope s) throw()
  {
    this->bind(s);
    return *this;
  }

};

class table
  : public container
  , public scope
{

public:
  table(::HSQUIRRELVM hvm, const ::SQChar* name) throw()
    : container(hvm)
    , scope(new table_registration(name, &obj))
  {
    ::sq_newtable(this->hvm);
    ::sq_getstackobj(this->hvm, -1, &this->obj);
    ::sq_addref(this->hvm, &this->obj);
    ::sq_pop(this->hvm, 1);
  }

  table& operator[](scope s) throw()
  {
    this->bind(s);
    return *this;
  }

};

template <class X, class Constructor>
class class_
  : public container
  , public scope
{

public:
  class_(::HSQUIRRELVM hvm, const ::SQChar* name) throw()
    : container(hvm)
    , scope(new table_registration(name, &obj))
  {
    ::sq_newclass(hvm, SQFalse);

    ::sq_getstackobj(this->hvm, -1, &this->obj);
    ::sq_addref(this->hvm, &this->obj);

    ::sq_pushstring(hvm, _SC("constructor"), -1);
    ::sq_newclosure(hvm, Constructor::template impl<X>, 0);
    ::sq_setparamscheck(hvm, Constructor::arity, Constructor::typemask());
    ::sq_setnativeclosurename(hvm, -1, _SC("constructor"));
    ::sq_newslot(hvm, -3, SQFalse);

    ::sq_pop(this->hvm, 1);
  }

  class_& operator[](scope s) throw()
  {
    this->bind(s);
    return *this;
  }

};

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_TABLE_HPP
