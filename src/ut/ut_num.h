/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
#ifndef UT_NUM0_H
#define UT_NUM0_H

extern double ut_num_atan2 (double, double);
extern int ut_num_d2ri (double);
extern int ut_num_min_int (int, int);
extern int ut_num_max_int (int, int);
extern double ut_num_min (double, double);
extern double ut_num_max (double, double);
extern double ut_num_max3 (double, double, double);
extern double ut_num_absmax3 (double, double, double);
extern int ut_num_absmax3_index (double, double, double);
extern int ut_num_tenlen (double);
extern double ut_num_p2 (double);
extern double ut_num_acos (double);
extern double ut_num_sqrt (double);
extern void ut_num_scale (double *, double);
extern double ut_num_sgn (double);
extern int ut_num_sgn_int (int);
extern int ut_num_fabs_int (int);
extern int ut_num_rotpos (int, int, int, int);
extern int ut_num_equal_int (int, int);
extern int ut_num_equal (double, double, double);
extern int ut_num_requal (double, double, double);
extern int ut_num_equal_double (double, double, double);
extern int ut_num_requal (double, double, double);
extern int ut_num_higher (double, double);
extern int ut_num_lower (double, double);
extern int ut_num_ahigher (double, double);
extern int ut_num_alower (double, double);

extern int ut_num_lcomden (int, int);

extern double ut_num_readwcomma (char *);
extern int ut_num_even (int);
extern int ut_num_odd (int);

extern int ut_num_int_ininter (int, int, int);

#endif /* UT_NUM0_H */
