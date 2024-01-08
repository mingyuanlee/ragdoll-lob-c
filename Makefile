# Define the C++ compiler
CXX=g++

# Define any compile-time flags
CXXFLAGS=-Wall -g

# Define any directories containing header files
INCLUDES=

# Define library paths in addition to /usr/lib
LFLAGS=

# Define any libraries to link into executable
LIBS=

# Define source directory
SRCDIR=src

# Define build directory
BUILDDIR=build

# Define the C++ source files
SRCS=$(wildcard $(SRCDIR)/*.cpp)

# Define the C++ header files
HDRS=$(wildcard $(SRCDIR)/*.h)

# Define the C++ object files
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Define the executable file
MAIN=orderbook

.PHONY: depend clean

all:	$(BUILDDIR)	$(MAIN)
	@echo	Simple compiler named orderbook has been compiled

$(MAIN):	$(OBJS) 
	$(CXX)	$(CXXFLAGS)	$(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	$(RM) $(BUILDDIR)/*.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
