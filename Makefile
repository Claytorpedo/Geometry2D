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
LINK_FLAGS =

DEBUG_FLAGS = -DDEBUG -g

.PHONY: all clean geometry dir testdir debug test runtest help

## --------- OPTIONS ----------
## Set THREADED=TRUE to compile thread-safe version.
## ----------------------------
ifeq ($(THREADED), TRUE)
 all test: LINK_FLAGS += -pthread
 all test: COMP_FLAGS += -DTHREADED
endif

all:         ## Build the project (without tests).
all: dir geometry

geometry: $(OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OBJS): $(ODIR)/%.o : $(TOPDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

debug:       ## Make debug build.
debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

test: dir testdir buildtests

runtest:     ## Build and run all tests.
runtest: test
	$(TOPDIR)/$(TESTPROG)

buildtests:  ## Build all tests.
buildtests: $(OBJS) $(TESTOBJS)
	$(CC) $^ $(LINK_FLAGS) -o $(TESTPROG)
	
$(TESTOBJS): $(TESTODIR)/%.o : $(TOPDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

testdir:
	mkdir -p $(ODIR)/$(TESTDIR)

dir:
	mkdir -p $(ODIR)
	$(foreach dir, $(SRCDIRS), mkdir -p $(ODIR)/$(dir))

clean:       ## Remove build files.
	rm -rf $(ODIR) $(TESTPROG)

help:        ## Display this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'
