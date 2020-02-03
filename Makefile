#------------------------------------------------------------------
#Project
#------------------------------------------------------------------
#Name of the program or library.
NAME := geom

#Build with debug or release configuration. Set from command line like "make CONFIG=release".
CONFIG := debug

#Type of output. If making a static or shared library, set that here.
#Options: EXE for executable, A for static library, SO for shared library.
TYPE := A

#What file extension you're compiling.
COMPILE_EXT := .cpp

#The directory we're working from.
#Use this if your main project is in a directory other than the makefile.
TOPDIR := .
#Directory to put build files.
BUILDDIR := $(TOPDIR)/build
#Build output directory (the program or library).
OUTDIR := $(TOPDIR)/lib

#Where your project files are located, relative to TOPDIR.
#Can include multiple locations separated by a space.
#A useful rule to use may be:
# $(sort $(dir $(wildcard $(TOPDIR)/*/)))
#This will get a list of sub directories in your top directory, though not recursively.
#The dir function gets the path part of each file, and use sort as it removes duplicates.
#If you want recursion, probably best to use a shell command:
# $(sort $(dir $(shell find "$(TOPDIR)/" -name "*$(COMPILE_EXT)")))
#Watch out for grabbing build folders when grabbing sub directories.
SRCDIRS := $(sort $(dir $(wildcard $(TOPDIR)/geom/*/)))

#------------------------------------------------------------------
#Compiler settings
#------------------------------------------------------------------
#Set up compile and link flags here.

#This makes output files have suffixes depending on debug/release build.
APPEND_CONFIG_TYPE := YES
CONFIG_APPEND.debug   := _d
CONFIG_APPEND.release := _r

COMPILER := g++
#Set language level or extra warnings here.
COMP_FLAGS := -std=c++17 -Wall -Wextra -Werror -pedantic
#Set libraries for linking here.
#Useful to use either package config for an instaled dependency or a direct path to a library (e.g. a submodule):
#`pkg-config --libs sdl2`
#-Lpath/to/my/lib/ -lmylib$(CONFIG_APPEND.$(CONFIG))
LDFLAGS := 
#Set include directories for compilation here, similar to LDFLAGS.
#`pkg-config --cflags sdl2`
#-Ipath/to/my/include/dir
INCL_DIRS := 

#Debug mode flags.
DEBUG_FLAGS := -DDEBUG -g
#Release mode flags.
RELEASE_FLAGS := -DNDEBUG -O2

#------------------------------------------------------------------
#Tests
#------------------------------------------------------------------
#Set up optional tests here to use with "make test" and "make runtest".
#Specify any additional test settings. Settings here miror above rules.
#Expects that tests compilie to an executable (even if the main project doesn't).

#Indicate whether this makefile will define runnable tests. YES/NO
TESTS_ENABLED := YES

ifeq ($(TESTS_ENABLED),YES)
 #If you want to remove some items without redefining a whole category, you can filter them.
 # TESTDIRS := $(filter-out srcDir1 srcDir2, $(SRCDIRS)) testDir
 #This will get all source dirs except for srcDir1 and srcDir2, and adds testDir.
 TESTDIRS := $(SRCDIRS) $(TOPDIR)/test
 TESTOUTDIR := $(TOPDIR)
 TEST_COMP_FLAGS := $(COMP_FLAGS)
 TEST_LDFLAGS := $(LDFLAGS)
 TEST_INCL_DIRS := $(INCL_DIRS)

 #If there are specific source files to ignore in test builds (e.g. src/main.cpp), name them here.
 TEST_IGNORE_SRCS := 
endif

#------------------------------------------------------------------
#Configuration Generation
#------------------------------------------------------------------
#These next steps are automatic and hopefully shouldn't need modification for simple projects.

#Build dir with subdirectory of build configuration, e.g. build/debug
WORKINGDIR := $(BUILDDIR)/$(CONFIG)

ifeq ($(TESTS_ENABLED),YES)
 #Override SRCDIRS on test builds so we can reuse the same variables below.
 #Use a filter (rather than a target) so it runs immediately.
 ifneq (,$(filter $(MAKECMDGOALS), test runtest))
  SRCDIRS := $(TESTDIRS)
 endif
endif

#Get the sources to compile from source directories.
SRCS = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*$(COMPILE_EXT)))

ifeq ($(TESTS_ENABLED),YES)
 #Filter out any unwanted sources for test builds. Similar to above, use a filter so it runs immediately.
 ifneq (,$(filter $(MAKECMDGOALS), test runtest))
  SRCS := $(filter-out $(TEST_IGNORE_SRCS), $(SRCS))
 endif
endif

#Make names for objects. Object files let us reuse a built file if it hasn't changed between compilations.
OBJS = $(patsubst $(TOPDIR)/%$(COMPILE_EXT),$(WORKINGDIR)/%.o,$(SRCS))
#Make names for dependencies. Dependencies allow us to rebuild a file when included headers change.
DEPS = $(patsubst $(TOPDIR)/%$(COMPILE_EXT),$(WORKINGDIR)/%.d,$(SRCS))

#Directories to create.
MKDIRS = $(WORKINGDIR) $(patsubst $(TOPDIR)/%,$(WORKINGDIR)/%,$(SRCDIRS)) $(OUTDIR)

#Flags for handling dependencies. MMD generates dependencies on non-system header files.
#MP makes a phoney target for each dependency, to avoid errors if you delete a header file and recompile
DEPS_FLAGS := -MMD -MP

CXXFLAGS.debug   := $(DEBUG_FLAGS)
CXXFLAGS.release := $(RELEASE_FLAGS)
CXXFLAGS := $(COMPILER) $(INCL_DIRS) $(COMP_FLAGS) $(DEPS_FLAGS) $(CXXFLAGS.$(CONFIG))

ifeq ($(TESTS_ENABLED),YES)
 #When building tests, override the type to an executable.
 test runtest: TYPE := EXE
 CXXFLAGS_TEST := $(COMPILER) $(TEST_INCL_DIRS) $(TEST_COMP_FLAGS) $(DEPS_FLAGS) $(CXXFLAGS.$(CONFIG))
endif

ifeq ($(TYPE), SO)
 #Use fPIC for "Position Independent Code" while compiling shared libs.
 CXXFLAGS += -fPIC
endif

#Name of the final file to output ----
OUTPUT_FILE := $(OUTDIR)/$(NAME)
ifneq ($(TYPE),EXE)
 #Prepend "lib" for library files.
 OUTPUT_FILE := $(OUTDIR)/lib$(NAME)
endif
ifeq ($(APPEND_CONFIG_TYPE), YES)
 OUTPUT_FILE := $(OUTPUT_FILE)$(CONFIG_APPEND.$(CONFIG))
endif

ifeq ($(TESTS_ENABLED),YES)
 TEST_OUTPUT_FILE := $(TESTOUTDIR)/$(NAME)_test
 ifeq ($(APPEND_CONFIG_TYPE),YES)
  TEST_OUTPUT_FILE := $(TEST_OUTPUT_FILE)$(CONFIG_APPEND.$(CONFIG))
 endif
endif
#-------------------------------------

#Set up our link flags for different output types.
LINK.A   = ar -rcs $(OUTPUT_FILE).a $^
LINK.SO  = $(COMPILER) $^ -shared $(LDFLAGS) -o $(OUTPUT_FILE).so
LINK.EXE = $(COMPILER) $^ $(LDFLAGS) -o $(OUTPUT_FILE)

#------------------------------------------------------------------
#Rules
#------------------------------------------------------------------
#The main build step. Make directories, build any submodules, then build the program/library.

#Function to generate a list for use in regex.
#First item inserted normally, every item following separated by | like one|two|three.
build_regex_list = $(if $(1),$(1)|$(2),$(2))

#Default build step. Makes directories, builds submodules, then builds target.
.PHONY: all
.DEFAULT_GOAL := all
all:            ## Default target to build.
all: directories $(SUBMODS) build

#Make any needed directories.
#The @ in front makes the command silent (no console output).
.PHONY: directories
directories:
	@mkdir -p $(MKDIRS)

#Build sub modules, passing down the build command(s).
.PHONY: $(SUBMODS)
$(SUBMODS):
	$(MAKE) -C $@ $(SUBMODCMD)

#If build order of submodules is important, declare dependencies here.
# subdir2: subdir1 #Build subdir2 after subdir1 has completed building.

#Main build step. Build submodules, then objects, then grab the right link rule.
.PHONY: build
build: $(OBJS) | $(SUBMODS)
	$(LINK.$(TYPE))

#Build step.
#Build each object with the prerequisite that either there isn't a matching object in the build dir,
#or its last modified date is older than the source file in the source dir.
$(OBJS): $(WORKINGDIR)/%.o : $(TOPDIR)/%$(COMPILE_EXT)
	$(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:          ## Clean this project and all submodules.
clean: clean_local clean_submods

.PHONY: clean_local
clean_local:    ## Clean only this project, ignoring submodules.
	rm -rf $(WORKINGDIR) $(OUTPUT_FILE) $(TEST_OUTPUT_FILE)

.PHONY: clean_submods
clean_submods:  ## Clean only submodules.
clean_submods: SUBMODCMD := clean
clean_submods: $(SUBMODS)

ifeq ($(TYPE),EXE) # run is only valid for EXE output.
 .PHONY: run
 run:            ## Build then run an executable.
 run: SUBMODCMD := all
 run: all
	./$(OUTPUT_FILE)
else
 #Not an executable. Remove run target from help output.
 IGNORED_HELP := $(call build_regex_list,$(IGNORED_HELP),run:)
endif

ifeq ($(TESTS_ENABLED),YES)
 .PHONY: test
 test:           ## Build tests.
 test: CXXFLAGS := $(CXXFLAGS_TEST)
 test: OUTPUT_FILE := $(TEST_OUTPUT_FILE)
 test: SUBMODCMD := all # Default build submoduels -- no recursive testing.
 test: all

 .PHONY: runtest
 runtest:        ## Build then run tests.
 runtest: test
	./$(TEST_OUTPUT_FILE)
else
 #Tests not set up. Remove the test and runtest targets from help output.
 IGNORED_HELP := $(call build_regex_list,$(IGNORED_HELP),test:|runtest:)
endif

#If ignored help isn't empty, create regex filter to ignore some rules.
ifneq ($(IGNORED_HELP),)
 IGNORE_REGEX = (?!$(IGNORED_HELP))
endif
#Filter out any ignored rules, then look for rules with a double # after them.
HELP_REGEX = ^ *$(IGNORE_REGEX)([.a-zA-Z_-]+):.*?\#\# .*$$
.PHONY: help
help:           ## Display this help.
	@grep -P '$(HELP_REGEX)' $(firstword $(MAKEFILE_LIST)) | sed 's/^ *//' | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

# Include all dependency files. Use "-" flavour because they might not exist yet (e.g. new file being compiled).
-include $(DEPS)
