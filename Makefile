CC=g++
CFLAGS=-g -O2 -std=c++17 -I include -I D:\ProgrammingStuff\SFMLStatic\SFMLFinal\include -DSFML_STATIC
LDFLAGS=-L lib $(OPTLIBS) -L D:\ProgrammingStuff\SFMLStatic\SFMLFinal\lib
LIBS=-lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-network-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype -ljpeg  -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32 -lcomdlg32 -mwindows
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst src/%.cpp,build/%.o,$(SOURCES))
DEPS=$(patsubst %.o,%.d,$(OBJECTS))

TARGET=bin/main

all: $(TARGET)

dev: CFLAGS=-g -Wall -Iinclude -Wall -Wextra $(OPTFLAGS)

dev: all

$(TARGET): build $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -MMD -o $@ -c $< $(LIBS)

include $(DEPS)

$(DEPS): ;

build:
	@mkdir -p build
	@mkdir -p bin

clean:
	rm -rf bin build
