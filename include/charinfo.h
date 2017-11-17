#ifndef CHARINFO_H
#define CHARINFO_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDDEF_H
#  include <stddef.h>
#endif /* HAVE_STDDEF_H */

typedef struct {
    int len;
    int mask;
    int error;
    unsigned int codepoint;
    int width;
} charinfo_t;

typedef struct {
    unsigned int from;
    unsigned int to;
    int width;
} width_range_t;

extern int tab_width;

void shorten (char *str, int n);
int strwidth (const char *str);
int strnwidth (const char *str, size_t len);
void get_char_info (charinfo_t *cinfo, const char *buf, int i);

#endif /* CHARINFO_H */

