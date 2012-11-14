/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef ALLOCUTILS0_H
#define ALLOCUTILS0_H

extern int *ut_alloc_1d_int (unsigned int);
extern float *oneDFloatDAlloc (unsigned int);
extern double *oneDDoubleDAlloc (unsigned int);
extern FILE **oneDpFileDAlloc (unsigned int);
extern int **twoDIntDAlloc (unsigned int, unsigned int);
extern int ***threeDIntDAlloc (unsigned int, unsigned int, unsigned int);
extern float **twoDFloatDAlloc (unsigned int, unsigned int);
extern double **twoDDoubleDAlloc (unsigned int, unsigned int);
extern double ***threeDDoubleDAlloc (unsigned int, unsigned int,
				     unsigned int);
extern char *oneDCharDAlloc (unsigned int);
extern char **twoDCharDAlloc (unsigned int, unsigned int);
extern char ***threeDCharDAlloc (unsigned int, unsigned int, unsigned int);
extern int **oneDpIntDAlloc (unsigned int);
extern double **oneDpDoubleDAlloc (unsigned int);
extern int ***oneDppIntDAlloc (unsigned int);
extern double ***oneDppDoubleDAlloc (unsigned int);

#endif /* ALLOCUTILS0_H */
