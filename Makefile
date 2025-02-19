# Makefile for fw_printenv

CC = gcc
CFLAGS = -Wall -Wextra -g -O2  # Enable all warnings and optimizations
#CFLAGS += -D_XOPEN_SOURCE=700   # Uncomment if using wordexp()
LDFLAGS =  # Add any linker flags here (e.g., -lm for math library)

TARGET = fw_printenv
SOURCES = fw_printenv.c
OBJECTS = $(SOURCES:.c=.o)

# By default, build the fw_printenv executable
all: $(TARGET)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link the object files into the executable
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install rule (optional, requires sudo)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin
	sudo chown root:root /usr/local/bin/$(TARGET)  # Ensure proper ownership
	sudo chmod 755 /usr/local/bin/$(TARGET)   # Set executable permissions

# Uninstall rule (optional, requires sudo)
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

.PHONY: all clean install uninstall

