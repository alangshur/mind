CXX = g++
CXXFLAGS = -std=c++17 -Wall
# CXXFLAGS_WARN_OFF += -Wno-unused-private-field
INCLUDE = -Iinclude
MODULES = $(shell find src -name *.cpp)
NAME = mind-engine

all: $(NAME)

$(NAME): $(MODULES)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_WARN_OFF) $(INCLUDE) $^ -o $@

.PHONY: clean
clean: 
	rm -f $(NAME) *.o