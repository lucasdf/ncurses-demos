FLAGS = -Wall -pthread -g -lncurses
INCLUDES =
LINT_LIBS = -lform -lmenu -lpanel -lncurses
OUTPUT_DIR = ./build
CC = gcc

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
all:
	rm -f ./compile_commands.json && bear make _all
_all: $(OBJS)

clean:
	rm -f ./build/*


%.o: %.c
	$(CC) $(FLAGS) $(INCLUDES) -o $(OUTPUT_DIR)/$@ $^ $(LINT_LIBS)
