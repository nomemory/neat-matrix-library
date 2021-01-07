CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = nna.o nna_util.o
TARGET = nna

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
