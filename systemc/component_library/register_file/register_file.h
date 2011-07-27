#ifndef _REGFILE
#define _REGFILE

#define W_SIZE 32
#define REGFILE_SIZE 32

#include <systemc.h>

class register_file : sc_module
{ 
 public:
  sc_in<bool> clk;
  sc_in<sc_logic> reset;
  
  sc_in<sc_lv<5> > read_01;
  sc_in<sc_lv<5> > read_02;
  sc_in<sc_lv<5> > read_11;
  sc_in<sc_lv<5> > read_12;
  
  sc_in<sc_lv<5> > write_0;
  sc_in<sc_lv<5> > write_1;
  sc_in<sc_logic> we_0;
  sc_in<sc_logic> we_1;
  sc_in<sc_lv<W_SIZE> > data_in_0;
  sc_in<sc_lv<W_SIZE> > data_in_1;
  
  sc_out<sc_lv<W_SIZE> > A0;
  sc_out<sc_lv<W_SIZE> > B0;
  sc_out<sc_lv<W_SIZE> > A1;
  sc_out<sc_lv<W_SIZE> > B1;

  void read()
  {
    A0.write( _registers[read_01.read().to_int()] );
    B0.write( _registers[read_02.read().to_int()] );
    A1.write( _registers[read_11.read().to_int()] );
    B1.write( _registers[read_12.read().to_int()] );
  }

  void write()
  {
    if (reset == SC_LOGIC_1)
      for(int i = 0; i < REGFILE_SIZE; i++)
	_registers[i] = 0b00000000000000000000000000000000;

    else
      {
	if (we_1 == SC_LOGIC_1)
	  _registers[write_1.read().to_int()] = data_in_1;
	
	if (we_0 == SC_LOGIC_1)
	  _registers[write_0.read().to_int()] = data_in_0;
      }
  }

 
 register_file(sc_module_name _name, 
	       sc_trace_file * tcf, 
	       bool trace_enabled) : sc_module(_name), _tcf(tcf)
  {
    /* register the two processes */
    SC_METHOD(read);
    sensitive << read_01 << read_02 
	      << read_11 << read_12;

    dont_initialize();
    
    SC_METHOD(write);
      sensitive << clk.neg() << reset;
    
    /* instantiate register vector */
    _registers = std::vector<sc_lv<W_SIZE> >(REGFILE_SIZE);

    /* initialize the register vector */
    for (int i = 0; i < REGFILE_SIZE; i ++)
      _registers[i] = 0b00000000000000000000000000000000;
    
    if (trace_enabled)
      {
	sc_trace(_tcf,_registers[0], "REGFILE_0");
	sc_trace(_tcf,_registers[1], "REGFILE_1");
	sc_trace(_tcf,_registers[2], "REGFILE_2");
	sc_trace(_tcf,_registers[3], "REGFILE_3");
	sc_trace(_tcf,_registers[4], "REGFILE_4");
	sc_trace(_tcf,_registers[5], "REGFILE_5");
	sc_trace(_tcf,_registers[6], "REGFILE_6");
	sc_trace(_tcf,_registers[7], "REGFILE_7");
	sc_trace(_tcf,_registers[8], "REGFILE_8");
	sc_trace(_tcf,_registers[9], "REGFILE_9");
	sc_trace(_tcf,_registers[10],"REGFILE_10");
	sc_trace(_tcf,_registers[31],"REGFILE_31");
	
	sc_trace(_tcf, A0, "REGFILE_A0_out");
	sc_trace(_tcf, B0, "REGFILE_B0_out");
	sc_trace(_tcf, A1, "REGFILE_A1_out");
	sc_trace(_tcf, B1, "REGFILE_B1_out");

	sc_trace(_tcf, write_0, "REGFILE_write_address_0");
	sc_trace(_tcf, write_1, "REGFILE_write_address_1");
	sc_trace(_tcf, we_0, "REGFILE_write_enable_0");
	sc_trace(_tcf, we_1, "REGFILE_write_enable_1");
	sc_trace(_tcf, data_in_0, "REGFILE_data_in_0");
	sc_trace(_tcf, data_in_1, "REGFILE_data_in_1");
      }
  }
  
  SC_HAS_PROCESS(register_file);	

 private:
  sc_trace_file *_tcf;
  std::vector<sc_lv<W_SIZE> > _registers;
};
#endif
