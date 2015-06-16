#ifndef _COMMON_H_
#define _COMMON_H_
#include "nde_types.h"
#include "nderror.h"

#define SWAP(x,y) { unsigned char tmp = x; x = y; y = tmp; }

#define SWAP32(x)       \
    x = ( ( ( x >> 24 ) & 0x000000FF ) | \
          ( ( x >>  8 ) & 0x0000FF00 ) | \
          ( ( x <<  8 ) & 0x00FF0000 ) | \
          ( ( x << 24 ) & 0xFF000000 ) );

#ifndef MAX
	#define MAX(x,y) ( (x)>(y) ? (x) : (y) )
#endif

#ifndef MIN
	#define MIN(x,y) ( (x)>(y) ? (y) : (x) )
#endif

#ifndef ABS
	#define ABS(a)          ((a)>=0?(a):(-(a)))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

/* struct definition */
typedef struct _ScanToken
{
    uint8_t *token_begin;
    uint8_t *token_end;    
}ScanToken;


/* function definition */
int hex_to_int(char s[], int len);
int is_string_number(const char * str);
int maccmp(unsigned char *mac1, unsigned char *mac2);
char *mac2string(unsigned char *mac_address );
int hex_char_to_int(unsigned char c);
int hexstring_to_hex(char* in, int length, unsigned char* out);
int getmac(char * macAddress, int strict, unsigned char * mac);
void trim(char *str);
void urldecode(char *str, int len, char *output, int outlen);

static inline void mac_to_str(unsigned char *mac, char *out)
{
    snprintf(out, 18, "%02x-%02x-%02x-%02x-%02x-%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}


#endif /* _COMMON_H_ */
