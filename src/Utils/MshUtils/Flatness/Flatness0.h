/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef FLATNESS0_H
#define FLATNESS0_H

extern int *ReadMod (char *);
extern void Flatness (struct NODES, struct MESH, int *, double **);
extern void WriteFaceFF (double **, int *, FILE *);
extern double ThisFaceFF (struct NODES, struct MESH, int, int);

#endif /* FLATNESS0_H */
