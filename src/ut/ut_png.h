/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
#ifndef UT_PNG0_H
#define UT_PNG0_H
#include "ut.h"

#ifdef HAVE_PNG

#include <png.h>

extern void ut_png_init_png_file_rgb (int, int, png_structp *, png_infop *,
			       png_bytep **);
extern void ut_png_set_png_pixel  (int, int, int *, png_bytep **);
extern void ut_png_read_png_file  (png_structp*, png_infop*, png_bytep**, char*);
extern void ut_png_write_png_file (png_structp, png_infop, png_bytep*, char *);
extern void ut_png_free_png_file (png_infop, png_bytep *);

extern int ut_png_average_png_images (png_infop, png_bytep*, double, png_infop,
		    png_bytep*, double, png_structp*, png_infop*,
		    png_bytep**);
#endif

#endif /* UT_PNG0_H */
