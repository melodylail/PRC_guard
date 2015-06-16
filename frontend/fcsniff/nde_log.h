#ifndef _NDE_LOG_H
#define _NDE_LOG_H
#include "nde_types.h"

#ifdef USE_SYSLOG
#include <syslog.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int LOG_LEVEL_FLAG_T;

/* basic level flag (level) */
#define LOG_LEVEL_ERR         1
#define LOG_LEVEL_WARN        2
#define LOG_LEVEL_INFO        3
#define LOG_LEVEL_DEBUG       4


typedef void (*LOG_CALLBACK)(LOG_LEVEL_FLAG_T debug_level, const char* msg);

extern LOG_LEVEL_FLAG_T  g_log_level;
extern volatile LOG_CALLBACK g_plog_callback;


/* functions about log callback */
/*
 *  @retval 0           Success
 *  @retval -2          bad parameter
 */
int32_t set_log_callback(LOG_CALLBACK plogcallback);


/* log output functions */
void log_write(LOG_LEVEL_FLAG_T flag, const char* fmt, ...);

void log_dump_buf(LOG_LEVEL_FLAG_T flag, const uint8_t* p_buf, uint32_t p_len);

/* Macros */
/* Check log level is enabled or not */
#define LOG_CHECK_LEVEL(log_level_flag) (log_level_flag <= g_log_level)

/* Log function macros */
/*
 * sample code:
 *
 *  - Dump logs when LOG_ERR flag was set:
 *      NLOG(LOG_ERR, "This is a log string (number = %d)", 100);
 *      NLOG_ERR("This is a log string (number = %d)", 100);
 *
 */

#define NLOG(flag, ...) \
    { if (LOG_CHECK_LEVEL((flag))) { log_write((flag), __VA_ARGS__); } }

#define NLOG_ERR(...) \
    { if (LOG_CHECK_LEVEL(LOG_LEVEL_ERR)) { log_write(LOG_LEVEL_ERR, __VA_ARGS__); } }

#define NLOG_WARN(...) \
    { if (LOG_CHECK_LEVEL(LOG_LEVEL_WARN)) { log_write(LOG_LEVEL_WARN, __VA_ARGS__); } }

#define NLOG_INFO(...) \
    { if (LOG_CHECK_LEVEL(LOG_LEVEL_INFO)) { log_write(LOG_LEVEL_INFO, __VA_ARGS__); } }

#define NLOG_DEBUG(...) \
    { if (LOG_CHECK_LEVEL(LOG_LEVEL_DEBUG)) { log_write(LOG_LEVEL_DEBUG, __VA_ARGS__); } }


#define NLOG_E_DUMP_HEX(p_buf, len) \
    { if (LOG_CHECK_LEVEL(LOG_LEVEL_ERR)) { log_dump_buf(LOG_LEVEL_ERR, p_buf, len); } }

#define NLOG_W_DUMP_HEX(p_buf, len) \
    { if (LOG_CHECK_LEVEL(LOG_LEVEL_WARN)) { log_dump_buf(LOG_LEVEL_WARN, p_buf, len); } }

#define NLOG_I_DUMP_HEX(p_buf, len) \
    { if (LOG_CHECK_LEVEL(LOG_LEVEL_INFO)) { log_dump_buf(LOG_LEVEL_INFO, p_buf, len); } }

#define NLOG_D_DUMP_HEX(p_buf, len) \
    { if (LOG_CHECK_LEVEL(LOG_LEVEL_DEBUG)) { log_dump_buf(LOG_LEVEL_DEBUG, p_buf, len); } }


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _NDE_LOG_H */
