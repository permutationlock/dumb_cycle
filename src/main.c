typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long i64;
typedef unsigned long u64;

typedef i32 e32;
typedef i32 b32;

static void *memcpy(void *restrict dest, const void *restrict src, u64 count);
static void *memset(void *dest, int val, u64 count);

extern void halt(void);

enum syscall {
    SYS_READ = 0,
    SYS_WRITE = 1,
    SYS_OPEN = 2,
    SYS_CLOSE = 3,
    SYS_MMAP = 9,
    SYS_MUNMAP = 11,
    SYS_IOCTL = 16,
    SYS_EXIT = 60,
    SYS_GETDENTS = 78,
    SYS_CLOCK_GETTIME = 228,
    SYS_EPOLL_WAIT = 232,
    SYS_EPOLL_CTL = 233,
    SYS_EPOLL_CREATE1 = 291
};

extern u64 syscall0(u64 scid);
extern u64 syscall1(u64 scid, u64 a1);
extern u64 syscall2(u64 scid, u64 a1, u64 a2);
extern u64 syscall3(u64 scid, u64 a1, u64 a2, u64 a3);
extern u64 syscall4(u64 scid, u64 a1, u64 a2, u64 a3, u64 a4);
extern u64 syscall5(u64 scid, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5);
extern u64 syscall6(u64 scid, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6);

static e32 syscall_error(u64 rvalue);

enum error_code { EINTR = 4 };
enum open { O_RDONLY = 00, O_WRONLY = 01, O_RDWR = 02, O_CLOEXEC = 02000000 };
enum prot { PROT_READ = 1, PROT_WRITE = 2 };
enum map { MAP_SHARED = 0x01, MAP_PRIVATE = 0x02, MAP_ANON = 0x20 };
enum clock { CLOCK_MONOTONIC = 1 };

typedef char *cstring;

struct timespec {
    i64 sec;
    i64 nsec;
};

union epoll_data {
    void *data;
    i32 fd;
};

struct epoll_event {
    u32 events;
    union epoll_data data;
};

enum epoll_ctl { EPOLL_CTL_ADD = 1 };
enum epoll_event_code { EPOLLIN = 0x001 };

struct linux_dirent {
    u64 ino;
    u64 off;
    u16 reclen;
    char name[];
};

static i64 read(i32 fd, u8 *bytes, i64 bytes_len);
static i64 write(i32 fd, u8 *bytes, i64 bytes_len);
static i32 open(cstring fname, i32 flags, i32 mode);
static e32 close(i32 fd);
static void *mmap(
    void *hint,
    i64 size,
    i32 prot,
    i32 flags,
    i32 fd,
    i64 offset
);
static e32 munmap(void *start, i64 size);
static e32 ioctl(i32 fd, i32 request, u8 *arg);
static void exit(e32 error_code);
static i64 getdents(i32 fd, struct linux_dirent *dents, i64 dents_len);
static e32 clock_gettime(i32 clock_id, struct timespec *timespec);
static i32 epoll_wait(
    i32 epfd,
    struct epoll_event *events,
    i32 events_len,
    i32 timeout_ms
);
static e32 epoll_ctl(i32 epfd, i32 op, i32 fd, struct epoll_event *event);
static i32 epoll_create1(void);

struct arena {
    u8 *start;
    u8 *end;
};

static void *alloc(struct arena *arena, i64 size, i64 align);

static i64 nsec_since(struct timespec *end, struct timespec *start);

enum ev_code { EV_KEY = 0x01, EV_MAX = 0x1f };
enum ev_keys {
    KEY_ESC = 1,
    KEY_W = 17,
    KEY_A = 30,
    KEY_S = 31,
    KEY_D = 32,
    KEY_MAX = 0x2ff
};

struct input_event {
    struct timespec time;
    u16 type;
    u16 code;
    i32 value;
};

static i32 test_bit(u8 *bytes, i32 len, i32 bit_num);
static e32 evio_cg_getbits(i32 fd, u8 event_code, i16 size, u8 *data);
static b32 is_keyboard(i32 fd);
static i32 open_keyboard(struct arena temp_arena);

