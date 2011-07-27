#ifndef _SIGN_EXTENSOR
#define _SIGN_EXTENSOR

#include <systemc.h>

class sign_extensor : sc_module
{
 public:
  sc_in<sc_lv<16> > data_in;
  sc_out<sc_lv<32> > data_out;

  void process();
  SC_CTOR(sign_extensor)
  {
    SC_METHOD(process);
    sensitive << data_in;
    dont_initialize();
  }
};
void sign_extensor::process()
{
  data_out.write(data_in.read().to_int());
}
#endif
