CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g


SRC_FILES = main.c Read.c Util.c ReadMore.c
OBJ_FILES = $(SRC_FILES:.c=.o)
EXECUTABLE = tema3

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $(OBJ_FILES) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)

