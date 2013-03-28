CC=g++

INCS= -I ./include

CFLAGS= $(INCS) -std=gnu++0x -O3 -Wall -g
LFLAGS= -lglut -lGLU -lGL

CPP_FILES= $(wildcard src/*.cpp)
OBJ_FILES= $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

all: cav2

cav2: $(OBJ_FILES) cav2.cpp
	$(CC) $(OBJ_FILES) cav2.cpp $(CFLAGS) $(LFLAGS) -o $@
	
obj/%.o: src/%.cpp | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj:
	mkdir obj
	
clean:
	rm $(OBJ_FILES)
	

