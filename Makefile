all:
	clang-format -i *.c
	gcc map_reader.c -o map_reader.bin
clean:
	rm -vf *.bin
