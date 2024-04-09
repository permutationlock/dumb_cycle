# Dumb Cycle

A simple Snake/LightCycle game for `x86_64` Linux implemented in
assembly and C99 with no linked libraries (no libc) and no preprocessor
directives (no `#ifdefs`,  `#define`, `#include`, etc).

Images are drawn to the screen using the "dumb buffer" API provided by the
Direct Rendering Manager (DRM) subsystem of Linux. Keyboard input is read
directly from a keyboard device file. Dumb Cycle should run on any `x86_64`
Linux system with DRM support, a video card, and a keyboard.

Dumb Cycle can be compiled with any C compiler that supports C99 and ATT
syntax `x86_64` assembly. The primary motivation for eschewing the C
preprocessor was to allow the project to compile using only
[cproc-qbe][1], [QBE][2], and [GNU binutils][3] (for `as` and `ld`)].
Passing the game code through cproc and QBE results just over 2000
lines of simple and readable `x86_64` assembly.

[1]: https://sr.ht/~mcf/cproc/
[2]: https://c9x.me/compile/
[3]: https://sourceware.org/binutils/
