# Décomenter une de ces trois lignes

GC=USE_TINY    # utiliser le mini GC fourni
#GC=USE_BOEHM   # utiliser le GC de Boehm si installé
#GC=NO_GC       # pas de GC (toute la mémoire ne sera pas libéré.

OSX=0 # mettre 1 sur Mac

ifeq '$(strip $(GC))' 'USE_TINY'
GC_SRC=tiny_gc.c
else
GC_SRC=
endif

ifeq '$(strip $(GC))' 'USE_BOEHM'
GC_LIB=-lgc
else
GC_LIB=
endif

SRCS=$(GC_SRC) triangulation.c geometry.c array.c triangulation_tools.c convex.c bezier.c route.c main.c 
OBJS=$(SRCS:.c=.o)
INCLUDES=
CC=gcc

ifeq '$(strip $(OSX))' '0'
CFLAGS= $(INCLUDES) -Wall -g -D $(GC)
LINKFLAGS=$(GC_LIB) -lm -lSDL -lSDL_image -lGL -lGLU
FRAMEWORK=
else
CFLAGS= $(INCLUDES) -Wall -g -D $(GC) -fnested-functions
LINKFLAGS=$(GC_LIB) -lm -lSDL
FRAMEWORK = -framework GLUT
FRAMEWORK += -framework OpenGL
FRAMEWORK +=  SDLmain.m -framework SDL -framework Cocoa
endif

.PHONY: depend clean

main.exe: $(OBJS)
	gcc $(FRAMEWORK) $(CFLAGS) -o $@ $(OBJS) $(LINKFLAGS)

depend:
	makedepend $(CFLAGS) $(SRCS)

clean:
	- rm main.exe $(OBJS) *~ #*#

# DO NOT DELETE

tiny_gc.o: /usr/include/stdlib.h /usr/include/features.h
tiny_gc.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/libio.h
tiny_gc.o: /usr/include/_G_config.h /usr/include/wchar.h
tiny_gc.o: /usr/include/assert.h /usr/include/setjmp.h tiny_gc.h
triangulation.o: /usr/include/stdlib.h /usr/include/features.h
triangulation.o: /usr/include/alloca.h /usr/include/stdio.h
triangulation.o: /usr/include/libio.h /usr/include/_G_config.h
triangulation.o: /usr/include/wchar.h /usr/include/assert.h tiny_gc.h
triangulation.o: triangulation.h
geometry.o: /usr/include/math.h /usr/include/features.h geometry.h
array.o: /usr/include/assert.h /usr/include/features.h /usr/include/string.h
array.o: /usr/include/xlocale.h /usr/include/stdio.h /usr/include/libio.h
array.o: /usr/include/_G_config.h /usr/include/wchar.h tiny_gc.h
array.o: /usr/include/stdlib.h /usr/include/alloca.h array.h
array.o: /usr/include/GL/gl.h /usr/include/GL/glext.h /usr/include/inttypes.h
array.o: /usr/include/stdint.h geometry.h triangulation.h
triangulation_tools.o: /usr/include/assert.h /usr/include/features.h
triangulation_tools.o: triangulation_tools.h array.h /usr/include/GL/gl.h
triangulation_tools.o: /usr/include/GL/glext.h /usr/include/inttypes.h
triangulation_tools.o: /usr/include/stdint.h geometry.h triangulation.h
convex.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
convex.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/assert.h
convex.o: /usr/include/stdlib.h /usr/include/alloca.h geometry.h array.h
convex.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
convex.o: /usr/include/inttypes.h /usr/include/stdint.h triangulation.h
convex.o: triangulation_tools.h convex.h tiny_gc.h
bezier.o: /usr/include/string.h /usr/include/features.h
bezier.o: /usr/include/xlocale.h geometry.h bezier.h
route.o: /usr/include/GL/gl.h /usr/include/GL/glext.h /usr/include/inttypes.h
route.o: /usr/include/features.h /usr/include/stdint.h /usr/include/GL/glu.h
route.o: /usr/include/SDL/SDL_image.h /usr/include/SDL/SDL.h
route.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
route.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
route.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
route.o: /usr/include/wchar.h /usr/include/stdlib.h /usr/include/alloca.h
route.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/strings.h
route.o: /usr/include/ctype.h /usr/include/endian.h /usr/include/iconv.h
route.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
route.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
route.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
route.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
route.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
route.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
route.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
route.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
route.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
route.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
route.o: /usr/include/SDL/SDL_version.h /usr/include/math.h bezier.h
route.o: geometry.h route.h
main.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/stdlib.h
main.o: /usr/include/alloca.h /usr/include/GL/gl.h /usr/include/GL/glext.h
main.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/GL/glu.h
main.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
main.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
main.o: /usr/include/SDL/SDL_platform.h /usr/include/string.h
main.o: /usr/include/xlocale.h /usr/include/strings.h /usr/include/ctype.h
main.o: /usr/include/endian.h /usr/include/iconv.h
main.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
main.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
main.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
main.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
main.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
main.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
main.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
main.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
main.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
main.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
main.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_image.h
main.o: /usr/include/SDL/SDL.h /usr/include/math.h tiny_gc.h geometry.h
main.o: array.h triangulation.h triangulation_tools.h convex.h route.h
main.o: bezier.h
