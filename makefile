aman: fact shell

fact: fact.c
	gcc -o fact fact.c

shell: shell.c
	gcc -o shell shell.c

clean:
	rm -f fact shell