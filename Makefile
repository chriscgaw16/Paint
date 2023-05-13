paint.out: canvas.o com_args.o commands.o main.o
	gcc -Wall -g canvas.o com_args.o commands.o main.o -o paint.out

canvas.o: canvas.c canvas.h
	gcc -Wall -g -c canvas.c -o canvas.o

com_args.o: com_args.c com_args.h
	gcc -Wall -g -c com_args.c -o com_args.o

commands.o: commands.c com_args.h commands.h canvas.h
	gcc -Wall -g -c commands.c -o commands.o

main.o: main.c commands.h canvas.h com_args.h
	gcc -Wall -g -c main.c -o main.o

clean:
	rm *.o paint.out
