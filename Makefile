# Set the compiler
CC = gcc
# Set the libraries to link
LFLAGS += -lm `pkg-config --cflags --libs opencv`
CFLAGS += -Wall -Werror
# Debug
#CFLAGS += -g

# The target
TARGET = homography
# Object files
OBJECTS = circles.o transformation.o main.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $(TARGET)

# Dependencies:

circles.o: circles.h

transformation.o: transformation.h 

main.o: circles.h transformation.h 


# This is a predefined rule of makefile, it knows the dependencies and thus can compile the related ".c" files in to ".o"
# The macro $< means "the first suffix of current target", i.e. ".c"
.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o
	rm homography
