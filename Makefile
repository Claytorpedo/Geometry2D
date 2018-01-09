SRCDIR = .
ODIR = $(SRCDIR)/build

DIR = $(SRCDIR)/geom
SRC = $(shell find $(GEOMDIR)/ -type f -name '*.cpp')
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(GEOMSRC))

TESTDIR = $(SRCDIR)/test
TESTODIR = $(ODIR)
TESTSRCS = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS = $(patsubst $(SRCDIR)/%.cpp,$(TESTODIR)/%.o,$(TESTSRCS))

PROG = collisions
TESTPROG = geom_test

CC = g++

COMP_FLAGS = -std=c++11 -Wall -pedantic

DEBUG_FLAGS = -DDEBUG -g

.PHONY: all clean help dir testdir debug test runtest

all: dir $(PROG)

$(PROG): $(OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OBJS): $(ODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

test: dir testdir buildtests

runtest: test
	$(SRCDIR)/$(TESTPROG)

buildtests: $(OBJS) $(TESTOBJS)
	$(CC) $^ $(LINK_FLAGS) -o $(TESTPROG)
	
$(TESTOBJS): $(TESTODIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

testdir:
	mkdir -p $(ODIR)/$(TESTDIR)

dir:
	mkdir -p $(ODIR)

clean:
	rm -rf $(ODIR) $(PROG) $(TESTPROG)