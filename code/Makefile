CC := g++
CXX_STANDARD ?= c++17
OPT ?= -O3
WARN := -Wall -Wextra -Wpedantic
CXXFLAGS := $(OPT) $(WARN) -std=$(CXX_STANDARD)
LDFLAGS := -lm

TARGET := sim_cache
SIM_SRC := sim_cache.cc
SIM_OBJ := $(SIM_SRC:.cc=.o)

.PHONY: all clean clobber run help

all: $(TARGET)

$(TARGET): $(SIM_OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CC) $(CXXFLAGS) -c $<

clean:
	rm -f $(SIM_OBJ) $(TARGET)

clobber:
	rm -f $(SIM_OBJ)

run: $(TARGET)
	./$(TARGET) 32 8192 4 7 65536 8 ../ValidationTraces/gcc_trace.txt

help:
	@echo "Available targets:"
	@echo "  make all      - build simulator"
	@echo "  make clean    - remove binary and objects"
	@echo "  make clobber  - remove only objects"
	@echo "  make run      - run sample configuration"


