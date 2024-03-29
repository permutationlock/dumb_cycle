# Dumb Cycle

An online multi-player lightcycle game for Linux implemented in `x86_64`
assembly and C99 with no linked libraries (no libc) and no preprocessor
directives (no `#ifdefs`,  `#define`, `#include`, etc).

Images are drawn to the screen using the "dumb buffer" API provided by the
Direct Rendering Manager (DRM) subsystem of Linux. Keyboard input is read
directly from a keyboard device file. Dumb Cycle should run on any system
running a Linux kernel with DRM support, a video card, and a keyboard.

Dumb Cycle can be compiled with any C compiler that supports C99 and ATT
syntax `x86_64` assembly. The primary motivation for eschewing the C
preprocessor was to allow the project to compile using only
[cproc-qbe][1], [QBE][2], [as][3], and [ld][4].
Passing the game code through cproc and QBE results in ~3000
lines of simple and readable `x86_64` assembly.
