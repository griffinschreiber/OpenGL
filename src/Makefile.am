# src/Makefile.am

AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I$(top_srcdir)/include

AM_CFLAGS = -Wall -Werror

bin_PROGRAMS = $(top_builddir)/bin/template

__top_builddir__bin_template_LDADD = -lGL -lglfw -Llib/libcglm.a

__top_builddir__bin_template_SOURCES = main.c
