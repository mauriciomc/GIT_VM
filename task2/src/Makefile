clean: 
	rm task2.svg

task2: 
	../../task1/src/vm.o task2.bin < ../result.txt | grep -v "Virtual" | tr -s '\n' | tail -n +2 > task2.svg
