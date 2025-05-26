# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g

# OpenSSL library for encryption
LDFLAGS = -lssl -lcrypto

# Source files
SRCS = journal.c
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = journal

# Default rule to compile everything
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean command to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: all
	./$(TARGET)
