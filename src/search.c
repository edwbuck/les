#include "page.h"
#include "readfile.h"
#include "linewrap.h"
#include "prompt.h"
#include "charinfo.h"
#include "movement.h"
#include "tabs.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif /* HAVE_STDIO_H */

#include <ctype.h>
#include <term.h>
#include <errno.h>
#include <string.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

prompt_t *spr = NULL;
pcre2_code *re = NULL;
int active_search = 0;
int search_version = 0;

// First one that starts after tabb->pos, or the first match
void get_current_match () {
    tabb->current_match = 0;
    int i;
    for (i = 0; i < tabb->matches_len; i++) {
        if (tabb->matches[i].start >= tabb->pos) {
            tabb->current_match = i;
            return;
        }
    }
}

// first one at the bottom of the screen, or the last match
void get_current_match_backward () {
    tabb->current_match = tabb->matches_len - 1;
    int i;
    for (i = tabb->matches_len - 1; i >= 0; i--) {
        if (tabb->matches[i].start < tlines[tlines_len - 1].end_pos) {
            tabb->current_match = i;
            return;
        }
    }
}

void move_to_match () {
    size_t pos = tabb->matches[tabb->current_match].start;
    size_t pos2;
    int retval;
    // Move the page to the right to be able to see the match
    if (!line_wrap) {
        int bol = prev_line(tabb->buf, tabb->buf_len, pos, 0);
        int width = strnwidth(tabb->buf + bol, pos - bol);
        if (width < columns) {
            tabb->column = 0;
        }
        else {
            int end = tabb->matches[tabb->current_match].end;
            int eol = next_line(tabb->buf, tabb->buf_len, pos, 1);
            if (eol < end) {
                end = eol;
            }
            int width2 = strnwidth(tabb->buf + pos, end - pos);
            if (width2 > columns) {
                tabb->column = width - (columns / 2);
            }
            else {
                tabb->column = width - columns + width2;
            }
        }
    }
    if (pos >= tabb->pos && pos < tlines[tlines_len - 1].end_pos) {
        return;
    }
    else if (pos < tabb->pos) {
        retval = find_pos_backward(lines - line1 - 1, pos, &pos2);
        if (retval) {
            tabb->line += count_lines_atob(tabb->buf, tabb->pos, pos2);
            tabb->pos = pos2;
        }
        else {
            pos2 = line_before_pos(pos, (lines - line1 - 1) / 2);
            tabb->line += count_lines_atob(tabb->buf, tabb->pos, pos2);
            tabb->pos = pos2;
        }
    }
    else if (pos >= tlines[tlines_len - 1].end_pos) {
        retval = find_pos_forward(lines - line1 - 1, pos, &pos2);
        if (retval) {
            tabb->line += count_lines_atob(tabb->buf, tabb->pos, pos2);
            tabb->pos = pos2;
        }
        else {
            pos2 = line_before_pos(pos, (lines - line1 - 1) / 2);
            tabb->line += count_lines_atob(tabb->buf, tabb->pos, pos2);
            tabb->pos = pos2;
        }
    }
}

void find_matches () {
    active_search = 0;
    tabb->search_version = search_version;
    tabb->matches_len = 0;
    tabb->current_match = 0;
    if (!re) {
        return;
    }
    active_search = 1;

    pcre2_match_data *mdata = pcre2_match_data_create_from_pattern(re, NULL);
    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(mdata);
    int i = 0;

    while (i < tabb->buf_len) {
        int retval = pcre2_match(
            re,
            (PCRE2_SPTR) tabb->buf,
            tabb->buf_len,
            i,
            0,
            mdata,
            NULL
        );
        if (retval == PCRE2_ERROR_NOMATCH) {
            break;
        }
        else if (retval <= 0) {
            alert("Match error %d", retval);
            goto end;
        }
        if (tabb->matches_len + 1 > tabb->matches_size) {
            if (!tabb->matches_size) {
                tabb->matches_size = 16;
                tabb->matches = malloc(tabb->matches_size * sizeof (match_t));
            }
            else {
                tabb->matches_size *= 2;
                tabb->matches = realloc(tabb->matches, tabb->matches_size * sizeof (match_t));
            }
        }
        tabb->matches_len++;
        tabb->matches[tabb->matches_len - 1].start = ovector[0];
        tabb->matches[tabb->matches_len - 1].end = ovector[1];
        i = ovector[1];
        if (i == ovector[0]) {
            i += UTF8_LENGTH(tabb->buf[i]);
        }
    }

    if (tabb->matches_len == 0) {
        goto end;
    }

    get_current_match();
    move_to_match();

    end:
    pcre2_match_data_free(mdata);
    draw_tab();
}

