#include <stdio.h>
#include <stdlib.h>
#include "nde_types.h"
#include "common.h"

#define ISHEX(c) (hex_to_int(c) != -1)
#define HEX_BASE 16

int is_string_number(const char * str)
{
	int i;
	if (str == NULL) {
		return 0;
	}

	if (*str != '-' && !(isdigit(*str))) {
		return 0;
	}

	for (i = 1; str[i] != 0; i++) {
		if (!isdigit(str[i])) {
			return 0;
		}
	}

	return 1;
}

//compares two MACs
int maccmp(unsigned char *mac1, unsigned char *mac2)
{
    int i=0;

    if(mac1 == NULL || mac2 == NULL)
        return -1;

    for(i=0; i<6; i++)
    {
        if( toupper(mac1[i]) != toupper(mac2[i]) )
            return -1;
    }
    return 0;
}

// Converts a mac address in a human-readable format
char * mac2string(unsigned char *mac_address )
{
	char * mac_string = (char *)malloc(sizeof(char)*18);
	snprintf(mac_string, 18, "%02X:%02X:%02X:%02X:%02X:%02X", *mac_address,
						*(mac_address+1), *(mac_address+2), *(mac_address+3),
						*(mac_address+4), *(mac_address+5));
	return mac_string;
}

/* Return -1 if it's not an hex value and return its value when it's a hex value */
int hex_char_to_int(unsigned char c)
{
	static int table_created = 0;
	static int table[256];

	int i;

	if (table_created == 0)
	{
		/*
		 * It may seem a bit long to calculate the table
		 * but character position depend on the charset used
		 * Example: EBCDIC
		 * but it's only done once and then conversion will be really fast
		 */
		for (i=0; i < 256; i++)
		{

			switch ((unsigned char)i)
			{
				case '0':
					table[i] = 0;
					break;
				case '1':
					table[i] = 1;
					break;
				case '2':
					table[i] = 2;
					break;
				case '3':
					table[i] = 3;
					break;
				case '4':
					table[i] = 4;
					break;
				case '5':
					table[i] = 5;
					break;
				case '6':
					table[i] = 6;
					break;
				case '7':
					table[i] = 7;
					break;
				case '8':
					table[i] = 8;
					break;
				case '9':
					table[i] = 9;
					break;
				case 'A':
				case 'a':
					table[i] = 10;
					break;
				case 'B':
				case 'b':
					table[i] = 11;
					break;
				case 'C':
				case 'c':
					table[i] = 12;
					break;
				case 'D':
				case 'd':
					table[i] = 13;
					break;
				case 'E':
				case 'e':
					table[i] = 14;
					break;
				case 'F':
				case 'f':
					table[i] = 15;
					break;
				default:
					table[i] = -1;
			}
		}

		table_created = 1;
	}

	return table[c];
}

int hexstring_to_hex(char* in, int length, unsigned char* out)
{
    int i=0;
    int char1, char2;

    char *input=in;
    unsigned char *output=out;

    if(length < 1)
        return 1;

    for(i=0; i<length; i+=2)
    {
        if(input[i] == '-' || input[i] == ':' || input[i] == '_' || input[i] == ' ')
        {
            input++;
            length--;
        }
        char1 = hex_char_to_int(input[i]);
        char2 = hex_char_to_int(input[i+1]);
        if(char1 < 0 || char1 > 15)
            return -1;
        output[i/2] = ((char1 << 4) + char2) & 0xFF;
    }
    return (i/2);
}

//Return the mac address bytes (or null if it's not a mac address)
int getmac(char * macAddress, int strict, unsigned char * mac)
{
	char byte[3];
	int i, nbElem, n;

	if (macAddress == NULL)
		return 1;

	/* Minimum length */
	if ((int)strlen(macAddress) < 12)
		return 1;

	memset(mac, 0, 6);
	byte[2] = 0;
	i = nbElem = 0;

	while (macAddress[i] != 0)
	{
		if (macAddress[i] == '\n' || macAddress[i] == '\r')
			break;

		byte[0] = macAddress[i];
		byte[1] = macAddress[i+1];

		if (sscanf( byte, "%x", &n ) != 1
			&& strlen(byte) == 2)
			return 1;

		if (hex_char_to_int(byte[1]) < 0)
			return 1;

		mac[nbElem] = n;

		i+=2;
		nbElem++;

		if (macAddress[i] == ':' || macAddress[i] == '-'  || macAddress[i] == '_')
			i++;
	}

	if ((strict && nbElem != 6)
		|| (!strict && nbElem > 6))
		return 1;

	return 0;
}

int hex_to_int(char s[], int len)
{
	int i = 0;
	int convert = -1;
	int value = 0;

	// Remove leading 0 (and also the second char that can be x or X)

	while (i < len)
	{
		if (s[i] != '0' || (i == 1 && toupper((int)s[i]) != 'X'))
			break;

		++i;
	}

	// Convert to hex

	while (i < len)
	{
		convert = hex_char_to_int((unsigned char)s[i]);

		// If conversion failed, return -1
		if (convert == -1)
			return -1;

		value = (value * HEX_BASE) + convert;

		++i;
	}


	return value;
}

void trim(char *str)
{
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((int)str[begin])) begin++;
    while ((end >= begin) && isspace((int)str[end])) end--;
    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];
    str[i - begin] = '\0'; // Null terminate string.
}

void urldecode(char *str, int len, char *output, int outlen)
{
    char *dest = output;
    char *data = str;

    int value;
    int c;

    while (len--) {
        if (*data == '+') 
        {
            *dest = ' ';
        }
        else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1))
                 && isxdigit((int) *(data + 2)))
        {

            c = ((unsigned char *)(data+1))[0];
            if (isupper(c))
                c = tolower(c);
            value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;
            c = ((unsigned char *)(data+1))[1];
            if (isupper(c))
                c = tolower(c);
            value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

            *dest = (char)value ;
            data += 2;
            len -= 2;
        } else {
            *dest = *data;
        }
        data++;
        dest++;
    }

    *dest = '\0';
    outlen = dest - output;

    return;
}

/* Converts a hex character to its integer value */
char from_hex(char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}




