all: dumb_cycle

debug: dumb_cycle_dbg

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

dumb_cycle_dbg: src/main.c src/syscall.o src/runtime.o
	zig cc --target=x86_64-linux-musl -g3 -std=c99 -nostdlib \
	 -fstrict-aliasing \
     -fsanitize=undefined -fsanitize-trap \
	 -Wcast-align -Wstrict-prototypes -Wold-style-definition \
	 -Werror -Wall -Wextra -Wconversion -Wdouble-promotion \
     -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion \
	-o dumb_cycle_dbg src/main.c src/syscall.o src/runtime.o

clean_dumb_cycle_dbg:
	rm -f dumb_cycle_dbg

clean: clean_dumb_cycle clean_dumb_cycle_dbg \
	clean_main clean_runtime clean_syscall
