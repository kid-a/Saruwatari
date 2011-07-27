EXE	  = executable
COMPONENT = ram

CC = g++
FLAGS = -DSC_INCLUDE_FX
INCDIR = -I$(SYSTEMC)/include
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
