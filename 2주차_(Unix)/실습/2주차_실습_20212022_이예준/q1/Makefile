CC = gcc
Target = animal
Objects = dog.o blackcow.o turtle.o main.o

$(Target) : $(Objects)
	$(CC) -o $@ $^

.PHONY : clean
clean :
	rm $(Targets) $(Objects)
