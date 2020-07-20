tcc:
	$(CC) -o bin/tcc *.c -v

.PHONY : clean
clean:
	rm bin/tcc