struct drm_mode_resources {
    u32 *fbs;
    u32 *crtcs;
    u32 *connectors;
    u32 *encoders;

    i32 fbs_len;
    i32 crtcs_len;
    i32 connectors_len;
    i32 encoders_len;

    u32 min_width;
    u32 max_width;
    u32 min_height;
    u32 max_height;
};

struct drm_mode_modeinfo {
    u32 clock;

    u16 hdisplay;
    u16 hsync_start;
    u16 hsync_end;
    u16 htotal;
    u16 hskew;

    u16 vdisplay;
    u16 vsync_start;
    u16 vsync_end;
    u16 vtotal;
    u16 vscan;

    u32 vrefresh;

    u32 flags;
    u32 type;
    char name[32];
};

struct drm_mode_connector {
    u32 *encoders;
    struct drm_mode_modeinfo *modes;
    u32 *props;
    u64 *prop_values;

    u32 modes_len;
    u32 props_len;
    u32 encoders_len;

    u32 encoder_id;
    u32 connector_id;

    u32 connector_type;
    u32 connector_type_id;
    u32 connection;
    u32 mm_width;
    u32 mm_height;
    u32 subpixel;
    u32 pad;
};

struct drm_mode_encoder {
    u32 encoder_id;
    u32 encoder_type;

    u32 crtc_id;

    u32 possible_crtcs;
    u32 possible_clones;
};

struct drm_mode_crtc {
    u32 *set_connectors;
    u32 connectors_len;

    u32 crtc_id;
    u32 fb_id;

    u32 x;
    u32 y;

    u32 gamma_size;
    u32 mode_valid;
    struct drm_mode_modeinfo mode;
};

struct drm_mode_fb_cmd {
    u32 fb_id;
    u32 width;
    u32 height;
    u32 pitch;
    u32 bpp;
    u32 depth;
    u32 handle;
};

struct drm_mode_create_dumb {
    u32 height;
    u32 width;
    u32 bpp;
    u32 flags;
    u32 handle;
    u32 pitch;
    u64 size;
};

struct drm_mode_map_dumb {
    u32 handle;
    u32 pad;
    u64 offset;
};

struct drm_mode_dumb_buffer {
    u32 width;
    u32 height;
    u32 stride;
    u32 handle;
    u32 fb_id;
    u32 *map;
    u64 size;
};

enum drm_ioctl_mode {
    DRM_IOCTL_MODE_GETRESOURCES = (i32)0xc04064a0,
    DRM_IOCTL_MODE_GETCONNECTOR = (i32)0xc05064a7,
    DRM_IOCTL_MODE_GETENCODER = (i32)0xc01464a6,
    DRM_IOCTL_MODE_GETCRTC = (i32)0xc06864a1,
    DRM_IOCTL_MODE_SETCRTC = (i32)0xc06864a2,
    DRM_IOCTL_MODE_PAGE_FLIP = (i32)0xc01864b0,
    DRM_IOCTL_MODE_ADDFB = (i32)0xc01c64ae,
    DRM_IOCTL_MODE_CREATE_DUMB = (i32)0xc02064b2,
    DRM_IOCTL_MODE_MAP_DUMB = (i32)0xc01064b3
};
enum drm_mode { DRM_MODE_CONNECTED = 1 };

static struct drm_mode_resources *drm_mode_get_resources(
    struct arena *perm_arena,
    i32 fd
);
static struct drm_mode_connector *drm_mode_get_connector(
    struct arena *perm_arena,
    i32 fd,
    u32 connector_id
);
static struct drm_mode_encoder *drm_mode_get_encoder(
    struct arena *perm_arena,
    i32 fd,
    u32 encoder_id
);
static struct drm_mode_crtc *drm_mode_get_crtc(
    struct arena *perm_arena,
    i32 fd,
    u32 crtc_id
);
static e32 drm_mode_set_crtc(
    i32 fd,
    struct drm_mode_crtc *crtc,
    u32 *connectors,
    i32 connectors_len,
    u32 fb_id
);
static struct drm_mode_dumb_buffer *drm_mode_create_dumb_buffer(
    struct arena *perm_arena,
    i32 fd,
    u32 width,
    u32 height
);

