#include "utf8.h"

static uint32_t utf8_byte_lookup[5][5] = {
/*  mask         lead        start    end       bits */
    {0b00111111, 0b10000000, 0x0,     0x0,      6},
    {0b01111111, 0b00000000, 0x0,     0x7f,     7},
    {0b00011111, 0b11000000, 0x80,    0x7ff,    5},
    {0b00001111, 0b11100000, 0x800,   0xffff,   4},
    {0b00000111, 0b11110000, 0x10000, 0x10ffff, 3}
};

/* exp by squaring */
int ipow(int base, int exp) {
    if (base == 0) return 0;
    if (exp < 0) return -1;
    int result = 1;
    while (1) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) break;
        base *= base;
    }
    return result;
}

void mb_clear(utf8_multibyte_t *mb) {
    mb->used = 0;
    for (int i = 0; i < 4; ++i) mb->bytes[i] = 0;
}

int cplen(uint32_t cp) {
    int len = 0;
    for (int i = 0; i < 5; ++i) {
        if ((cp >= utf8_byte_lookup[i][2]) && (cp <= utf8_byte_lookup[i][3])) break;
        len++;
    }
    if (len > 4) return -1;
    return len;
}

int utf8len(uint8_t ch) {
    int len = 0;
    for (int i = 0; i < 5; ++i) {
        if ((ch & ~utf8_byte_lookup[i][0]) == utf8_byte_lookup[i][1]) break;
        len++;
    }
    if (len > 4) return -1;
    return len;
}

uint32_t utf8_decode(utf8_multibyte_t *mb) {
    int bytes = utf8len(mb->bytes[0]);
    if (mb->used == 0 || bytes < 0) return -1;
    int shift = 6 * (bytes - 1);
    uint32_t codepoint = (mb->bytes[0] & utf8_byte_lookup[bytes][0]) << shift;
    for (int i = 1; i < bytes; ++i) {
        shift -= 6;
        codepoint |= ((char)mb->bytes[i] & utf8_byte_lookup[0][0]) << shift;
    }
    return codepoint;
}

utf8_multibyte_t utf8_encode(uint32_t cp) {
    utf8_multibyte_t mb;
    mb_clear(&mb);
    int bytes = cplen(cp);
    if (bytes < 0) return mb;
    mb.used = bytes;
    int shift = 6 * (bytes - 1);
    mb.bytes[0] = (cp >> shift & utf8_byte_lookup[bytes][0]) | utf8_byte_lookup[bytes][1];
    shift -= 6;
    for (int i = 1; i < bytes; ++i) {
        mb.bytes[i] = (cp >> shift & utf8_byte_lookup[0][0]) | utf8_byte_lookup[0][1];
        shift -= 6;
    }
    return mb;
}

uint64_t utf8_strlen(const char *s) {
    char *p = (char*)s;
    uint64_t total = 0;
    while (*p != '\0') {
        if (utf8len(*p) > 0) total++;
        p++;
    }
    return total;
}

utf8_string_t *utf8_init(const char *s) {
    utf8_string_t *str = (utf8_string_t*)malloc(sizeof(utf8_string_t*));
    if (!str) return NULL;
    str->raw = (char*)malloc(strlen(s));
    if (!str->raw) return NULL;
    memcpy(str->raw, s, strlen(s));
    str->length = utf8_strlen(s);
    str->chars = (uint32_t*)malloc(str->length * sizeof(uint32_t));
    if (!str->chars) return NULL;
    char *p = (char*)s;
    utf8_multibyte_t mb;
    mb_clear(&mb);
    int j = 0;
    while (*p != '\0') {
        int len = utf8len(*p);
        mb.used = len;
        mb.bytes[0] = *p++;
        for (int i = 1; i < len; ++i) mb.bytes[i] = *p++;
        uint32_t cp = utf8_decode(&mb);
        str->chars[j++] = cp;
        mb_clear(&mb);
    }
    return str;
}

void utf8_free(utf8_string_t *str) {
    free(str->raw);
    free(str->chars);
    return;
}

utf8_string_t *utf8_strcat(utf8_string_t *dest, const utf8_string_t *src) {
    utf8_string_t *out = (utf8_string_t*)malloc(sizeof(utf8_string_t*));
    if (!out) return NULL;
    out->raw = (char*)malloc(strlen(dest->raw) + strlen(src->raw) + 1);
    out->length = dest->length + src->length;
    out->chars = (uint32_t*)malloc(out->length * sizeof(uint32_t));
    memcpy(out->chars, dest->chars, dest->length);
    memcpy(out->chars, src->chars, src->length);
    return out;
}