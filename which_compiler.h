#ifndef __which_compiler_h
#define __which_compiler_h

enum COMPILERS {
        NA_COMPILER,
        CLANG,
        GCC,
        INTEL,
        HP,
        IMB,
        MSVC,
        PGI,
        SUN
};

typedef enum COMPILERS COMPILERS;

inline COMPILERS which_compiler()
{
#if defined(__clang__)
        return CLANG;

#elif defined(__GNUC__) || defined(__GNUG__)
        return GCC;

#elif defined(__ICC) || defined(__INTEL_COMPILER)
        return INTEL;

#elif defined(__HP_cc) || defined(__HP_aCC)
        return HP;

#elif defined(__IBMC__) || defined(__IBMCPP__)
        return IMB;

#elif defined(_MSC_VER)
        return MSVC;

#elif defined(__PGI)
        return PGI;

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
        return SUN;

#else
        return NA_COMPILER;

#endif
}

#endif
