/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_TESS_IMPORT_H
#define NEUT_TESS_IMPORT_H

extern void neut_tess_fscanf (FILE*, struct TESS*);
extern void neut_tess_fscanf_verbosity (FILE*, struct TESS*, int);
extern void neut_tess_fscanf_head (struct TESS*, FILE *);
extern void neut_tess_name_fscanf (char*, struct TESS*);

#endif /* NEUT_TESS_IMPORT_H */
