#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "common.h"
#include "nde_log.h"

#define MAX_LOG_LINE_LEN           (1024-1)

/* global variables */
LOG_LEVEL_FLAG_T  g_log_level = LOG_LEVEL_DEBUG;
volatile LOG_CALLBACK g_plog_callback = NULL;

char *log_level_desc(LOG_LEVEL_FLAG_T flag)
{
    if(LOG_LEVEL_ERR == flag)
    {
        return "[ERROR] ";
    }
    else if(LOG_LEVEL_WARN == flag)
    {
        return "[WARN] ";
    }
    else if(LOG_LEVEL_INFO == flag)
    {
        return "[INFO] ";
    }
    else if(LOG_LEVEL_DEBUG == flag)
    {
        return "[DEBUG] ";
    }
    else
    {
        return "";
    }
}

int32_t set_log_callback(LOG_CALLBACK plogcallback)
{
    if (NULL == plogcallback)
    {
        return -2;
    }

    g_plog_callback = plogcallback;

    return 0;
}

void log_write(LOG_LEVEL_FLAG_T flag, const char* fmt, ...)
{
    if (LOG_CHECK_LEVEL(flag))
    {
        char szbuf[MAX_LOG_LINE_LEN + 1];
        va_list ap;

        /* compose string */
        va_start(ap, fmt);
        vsnprintf(szbuf, sizeof(szbuf), fmt, ap);
        va_end(ap);
        szbuf[sizeof(szbuf)-1] = '\0';


#ifdef USE_SYSLOG
        if(flag == LOG_LEVEL_ERR)
        {
            syslog(LOG_DAEMON | LOG_ERR, szbuf);
        }
        else if(flag == LOG_LEVEL_WARN)
        {
            syslog(LOG_DAEMON | LOG_WARNING, szbuf);
        }
        if(flag == LOG_LEVEL_INFO)
        {
            syslog(LOG_DAEMON | LOG_INFO, szbuf);
        }
        if(flag == LOG_LEVEL_DEBUG)
        {
            syslog(LOG_DAEMON | LOG_DEBUG, szbuf);
        }
#else
        if (g_plog_callback)
        {   /* output by callback */
            g_plog_callback(flag, szbuf);
        }
        else
        {
            printf("%s", szbuf);
        }
#endif // USE_SYSLOG

    }
}

void log_dump_buf(LOG_LEVEL_FLAG_T flag, const uint8_t* p_buf, uint32_t p_len)
{
    static const char hex[]="0123456789abcdef";
    uint8_t linebuf[80];
    uint8_t *p, *q, c;
    uint32_t i, j, k;

    if (!LOG_CHECK_LEVEL(flag))
    {
        return;
    }

    for(i=0; i<p_len; i+=16) {
        j = (p_len-i)>16?16:(p_len-i);
        memset(linebuf, ' ', sizeof(linebuf));
        p = linebuf;
        *p++ = hex[(i>>28)&0x0f];
        *p++ = hex[(i>>24)&0x0f];
        *p++ = hex[(i>>20)&0x0f];
        *p++ = hex[(i>>16)&0x0f];
        *p++ = ':';
        *p++ = hex[(i>>12)&0x0f];
        *p++ = hex[(i>> 8)&0x0f];
        *p++ = hex[(i>> 4)&0x0f];
        *p++ = hex[(i>> 0)&0x0f];
        *p++ = ':';

        q = linebuf+60;
        *q++ = ' ';
        *q++ = ';';
        *q++ = ' ';
        for(k=0; k<j; k++) {
            if(k==8) {
                *p++ = ' ';
                *p++ = '-';
            }
            *p++ = ' ';
            c = p_buf[i+k];
            *p++ = hex[(c>>4)&0x0f];
            *p++ = hex[(c>>0)&0x0f];
            *q++ = (c>=0x20 && c<=0x7e)?c:'.';
        }
        linebuf[sizeof(linebuf)-1] = '\0';

        log_write(flag, "%s\n", linebuf);
    }
}
