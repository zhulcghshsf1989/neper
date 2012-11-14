/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef OPERUTILS0_H
#define OPERUTILS0_H

/* OPERATIONS ON 1D BOARDS -------------------------------------------------
 * (1) minimum and (2) maximum value of a double 1-2 Array
 */
extern int oneDIntMin (int *, int, int);
extern int oneDIntMax (int *, int, int);
extern int oneDIntSum (int *, int, int);
extern int oneDIntNbOfThisVal (int *, int, int, int);
extern float oneDFloatMin (float *, int, int);
extern float oneDFloatMax (float *, int, int);
extern double oneDDoubleMin (double *, int, int);
extern double oneDDoubleMax (double *, int, int);
extern double oneDDoubleSum (double *, int, int);
extern int oneDDoubleMaxPos (double *, int, int);
extern int oneDDoubleAMaxPos (double *, int, int);
extern void oneDDoubleScale (double *, int, int, double);
extern void oneDIntRevElts (int *, int, int);
extern void oneDDoubleRevElts (double *, int, int);
extern void oneDIntRotElts (int *, int, int, int);
extern int oneDIntEltPos (int *, int, int, int, int);
extern int oneDRotPos (int, int, int, int);
extern void oneDDoubleCpy (double *, int, int, double *, int, int);

extern void oneDIntSort (int *, int, int);
extern int oneDIntCompress (int *, int, int);
extern int oneDIntSortNCompress (int *, int, int);
extern int oneDIntDeleteNCompress (int *, int, int, int, int);
extern int *oneDIntCat (int *, int, int, int *, int, int, int);
extern void oneDIntCpy (int *, int, int, int *, int, int);
extern int oneDIntFindNReplace (int *, int, int, int, int, int);
extern int oneDIntCommonElts (int *, int, int, int *, int, int, int **, int);

#endif /* OPERUTILS0_H */
