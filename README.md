Saruwatari: a four stage, superscalar processor
===============================================

Description
-----------
Saruwatari is a simplified, four-staged, superscalar, general purpose processor, designed 
as the final project for the course of 'Hardware Design' at the University of Catania.

Architecturally, it is similar to the 'DLX', except that it has two datapaths and it has 
only four stages instead of five (the 'execution' and the 'memory' have been condensed).

Instruction set
---------------
For the complete instruction set, see
https://raw.github.com/kid-a/Saruwatari/master/instruction_set/is.dat

Schematic
---------
See https://github.com/kid-a/Saruwatari/raw/master/schematic/schematic_complete.png

Hazards
-------
As in every pipeline, there hazards can occur.
We have been given the following assumptions:
* only datapath 1 can perform memory operations
* given a couple of successive instructions, only one can be a memory operation
* the execute stage for instructions lw, sw, mult, multi, div requires two clock cycles
* no data dependency between couple of instructions executed concurrently

We have also been requested to handle hazards in the following way:
* structural hazards: stall
* data hazards: stall
* control hazards: predict untaken with branch outcome known during decode

In order to detect and handle hazards, 'saruwatari' has an 'hazard control unit', whose 
input/output signals mapping can be found in 
https://github.com/kid-a/Saruwatari/raw/master/doc/control_unit_signals.ods

A more detailed description of how 'saruwatari' handles hazards can be found in
https://raw.github.com/kid-a/Saruwatari/master/doc/hazards.txt


Dependencies
------------
In order to simulate saruwatari, you need:
* ghdl, for the vhdl description (http://ghdl.free.fr/)
* gcc, for the SystemC description 
* SystemC (http://www.systemc.org/home/)
* a waveform visualizer (such as http://gtkwave.sourceforge.net/)

Authors
-------
* Daniele Marletta 
* Loris Fichera

License
-------
'saruwatari' is free software, as it is released under the GPL ver 2.1.