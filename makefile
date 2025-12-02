# Makefile simples e funcional para BATALHA-NAVAL
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRCDIR = src
TESTDIR = tests
SRC = $(SRCDIR)/main.c \
      $(SRCDIR)/board.c \
      $(SRCDIR)/fleet.c \
      $(SRCDIR)/game.c \
      $(SRCDIR)/io.c \
      $(SRCDIR)/random.c

OBJ = $(SRC:.c=.o)

TARGET = batalha-naval
TEST_TARGET = test-suite

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# regra para compilar apenas objetos (útil se quiser debugar)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

test: $(TESTDIR)/main_test.c
	$(CC) $(CFLAGS) $(TESTDIR)/main_test.c $(SRCDIR)/board.c $(SRCDIR)/fleet.c $(SRCDIR)/random.c -o $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(SRCDIR)/*.o $(TESTDIR)/*.o $(TARGET) $(TEST_TARGET)