#ifndef LAZULI_DETAIL_SCOPE_HPP
#define LAZULI_DETAIL_SCOPE_HPP

#include <squirrel.h>

namespace lazuli { namespace detail {

class scope;

class registration
{

  friend class scope;

public:
  registration() throw()
    : next(0)
  {
  }

  virtual ~registration() throw()
  {
    delete this->next;
  }

  virtual void register_(::HSQUIRRELVM hvm) const throw() = 0;

private:
  registration* next;

};

class scope
{

public:
  scope() throw()
    : chain(0)
  {
  }

  explicit scope(registration* reg) throw()
    : chain(reg)
  {
  }

  scope(const scope& other) throw()
    : chain(other.chain)
  {
    const_cast<scope&>(other).chain = 0;
  }

  virtual ~scope() throw()
  {
    delete chain;
  }

  scope& operator,(scope s) throw()
  {
    if (this->chain == 0)
    {
      this->chain = s.chain;
      s.chain = 0;
    }
    else
    {
      for (registration* c = this->chain; ; c = c->next)
      {
        if (c->next == 0)
        {
          c->next = s.chain;
          s.chain = 0;
          break;
        }
      }
    }
    return *this;
  }

  void register_(::HSQUIRRELVM hvm) const throw()
  {
    for (registration* r = this->chain; r != 0; r = r->next)
    {
      r->register_(hvm);
    }
  }

  int length() const throw()
  {
    int len = 1;
    for (registration* c = this->chain; ; c = c->next)
    {
      ++len;
      if (c->next == 0)
      {
        break;
      }
    }
    return len;
  }

private:
  registration* chain;

};

} } // namespace lazuli::detail

#endif // LAZULI_DETAIL_SCOPE_HPP
