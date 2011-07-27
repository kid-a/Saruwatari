SYSTEMC	= /usr/local/systemc-2.2
EXE	= executable
GPP_OPTS= -Wall -DSC_INCLUDE_FX  -I$(SYSTEMC)/include -L$(SYSTEMC)/lib-linux -lm -lsystemc
VCDNAME	= adder_trace.vcd
WAVEOPT = ~/.scriptgtkwave

all: tb_adder.o adder.o
	g++ main.cc $^ \
	$(GPP_OPTS) -o $(EXE)

.cc.o: headers adder.cc tb_adder.cc
	g++ $(GPP_OPTS) -c $<

headers: adder.h tb_adder.h
	g++ $(GPP_OPTS) -c $^

run:
	./$(EXE)

wave:
	gtkwave $(VCDNAME) -S $(WAVEOPT)

clean:
	-rm $(EXE)
	-rm *.o
	-rm *.gch
	-rm *.vcd
