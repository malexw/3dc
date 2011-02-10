NAME=3dc
CFLAGS=-g -Wall
CC=g++
SRCDIR=src
BINDIR=bin
OBJECTS := $(patsubst %.cc,%.o,$(wildcard $(SRCDIR)/*.cc))

all: 3dc

3dc: $(OBJECTS)
	mkdir $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BINDIR)/$(NAME) $(OBJECTS)

clean:
	rm -f src/*.o
	rm -f $(BINDIR)/$(NAME)
	rmdir $(BINDIR)
