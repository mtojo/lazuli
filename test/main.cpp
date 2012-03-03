#include <iostream>
#include <boost/cstdint.hpp>
#include <lazuli/vm.hpp>
#include "colour.hpp"

void compile_error_handler(std::string desc,
  std::string src, unsigned int line, unsigned int col) throw()
{
  std::cout << desc << " in " << src << " at " << line << ":" << col << std::endl;
}

void runtime_error_handler(std::string desc,
  std::string src, unsigned int line) throw()
{
  std::cout << desc << " in " << src << " at " << line << std::endl;
}

void test(std::string s)
{
  std::cout << s << std::endl;
}

int main()
{
  using namespace boost;
  using namespace lazuli;

  try
  {
    vm v;
    v.register_stdlib();

    // Registers error handlers.
    v.register_compile_error_handler(compile_error_handler);
    v.register_runtime_error_handler(runtime_error_handler);

    // Binding C++ functions/classes.
    v.global()
    [

      // function "test"
      def<void (*)(std::string), &test>("test"),

      // namespace "mynamespace"
      v.table("mynamespace")
      [

        // class "colour"
        v.class_
        <
          colour,
          constructor<uint8_t, uint8_t, uint8_t>
        >
        ("colour")
        [
          def<void (colour::*)(uint8_t), &colour::set_red>("set_red"),
          def<uint8_t (colour::*)() const, &colour::get_red>("get_red"),
          def<void (colour::*)(uint8_t), &colour::set_green>("set_green"),
          def<uint8_t (colour::*)() const, &colour::get_green>("get_green"),
          def<void (colour::*)(uint8_t), &colour::set_blue>("set_blue"),
          def<uint8_t (colour::*)() const, &colour::get_blue>("get_blue")
        ]

      ]

    ];

    // Runs a script.
    std::ifstream ifs("test.nut");
    v.run(ifs, "test");
  }
  catch (const compile_error& e)
  {
    std::cerr << "COMPILE ERROR!" << std::endl;
  }
  catch (const runtime_error& e)
  {
    std::cerr << "RUNTIME ERROR!" << std::endl;
  }

  return 0;
}
