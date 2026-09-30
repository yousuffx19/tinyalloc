CC = gcc

TARGET = test
SOURCES = src/test.c src/talloc.c

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
