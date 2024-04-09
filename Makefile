CC = gcc
CFLAGS = -fno-stack-protector -nostdlib
AS = as
LINKER = ld

all: dumb_cycle

cproc: dumb_cycle_cproc

dumb_cycle: src/main.c src/syscall.s src/runtime.s
	$(CC) $(CFLAGS) -o dumb_cycle src/main.c src/syscall.s src/runtime.s

clean_dumb_cycle:
	rm -f dumb_cycle

dumb_cycle_cproc: src/main.o src/syscall.o src/runtime.o
	$(LINKER) -o dumb_cycle_cproc src/main.o src/syscall.o src/runtime.o

clean_dumb_cycle_cproc: clean_main clean_runtime clean_syscall
	rm -f dumb_cycle_crpoc

src/runtime.o: src/runtime.s
	$(AS) -o src/runtime.o src/runtime.s

clean_runtime:
	rm -f src/runtime.o

src/syscall.o: src/syscall.s
	$(AS) -o src/syscall.o src/syscall.s

clean_syscall:
	rm -f src/syscall.o

src/main.o: src/main.s
	$(AS) -o src/main.o src/main.s

src/main.s: src/main.ssa
	qbe -o src/main.s src/main.ssa

src/main.ssa: src/main.c
	cproc-qbe src/main.c > src/main.ssa

clean_main:
	rm -f src/main.ssa
	rm -f src/main.s
	rm -f src/main.o

clean: clean_dumb_cycle clean_dumb_cycle_cproc
