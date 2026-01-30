NAME=3dc
CXX ?= c++
CXXFLAGS ?= -g -Wall -std=c++11
CXXFLAGS += -I$(SRCDIR) -I$(SRCDIR)/codec
SRCDIR=src
BINDIR=bin
OBJECTS := $(patsubst %.cc,%.o,$(wildcard $(SRCDIR)/*.cc $(SRCDIR)/codec/*/*.cc))

all: 3dc

3dc: $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BINDIR)/$(NAME) $(OBJECTS)

clean:
	rm -f src/*.o src/codec/*/*.o
	rm -f $(BINDIR)/$(NAME)
	rm -rf $(BINDIR)
