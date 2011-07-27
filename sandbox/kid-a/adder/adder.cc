#include "adder.h"

void adder::execute()
{
  sum.write(a.read() + b.read());
}
