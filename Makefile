OBJDIR=objs

CXX=clang++
LIBS=sdl2 glew
CXXFLAGS+=-c -Wall -Wextra -pedantic -g -std=c++11
CXXFLAGS+=`pkg-config --cflags $(LIBS)`
LDFLAGS=`pkg-config --libs $(LIBS)` -lSOIL
SOURCES=$(wildcard *.cpp)

OBJECTS=$(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
TARGET=gltest

all: $(TARGET)

dir:
	mkdir -p $(OBJDIR)

$(TARGET): dir $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

gdbrun: $(TARGET)
	gdb $(TARGET)
