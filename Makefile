CXX = g++
CXXFLAGS = -std=c++17 -Wall
# CXXFLAGS_WARN_OFF += -Wno-unused-private-field
INCLUDE = -Iinclude
MODULES = $(shell find src -name *.cpp)
LIBS = # ...
NAME = mind-engine

all: $(NAME)

$(NAME): $(MODULES)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_WARN_OFF) $(LIBS) $(INCLUDE) $^ -o $@

run:
	@make all > /dev/null
	@./mind-engine
	@make clean > /dev/null

.PHONY: clean
clean: 
	rm -f $(NAME) *.o