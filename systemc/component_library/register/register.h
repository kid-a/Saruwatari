#ifndef REG_H
#define REG_H

#include <systemc.h>

template <int SIZE=32>
  class reg : sc_module {
	public:
		sc_in<bool > clk;
		sc_in<sc_logic > rst;
		sc_in<sc_logic > we;
		sc_in<sc_lv<SIZE> > data_in;
		sc_out<sc_lv<SIZE> > data_out;

  		void process();

		SC_CTOR(reg) {
			SC_METHOD(process);
			dont_initialize();
			sensitive << clk.pos() << rst;
			std::string s(SIZE,'0');
			zero_word = s;
		}

	private:
		std::string zero_word;
  
  };

template<int SIZE>
void reg<SIZE>::process(){
	if ( rst == SC_LOGIC_1) {
		data_out.write( sc_lv<SIZE>(zero_word.c_str())   );
	
	} else	if ( (clk.event()) && (clk==1) && (we == SC_LOGIC_1) ) {
			//cout << "Data in: " << data_in.read()<<endl;
			data_out.write(  data_in.read() );
			//cout << "Data out: " << data_out.read()<<endl;

	
	}
}

#endif 
