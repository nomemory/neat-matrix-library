CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = nml.o nml_util.o
TARGET = nml

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
