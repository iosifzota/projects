#ifndef __base_c
#define __base_c

#define struct(tag)                             \
    typedef struct tag tag, * tag ## _ptr;      \
    struct tag

#define header_fn static inline

// less stars :)
#define gen_ptrs(type) \
    typedef type * type ## _ptr ;

gen_ptrs(int)
gen_ptrs(long)
gen_ptrs(char)
gen_ptrs(short)
gen_ptrs(void)

#undef gen_ptrs

#define RET(expr) { return expr; }

header_fn
int avg(int arg1, int arg2) RET( (arg1 + arg2) / 2 );

#define MAX(x, y) ( (x > y) ? (x) : (y) )
#define MIN(x, y) ( (x < y) ? (x) : (y) )

#endif // !__base_c
