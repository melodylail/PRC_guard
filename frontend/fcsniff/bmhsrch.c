/*
**  Case-sensitive Boyer-Moore-Horspool pattern match
**
**  limitation: pattern length + string length must be less than 32767
**  
*/
#include <limits.h>                                         
#include <stddef.h>
#include "nde_types.h"
#include "bmhsrch.h"


#define LARGE 4096

uint8_t *bmh_search(const uint8_t *pbuf, const int buf_len, const uint8_t *pattern)
{
    int i, j;
    char *s;
    int patlen;
    int skip[UCHAR_MAX+1];                               
    int skip2;
    int lastpatchar;

    patlen = strlen(pattern);

    /* init bad char table */
    for (i = 0; i <= UCHAR_MAX; ++i)                  
        skip[i] = patlen;

    for (i = 0; i < patlen; ++i)
        skip[pattern[i]] = patlen - i - 1;

    lastpatchar = pattern[patlen - 1];
    skip[lastpatchar] = LARGE;
    skip2 = patlen;

    /* Horspool's fixed second shift */
    for (i = 0; i < patlen - 1; ++i)
    {
        if (pattern[i] == lastpatchar)
              skip2 = patlen - i - 1;
    }


    i = patlen - 1 - buf_len;

    if (i >= 0)
        return NULL;

    pbuf += buf_len;

    for ( ;; )
    {
        while ( (i += skip[((uint8_t *)pbuf)[i]]) < 0 )
              ;                           /* mighty fast inner loop */
        if (i < (LARGE - buf_len))
              return NULL;

        i -= LARGE;
        j = patlen - 1;
        s = (char *)pbuf + (i - j);

        while (--j >= 0 && s[j] == pattern[j])
              ;
        if ( j < 0 )                                    
              return s;                                 
        if ( (i += skip2) >= 0 )                        
              return NULL;                              
    }
}

