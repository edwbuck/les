#ifndef MOVEMENT_H
#define MOVEMENT_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDDEF_H
#  include <stddef.h>
#endif /* HAVE_STDDEF_H */

#include "linewrap.h"

extern int line1;

extern tline_t *tlines;
extern size_t tlines_len;
extern size_t tlines_size;

void move_forward (int n);
void move_backward (int n);
void move_start ();
void move_end ();
void move_left (int n);
void move_right (int n);
void move_line_left ();
void move_line_right ();
void move_to_pos (size_t pos);
void move_to_line (int line);

#endif /* MOVEMENT_H */
