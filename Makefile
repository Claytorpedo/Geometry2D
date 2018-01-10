TOPDIR = .
ODIR = $(TOPDIR)/build
LIBDIR = $(TOPDIR)/lib

SRCDIRS = $(sort $(dir $(wildcard $(TOPDIR)/geom/*/)))
SRCS = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.cpp))
OBJS = $(patsubst $(TOPDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

MKDIRS = $(ODIR) $(patsubst $(TOPDIR)/%,$(ODIR)/%,$(SRCDIRS)) $(LIBDIR)

TESTDIR = $(TOPDIR)/test
TESTODIR = $(ODIR)/test
TESTSRCS = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS = $(patsubst $(TESTDIR)/%.cpp,$(TESTODIR)/%.o,$(TESTSRCS))

LIBRARY = $(LIBDIR)/libgeom.a
TESTPROG = geom_test

CC = g++

COMP_FLAGS = -std=c++11 -Wall -pedantic
LINK_FLAGS =

DEBUG_FLAGS = -DDEBUG -g

.PHONY: all clean dir testdir debug test runtest help

## --------- OPTIONS ----------
## Set THREADED=TRUE to compile thread-safe version.
## ----------------------------
ifeq ($(THREADED), TRUE)
 all test: LINK_FLAGS += -pthread
 all test: COMP_FLAGS += -DTHREADED
endif

all:         ## Build the project library.
all: dir $(OBJS) $(LIBRARY)

$(OBJS): $(ODIR)/%.o : $(TOPDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

$(LIBRARY): $(OBJS)
	ar -rcs $(LINK_FLAGS) $@ $^

debug:       ## Build the debug version of the project library.
debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

test:        ## Build all tests.
test: MKDIRS += $(TESTODIR)
test: dir buildtests

runtest:     ## Build and run all tests.
runtest: test
	$(TOPDIR)/$(TESTPROG)

buildtests: $(OBJS) $(TESTOBJS)
	$(CC) $^ $(LINK_FLAGS) -o $(TESTPROG)
	
$(TESTOBJS): $(TESTODIR)/%.o : $(TOPDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

testdir:
	mkdir -p $(ODIR)/$(TESTDIR)

dir:
	$(foreach dir, $(MKDIRS), mkdir -p $(dir))

clean:       ## Remove build files.
	rm -rf $(ODIR) $(LIBDIR) $(TESTPROG)

help:        ## Display this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'
