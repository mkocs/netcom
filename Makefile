## Variables
# Compiler
CC = gcc

# Options for compiler
CFLAGS = -c

# cpp files & objects
SRC = $(wildcard src/*.c)
OBJ = $(addprefix build/,$(notdir $(SRC:.c=.o)))

# Directories
OBJDIR = build
SRCDIR = src


### Make rules

all: netcom

# Creates the final programm from the object files in build
netcom: $(OBJ)
	$(CC) -o ncomclient $(SRCDIR)/client.c
	$(CC) -o ncomserver $(SRCDIR)/server.c


# Creates all object files in build from the .cc files in src
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)/.dirstamp
	$(CC)	$(CFLAGS) -o $@ $<

# Creates the build directory
$(OBJDIR)/.dirstamp:
	mkdir -p $(OBJDIR)
	touch $@

client:
	$(CC) -o ncomclient $(SRCDIR)/client.c

server:
	$(CC) -o ncomserver $(SRCDIR)/server.c

# Cleans the build directory and removes the programm
clean:
	rm -rf $(OBJDIR) netcom
	rm ncomclient
	rm ncomserver
