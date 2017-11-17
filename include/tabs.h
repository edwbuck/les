#ifndef TABS_H
#define TABS_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDDEF_H
#  include <stddef.h>
#endif /* HAVE_STDDEF_H */

#ifdef TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
#else
#  ifdef HAVE_SYS_TIME
#      include <sys/time.h>
#  else /* HAVE_SYS_TIME */
#      include <time.h>
#  endif /* HAVE_SYS_TIME */
#endif /* TIME_WITH_SYS_TIME_H */

#define READY       0
#define OPENED      1
#define LOADED      2
#define MARKED      4
#define HELP        16
#define ERROR       32
#define FILEBACKED  128
#define LOADFOREVER 256
#define SPECIAL     512

#define UNDERLINED 1
#define BOLD 2

typedef struct {
    size_t start;
    size_t end;
    int type;
} highlight_t;

typedef struct {
    size_t start;
    size_t end;
} match_t;

typedef struct {
    const char *name;
    int name_width;
    char *name2;
    int fd;
    char *buf;
    size_t buf_size;
    size_t buf_len;
    char *stragglers;
    size_t stragglers_len;
    size_t pos;
    int state;
    int line;
    int nlines;
    int column;
    char *mesg;
    size_t mesg_size;
    size_t mesg_len;
    size_t mark;
    time_t opened;
    char *realpath;
    int last_line;
    int search_version;
    match_t *matches;
    size_t matches_len;
    size_t matches_size;
    size_t current_match;
    highlight_t *highlights;
    size_t highlights_len;
    size_t highlights_size;
    size_t highlights_processed;
} tab_t;

extern int current_tab;
extern tab_t **tabs;
extern size_t tabs_len;
extern tab_t *tabb;
extern int search_version;

void stage_tabs ();
void close_tab ();
void next_tab ();
void prev_tab ();
void add_tab (const char *name, int fd, int state);
void change_tab ();
void init_line1 ();

#endif /* TABS_H */
