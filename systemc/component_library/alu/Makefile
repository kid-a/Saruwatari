EXE	  = executable
COMPONENT = alu

CC = g++
FLAGS = -DSC_INCLUDE_FX
INCDIR = -I$(SYSTEMC)/include
LIBDIR = -L$(SYSTEMC)/lib-linux
LIBS = -lm -lsystemc
GPP_OPTS  = $(FLAGS) $(INCDIR) $(LIBDIR) $(LIBS)     
VCDNAME	  = wave.vcd
WAVEOPT   = ~/.scriptgtkwave

all:
	$(CC) $(COMPONENT)_tb.cc $^ \
	$(GPP_OPTS) -o $(EXE)

.cc.o: headers $(COMPONENT).cc
	$(CC) $(GPP_OPTS) -c $<

headers: $(COMPONENT).h
	$(CC) $(GPP_OPTS) -c $^

#compile:
#	$(CC) main.cc $(INCDIR) $(LIBDIR) -lsystemc

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