all:
	@gcc source/main.c -I./include -o dnaReader
	@./dnaReader main.dna -c

clean:
	rm dnaReader