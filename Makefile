all: dumb_cycle

dumb_cycle: src/main.o src/syscall.o src/runtime.o
	ld -o dumb_cycle src/main.o src/syscall.o src/runtime.o

clean_dumb_cycle:
	rm -f dumb_cycle

src/runtime.o: src/runtime.s
	as -o src/runtime.o src/runtime.s

clean_runtime:
	rm -f src/runtime.o

src/syscall.o: src/syscall.s
	as -o src/syscall.o src/syscall.s

clean_syscall:
	rm -f src/syscall.o

src/main.o: src/main.s
	as -o src/main.o src/main.s

src/main.s: src/main.ssa
	qbe -o src/main.s src/main.ssa

src/main.ssa: src/main.c
	cproc-qbe src/main.c > src/main.ssa

clean_main:
	rm -f src/main.ssa
	rm -f src/main.s
	rm -f src/main.o

clean: clean_dumb_cycle clean_main clean_runtime clean_syscall
