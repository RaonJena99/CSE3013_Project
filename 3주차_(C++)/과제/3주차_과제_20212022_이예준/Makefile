CC=g++
CFLAGS=-Wno-write-strings
Target=test
Object=main.o str.o

$(Target):$(Object)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY : clean
clean:
	rm $(Target) $(Object)