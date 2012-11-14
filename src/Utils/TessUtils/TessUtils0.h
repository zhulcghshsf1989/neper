/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef TESSUTILS0_H
#define TESSUTILS0_H

extern void PolyFree (struct POLY *, int);

extern void TessImport (struct TESS *, FILE *);
extern void TessExport (struct TESS, FILE *);

extern void T2P (struct TESS, int, struct POLY *);
extern int InPoly (double *, struct TESS, int);

#include"P2Pm/P2Pm0.h"

#endif /* TESSUTILS0_H */
