#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <boost/cstdint.hpp>
using boost::uint8_t;
using boost::uint32_t;

struct colour
{

  colour() throw();
  colour(uint8_t r, uint8_t g, uint8_t b) throw();
  colour(const colour& other) throw();
  colour(uint32_t rgb) throw();

  void set_red(uint8_t r) throw();
  uint8_t get_red() const throw();
  void set_green(uint8_t g) throw();
  uint8_t get_green() const throw();
  void set_blue(uint8_t b) throw();
  uint8_t get_blue() const throw();

  colour& operator=(const colour& other) throw();
  colour& operator=(uint32_t rgb) throw();
  bool operator==(const colour& other) throw();
  operator uint32_t() const throw();

  uint8_t r;
  uint8_t g;
  uint8_t b;

};

inline colour::colour() throw()
  : r(0), g(0), b(0)
{
}

inline colour::colour(uint8_t r, uint8_t g, uint8_t b) throw()
  : r(r), g(g), b(b)
{
}

inline colour::colour(const colour& other) throw()
{
  this->operator=(other);
}

inline colour::colour(uint32_t rgb) throw()
{
  this->operator=(rgb);
}

inline void colour::set_red(uint8_t r) throw()
{
  this->r = r;
}

inline uint8_t colour::get_red() const throw()
{
  return this->r;
}

inline void colour::set_green(uint8_t g) throw()
{
  this->g = g;
}

inline uint8_t colour::get_green() const throw()
{
  return this->g;
}

inline void colour::set_blue(uint8_t b) throw()
{
  this->b = b;
}

inline uint8_t colour::get_blue() const throw()
{
  return this->b;
}

inline colour& colour::operator=(const colour& other) throw()
{
  this->r = other.r;
  this->g = other.g;
  this->b = other.b;
  return *this;
}

inline colour& colour::operator=(uint32_t rgb) throw()
{
  this->r = static_cast<uint8_t>(rgb & 0xFF);
  this->g = static_cast<uint8_t>((rgb >> 8) & 0xFF);
  this->b = static_cast<uint8_t>((rgb >> 16) & 0xFF);
  return *this;
}

inline bool colour::operator==(const colour& other) throw()
{
  return ((this->r == other.r) && (this->g == other.g) &&
    (this->b == other.b));
}

inline colour::operator uint32_t() const throw()
{
  return (((this->b & 0xFF) << 16) |
    ((this->g & 0xFF) << 8) | (this->r & 0xFF));
}

#endif // COLOUR_HPP
