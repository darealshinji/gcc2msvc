BIN = gcc2msvc

CXXFLAGS := -Wall -O3
LDFLAGS := -s
CLEANFILES = $(BIN) test.c test.cpp *.obj *.ilk *.pdb *.exe


all: $(BIN)

clean:
	-rm -f $(CLEANFILES)

test: $(BIN)
	./test.sh

