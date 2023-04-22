# Set up compiler settings
# We will be compiling using GCC
CC = gcc

# Setting some useful flags:
# -std=c99 is the standard which you should be using
# -Wall means 'report all warnings' which is useful to help you write better code
# -Werror means 'make all warnings into errors' which means your code doesn't compile with warnings
# This is a good idea when code quality is important
# -g enables the use of GDB
CFLAGS = -std=c99 -Wall -Werror -g

# This is your list of executables which you want to compile with all
EXE = main

# We put 'all' as the first command as this will be run if you just enter 'make'
all: $(EXE)

# This is a rule to define how .o files will be compiled
# It means we do not have to write a rule for each .o file
# myprog.o will be compiled by this rule as:
# myprog.o: myprog.c
# gcc -c -std=c99 -g myprog.c -o myprog.o
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# For each executable, you need to tell the makefile the 'recipe' for your file
# At the moment, this is very simple as we are only using one .c file
# But as you refactor and add more .c and .h files
# These recipes will become more complex.

main: main.o hashTable.o mapAdjList.o mapReader.o
	$(CC) $(CFLAGS) $^ -o $@

# Clean removes all object files and executables
# Do not under any circumstances add .c or .h files
# rm is not reversible.
.PHONY: clean
clean: 
	rm -rf *.o $(EXE)
