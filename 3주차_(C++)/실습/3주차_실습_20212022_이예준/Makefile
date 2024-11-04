CC=g++
Target=test
Object=main.o array.o rangearray.o

$(Target):$(Object)
	$(CC) -o $@ $^

array.o: Array.h
rangearray.o: RangeArray.h

.PHONY : clean
clean:
	rm $(Target) $(Object)