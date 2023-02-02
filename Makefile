all:
	@make dnaReader
	@make output

output:
	@./dnaReader main.dna -c

dnaReader:
	@g++ source/main.cpp -o dnaReader