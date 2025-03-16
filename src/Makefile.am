# src/Makefile.am

AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I$(top_srcdir)/include

AM_CFLAGS = -Wall -Werror

build_PROGRAMS = $(top_builddir)/build/game

__top_builddir__build_game_LDADD = -lGL -lglfw

__top_builddir__build_game_SOURCES = glad.c utils/file_read.c main.c
