gcc -o engine main.c luamodule.c graphics.c alut_module.c gamepad_module.c -lglfw3 -lXi -lXrandr -lGLU -llua5.2 -I/usr/include/lua5.2 -lopenal -lalut
