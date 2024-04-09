# DumbCycle

A simple Snake/LightCycle game for `x86_64` Linux implemented in
assembly and C99 with no linked libraries (no libc) and no preprocessor
directives (no `#ifdef`,  `#define`, `#include`, etc).

Images are drawn to the screen using the "dumb buffer" API provided by the
Direct Rendering Manager (DRM) subsystem of Linux. Keyboard input is read
directly from a keyboard device file. DumbCycle should run on any `x86_64`
Linux system with DRM support, a video card, and a keyboard[^1]. 

DumbCycle can be compiled with any C compiler that supports C99 and ATT
syntax `x86_64` assembly. The primary motivation for eschewing the C
preprocessor is to allow the project to compile using only
[cproc-qbe][1], [QBE][2], and [GNU binutils][3] (for `as` and `ld`).
Passing the game code through cproc and QBE results just over 2000
lines of simple and readable `x86_64` assembly.

## References

 - [musl libc][4]
 - [libdrm][5]
 - [linux kernel][6]
 - [drm\_howto][7]

 [^1]: The executable must have permissions to open the video device
    `/dev/dri/card0` and the input devices in `/dev/input/`.
    No other programs should be drawing to `/dev/drm/card0`, i.e.
    you need to close your X11 and/or Wayland server.

[1]: https://sr.ht/~mcf/cproc/
[2]: https://c9x.me/compile/
[3]: https://sourceware.org/binutils/
[4]: https://musl-libc.org
[5]: https://gitlab.freedesktop.org/mesa/drm
[6]: https://github.com/torvalds/linux
[7]: https://github.com/dranger003/drm-howto
