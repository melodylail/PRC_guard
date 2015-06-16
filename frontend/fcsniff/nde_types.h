#ifndef _NDE_TYPES_H_
#define _NDE_TYPES_H_

/* primitive type definitions */
#if defined (_WIN32) /* for Windows platforms */

    #include <BaseTsd.h>

    /* signed */
    typedef __int64             int64_t;
    typedef long                int32_t;
    typedef short int           int16_t;
    typedef char                int8_t;

    /* unsigned */
    typedef unsigned __int64    uint64_t;
    typedef unsigned long       uint32_t;
    typedef unsigned short int  uint16_t;
    typedef unsigned char       uint8_t;

    /* ssize_t */
    /* (predefined)             size_t;    */
    typedef SSIZE_T             ssize_t;

    /* variable-size integer types */
    /* (predefined)             intptr_t;  */
    /* (predefined)             uintptr_t; */


#elif defined (__linux__) /* for Linux platforms */

#ifdef __KERNEL__
    #include <linux/types.h> /* for ssize_t type */
#else
    #include <sys/types.h> /* for ssize_t type */
    #include <stdint.h>
    #include <inttypes.h>
#endif

    /* signed */
    /* (predefined)             int64_t;   */
    /* (predefined)             int32_t;   */
    /* (predefined)             int16_t;   */
    /* (predefined)             int8_t;    */

    /* unsigned */
    /* (predefined)             uint64_t;  */
    /* (predefined)             uint32_t;  */
    /* (predefined)             uint16_t;  */
    /* (predefined)             uint8_t;   */

    /* size_t */
    /* (predefined)             size_t;    */
    /* (predefined)             ssize_t;   */

    /* variable-size integer types */
    #ifdef __KERNEL__
        #ifdef __LP64__
            /* 64-bit platforms */
            typedef long int        intptr_t;

            #ifdef NS_DEFINE_UINTPTR_T
                typedef unsigned long int   uintptr_t;
            #else
                /* (predefined)             uintptr_t; */
            #endif
        #else
            /* 32-bit platforms */
            typedef int             intptr_t;

            #ifdef NS_DEFINE_UINTPTR_T
                typedef unsigned int    uintptr_t;
            #else
                /* (predefined)         uintptr_t; */
            #endif
        #endif
    #else
        /* (predefined)             intptr_t;  */
        /* (predefined)             uintptr_t; */
    #endif

#elif defined (__FreeBSD__) /* for FreeBSD platforms */

    #include <sys/types.h> /* for ssize_t type */
    #include <stdint.h>
    #include <inttypes.h>

    /* signed */
    /* (predefined)             int64_t;   */
    /* (predefined)             int32_t;   */
    /* (predefined)             int16_t;   */
    /* (predefined)             int8_t;    */

    /* unsigned */
    /* (predefined)             uint64_t;  */
    /* (predefined)             uint32_t;  */
    /* (predefined)             uint16_t;  */
    /* (predefined)             uint8_t;   */

    /* size_t */
    /* (predefined)             size_t;    */
    /* (predefined)             ssize_t;   */

    /* variable-size integer types */
    /* (predefined)             intptr_t;  */
    /* (predefined)             uintptr_t; */

#elif defined (__sparc) /* for SPARC platforms */

    #include <sys/types.h> /* for ssize_t type */
    #include <inttypes.h>

    /* signed */
    /* (predefined)             int64_t;   */
    /* (predefined)             int32_t;   */
    /* (predefined)             int16_t;   */
    /* (predefined)             int8_t;    */

    /* unsigned */
    /* (predefined)             uint64_t;  */
    /* (predefined)             uint32_t;  */
    /* (predefined)             uint16_t;  */
    /* (predefined)             uint8_t;   */

    /* size_t */
    /* (predefined)             size_t;    */
    /* (predefined)             ssize_t;   */

    /* variable-size integer types */
    /* (predefined)             intptr_t;  */
    /* (predefined)             uintptr_t; */

#elif defined (_AIX) /* for AIX platforms */

    #include <sys/types.h> /* for ssize_t type */
    #include <inttypes.h>

    /* signed */
    /* (predefined)             int64_t;   */
    /* (predefined)             int32_t;   */
    /* (predefined)             int16_t;   */
    /* (predefined)             int8_t;    */

    /* unsigned */
    /* (predefined)             uint64_t;  */
    /* (predefined)             uint32_t;  */
    /* (predefined)             uint16_t;  */
    /* (predefined)             uint8_t;   */

    /* size_t */
    /* (predefined)             size_t;    */
    /* (predefined)             ssize_t;   */

    /* variable-size integer types */
    /* (predefined)             intptr_t;  */
    /* (predefined)             uintptr_t; */

#else
    #error Target platform is not supported.
#endif


#endif /* _NDE_TYPES_H_ */
