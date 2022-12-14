all:
		gcc -o run run.c basic.c
		gcc -o run2 run2.c basic.c
		gcc -o fast fast.c basic.c
		gcc -o findbestblock findbestblock.c basic.c
clean:
		rm -f run
		rm -f run2
		rm -f fast
		rm -f findbestblock