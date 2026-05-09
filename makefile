CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -Iinclude
LDFLAGS := -pthread

TARGET := sensor_monitor

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f src/*.o
	rm -f logs/*.log

.PHONY: all run clean