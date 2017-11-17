#ifndef PAGE_H
#define PAGE_H

#include "charinfo.h"
#include "linewrap.h"

void draw_tab ();
void stage_tab2 (int n, tline_t *tlines, size_t tlines_len);
void draw_status ();
void init_page ();
void set_ttybuf (charinfo_t *cinfo, char *buf, int len);

#endif /* PAGE_H */
