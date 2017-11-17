#ifndef PROMPT_H
#define PROMPT_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDDEF_H
#  include <stddef.h>
#endif /* HAVE_STDDEF_H */

typedef struct {
    char *buf;
    size_t len;
    size_t size;
    const char *prompt;
    size_t prompt_len;
    int nlines;
    int nlines2;
    size_t cursor;
    char **history;
    size_t history_len;
    size_t history_size;
    size_t history_skip;
    size_t history_new;
    char *hcstr;
    size_t hcstr_len;
    size_t hcstr_size;
} prompt_t;

extern prompt_t *pr;
extern int interrupt;
extern int tty;

void draw_prompt ();
char *gets_prompt (prompt_t *ppr);
prompt_t *init_prompt (const char *prompt);
void alert (char *fmt, ...);
void add_history (prompt_t *pr, char *str, size_t len);
int read_key (char *buf, int len);

#endif /* PROMPT_H */

