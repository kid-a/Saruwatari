#include <systemc.h>

SC_MODULE(adder) {

  sc_in<int> a;
  sc_in<int> b;
  sc_out<int> sum;

  void execute();

  SC_CTOR(adder) {
    SC_METHOD(execute);
    sensitive << a << b;
  }
};
