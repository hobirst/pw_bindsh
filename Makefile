sources = main.c background.c check.c sha256.c sock.c

release:
	gcc $(sources) -o build/pw_bindsh

debug:
	gcc -D_DEBUG -ggdb $(sources) -o build/pw_bindsh_dbg

