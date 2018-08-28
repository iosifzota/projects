#ifndef __buf_h
#define __buf_h

#include <stddef.h>
#include <string.h>

#include "base.h"
#include "req.h"
#include "memory.h"
#include "interval.h"

#define dobuf(buf, index, ...) {                                \
        size_t size = buf_size(buf);                            \
        for (size_t index = 0; index < size; ++ index) {        \
            __VA_ARGS__;                                        \
        }                                                       \
    }

void printbuf_int(int_ptr buf);
/* ------------------------------------------------------------ */

struct(vec) {
    size_t size, cap;
    char buf[];
};

header_fn
vec_ptr buf_to_vec(const void_ptr buf)
{
    req(buf);
    return (vec_ptr) (buf - sizeof (vec));
}

header_fn
size_t buf_size(const void_ptr buf)
    RET( (buf) ? buf_to_vec(buf)->size : 0 );

header_fn
size_t buf_cap(const void_ptr buf)
    RET( (buf) ? buf_to_vec(buf)->cap : 0 );

#define AT(buf, i) \
    ( req (i >= 0 && i < buf_size(buf)), buf + i )

header_fn
void_ptr buf_alloc(size_t this_manny, size_t of_size)
{
    const size_t bytes = sizeof(vec) + this_manny * of_size;
    vec_ptr v;

    v = (vec_ptr) emalloc(bytes);
    memset(v, 0, bytes);

    v->size = v->cap = this_manny;
    return v->buf;
}

header_fn
void_ptr buf_realloc(void_ptr buf, size_t this_manny, size_t of_size)
{
    const size_t bytes = sizeof(vec) + this_manny * of_size;
    vec_ptr v;

    if (!buf) return buf_alloc(this_manny, of_size);

    v = buf_to_vec(buf);

    v = (vec_ptr) erealloc(v, bytes);
    buf = v->buf;

    v->cap = this_manny;
    if (v->size > v->cap) {
        v->size = v->cap;
    }

    return buf;
}

#define buf_NEW(buf, this_manny) \
    (req(!buf), buf = buf_alloc(this_manny, sizeof *buf))


#define buf_free(buf) \
    ((buf ? (free(buf_to_vec(buf)), 0) : 0), buf = NULL)


#define buf_RESIZE(buf, this_manny) \
    (buf = buf_realloc(buf, this_manny, sizeof *buf))

#define GROWTH 5

header_fn
void_ptr buf_grow(void_ptr buf, size_t this_manny, size_t of_size) {
    const size_t inc = buf_cap(buf) + MAX(this_manny, GROWTH);
    return buf_realloc(buf, inc, of_size);
}

header_fn
void_ptr shrink(void_ptr buf, size_t elem_size)
{
    if ( int_big_enough(GROWTH, (Interval){ buf_size(buf), buf_cap(buf) }) ) {
        buf = buf_realloc(buf,
                          buf_cap(buf) - GROWTH,
                          elem_size);
    }
    return buf;
}


header_fn
void_ptr buf_fit(void_ptr buf, size_t this_manny, size_t of_size)
{
    if ( int_too_small(this_manny, (Interval){ buf_cap(buf), buf_size(buf) }) ) {
        puts("PROC");
        buf = buf_grow(buf, this_manny, of_size);
    }
    return buf;
}

header_fn
void buf_update_size(void_ptr buf, size_t new_size)
{
    vec_ptr v = buf_to_vec(buf);
    req(new_size <= v->cap);
    v->size = new_size;
}

#define buf_PUSH(buf, val) (                    \
        buf = buf_fit(buf, 1, sizeof *buf),     \
        buf[buf_size(buf)] = val,               \
        buf_update_size(buf, buf_size(buf) + 1) \
        )

#define buf_POP(buf) (                                  \
        req(buf_size(buf) >= 1, "Popping (null) buf!"), \
        buf_update_size(buf, buf_size(buf) - 1),        \
        buf = shrink(buf, sizeof *buf)                  \
        )

void printbuf_int(int_ptr buf)
    dobuf(buf, i, printf("%d ", *AT(buf, i)));

#endif // !__buf_h
