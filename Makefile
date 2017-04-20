BIN = gcc2msvc

CXXFLAGS := -Wall -O3
LDFLAGS := -s
CLEANFILES = $(BIN) test.c test.cpp test.obj test.*.exe


all: $(BIN)

clean:
	-rm -f $(CLEANFILES)

test:
	./test.sh

