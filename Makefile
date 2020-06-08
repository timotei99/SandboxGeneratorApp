# change application name here (executable output name)
TARGET=generator_app

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=    test.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
    
test.o: src/sandbox_generator.c
	$(CC) -c $(CCFLAGS) src/sandbox_generator.c $(GTKLIB) -o test.o
    
clean:
	rm -f *.o $(TARGET)