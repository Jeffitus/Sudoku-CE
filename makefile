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

# ----------------------------

include $(CEDEV)/meta/makefile.mk

$(OBJDIR)/myfonts.src: $(SRCDIR)/font/calvetica.inc

$(SRCDIR)/font/calvetica.inc: $(SRCDIR)/font/HELVE29.fnt
	convfont -o carray -f $(SRCDIR)/font/HELVE29.fnt $(SRCDIR)/font/calvetica.inc
