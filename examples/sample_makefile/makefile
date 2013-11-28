# Simple makefile showing the commandline arguments to g++
# used to compile a simple example with kodo
# Note, that the paths to the libraries depend
# on your specific machine and will need to be updated.

# By default, we assume that the required libraries (sak, fifi and boost)
# are cloned side-by-side with the kodo folder.
# You can clone the projects like this:
# git clone https://github.com/steinwurf/sak.git
# git clone https://github.com/steinwurf/fifi.git
# git clone https://github.com/steinwurf/external-boost-light.git
# git clone https://github.com/steinwurf/kodo.git
# Then build this example:
# cd kodo/examples/sample_makefile
# make

UNAME_S := $(shell uname -s)

# The include path to the kodo sources
KODO_DIR ?= ../../src

# The include path to the sak sources
SAK_DIR ?= ../../../sak/src

# The include path to the fifi sources
FIFI_DIR ?= ../../../fifi/src

# The include path to the boost sources
BOOST_DIR ?= ../../../external-boost-light

# Set default flags
INCLUDES = -I $(BOOST_DIR) -I $(SAK_DIR) -I $(FIFI_DIR) -I $(KODO_DIR)
CXXFLAGS = -std=c++0x -O2 -ftree-vectorize

# Choose default compiler based on the OS
ifeq ($(CXX),)
    ifeq ($(UNAME_S),Linux)
        CXX = g++
    endif
    ifeq ($(UNAME_S),Darwin)
        CXX = clang++
    endif
endif

ifeq ($(UNAME_S),Darwin)
    ifneq (,$(findstring clang++,$(CXX)))
        CXXFLAGS = -std=c++0x -O2 -stdlib=libc++ -lc++
    endif
    ifneq (,$(findstring c++,$(CXX)))
        CXXFLAGS = -std=c++0x -O2 -stdlib=libc++ -lc++
    endif
endif



# Invoke the compiler
all:
	$(CXX) main.cpp -o example $(CXXFLAGS) $(INCLUDES)
