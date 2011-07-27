#include <systemc.h>
#include "tb_adder.h"

void test_bench::generate_signals()
{
  a.write(4);
  b.write(6);
  wait();
  a.write(1);
  wait();
  b.write(7);
  wait();
}
