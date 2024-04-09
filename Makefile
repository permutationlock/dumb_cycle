CC = gcc
CFLAGS = -std=c99 -O2 -nostdlib -fstrict-aliasing -fno-stack-protector \
		 -fno-unwind-tables -fno-asynchronous-unwind-tables \
		 -pedantic -Wall -Wextra -Werror -Wconversion -Wdouble-promotion \
		 -Wcast-align -Wstrict-prototypes -Wold-style-definition \
		 -Wno-unused-parameter -fsanitize=undefined -fsanitize-trap
AS = as
LINKER = ld

all: dumb_cycle

run: dumb_cycle
	./dumb_cycle && clear

cproc: dumb_cycle_cproc

run_cproc: dumb_cycle_cproc
	./dumb_cycle_cproc && clear

dumb_cycle: src/main.o src/syscall.o src/runtime.o
	$(LINKER) -o dumb_cycle src/main.o src/syscall.o src/runtime.o

clean_dumb_cycle: clean_main clean_runtime clean_syscall
	rm -f dumb_cycle

dumb_cycle_cproc: src/main_cproc.o src/syscall.o src/runtime.o
	$(LINKER) -o dumb_cycle_cproc src/main_cproc.o src/syscall.o src/runtime.o

clean_dumb_cycle_cproc: clean_main_cproc clean_runtime clean_syscall
	rm -f dumb_cycle_crpoc

src/runtime.o: src/runtime.s
	$(AS) -o src/runtime.o src/runtime.s

clean_runtime:
	rm -f src/runtime.o

src/syscall.o: src/syscall.s
	$(AS) -o src/syscall.o src/syscall.s

clean_syscall:
	rm -f src/syscall.o

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o src/main.o src/main.c

clean_main:
	rm -f src/main.o

src/main_cproc.o: src/main_cproc.s
	$(AS) -o src/main_cproc.o src/main_cproc.s

src/main_cproc.s: src/main_cproc.ssa
	qbe -o src/main_cproc.s src/main_cproc.ssa

src/main_cproc.ssa: src/main.c
	cproc-qbe src/main.c > src/main_cproc.ssa

clean_main_cproc:
	rm -f src/main_cproc.ssa
	rm -f src/main_cproc.s
	rm -f src/main_cproc.o

clean: clean_dumb_cycle clean_dumb_cycle_cproc
