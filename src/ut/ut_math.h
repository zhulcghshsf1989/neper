/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
#ifndef UT_MATH0_H
#define UT_MATH0_H

extern int ut_math_eval_ww (char*, int, char**, double*, double*);
extern int ut_math_eval (char*, int, char**, double*, double*);
extern int ut_math_eval_substitute (char*, char**);
extern int ut_math_eval_substitute_max (char*, char**);
extern int ut_math_eval_substitute_min (char*, char**);
extern int ut_math_eval_subeval (char*, char*, char*, char*);

#endif /* UT_MATH0_H */
