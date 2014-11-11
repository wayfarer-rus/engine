IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj

LIBS=-lglfw -lXi -lXrandr -lGL -lGLU -llua -lm

_DEPS = graphics.h luamodule.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = graphics.o luamodule.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

engine2: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
