BIN = gcc2msvc

CXXFLAGS := -Wall -Wextra -O3
LDFLAGS := -s
CLEANFILES = $(BIN) $(BIN)_test tmp_test.* *.obj *.ilk *.pdb *.exe
DISTCLEANFILES = config.h


all: $(BIN)

clean:
	-rm -f $(CLEANFILES)

distclean: clean
	-rm -f $(DISTCLEANFILES)

$(BIN): main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

main.cpp: config.h
config.h: config_default.h
	cp $< $@

test: $(BIN)
	./test.sh

