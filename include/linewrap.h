#ifndef LINEWRAP_H
#define LINEWRAP_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDDEF_H
#  include <stddef.h>
#endif /* HAVE_STDDEF_H */

typedef struct {
    size_t pos;
    size_t end_pos;
} tline_t;

extern tline_t *tlines2;
extern size_t tlines2_len;
extern size_t tlines2_size;
extern int line_wrap;

extern tline_t *tlines3;
extern size_t tlines3_len;
extern size_t tlines3_size;

void get_tlines (char *buf, size_t len, size_t pos, int max, tline_t **tlines, size_t *tlines_len, size_t *tlines_size);
void get_wrap_tlines (char *buf, size_t len, size_t pos, int max, tline_t **tlines, size_t *tlines_len, size_t *tlines_size);
void get_nowrap_tlines (char *buf, size_t len, size_t pos, int max, tline_t **tlines, size_t *tlines_len, size_t *tlines_size);
int prev_line (char *buf, size_t len, size_t pos, int n);
int next_line (char *buf, size_t len, size_t pos, int n);
void get_tlines_backward (char *buf, size_t len, size_t pos, int max);
int find_pos_backward (int max, size_t pos2, size_t *pos3);
int find_pos_forward (int max, size_t pos2, size_t *pos3);
size_t line_before_pos (size_t pos, int n);

#endif /* LINEWRAP_H */

