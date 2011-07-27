EXE	  = executable
COMPONENT = pipeline

CC = g++
FLAGS = -DSC_INCLUDE_FX -g
COMP_ROOT = component_library
INCDIR = -I$(SYSTEMC)/include  -I. -I if_stage -I id_stage -I exmem_stage -I wb_stage -I$(COMP_ROOT)/alu -I$(COMP_ROOT)/control_unit  -I$(COMP_ROOT)/RAM -I$(COMP_ROOT)/hazard_detection_unit -I$(COMP_ROOT)/register_file
LIBDIR = -L$(SYSTEMC)/lib-linux
LIBS = -lm -lsystemc
CC_OPTS  = $(FLAGS) $(INCDIR) $(LIBDIR) $(LIBS)     
VCDNAME	  = wave.vcd
WAVEOPT   = ~/.scriptgtkwave

all:
	$(CC) $(COMPONENT).cc $^ \
	$(CC_OPTS) -o $(EXE)

run:
	cp if_stage/mem_dump.dat .
	./$(EXE)

wave:
	gtkwave $(VCDNAME) -S $(WAVEOPT)

clean:
	-rm $(EXE)
	-rm *.o
	-rm *.out
	-rm *.gch
	-rm *.vcd
	-rm mem_dump.dat
