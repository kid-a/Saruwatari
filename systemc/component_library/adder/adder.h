#ifndef ADDER_H
#define ADDER_H

#include <systemc.h>


template <int operand_size=32>
class adder : sc_module {
	private:
		int temp_sum;
		sc_trace_file *fp;

	public:
		// Ports
		sc_in<sc_lv<operand_size> > operand_A; 
		sc_in<sc_lv<operand_size> > operand_B; 
		sc_out<sc_lv<operand_size> > sum; 

		// Process
		void add(); 
		SC_HAS_PROCESS(adder);
		adder(sc_module_name _n, sc_trace_file* _fp) : sc_module(_n), fp(_fp){
			SC_METHOD(add);
				sensitive << operand_A << operand_B;
			temp_sum = 0;
			sc_trace(fp,operand_A, std::string (_n) + ".opa");
		}
};


template <int operand_size> void adder<operand_size>::add() 
{
	temp_sum = operand_A.read().to_int() + operand_B.read().to_int();
	sum.write ( static_cast<sc_lv<operand_size> > (temp_sum) );
}

#endif