enum color {
    COLOR_BLACK = 0x000000,
    COLOR_RED = 0xff0000,
    COLOR_GREEN = 0x00ff00,
    COLOR_BLUE = 0x0000ff,
    COLOR_WHITE = 0xffffff,
    COLOR_GRAY = 0xededed
};

struct game_state {
    i32 x;
    i32 y;
    i32 vx;
    i32 vy;
    i32 dead;
    u8 board[90 * 90];
};

static void update_game_state(struct game_state *state);
static void clear_game_state(struct game_state *state);
static void draw_board(
    struct drm_mode_dumb_buffer *buf,
    struct game_state *state,
    i32 x,
    i32 y,
    i32 scale
);

static void *memcpy(void *restrict dest, const void *restrict src, u64 count) {
    u8 *d = dest;
    const u8 *s = src;
    for (i64 i = 0; i < (i64)count; ++i) {
        d[i] = s[i];
    }
    return dest;
}

static void *memset(void *dest, int val, u64 count) {
    u8 *d = dest;
    for (i64 i = 0; i < (i64)count; ++i) {
        d[i] = (u8)val;
    }
    return dest;
}

static e32 syscall_error(u64 rvalue) {
    if (rvalue > -4096UL) {
        return (e32)(-rvalue);
    }
    return 0;
}

static i64 read(i32 fd, u8 *bytes, i64 bytes_len) {
    u64 return_value;
    e32 error;
    do {
        return_value = syscall3(SYS_READ, (u64)fd, (u64)bytes, (u64)bytes_len);
        error = syscall_error(return_value);
    } while (error == EINTR);

    if (error != 0) {
        return -error;
    }
    return (i64)return_value;
}

static i64 write(i32 fd, u8 *bytes, i64 bytes_len) {
    u64 return_value;
    e32 error;
    do {
        return_value = syscall3(SYS_WRITE, (u64)fd, (u64)bytes, (u64)bytes_len);
        error = syscall_error(return_value);
    } while (error == EINTR);

    if (error != 0) {
        return -error;
    }
    return (i64)return_value;
}

static i32 open(cstring fname, i32 mode, i32 flags) {
    u64 return_value;
    e32 error;
    do {
        return_value = syscall3(SYS_OPEN, (u64)fname, (u64)mode, (u64)flags);
        error = syscall_error(return_value);
    } while (error == EINTR);

    if (error != 0) {
        return -error;
    }
    return (i32)return_value;
}

static e32 close(i32 fd) {
    u64 return_value;
    e32 error;
    do {
        return_value = syscall1(SYS_CLOSE, (u64)fd);
        error = syscall_error(return_value);
    } while (error == EINTR);
    return error;
}

static void *mmap(
    void *hint,
    i64 size,
    i32 prot,
    i32 flags,
    i32 fd,
    i64 offset
) {
    i64 return_value =
        syscall6(SYS_MMAP, (u64)hint, (u64)size, prot, flags, fd, offset);
    e32 error = syscall_error(return_value);
    if (error != 0) {
        return 0;
    }
    return (void *)return_value;
}

static e32 munmap(void *start, i64 size) {
    u64 return_value = syscall2(SYS_MUNMAP, (u64)start, (u64)size);
    return syscall_error(return_value);
}

static e32 ioctl(i32 fd, i32 request, u8 *arg) {
    u64 return_value;
    e32 error;
    do {
        return_value = syscall3(SYS_IOCTL, (u64)fd, (u64)request, (u64)arg);
        error = syscall_error(return_value);
    } while (error == EINTR);
    return error;
}

static void exit(e32 error_code) {
    syscall1(SYS_EXIT, (u64)error_code);
}

static i64 getdents(i32 fd, struct linux_dirent *dents, i64 dents_size_bytes) {
    u64 return_value =
        syscall3(SYS_GETDENTS, (u64)fd, (u64)dents, (u64)dents_size_bytes);
    e32 error = syscall_error(return_value);
    if (error != 0) {
        return -error;
    }
    return return_value;
}

