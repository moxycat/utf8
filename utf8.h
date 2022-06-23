#ifndef _UTF_8_H_
#define _UTF_8_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UTF8_BOM     "\xef\xbb\xbf"
#define UTF16_LE_BOM "\xff\xfe"
#define UTF16_BE_BOM "\xfe\xff"
#define UTF32_LE_BOM "\xff\xfe\x00\x00"
#define UTF32_BE_BOM "\x00\x00\xfe\xff"

#define UTF8_BYTE_TYPE_INVALID      -1
#define UTF8_BYTE_TYPE_ASCII         0
#define UTF8_BYTE_TYPE_SEQ           1
#define UTF8_BYTE_TYPE_START_2_BYTE  2
#define UTF8_BYTE_TYPE_START_3_BYTE  3
#define UTF8_BYTE_TYPE_START_4_BYTE  4

#define UTF8_IS_VALID(x) (0x0 <= x && x <= 0x10ffff) ? 1 : 0

typedef unsigned char      uint8_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;


typedef struct _utf8_multibyte {
    uint8_t bytes[4]; /* always max of 4 */
    int used; /* how many of the 4 bytes are used */
} utf8_multibyte_t;

typedef struct _utf8_string {
    char *raw; /* raw string buffer */
    uint32_t *chars; /* codepoints comprising the string */
    uint64_t length; /* number of codepoints in string */
} utf8_string_t;

/* decode a utf8 multibyte sequence into a unicode codepoint */
uint32_t utf8_decode(utf8_multibyte_t *bytes);

/* encode a unicode codepoint into a utf8 multibyte sequence */
utf8_multibyte_t utf8_encode(uint32_t codepoint);

utf8_string_t *utf8_init(const char *s);
void utf8_free(utf8_string_t *str);

int cplen(uint32_t cp);
int utf8len(uint8_t ch);

// string functions

utf8_string_t *utf8_strcat(utf8_string_t *dest, const utf8_string_t *src);              // not implemented
utf8_string_t *utf8_strncat(utf8_string_t *dest, const utf8_string_t *src, uint64_t n); // not implemented
utf8_string_t *utf8_strchr(const utf8_string_t *str, uint32_t c);                       // not implemented
utf8_string_t *utf8_strcpy(utf8_string_t *dest, const utf8_string_t *src);              // not implemented
utf8_string_t *utf8_strncpy(utf8_string_t *dest, const utf8_string_t *src, uint64_t n); // not implemented
utf8_string_t *utf8_strpbrk(const utf8_string_t *str, const utf8_string_t *set);        // not implemented
utf8_string_t *utf8_strrchr(const utf8_string_t *str, uint32_t c);                      // not implemented
utf8_string_t *utf8_strstr(const utf8_string_t *haystack, const utf8_string_t *needle); // not implemented
utf8_string_t *utf8_strtok(utf8_string_t *str, const utf8_string_t *delim);             // not implemented
uint64_t utf8_strcspn(const utf8_string_t *str, const utf8_string_t *set);              // not implemented
uint64_t utf8_strlen(const char *s);
uint64_t utf8_strspn(const utf8_string_t *str, const utf8_string_t *set);               // not implemented
int utf8_strcmp(const utf8_string_t *a, const utf8_string_t *b);                        // not implemented
int utf8_strncmp(const utf8_string_t *a, const utf8_string_t *b, uint64_t n);           // not implemented

// file i/o functions

int utf8_has_bom(FILE *f);                                                              // not implemented
int utf8_fgetpos(FILE *f, uint64_t *pos);                                               // not implemented
uint64_t utf8_fread(utf8_string_t *str, uint64_t n, FILE *f);                           // not implemented
int utf8_fseek(FILE *f, uint64_t offset, int whence);                                   // not implemented
int utf8_fsetpos(FILE *f, uint64_t *pos);                                               // not implemented
uint64_t utf8_ftell(FILE *f);                                                           // not implemented
uint64_t utf8_fwrite(const utf8_string_t *str, uint64_t n, FILE *f);                    // not implemented
uint32_t utf8_fgetc(FILE *f);                                                           // not implemented
utf8_string_t *utf8_fgets(utf8_string_t *str, uint64_t n, FILE *f);                     // not implemented
int utf8_fputc(uint32_t c, FILE *f);                                                    // not implemented
int utf8_fputs(const utf8_string_t *str, FILE *f);                                      // not implemented

#endif