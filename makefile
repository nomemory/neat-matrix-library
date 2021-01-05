CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = smlc.o smlc_util.o
TARGET = smlc

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
