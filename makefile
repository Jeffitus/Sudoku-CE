# ----------------------------
# Set NAME to the program name
# Set ICON to the png icon file name
# Set DESCRIPTION to display within a compatible shell
# Set COMPRESSED to "YES" to create a compressed program
# ----------------------------

NAME        ?= SUDOKU
COMPRESSED  ?= YES
COMPRESSED_MODE ?= zx0
ICON        ?= iconc.png
DESCRIPTION ?= "Sudoku CE by Jeffitus"
ARCHIVED	?= YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)

FONTDIR ?= ce-fonts/calvetica
FONTNAME ?= Calvetic

font:
	$(MAKE) -C $(FONTDIR) CALCNAME=$(FONTNAME)
	mv $(FONTDIR)/$(FONTNAME).bin $(BINDIR)/$(FONTNAME).bin
	mv $(FONTDIR)/$(FONTNAME).8xv $(BINDIR)/$(FONTNAME).8xv