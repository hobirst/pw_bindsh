sources = main.c daemon.c check.c sha256.c sock.c
outdir = build

.prep:
	mkdir -p $(outdir)

release: .prep
	gcc $(sources) -o $(outdir)/pw_bindsh

debug: .prep
	gcc -D_DEBUG -ggdb $(sources) -o $(outdir)/pw_bindsh_dbg