static e32 clock_gettime(i32 clock_id, struct timespec *timespec) {
    u64 return_value =
        syscall2(SYS_CLOCK_GETTIME, (u64)clock_id, (u64)timespec);
    return syscall_error(return_value);
}

static i32 epoll_wait(
    i32 epfd,
    struct epoll_event *events,
    i32 events_len,
    i32 timeout_ms
) {
    u64 return_value = syscall4(
        SYS_EPOLL_WAIT, (u64)epfd, (u64)events, (u64)events_len, (u64)timeout_ms
    );
    e32 error = syscall_error(return_value);
    if (error != 0) {
        return -error;
    }
    return (i32)return_value;
}

static e32 epoll_ctl(i32 epfd, i32 op, i32 fd, struct epoll_event *event) {
    u64 return_value =
        syscall4(SYS_EPOLL_CTL, (u64)epfd, (u64)op, (u64)fd, (u64)event);
    return syscall_error(return_value);
}

static i32 epoll_create1(void) {
    u64 return_value = syscall1(SYS_EPOLL_CREATE1, O_CLOEXEC);
    e32 error = syscall_error(return_value);
    if (error != 0) {
        return -error;
    }
    return (i32)return_value;
}

static void *alloc(struct arena *arena, i64 size, i64 align) {
    i64 available = arena->end - arena->start;
    i64 padding = -(u64)arena->start & (align - 1);
    if (size > (available - padding)) {
        return 0;
    }
    u8 *p = arena->start + padding;
    arena->start += padding + size;
    return memset(p, 0, size);
}

static i64 nsec_since(struct timespec *end, struct timespec *start) {
    i64 seconds = end->sec - start->sec;
    i64 elapsed = (seconds * 1000L * 1000L * 1000L) + end->nsec;
    return elapsed - start->nsec;
}

static i32 test_bit(u8 *bytes, i32 len, i32 bit_num) {
    i32 byte_index = bit_num / 8;
    i32 bit_index = bit_num % 8;
    if (byte_index >= len) {
        return 0;
    }

    return (bytes[byte_index] & (1 << bit_index)) != 0;
}

static e32 evio_cg_getbits(i32 fd, u8 event_code, i16 size, u8 *data) {
    return ioctl(
        fd,
        ((u32)event_code + 0x20) | ((u32)'E' << 8) | ((u32)size << 16) |
            (2U << 30),
        data
    );
}

static i32 is_keyboard(i32 fd) {
    u8 evbits[(EV_MAX + 1) / 8];
    e32 error = evio_cg_getbits(fd, 0, sizeof(evbits), evbits);
    if (error != 0) {
        return 0;
    }
    if ((evbits[0] & EV_KEY) == 0) {
        return 0;
    }

    u8 keybits[(KEY_MAX + 1) / 8];
    error = evio_cg_getbits(fd, EV_KEY, sizeof(keybits), keybits);
    if (error != 0) {
        return 0;
    }

    if (!test_bit(keybits, sizeof(keybits), KEY_ESC)) {
        return 0;
    }
    if (!test_bit(keybits, sizeof(keybits), KEY_W)) {
        return 0;
    }
    if (!test_bit(keybits, sizeof(keybits), KEY_A)) {
        return 0;
    }
    if (!test_bit(keybits, sizeof(keybits), KEY_S)) {
        return 0;
    }
    if (!test_bit(keybits, sizeof(keybits), KEY_D)) {
        return 0;
    }

    return 1;
}

