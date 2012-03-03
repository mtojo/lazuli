test("hello world!");

class color extends mynamespace.colour
{
  constructor(r = 0, g = 0, b = 0)
  {
    ::mynamespace.colour.constructor(r, g, b);
  }
}

local c = color(30, 50, 127);
print(format("#%02X%02X%02X", c.get_red(), c.get_green(), c.get_blue()));
