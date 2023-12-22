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

# Define the C++ source files
SRCS=RBTree.cpp LOB.cpp main.cpp

# Define the C++ header files
HDRS=RBTree.h LOB.h

# Define the C++ object files
OBJS=$(SRCS:.cpp=.o)

# Define the executable file
MAIN=orderbook

.PHONY: depend clean

all:	$(MAIN)
	@echo	Simple compiler named orderbook has been compiled

$(MAIN):	$(OBJS) 
	$(CXX)	$(CXXFLAGS)	$(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# This is a suffix replacement rule for building .o's from .cpp's
# It uses automatic variables $<: the name of the prerequisite of the rule(a .cpp file)
# and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX)	$(CXXFLAGS)	$(INCLUDES) -c $<  -o $@

clean:
	$(RM)	*.o *~	$(MAIN)

depend: $(SRCS)
	makedepend	$(INCLUDES)	$^

# DO NOT DELETE THIS LINE -- make depend needs it
