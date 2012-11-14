/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
#ifndef UT_FIG0_H
#define UT_FIG0_H

extern void ut_fig_header (FILE *);
extern void ut_fig_polyline_box (FILE *, int, int, int, int, int, int);
extern void ut_fig_polyline_fillbox (FILE *, int, int, int, int, int, int);
extern void ut_fig_text (FILE *, int, int, int, int, int, char *);

#endif /* UT_FIG0_H */
