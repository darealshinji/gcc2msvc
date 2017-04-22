BIN = gcc2msvc

CXXFLAGS := -Wall -Wextra -O3
LDFLAGS := -s
CLEANFILES = $(BIN) $(BIN)_test tmp_test.* *.obj *.ilk *.pdb *.exe


all: $(BIN)

clean:
	-rm -f $(CLEANFILES)

test: $(BIN)
	./test.sh

