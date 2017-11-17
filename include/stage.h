#ifndef STAGE_H
#define STAGE_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDARG_H
#  include <stdarg.h>
#endif /* HAVE_STDARG_H */

#ifdef HAVE_STDDEF_H
#  include <stddef.h>
#endif /* HAVE_STDDEF_H */

void stage_init ();
void stage_cat (const char *str);
void stage_ncat (const char *str, size_t n);
void stage_vprintf (const char *fmt, va_list args);
void stage_printf (const char *fmt, ...);
void stage_write ();

#endif /* STAGE_H */
