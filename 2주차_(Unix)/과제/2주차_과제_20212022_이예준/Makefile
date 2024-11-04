CC=gcc
Target=Fox
Objects=main.o Count.o Print.o Init.o

$(Target):$(Objects) Header.h
	$(CC) -o $@ $^

.PHONY:clean
clean:
	rm $(Target) $(Objects)
