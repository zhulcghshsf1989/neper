/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef ZEROSUTILS0_H
#define ZEROSUTILS0_H

/* BOARD INITIALIZATIONS ---------------------------------------------------- */
/* Initialization to 0 of a array whose name is specified as first parameter
 * of the function.
 */
extern void oneDIntZeros (int *, int, int);
extern void oneDFloatZeros (float *, int, int);
extern void oneDDoubleZeros (double *, int, int);
extern void twoDIntZeros (int **, int, int, int, int);
extern void twoDFloatZeros (float **, int, int, int, int);
extern void twoDDoubleZeros (double **, int, int, int, int);

#endif /* ZEROSUTILS0_H */
