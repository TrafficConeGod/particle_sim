TARGET := app

SOURCES := $(wildcard src/*.c) $(wildcard src/*.cpp)
LIBS := -lGL -lglut -lGLEW -lglfw -lX11 -lXi -lXrandr -lm
WARNS := -Wall -Wextra -Wdouble-promotion -Wduplicate-decl-specifier -Wformat=2 -Wnull-dereference -Wimplicit-fallthrough
OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
DEPENDS := $(patsubst %.c,%.d,$(patsubst %.cpp,%.d,$(SOURCES)))

CFLAGS = -O3 -Werror -Wno-error=unused-variable -std=c2x -isystem /usr/include/freetype2 -g
CXXFLAGS = $(CFLAGS) -fno-exceptions -std=c++2a

.PHONY: build run clean

build: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET).elf $(OBJECTS) $(LIBS)

run: build
	@./$(TARGET).elf

clean:
	$(RM) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(WARNS) -MMD -MP -c $< -o $@

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) $(WARNS) -MMD -MP -c $< -o $@