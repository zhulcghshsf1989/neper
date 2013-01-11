/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_TESS_EXPORT_H
#define NEUT_TESS_EXPORT_H

extern void neut_tess_fprintf (FILE*, struct TESS);
extern void neut_tess_fprintf_gmsh (FILE*, struct TESS);
extern void neut_tess_fprintf_ply  (FILE*, struct TESS);
extern void neut_tess_fprintf_dec  (FILE*, struct TESS);

#endif /* NEUT_TESS_EXPORT_H */
