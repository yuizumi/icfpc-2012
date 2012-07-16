SRCS = $(wildcard *.cc)
HDRS = $(wildcard *.h)

BINDIR=../bin

OBJS = debugutil.o input.o map.o state.o warps.o
TARGETS = $(BINDIR)/lifter $(BINDIR)/validate

CXX = clang++
CXXFLAGS = -O3 -I/opt/local/include -Wall $(NDEBUG)

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

$(BINDIR)/%: $(OBJS) %.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/lifter: solve.o solveutil.o

.depend: $(SRCS) $(HDRS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) >> ./.depend
-include .depend

.PHONY: clean