char *history_file () {
    char *home = getenv("HOME");
    if (!home) {
        return NULL;
    }
    static char file[256];
    snprintf(file, sizeof file, "%s/.les_history", home);
    return file;
}

void load_search_history () {
    spr = init_prompt("/");
    char *file = history_file();
    if (!file) {
        return;
    }
    FILE *fp = fopen(file, "r");
    if (!fp) {
        return;
    }

    char str[512];
    while (fgets(str, sizeof str, fp)) {
        int len = strlen(str);
        if (str[len - 1] == '\n') {
            str[len - 1] = '\0';
            len--;
        }
        if (len) {
            add_history(spr, str, len);
            spr->history_new++;
        }
    }

    fclose(fp);
}

void save_search_history () {
    if (spr->history_len == spr->history_new) {
        return;
    }
    char *file = history_file();
    if (!file) {
        return;
    }
    FILE *fp = fopen(file, "a");
    if (!fp) {
        fprintf(stderr, "Couldn't open %s: %s\n", file, strerror(errno));
        exit(1);
    }
    int i;
    for (i = spr->history_new; i < spr->history_len; i++) {
        fprintf(fp, "%s\n", spr->history[i]);
    }
    fclose(fp);
}

void search2 (char *pattern) {
    if (re) {
        pcre2_code_free(re);
    }
    re = NULL;
    active_search = 0;
    search_version++;
    tabb->search_version = search_version;
    tabb->matches_len = 0;
    tabb->current_match = 0;
    if (!pattern || !pattern[0]) {
        draw_tab();
        return;
    }

    static char buf[256];
    int retval = 0;
    PCRE2_SIZE offset = 0;
    re = pcre2_compile(
        (PCRE2_SPTR) pattern,
        PCRE2_ZERO_TERMINATED,
        PCRE2_MULTILINE|PCRE2_DOTALL|PCRE2_CASELESS,
        &retval,
        &offset,
        NULL
    );

    if (!re) {
        pcre2_get_error_message(retval, (PCRE2_UCHAR *) buf, sizeof buf);
        buf[0] = toupper(buf[0]);
        alert(buf);
        return;
    }
    find_matches();
}

void search () {
    char *pattern = gets_prompt(spr);
    if (interrupt) {
        draw_tab();
        return;
    }
    search2(pattern);
}

void next_match () {
    if (search_version == 0 && spr->history_len) {
        search2(spr->history[spr->history_len - 1]);
        return;
    }
    if (tabb->search_version != search_version) {
        find_matches();
        return;
    }
    if (!tabb->matches_len) {
        return;
    }
    size_t start = tabb->matches[tabb->current_match].start;
    if (start >= tlines[0].pos && start < tlines[tlines_len - 1].end_pos) {
        tabb->current_match = (tabb->current_match + 1) % tabb->matches_len;
    }
    else {
        get_current_match();
    }
    move_to_match();
    draw_tab();
}

void prev_match () {
    if (search_version == 0 && spr->history_len) {
        search2(spr->history[spr->history_len - 1]);
        return;
    }
    if (tabb->search_version != search_version) {
        find_matches();
        return;
    }
    if (!tabb->matches_len) {
        return;
    }
    if (tabb->matches[tabb->current_match].start >= tlines[0].pos &&
        tabb->matches[tabb->current_match].end < tlines[tlines_len - 1].end_pos) {
        tabb->current_match = (tabb->matches_len + tabb->current_match - 1) % tabb->matches_len;
    }
    else {
        get_current_match_backward();
    }
    move_to_match();
    draw_tab();
}

