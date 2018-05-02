#define EXPAND__MACRO( x ) x

/* Count number of arguments. */
#define NARG(...) EXPAND__MACRO(NARG_(__VA_ARGS__, PAD_N))
#define NARG_(...) EXPAND__MACRO(ARG_N(__VA_ARGS__))
#define ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define PAD_N 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define ___get_1(TYPE, ...)  typename TYPE
#define ___get_2(TYPE, ...)  typename TYPE, EXPAND__MACRO(___get_1(__VA_ARGS__))
#define ___get_3(TYPE, ...)  typename TYPE, EXPAND__MACRO(___get_2(__VA_ARGS__))
#define ___get_4(TYPE, ...)  typename TYPE, EXPAND__MACRO(___get_3(__VA_ARGS__))
#define ___get_5(TYPE, ...)  typename TYPE, EXPAND__MACRO(___get_4(__VA_ARGS__))
#define ___get_6(TYPE, ...)  typename TYPE, EXPAND__MACRO(___get_5(__VA_ARGS__))
#define ___get_7(TYPE, ...)  typename TYPE, EXPAND__MACRO(___get_6(__VA_ARGS__))
#define ___get_8(TYPE, ...)  typename TYPE, EXPAND__MACRO(___get_7(__VA_ARGS__))
#define ___get_9(TYPE, ...)  typename TYPE, EXPAND__MACRO(___get_8(__VA_ARGS__))
#define ___get_10(TYPE, ...) typename TYPE, EXPAND__MACRO(___get_9(__VA_ARGS__))

#define ___get_n(nargs, ...) EXPAND__MACRO(___get_ ## nargs(__VA_ARGS__))
#define ___redirect_generic_again(nargs, ...) EXPAND__MACRO(___get_n(nargs, __VA_ARGS__))
#define ___redirect_generic(nargs, ...) EXPAND__MACRO(___redirect_generic_again(nargs, __VA_ARGS__))

#define generic(...) EXPAND__MACRO(template < ___redirect_generic( NARG(__VA_ARGS__), __VA_ARGS__) >)
