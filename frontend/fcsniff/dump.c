#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include "common.h"
#include "dump.h"

#define MAX_LOG_LINE_LEN           (1024)


int dump_init(DumpCtx *ctx)
{
    if(NULL == ctx->filename || ctx->filename[0] == '\0')
    {
        fprintf(stderr, "Invalid dump file path\n");
        return -1;
    }

    ctx->fd = open(ctx->filename, O_CREAT|O_APPEND|O_WRONLY, S_IRWXU|S_IRWXG|S_IRWXO);
    
    if(ctx->fd == -1)
    {
        fprintf(stderr, "Failed to open %s, error no: %d\n", ctx->filename, errno);
        return -1;
    }

    ctx->isopen = 1;

    return 0;
}

int dump_fini(DumpCtx *ctx)
{
    if(ctx->isopen == 0)
    {
        return 0;
    }

    close(ctx->fd);
    ctx->isopen = 0;

    return 0;
}

int dump_write(DumpCtx *ctx, const char * format, ...)
{
    char szbuf[MAX_LOG_LINE_LEN + 1];
    va_list args;
    int len = 0;

    if(ctx->isopen == 0)
    {
        return -1;
    }

    memset(szbuf, 0x00, sizeof(szbuf));

    va_start(args, format);
    vsnprintf(szbuf, sizeof(szbuf), format, args);
    va_end(args);
    szbuf[sizeof(szbuf)-1] = '\0';

    return write(ctx->fd, szbuf, strlen(szbuf));
}

int dump_one_record(const char *filename, const char * format, ...)
{
    char szbuf[MAX_LOG_LINE_LEN + 1];
    va_list args;
    int fd = -1;
    int len = 0;

    if(NULL == filename || filename[0] == '\0')
    {
        fprintf(stderr, "Invalid dump file path\n");
        return -1;
    }

    fd = open(filename, O_CREAT|O_APPEND|O_WRONLY, S_IRWXU|S_IRWXG|S_IRWXO);
    
    if(fd == -1)
    {
        fprintf(stderr, "Failed to open %s, error no: %d\n", filename, errno);
        return -1;
    }

    memset(szbuf, 0x00, sizeof(szbuf));

    va_start(args, format);
    vsnprintf(szbuf, sizeof(szbuf), format, args);
    va_end(args);
    szbuf[sizeof(szbuf)-1] = '\0';

    write(fd, szbuf, strlen(szbuf));

    close(fd);

    return 0;
}

