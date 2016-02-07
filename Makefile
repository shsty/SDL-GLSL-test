DIR = bin
BIN = $(DIR)/main
OBJS = $(DIR)/main.o $(DIR)/display.o
CXX = g++ -std=c++0x -g

$(BIN): $(DIR) $(OBJS)
	$(CXX) $(OBJS) -lGL -lSDL2 -o$@

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