static i32 open_keyboard(struct arena temp_arena) {
    u8 input_dir[] = "/dev/input";
    i32 input_dir_fd = open((cstring)input_dir, O_RDONLY | O_CLOEXEC, 0);
    if (input_dir_fd < 0) {
        return -1;
    }

    struct linux_dirent *dents = alloc(&temp_arena, 1024, 16);
    u8 path_buffer[(sizeof(input_dir) - 1) + 1023];
    memcpy(path_buffer, input_dir, sizeof(input_dir));
    path_buffer[sizeof(input_dir) - 1] = '/';
    u8 *name_buffer = &path_buffer[sizeof(input_dir)];

    i64 dents_pos = 0;
    i64 dents_len = 0;
    i32 keyboard_fd = -1;
    while (keyboard_fd < 0) {
        if (dents_pos >= dents_len) {
            dents_len = getdents(input_dir_fd, dents, 1023);
            if (dents_len <= 0) {
                break;
            }
        }
        struct linux_dirent *dent = (void *)(((u8 *)dents) + dents_pos);
        memcpy(name_buffer, dent->name, dent->reclen - 18);
        path_buffer[sizeof(path_buffer) - 1] = 0;
        keyboard_fd = open((cstring)path_buffer, O_RDONLY, 0);
        if (keyboard_fd >= 0) {
            if (!is_keyboard(keyboard_fd)) {
                close(keyboard_fd);
                keyboard_fd = -1;
            }
        }
        dents_pos += dent->reclen;
    }

    close(input_dir_fd);

    return keyboard_fd;
}

static struct drm_mode_resources *drm_mode_get_resources(
    struct arena *perm_arena,
    i32 fd
) {
    struct drm_mode_resources prev_res;
    struct drm_mode_resources *res;
    struct arena setup_arena;
    e32 error;

    do {
        setup_arena = *perm_arena;
        res = alloc(&setup_arena, sizeof(*res), 16);
        if (res == 0) {
            return 0;
        }
        error = ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, (u8 *)res);
        if (error != 0) {
            return 0;
        }

        prev_res = *res;

        if (res->fbs_len > 0) {
            res->fbs =
                alloc(&setup_arena, res->fbs_len * sizeof(*res->fbs), 16);
            if (res->fbs == 0) {
                return 0;
            }
        }
        if (res->crtcs_len > 0) {
            res->crtcs =
                alloc(&setup_arena, res->crtcs_len * sizeof(*res->crtcs), 16);
            if (res->crtcs == 0) {
                return 0;
            }
        }
        if (res->connectors_len > 0) {
            res->connectors = alloc(
                &setup_arena, res->connectors_len * sizeof(*res->connectors), 16
            );
            if (res->connectors == 0) {
                return 0;
            }
        }
        if (res->encoders_len > 0) {
            res->encoders = alloc(
                &setup_arena, res->encoders_len * sizeof(*res->encoders), 16
            );
            if (res->encoders == 0) {
                return 0;
            }
        }

        error = ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, (u8 *)res);
        if (error != 0) {
            return 0;
        }
    } while (prev_res.fbs_len < res->fbs_len ||
             prev_res.crtcs_len < res->crtcs_len ||
             prev_res.connectors_len < res->connectors_len ||
             prev_res.encoders_len < res->encoders_len);

    *perm_arena = setup_arena;
    return res;
}

static struct drm_mode_connector *drm_mode_get_connector(
    struct arena *perm_arena,
    i32 fd,
    u32 connector_id
) {
    struct drm_mode_connector prev_conn;
    struct drm_mode_connector *conn;
    struct arena setup_arena;
    e32 error;

    do {
        setup_arena = *perm_arena;
        conn = alloc(&setup_arena, sizeof(*conn), 16);
        if (conn == 0) {
            return 0;
        }
        conn->connector_id = connector_id;
        error = ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, (u8 *)conn);
        if (error != 0) {
            return 0;
        }

        prev_conn = *conn;

        if (conn->props_len > 0) {
            conn->props =
                alloc(&setup_arena, conn->props_len * sizeof(*conn->props), 16);
            conn->prop_values = alloc(
                &setup_arena, conn->props_len * sizeof(*conn->prop_values), 16
            );
            if (conn->props == 0 || conn->prop_values == 0) {
                return 0;
            }
        }
        if (conn->modes_len > 0) {
            conn->modes =
                alloc(&setup_arena, conn->modes_len * sizeof(*conn->modes), 16);
            if (conn->modes == 0) {
                return 0;
            }
        }
        if (conn->encoders_len > 0) {
            conn->encoders = alloc(
                &setup_arena, conn->encoders_len * sizeof(*conn->encoders), 16
            );
            if (conn->encoders == 0) {
                return 0;
            }
        }

        error = ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, (u8 *)conn);
        if (error != 0) {
            return 0;
        }
    } while (prev_conn.props_len < conn->props_len ||
             prev_conn.modes_len < conn->modes_len ||
             prev_conn.encoders_len < conn->encoders_len);

    *perm_arena = setup_arena;
    return conn;
}

