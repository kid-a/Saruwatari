#ifndef ROM_H
#define ROM_H

#include <systemc.h>

template <int WORDS, int WORD_SIZE>
  class rom : sc_module {
 public:
 	sc_in<sc_lv<WORD_SIZE> >	addr;
	sc_in<sc_logic > 				ready;
	sc_out<sc_lv<WORD_SIZE*2> > data;

  	void load_memory_from_file(const char* filename);

  	void process(); 

  	SC_CTOR(rom) {
    	SC_METHOD(process);
    		dont_initialize();
    		sensitive << addr;
    	cells = std::vector<sc_lv<WORD_SIZE> >(WORDS);
    	load_memory_from_file("mem_dump.dat");
  	}

 	private:
		std::vector<sc_lv<WORD_SIZE> > cells;
};

template<int WORDS, int WORD_SIZE>
void rom<WORDS, WORD_SIZE>::process() {
	cout << "Check : "<< cells[0] << endl;
    
	int vector_address = addr.read().to_uint() / (WORD_SIZE / 8);
	cout << "Address : "<< vector_address << endl;
	sc_lv<WORD_SIZE> ir_0 = cells[vector_address];
	cout << "IR0 : "<< ir_0<<endl;
	vector_address++;
	sc_lv<WORD_SIZE> ir_1 = cells[vector_address];
	cout << "IR1 : "<< ir_1<<endl;
	data.write( (ir_0, ir_1) );
}

template<int WORDS, int WORD_SIZE>
void rom<WORDS, WORD_SIZE>::load_memory_from_file(const char* filename) {
    std::string instruction;
    ifstream memory(filename);
    int address = 0;
    if(memory.is_open()) {
        while(!memory.eof()) {
            getline(memory,instruction );
	    cout<<"IRR:"<<instruction<<endl;
            if(instruction.size() > 0) {
                cells[address++] = instruction.c_str();
            }
        }
    }
    else
        std::cerr << "Cannot open ROM file " << filename << std::endl;
}

#endif
