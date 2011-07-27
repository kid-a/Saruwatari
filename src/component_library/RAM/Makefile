# sorgenti necessari presenti nella stessa directory
SOURCES       = $(shell ls *.vhd)

# file oggetto, uno per ogni sorgente
OBJECTS        = $(SOURCES:.vhd=.o)

# nome completo del file di testbench
TBNAME    = $(shell ls | grep "_tb.vhd")

# ghdl vuole il nome dell'eseguibile uguale a quello del testbench (senza estensione ovviamente)
EXENAME		= $(shell grep entity $(TBNAME) | sed -r "s/entity ([^ ]*) is/\1/")

# nome del file su cui saranno scritti i valori dell'esecuzione della simulazione
VCDNAME     = data.vcd

# opzioni da da dare a ghdl per la simulazione
# file su cui scrivere i valori della simulazione
VCDOPT    = --vcd=${VCDNAME}

# tempo di esecuzione della simulazione
EXETIME       = --stop-time=100ns

# opzione per mostrare i valori delle variabili interne delle entità coinvolte
DISPTREE      = --disp-tree='inst'

# opzione per gtkwave
# percorso del file per personalizzare la GUI di gtkwave
WAVEOPT     = ~/.scriptgtkwave

IEEE	= --ieee=synopsys


# comando main, permette di creare l'eseguibile
# devono essere già presenti i file oggetto di ogni entità, compreso quello del testbench
main: $(OBJECTS)
	ghdl -e $(IEEE) $(EXENAME)

# comando implicito, ogni file oggetto dipende dal proprio file sorgente

%.o: %.vhd
	ghdl -a $(IEEE) $<

# comando che permette di controllare la sintassi dei file sorgenti, utile se manca il file di testbench e non si può procedere con la simulazione, rendendo quindi la compilazione del tutto inutile

control:
	ghdl -s $(SOURCES)

# pulisce tutto quello che avete creato: file oggetto, file eseguibile e file per la libreria.

clean:
	ghdl --clean; 
	-rm work-obj93.cf;
	-rm data.vcd

# ripetizione del comando implicito sopra, di non norma non dovrebbe quindi essere necessario utilizzarlo

compile:
	ghdl -a $(IEEE) $(SOURCES)

# attiva l'esecuzione della simulazione, scrivendo i dati nel file indicato nelle macro, per il tempo espresso sempre nelle macro

run:
	ghdl -r $(EXENAME) $(VCDOPT) $(EXETIME) $(DISPTREE)

# avvia gtkwave per visualizzare i valori dei segnali; l'opzione -S indica il file di script di gtkwave

wave:
	gtkwave $(VCDNAME) -S $(WAVEOPT)