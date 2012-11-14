/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef FREEUTILS0_H
#define FREEUTILS0_H

/* MEMORY FREEING ----------------------------------------------------------- */
extern void oneDIntFree (int *);
extern void oneDFloatFree (float *);
extern void oneDDoubleFree (double *);
extern void oneDCharFree (char *);
extern void twoDIntFree (int **, unsigned int);
extern void threeDIntFree (int ***, unsigned int, unsigned int);
extern void twoDFloatFree (float **, unsigned int);
extern void twoDDoubleFree (double **, unsigned int);
extern void threeDDoubleFree (double ***, unsigned int, unsigned int);
extern void oneDCharFree (char *);
extern void twoDCharFree (char **, unsigned int);
extern void oneDpFileFree (FILE **);


#endif /* FREEUTILS0_H */
