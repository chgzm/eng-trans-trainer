PROGRAM := eng-trans-trainer
OBJS := Main.o TestGenerator.o
CXX := g++
FLGS := -std=c++17
CXXFLAGS := -Wall -O3

.PHONY: all
$(PROGRAM): $(OBJS)
	$(CXX) $(FLGS) $(CXXFLAGS) -o $(PROGRAM) $(OBJS)

.cpp.o:
	$(CXX) $(FLGS) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROGRAM)
