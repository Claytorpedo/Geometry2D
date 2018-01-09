TOPDIR = .
ODIR = $(TOPDIR)/build

SRCDIRS = $(sort $(dir $(wildcard $(TOPDIR)/geom/*/)))
SRCS = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.cpp))
OBJS = $(patsubst $(TOPDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

TESTDIR = $(TOPDIR)/test
TESTODIR = $(ODIR)
TESTSRCS = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS = $(patsubst $(TOPDIR)/%.cpp,$(TESTODIR)/%.o,$(TESTSRCS))

TESTPROG = geom_test

CC = g++

COMP_FLAGS = -std=c++11 -Wall -pedantic

DEBUG_FLAGS = -DDEBUG -g

.PHONY: all clean collisions help dir testdir debug test runtest

all: dir collisions

collisions: $(OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OBJS): $(ODIR)/%.o : $(TOPDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

test: dir testdir buildtests

runtest: test
	$(TOPDIR)/$(TESTPROG)

buildtests: $(OBJS) $(TESTOBJS)
	$(CC) $^ $(LINK_FLAGS) -o $(TESTPROG)
	
$(TESTOBJS): $(TESTODIR)/%.o : $(TOPDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

testdir:
	mkdir -p $(ODIR)/$(TESTDIR)

dir:
	mkdir -p $(ODIR)
	$(foreach dir, $(SRCDIRS), mkdir -p $(ODIR)/$(dir))

clean:
	rm -rf $(ODIR) $(TESTPROG)