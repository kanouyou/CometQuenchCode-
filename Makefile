
TARGET = SimQuench.exe
GXX    = g++
VPATH  = src
MAIN   = main.cpp

SRCS   = $(wildcard $(VPATH)/*.cpp) $(MAIN)
OBJS   = $(SRCS:.cpp=.o)

CXXFLAGS = -Wall -O3 -I include
CXXLIBS  = -lpython2.7

ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs)

CXXFLAGS += $(ROOTFLAGS)
CXXLIBS  += $(ROOTLIBS)


.PHONY: all install clean clear
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXLIBS) $^ -o $@

.o:.cpp
	$(CXX) $(CXXFLAGS) -c $<


clean:
	$(RM) -rf $(OBJS) $(TARGET)


install:
	mkdir -p bin
	mv -f $(TARGET)

clear:
	$(RM) -r build latex html *.log
