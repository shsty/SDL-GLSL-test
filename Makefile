DIR = bin
BIN = $(DIR)/main
OBJS = $(DIR)/main.o $(DIR)/display.o

$(BIN): $(OBJS) $(DIR)
	g++ -std=c++0x $(OBJS) -lGL -lSDL2 -o$@

$(DIR):
	mkdir $(DIR)

$(OBJS): $(DIR)/%.o: %.cpp $(DIR)
	g++ -std=c++0x -c $(cflags) $< -o$@

$(DIR)/main.o: main.cpp display.h
$(DIR)/display.o: display.cpp

.PHONY: clean

clean:
	rm -v $(BIN) $(OBJS)
