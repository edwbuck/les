#ifndef RECENTFILES_H
#define RECENTFILES_H

#include "tabs.h"

#define RECENTS     8

void add_recent_tab (tab_t *tabb);
void save_recents_file ();
void add_recents_tab ();
void load_recents_file ();
void get_last_line ();

#endif /* RECENTFILES_H */