static struct drm_mode_encoder *drm_mode_get_encoder(
    struct arena *perm_arena,
    i32 fd,
    u32 encoder_id
) {
    struct arena setup_arena = *perm_arena;
    struct drm_mode_encoder *enc = 0;

    enc = alloc(&setup_arena, sizeof(*enc), 16);
    if (enc == 0) {
        return 0;
    }

    enc->encoder_id = encoder_id;
    e32 error = ioctl(fd, DRM_IOCTL_MODE_GETENCODER, (u8 *)enc);
    if (error != 0) {
        return 0;
    }

    *perm_arena = setup_arena;
    return enc;
}

static struct drm_mode_crtc *drm_mode_get_crtc(
    struct arena *perm_arena,
    i32 fd,
    u32 crtc_id
) {
    struct arena setup_arena = *perm_arena;
    struct drm_mode_crtc *crtc = 0;

    crtc = alloc(&setup_arena, sizeof(*crtc), 16);
    if (crtc == 0) {
        return 0;
    }

    crtc->crtc_id = crtc_id;
    e32 error = ioctl(fd, DRM_IOCTL_MODE_GETCRTC, (u8 *)crtc);
    if (error != 0) {
        return 0;
    }

    *perm_arena = setup_arena;
    return crtc;
}

static e32 drm_mode_set_crtc(
    i32 fd,
    struct drm_mode_crtc *crtc,
    u32 *connectors,
    i32 connectors_len,
    u32 fb_id
) {
    crtc->set_connectors = connectors;
    crtc->connectors_len = connectors_len;
    crtc->fb_id = fb_id;
    return ioctl(fd, DRM_IOCTL_MODE_SETCRTC, (u8 *)crtc);
}

static struct drm_mode_dumb_buffer *drm_mode_create_dumb_buffer(
    struct arena *perm_arena,
    i32 fd,
    u32 width,
    u32 height
) {
    struct drm_mode_create_dumb creq = {
        .width = width,
        .height = height,
        .bpp = 32,
    };
    e32 error;

    error = ioctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, (u8 *)&creq);
    if (error != 0) {
        return 0;
    }

    struct drm_mode_fb_cmd fb_cmd = {
        .width = width,
        .height = height,
        .pitch = creq.pitch,
        .bpp = 32,
        .depth = 24,
        .handle = creq.handle,
    };
    error = ioctl(fd, DRM_IOCTL_MODE_ADDFB, (u8 *)&fb_cmd);
    if (error != 0) {
        return 0;
    }

    struct drm_mode_map_dumb mreq = { .handle = creq.handle };
    error = ioctl(fd, DRM_IOCTL_MODE_MAP_DUMB, (u8 *)&mreq);
    if (error != 0) {
        return 0;
    }

    u32 *mem =
        mmap(0, creq.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);
    if (mem == 0) {
        return 0;
    }

    struct drm_mode_dumb_buffer *buf;
    buf = alloc(perm_arena, sizeof(*buf), 16);
    buf->width = width;
    buf->height = height;
    buf->stride = creq.pitch / sizeof(u32);
    buf->size = creq.size / sizeof(u32);
    buf->handle = creq.handle;
    buf->map = mem;
    buf->fb_id = fb_cmd.fb_id;

    memset(buf->map, 0, buf->size);

    return buf;
}

static void update_game_state(struct game_state *state) {
    state->board[state->y * 90 + state->x] = 1;
    state->y += state->vy;
    state->x += state->vx;

    if (state->board[state->y * 90 + state->x] != 0 || state->x == 89 ||
        state->x == 0 || state->y == 89 || state->y == 0) {
        state->board[state->y * 90 + state->x] = 4;
        state->dead = 1;
    }

    state->board[state->y * 90 + state->x] = 3;
}

