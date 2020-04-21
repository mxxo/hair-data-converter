ifeq ($(OS),Windows_NT)
	REMOVE = del
	SUFFIX = .exe
else
	REMOVE = -rm -f
	SUFFIX = $()
endif

default: hair2gmsh

all: hair2gmsh hair2obj

hair2gmsh: src/hair2gmsh.cpp
	g++ -std=c++11 src/Hair.cc src/hair2gmsh.cpp -o hair2gmsh -Iinclude -Ivendor/trimesh2/include -Wall -Wextra -Wno-unknown-pragmas

hair2obj: src/hair2obj.cpp
	g++ -std=c++11 src/Hair.cc src/hair2obj.cpp -o hair2obj -Iinclude -Ivendor/trimesh2/include -Wall -Wextra -Wno-unknown-pragmas

clean:
	$(REMOVE) hair2gmsh$(SUFFIX) hair2obj$(SUFFIX)

.PHONY:
	all default clean
