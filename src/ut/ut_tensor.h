/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
#ifndef UT_TENSOR0_H
#define UT_TENSOR0_H

extern void ut_tensor_vec2leq (double *, double *);
extern void ut_tensor_leq2vec (double *, double *);
extern void ut_tensor_std2leq (double *, double *);
extern void ut_tensor_leq2std (double *, double *);
extern void ut_tensor_leq2mat (double *, double **);
extern void ut_tensor_mat2leq (double **, double *);

#endif /* UT_TENSOR0_H */
