ringmaster: main.o terminal.o functions.o
			gcc main.o terminal.o functions.o -o ringmaster

%.o: %.c
	 gcc -c $*.c
