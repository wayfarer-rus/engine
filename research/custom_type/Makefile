IDIR =.
CC=gcc
ODIR=obj

INCLUDES=-I$(IDIR)

CFLAGS=-g
LIBS=-llua -luuid

src = numarray.c tree.c main.c
objects = $(patsubst %.c,obj/%.o,$(src))

program=custom_type_test

.PHONY: all
all: $(program)

$(program): $(objects)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(objects): | obj

obj:
	@mkdir -p $@

obj/%.o : %.c
	@echo $<
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ $(program)


