#ifndef READFILE_H
#define READFILE_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDDEF_H
#  include <stddef.h>
#endif /* HAVE_STDDEF_H */

#define POSITIONED  64
#define PREFIX "/usr/bin"

#define UTF8_LENGTH(c)        \
    ((c & 0x80) == 0x00 ? 1 : \
     (c & 0xc0) == 0x80 ? 1 : \
     (c & 0xe0) == 0xc0 ? 2 : \
     (c & 0xf0) == 0xe0 ? 3 : \
     (c & 0xf8) == 0xf0 ? 4 : \
     (c & 0xfc) == 0xf8 ? 5 : \
     (c & 0xfe) == 0xfc ? 6 : \
                          6)
extern char *lespipe;
extern int man_page;

void read_file ();
void set_input_encoding (char *encoding);
void open_tab_file ();
int count_lines (char *buf, size_t len);
int count_lines_atob (char *buf, size_t a, size_t b);
void reload ();
void set_man_page_name ();

#endif /* READFILE_H */
