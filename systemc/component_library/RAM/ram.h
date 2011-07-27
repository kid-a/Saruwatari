#ifndef RAM_H
#define RAM_H

#include <systemc.h>

template <int WORDS=256, int WORD_SIZE=32>
  class ram : sc_module{
 public:
 sc_in<bool > clk;
 sc_in<sc_logic > write_enable;
 sc_in<sc_lv<WORD_SIZE> > addr;
 sc_in<sc_lv<WORD_SIZE> > data_in;
 sc_out<sc_lv<WORD_SIZE> > data_out;

 void write_ram();
 void read_ram();
 void init_ram();

    ram(sc_module_name _name, sc_trace_file* tcf, bool trace_enabled) : sc_module(_name), _tcf(tcf)
      {
   SC_METHOD(write_ram);
   dont_initialize();
   sensitive << clk.neg() << addr;

   SC_METHOD(read_ram);
   dont_initialize();
   sensitive << addr;
    		
   cells = std::vector<sc_lv<WORD_SIZE> >(WORDS);
   std::string zero_word(WORD_SIZE,'0');
   for (int i=0; i<cells.size()-1; i++ ) {
     cells[i] = sc_lv<WORD_SIZE>(zero_word.c_str());
   }
   init_ram();
    if (trace_enabled) 
    {
       sc_trace(_tcf,cells[0],"DM_WORD_0");
       sc_trace(_tcf,cells[1],"DM_WORD_1");
       sc_trace(_tcf,cells[2],"DM_WORD_2");
       sc_trace(_tcf,cells[3],"DM_WORD_3");
       sc_trace(_tcf,cells[4],"DM_WORD_4");
       sc_trace(_tcf,cells[5],"DM_WORD_5");
       sc_trace(_tcf,cells[6],"DM_WORD_6");
       sc_trace(_tcf,cells[7],"DM_WORD_7");
       sc_trace(_tcf,cells[8],"DM_WORD_8");
       sc_trace(_tcf,cells[9],"DM_WORD_9");
       sc_trace(_tcf,cells[10],"DM_WORD_10");
       sc_trace(_tcf,cells[11],"DM_WORD_11");
       sc_trace(_tcf,cells[12],"DM_WORD_12");
       sc_trace(_tcf,cells[13],"DM_WORD_13");
       sc_trace(_tcf,cells[14],"DM_WORD_14");
       sc_trace(_tcf,cells[15],"DM_WORD_15");
       sc_trace(_tcf,cells[16],"DM_WORD_16");
       sc_trace(_tcf,cells[17],"DM_WORD_17");
       sc_trace(_tcf,cells[18],"DM_WORD_18");
       sc_trace(_tcf,cells[19],"DM_WORD_19");
       sc_trace(_tcf,addr,"DM_addr");
       sc_trace(_tcf,write_enable,"DM_we");
       sc_trace(_tcf,data_in,"DM_data_in");
       sc_trace(_tcf,data_out,"DM_data_out");
    }
 				
 }
 SC_HAS_PROCESS(ram);
 private:
 std::vector<sc_lv<WORD_SIZE> > cells;
 sc_trace_file *_tcf;
};

template<int WORDS, int WORD_SIZE>
  void ram<WORDS, WORD_SIZE>::init_ram() {
      const char* filename = "ram.dat"; 
    std::string data;
    ifstream data_src(filename);
    int address = 0;
    if(data_src.is_open()) {
        while(!data_src.eof()) {
            getline(data_src,data );
	   // cout<<"IRR:"<<instruction<<endl;
            if(data.size() > 0) {
                cells[address++] = data.c_str();
            }
        }
    }
    else
        std::cerr << "Cannot open RAM file " << filename << std::endl;
}
  

 


template<int WORDS, int WORD_SIZE>
  void ram<WORDS, WORD_SIZE>::write_ram() {
  if ( write_enable.read() == SC_LOGIC_1 ) 	{
    int vector_address = (addr.read().to_uint()) / (WORD_SIZE / 8);
    cells[vector_address] = data_in.read();
  }
}

template<int WORDS, int WORD_SIZE>
  void ram<WORDS, WORD_SIZE>::read_ram() {
  int vector_address = (addr.read().to_uint()) / (WORD_SIZE / 8);
		
  if ( vector_address <= WORDS -1 ) {
    sc_lv<WORD_SIZE > value = cells[vector_address];
    data_out.write(value) ;
  } else {
    cout << "RAM address out of bound error"<<endl;
    //data_out.range(WORD_SIZE,0).write(SC_LOGIC_0);
  }
} 
#endif
