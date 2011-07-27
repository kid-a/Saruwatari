EXE	  = executable
TEST = if_id_test

CC = g++
FLAGS = -DSC_INCLUDE_FX
COMP_ROOT = ../component_library
INCDIR = -I$(SYSTEMC)/include -I../ -I../if_stage/ 
LIBDIR = -L$(SYSTEMC)/lib-linux
LIBS = -lm -lsystemc
CC_OPTS  = $(FLAGS) $(INCDIR) $(LIBDIR) $(LIBS)     
VCDNAME	  = wave.vcd
WAVEOPT   = ~/.scriptgtkwave

all:
	$(CC) $(TEST).cc $^ \
	$(CC_OPTS) -o $(EXE)

.cc.o: $(TEST).cc
	$(CC) $(CC_OPTS) -c $<

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
