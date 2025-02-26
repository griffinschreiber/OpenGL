# src/Makefile.am

AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I$(top_srcdir)/include

AM_CFLAGS = -Wall -Werror

bin_PROGRAMS = $(top_builddir)/bin/game

__top_builddir__bin_game_LDADD = -lGL -lglfw

__top_builddir__bin_game_SOURCES = glad.c utils/file_read.c renderer.c main.c
