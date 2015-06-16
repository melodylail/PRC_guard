#ifndef _DUMP_H
#define _DUMP_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAXLEN_FILENAME
#define MAXLEN_FILENAME     256
#endif

typedef struct _DumpCtx
{
    int fd;
    char filename[MAXLEN_FILENAME + 1];
    int isopen;
}DumpCtx;

int dump_init(DumpCtx *ctx);
int dump_fini(DumpCtx *ctx);
int dump_write(DumpCtx *ctx, const char * format, ...);
int dump_one_record(const char *filename, const char * format, ...);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _DUMP_H */
