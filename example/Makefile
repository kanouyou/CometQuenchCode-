## Makefile for example
## @author: Y. Yang
#

NAME   = Construct
TARGET = $(NAME).exe
CXX  = g++
SRCS = construct.cpp
OBJS = $(SRCS:.cpp=.o)

CXXLIBS  = -L/Users/yangye/Documents/Software/quenchfdm/build/bin -lSimQuench
CXXFLAGS = -Wall -O3 -I/Users/yangye/Documents/Software/quenchfdm/v.1.0/include

ROOTLIBS  = $(shell root-config --glibs)
ROOTFLAGS = $(shell root-config --cflags)

CXXLIBS  += $(ROOTLIBS)
CXXFLAGS += $(ROOTFLAGS)

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXLIBS) $^ -o $@

.o:.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	$(RM) -r $(OBJS) $(TARGET)


