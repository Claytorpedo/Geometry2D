TOPDIR := .
ODIR := $(TOPDIR)/build
LIBDIR := $(TOPDIR)/lib

SRCDIRS := $(sort $(dir $(wildcard $(TOPDIR)/geom/*/)))
SRCS := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.cpp))
OBJS := $(patsubst $(TOPDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

MKDIRS := $(ODIR) $(patsubst $(TOPDIR)/%,$(ODIR)/%,$(SRCDIRS)) $(LIBDIR)

TESTDIR := $(TOPDIR)/test
TESTODIR := $(ODIR)/test
TESTSRCS := $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS := $(patsubst $(TESTDIR)/%.cpp,$(TESTODIR)/%.o,$(TESTSRCS))

LIBRARY := $(LIBDIR)/libgeom.a
TESTPROG := $(TOPDIR)/geom_test

CC = g++
COMP_FLAGS = -std=c++11 -Wall -pedantic
INCL_DIRS =
LINK_FLAGS =

DEBUG_FLAGS = -DDEBUG -g

.PHONY: all debug test runtest buildtests mktestdir clean help

##--------- OPTIONS ----------
##Set THREADED=TRUE to compile thread-safe version.
##----------------------------
ifeq ($(THREADED), TRUE)
 test: LINK_FLAGS += -pthread
 all test: COMP_FLAGS += -DTHREADED
endif

all:         ## Build the project library.
all: $(MKDIRS) $(OBJS) $(LIBRARY)

$(OBJS): $(ODIR)/%.o : $(TOPDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

$(LIBRARY): $(OBJS)
	ar -rcs $@ $^

debug:       ## Build the debug version of the project library.
debug: COMP_FLAGS += $(DEBUG_FLAGS)
debug: all

test:        ## Build all tests.
test: $(MKDIRS) mktestdir buildtests

runtest:     ## Build and run all tests.
runtest: test
	./$(TESTPROG)

buildtests: $(OBJS) $(TESTOBJS)
	$(CC) $^ $(LINK_FLAGS) -o $(TESTPROG)
	
$(TESTOBJS): $(TESTODIR)/%.o : $(TESTDIR)/%.cpp
	$(CC) -c $(INCL_DIRS) $(COMP_FLAGS) $< -o $@

$(MKDIRS):
	@mkdir -p $@

mktestdir:
	@mkdir -p $(TESTODIR)

clean:       ## Remove build files.
	rm -rf $(ODIR) $(LIBDIR) $(TESTPROG)

help:        ## Display this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'
