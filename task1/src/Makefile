all: 
	gcc vm.c -o vm.o

debug: 
	gcc -g vm.c -o vm.o

validation:
	gcc vm.c -o vm.o; \
	./validate.sh; 
clean: 
	rm vm.o

task1:
	./vm.o bin/task1.bin
