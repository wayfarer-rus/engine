vpath %.c ./src
IDIR =./include
CC=gcc
ODIR=obj
CFLAGS=-O2 -I$(IDIR) -I/usr/include/chipmunk
LIBS=-lglfw -lXi -lXrandr -lGL -lGLU -lm -pthread -lchipmunk -lopenal -lalut -llua

src = crossplatform_utils.c gamepad_module.c graphics.c stb_image.c alut_module.c lua_module.c engine_core.c engine_main.c
objects = $(patsubst %.c,obj/%.o,$(src))

program=engine

.PHONY: all
all: $(program)

$(program): $(objects)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(objects): | obj

obj:
	@mkdir -p $@

obj/%.o : %.c
	@echo $<
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ $(program)


