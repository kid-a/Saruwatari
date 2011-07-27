EXE	  = executable
COMPONENT = wb_stage

CC = g++
FLAGS = -DSC_INCLUDE_FX
COMP_ROOT = ../component_library
INCDIR = -I$(SYSTEMC)/include  -I..
LIBDIR = -L$(SYSTEMC)/lib-linux
LIBS = -lm -lsystemc
CC_OPTS  = $(FLAGS) $(INCDIR) $(LIBDIR) $(LIBS)     
VCDNAME	  = wave.vcd
WAVEOPT   = ~/.scriptgtkwave

all:
	$(CC) $(COMPONENT)_tb.cc $^ \
	$(CC_OPTS) -o $(EXE)

.cc.o: headers $(COMPONENT).cc
	$(CC) $(CC_OPTS) -c $<

headers: $(COMPONENT).h
	$(CC) $(CC_OPTS) -c $^

run:
	./$(EXE)

wave:
	gtkwave $(VCDNAME) -S $(WAVEOPT)

clean:
	-rm $(EXE)
	-rm *.o
	-rm *.out
	-rm *.gch
	-rm *.vcd
