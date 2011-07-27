# Saruwatari root Makefile
# Made on 05 Feb 2010
SOURCES         = $(shell ls *.vhd)
COMPONENTS 	= if_stage id_stage exmem_stage wb_stage
MEM_DUMP	= if_stage/mem_dump.dat

#compile options
IEEE		= --ieee=synopsys -fexplicit
VCDNAME		= data.vcd
VCDOPT		= --vcd=${VCDNAME}
EXETIME		= --stop-time=2000ns
EXENAME		= pipeline
DISPTREE	= --disp-tree='inst'
WAVEOPT		= ~/.scriptgtkwave


all:	obj compile
	ghdl -e $(IEEE) $(EXENAME)

obj:	
	cp $(MEM_DUMP) .
	cp component_library/flag_register/flag.vhd .
	for dir in $(COMPONENTS); do \
	(cd $$dir; make vhd; cp *.vhd ..); \
	done

compile:
	ghdl -a $(IEEE) $(SOURCES)

run:
	ghdl -r $(EXENAME) $(VCDOPT) $(EXETIME) $(DISPTREE)

wave:
	gtkwave $(VCDNAME) -S $(WAVEOPT)

main: $(OBJECTS)


clean:
	ghdl --clean;
	find . -maxdepth 1 -type f \! \( -name "Makefile" \
	-o -name "pipeline.vhd" -o -name "if_id_latch.vhd" -o \
	-name "id_exmem_latch.vhd" -o -name "exmem_wb_latch.vhd" \
	-o -name "hazard_detection_unit.vhd" \) -delete;
	for dir in $(COMPONENTS); do \
	(cd $$dir; make clean); \
	done
