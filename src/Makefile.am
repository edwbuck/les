bin_PROGRAMS=les

les_SOURCES=main.c \
charinfo.c \
linewrap.c \
movement.c \
page.c \
prompt.c \
readfile.c \
recentfiles.c \
search.c \
stage.c \
tabs.c

les_CPPFLAGS= -I$(top_srcdir)/include -Werror
les_LDADD = @CURSES_LIBS@ -lpcre2-8
