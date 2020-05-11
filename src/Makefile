IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = fsLow.h BitMap.h BlockManagementController.h Dir_Entry.h File_System_Info.h Free_Blocks.h SystemCalls.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o fsLow.o BitMap.o BlockManagementController.o Dir_Entry.o File_System_Info.o Free_Blocks.o SystemCalls.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 