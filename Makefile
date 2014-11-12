IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj

LIBS=-lglfw -lXi -lXrandr -lGL -lGLU -llua -lm -lopenal -lalut

_DEPS = graphics.h luamodule.h alut_module.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = alut_module.o graphics.o luamodule.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

engine2: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