static void clear_game_state(struct game_state *state) {
    memset(state, 0, sizeof(*state));
    state->x = 45;
    state->y = 45;
    state->vx = 1;
    state->vy = 0;
    state->dead = 0;
}

static void draw_board(
    struct drm_mode_dumb_buffer *buf,
    struct game_state *state,
    i32 x,
    i32 y,
    i32 scale
) {
    for (i32 i = 0; i < 90; ++i) {
        for (i32 yoff = 0; yoff < scale; ++yoff) {
            i32 cy = cy = y + i * scale + yoff;
            for (i32 j = 0; j < 90; ++j) {
                for (i32 xoff = 0; xoff < scale; ++xoff) {
                    i32 cx = x + j * scale + xoff;
                    i32 pixel_index = cy * buf->stride + cx;
                    if (state->board[i * 90 + j] == 0) {
                        buf->map[pixel_index] = (u32)COLOR_GRAY;
                    } else {
                        buf->map[pixel_index] = (u32)COLOR_BLUE;
                    }
                }
            }
        }
    }
}

static i32 cmain(i32 argc, cstring *argv) {
    e32 error;
    i64 arena_size = 2000 * 4096;
    u8 *mem = mmap(
        0, arena_size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANON, -1, 0
    );
    if (mem == 0) {
        return 1;
    }
    struct arena perm_arena = { .start = mem, .end = mem + arena_size };

    i32 keyboard_fd = open_keyboard(perm_arena);
    if (keyboard_fd < 0) {
        return 2;
    }

    error = ioctl(
        keyboard_fd,
        ((u32)0x90) | ((u32)'E' << 8) | (4 << 16) | (1U << 30),
        (u8 *)1
    );
    if (error != 0) {
        return 3;
    }

    i32 card_fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC, 0);
    if (card_fd < 0) {
        return 4;
    }

    struct drm_mode_resources *res =
        drm_mode_get_resources(&perm_arena, card_fd);
    if (res == 0) {
        return 5;
    }

    i32 conn_index;
    struct drm_mode_connector *conn = 0;
    for (conn_index = 0; conn_index < res->connectors_len; ++conn_index) {
        conn = drm_mode_get_connector(
            &perm_arena, card_fd, res->connectors[conn_index]
        );
        if (conn == 0) {
            continue;
        }
        if (conn->connection == DRM_MODE_CONNECTED && conn->modes_len != 0) {
            break;
        }
    }
    if (conn_index == res->connectors_len || conn == 0) {
        return 6;
    }

    struct drm_mode_encoder *enc =
        drm_mode_get_encoder(&perm_arena, card_fd, conn->encoder_id);
    if (enc == 0) {
        return 7;
    }

    i32 buf_index = 0;
    struct drm_mode_dumb_buffer *bufs[2];
    bufs[0] = drm_mode_create_dumb_buffer(
        &perm_arena, card_fd, conn->modes[0].hdisplay, conn->modes[0].vdisplay
    );
    bufs[1] = drm_mode_create_dumb_buffer(
        &perm_arena, card_fd, conn->modes[0].hdisplay, conn->modes[0].vdisplay
    );
    if (bufs[0] == 0 || bufs[1] == 0) {
        return 8;
    }

    struct drm_mode_crtc *crtc =
        drm_mode_get_crtc(&perm_arena, card_fd, enc->crtc_id);
    if (crtc == 0) {
        return 9;
    }

    crtc->mode = conn->modes[0];

    struct game_state game_state;
    clear_game_state(&game_state);

    i32 width = (i32)bufs[0]->width;
    i32 height = (i32)bufs[0]->height;
    i32 square_len = (height > width) ? width : height;
    i32 scale = square_len / 90;
    i32 board_size = square_len - (square_len % 90);
    i32 board_x = (width / 2) - (board_size / 2);
    i32 board_y = (height / 2) - (board_size / 2);

    u8 msg[5];
    msg[0] = '0' + (u8)((width / 1000) % 10);
    msg[1] = '0' + (u8)((width / 100) % 10);
    msg[2] = '0' + (u8)((width / 10) % 10);
    msg[3] = '0' + (u8)(width % 10);
    msg[4] = '\n';
    write(1, msg, sizeof(msg));

    for (i32 bi = 0; bi < 2; ++bi) {
        for (i32 i = 0; i < (i32)bufs[bi]->size; ++i) {
            bufs[bi]->map[i] = COLOR_BLACK;
        }
    }
    draw_board(bufs[buf_index], &game_state, board_x, board_y, scale);
    error = drm_mode_set_crtc(
        card_fd, crtc, &conn->connector_id, 1, bufs[0]->fb_id
    );
    if (error != 0) {
        return 12;
    }
    buf_index ^= 1;

    i32 epoll_fd = epoll_create1();
    if (epoll_fd < 0) {
        return 10;
    }

    struct epoll_event epoll_event = {
        .events = EPOLLIN,
        .data.fd = keyboard_fd,
    };
    error = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, keyboard_fd, &epoll_event);
    if (error != 0) {
        return 11;
    }

    struct epoll_event ep_events[32];
    struct input_event kb_events[32];
    struct timespec last_update, now;

    error = clock_gettime(CLOCK_MONOTONIC, &now);
    if (error != 0) {
        return 13;
    }
    last_update = now;

    while (1) {
        i32 ep_count = epoll_wait(
            epoll_fd, ep_events, sizeof(ep_events) / sizeof(*ep_events), 0
        );
        if (ep_count < 0) {
            return 15;
        }

        error = clock_gettime(CLOCK_MONOTONIC, &now);
        if (error != 0) {
            return 16;
        }

        i64 ns_elapsed = nsec_since(&now, &last_update);

        for (i32 ep_index = 0; ep_index < ep_count; ++ep_index) {
            if (ep_events[ep_index].data.fd == keyboard_fd) {
                i64 len = read(keyboard_fd, (u8 *)kb_events, sizeof(kb_events));
                if (len < 0) {
                    return 16;
                }
                for (i32 i = 0; i < (i32)(len / sizeof(*kb_events)); ++i) {
                    if (kb_events[i].type == 1 && kb_events[i].value == 1) {
                        switch (kb_events[i].code) {
                            case KEY_ESC:
                                return 0;
                            case KEY_A:
                                if (game_state.vx <= 0) {
                                    game_state.vx = -1;
                                    game_state.vy = 0;
                                }
                                break;
                            case KEY_D:
                                if (game_state.vx >= 0) {
                                    game_state.vx = 1;
                                    game_state.vy = 0;
                                }
                                break;
                            case KEY_W:
                                if (game_state.vy <= 0) {
                                    game_state.vx = 0;
                                    game_state.vy = -1;
                                }
                                break;
                            case KEY_S:
                                if (game_state.vy >= 0) {
                                    game_state.vx = 0;
                                    game_state.vy = 1;
                                }
                                break;
                            default:
                                continue;
                        }
                    }
                }
            }
        }

        if (ns_elapsed >= 32L * 1000L * 1000L) {
            last_update = now;

            update_game_state(&game_state);
            draw_board(bufs[buf_index], &game_state, board_x, board_y, scale);

            error = drm_mode_set_crtc(
                card_fd, crtc, &conn->connector_id, 1, bufs[buf_index]->fb_id
            );
            if (error != 0) {
                return 10;
            }

            buf_index ^= 1;

            if (game_state.dead != 0) {
                clear_game_state(&game_state);
            }

            // u8 msg[6];
            // msg[0] = 'u';
            // msg[1] = '0' + (u8)((ns_elapsed / (1000 * 1000 * 1000)) % 10);
            // msg[2] = '0' + (u8)((ns_elapsed / (100 * 1000 * 1000)) % 10);
            // msg[3] = '0' + (u8)((ns_elapsed / (10 * 1000 * 1000)) % 10);
            // msg[4] = '0' + (u8)((ns_elapsed / (1000 * 1000)) % 10);
            // msg[5] = '\n';
            // write(1, msg, sizeof(msg));
        }
    }

    return 0;
}

struct arg_data {
    i64 count;
    cstring args[];
};

void _cstart(struct arg_data *arg_data) {
    i32 exit_code = cmain((i32)arg_data->count, arg_data->args);
    exit(exit_code);
}
