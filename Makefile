OUT = chaos

SRC = $(wildcard src/*.cc) $(wildcard src/**/*.cc)
OBJ = $(SRC:.cc=.o)
CXX = clang++
CXXFLAGS = -Wall -std=c++14 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lSDL2 -lSDL2_mixer
DEP = $(OBJ:.o=.d)

$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

-include $(DEP)

%.d: %.cc
	$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(OBJ) $(DEP) $(OUT)
