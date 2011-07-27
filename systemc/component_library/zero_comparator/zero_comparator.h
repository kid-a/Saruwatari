#ifndef _ZEROCOMP
#define _ZEROCOMP

#include <iostream>
#include <systemc.h>

using namespace std;


class zero_comparator : sc_module
{
 public: 
  sc_in<sc_lv<32> > value_in;
  sc_out<sc_logic > is_zero;

  void process();

  SC_CTOR(zero_comparator)
  {
    SC_METHOD(process);
    sensitive << value_in;
    dont_initialize();
  }
};

void zero_comparator::process()
{
  if (value_in.read().to_int() == 0)
    {
      cout << "NOTE: Branch operand value is zero." << endl;
      is_zero.write(SC_LOGIC_1);
    }

  else
    {
      cout << "NOTE: Branch operand is NOT zero." << endl;
      is_zero.write(SC_LOGIC_0);
    }
}
#endif

