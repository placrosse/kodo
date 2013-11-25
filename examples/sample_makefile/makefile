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

# The include path to the kodo sources
kodo_dir = ../../src

# The include path to the sak sources
sak_dir = ../../../sak/src

# The include path to the fifi sources
fifi_dir = ../../../fifi/src

# The include path to the boost sources
boost_dir = ../../../external-boost-light

# Invoke the compiler
all:
	g++ main.cpp -o example -std=c++0x -I $(boost_dir) -I $(fifi_dir) -I $(kodo_dir) -I $(sak_dir)

# Use the following command on Mac OSX to compile with clang++
#all:
#	clang++ main.cpp -o example -std=gnu++0x -stdlib=libc++ -lc++ -I $(boost_dir) -I $(fifi_dir) -I $(kodo_dir) -I $(sak_dir)