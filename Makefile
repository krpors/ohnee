CC = $(CXX)
CXXFLAGS = -std=c++11 -Wall -pedantic -I./include/ -MMD -MP
LDLIBS =  $(shell pkg-config --libs sfml-all)

sources = $(wildcard src/*.cpp)
objects = $(patsubst src/%.cpp, src/%.o, $(sources))

all: src/main
all: CPPFLAGS = -DNDEBUG
all: CXXFLAGS += -Werror

src/main: $(objects)

debug: src/main
debug: CPPFLAGS = -UNDEBUG
debug: CXXFLAGS += -ggdb -O0
debug: LDFLAGS += -ggdb -O0

clean:
	$(RM) $(objects) $(objects:.o=.d) ./src/main

-include $(objects:.o=.d)

.PHONY: all clean
