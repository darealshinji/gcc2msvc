BIN  = gcc2msvc
OBJS = main.o system_return.o

CXXFLAGS := -Wall -Wextra -O3
CFLAGS   := -Wall -Wextra -O3
LDFLAGS  := -s

CLEANFILES = $(BIN) $(OBJS) $(BIN)_test tmp_test.* *.obj *.ilk *.pdb *.exe
DISTCLEANFILES = config.h


all: $(BIN)

clean:
	-rm -f $(CLEANFILES)

distclean: clean
	-rm -f $(DISTCLEANFILES)

$(BIN): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

main.cpp: config.h
config.h: config_default.h
	cp $< $@

test: $(BIN)
	./test.sh

