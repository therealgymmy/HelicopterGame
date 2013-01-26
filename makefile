CC := g++
CFLAGS := -Wall -Wextra
LIBS := -L/usr/X11R6/lib -lX11 -lpthread

PROG := p1
OBJ_FILES := main.o \
             tower.o \
             attributes.o \
             x_handler.o \
             events.o \
             helicopter.o \
             control.o \
             bomb.o \
             missile.o \
             explosion.o \
             misc.o \

OBJ := $(patsubst %, obj/%, $(OBJ_FILES))

obj/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^ $(LIBS)

$(PROG) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ) : | obj

obj :
	mkdir -p $@

.PHONY : clean
clean:
	rm $(OBJ) $(PROG)

.PHONY : run
run:
	./${PROG}
