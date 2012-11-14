/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
extern int ut_color_name_valid (char*);
extern int ut_color_name_rgb (char *, int *);
extern int name2rgb (char *, int *);
extern void rgb_intensity (int *, double, int *);
extern int ut_color_scheme_val_color (char*, double, double, double, int*);
