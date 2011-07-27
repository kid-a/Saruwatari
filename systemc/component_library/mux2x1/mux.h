#ifndef MUX_H
#define MUX_H

#include <systemc.h>

template <int SIZE=32>
 class mux : sc_module {
 	public:
		sc_in<sc_lv<SIZE> > a;
		sc_in<sc_lv<SIZE> > b;
		sc_in<sc_logic > selector;
		sc_out<sc_lv<SIZE> > output;

		void process();

		SC_CTOR(mux){
			SC_METHOD(process);
				dont_initialize();
				sensitive << a << b << selector;
		
		}


 };

template<int SIZE>
 void mux<SIZE>::process() {
	
	if ( selector == SC_LOGIC_1 ) {
		output.write( b.read()  );
	} else if (selector == SC_LOGIC_0) {
		output.write( a.read()  );
	} else {
		cout<<"Invalid selector in mux"<<endl;
	}
 }


#endif
