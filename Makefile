DIR = bin
BIN = $(DIR)/main
OBJS = $(DIR)/main.o $(DIR)/display.o
CXX = g++ -std=c++0x -g
LIBS = -lGL -lSDL2_image -lSDL2

$(BIN): $(DIR) $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o$@

$(DIR):
	mkdir $(DIR)

$(OBJS): $(DIR)/%.o: %.cpp
	$(CXX) -std=c++0x -c $(cflags) $< -o$@

$(DIR)/main.o: display.h

.PHONY: clean test

test:
	./$(BIN)

clean:
	rm -v $(BIN) $(OBJS)
