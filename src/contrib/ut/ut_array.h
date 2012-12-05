/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifndef UT_ARRAY_H
#define UT_ARRAY_H

extern void ut_array_1d_scale (double *, int, double);
extern void ut_array_1d_int_scale (int *, int, int);
extern void ut_array_1d_add (double *, double *, int, double *);
extern void ut_array_1d_addval (double *, int, double, double *);
extern void ut_array_2d_add (double **, double **, int, int, double **);
extern void ut_array_3d_add (double ***, double ***, int, int, int, double ***);
extern void ut_array_3d_add_float (float ***, float ***, int, int, int, float ***);
extern void ut_array_1d_int_addval (int *, int, int, int *);
extern double ut_array_1d_norm (double *, int);
extern double ut_array_1d_norm_int (int *, int);
extern void ut_array_1d_zero (double *, int);
extern void ut_array_1d_int_zero (int *, int);
extern void ut_array_2d_zero (double **, int, int);
extern void ut_array_3d_zero (double ***, int, int, int);
extern void ut_array_3d_zero_float (float ***, int, int, int);
extern void ut_array_2d_int_zero (int **, int, int);
extern void ut_array_3d_int_zero (int ***, int, int, int);
extern void ut_array_1d_zero_be (double *, int, int);
extern void ut_array_1d_float_zero_be (float *, int, int);
extern void ut_array_1d_int_zero_be (int *, int, int);
extern void ut_array_2d_zero_be (double **, int, int, int, int);
extern void ut_array_2d_float_zero_be (float **, int, int, int, int);
extern void ut_array_2d_int_zero_be (int **, int, int, int, int);
extern int ut_array_1d_fscanf (FILE *, double *, int);
extern int ut_array_1d_int_fscanf (FILE *, int *, int);
extern int ut_array_2d_fscanf (FILE*, double**, int, int);
extern int ut_array_2d_fscanfn (char*, double**, int, int);
extern int ut_array_2d_fscanfn_wcard (char*, double**, int, int, char*);
extern int ut_array_1d_fscanfn_wcard (char*, double*, int, char*);
extern int ut_array_1d_fprintf (FILE *, double *, int, char *);
extern int ut_array_1d_float_fprintf (FILE *, float *, int, char *);
extern int ut_array_1d_int_fprintf (FILE *, int *, int, char *);
extern int ut_array_1d_uint_fprintf (FILE *, unsigned int *, int, char *);
extern int ut_array_2dns_int_fprintf (FILE*, int**, int, int*, char*);
extern int ut_array_2d_fprintf (FILE *, double **, int, int, char *);
extern int ut_array_2d_float_fprintf (FILE *, float **, int, int, char *);
extern int ut_array_2d_int_fprintf (FILE *, int **, int, int, char *);
extern int ut_array_3d_int_fprintf (FILE *, int ***, int, int, int, char *);

extern int ut_array_2d_int_fscanf (FILE *, int **, int, int);


extern void ut_array_2d_scale (double **, int, int, double);

extern double ut_array_1d_min (double *, int);
extern double ut_array_2d_col_min (double **, int, int);
extern double ut_array_2d_col_max (double **, int, int);
extern int ut_array_1d_min_index (double *, int);
extern int ut_array_1d_min2_index (double *, int);
extern double ut_array_1d_max (double *, int);
extern double ut_array_1d_mean (double *, int);
extern double ut_array_1d_gmean (double *, int);
extern double ut_array_1d_wmean (double *, double *, int);
extern double ut_array_1d_stddev (double *, double, int);
extern double ut_array_2d_stddev (double **, double, int, int);
extern double ut_array_1d_absmax (double *, int);
extern int ut_array_1d_max_index (double *, int);
extern int ut_array_1d_int_max_index (int *, int);
extern int ut_array_1d_absmax_index (double *, int);
extern int ut_array_1d_int_min (int *, int);
extern int ut_array_1d_int_max (int *, int);
extern float ut_array_1d_float_min (float *, int);
extern float ut_array_1d_float_max (float *, int);
extern float ut_array_2d_float_min (float **, int, int);
extern float ut_array_2d_float_max (float **, int, int);
extern double ut_array_2d_min (double **, int, int);
extern double ut_array_2d_mean (double **, int, int);
extern double ut_array_2d_gmean (double **, int, int);
extern double ut_array_3d_mean (double ***, int, int, int);
extern double ut_array_3d_gmean (double ***, int, int, int);
extern double ut_array_3d_min (double ***, int, int, int);
extern double ut_array_3d_max (double ***, int, int, int);
extern double ut_array_3d_stddev (double ***, double, int, int, int);
extern float  ut_array_3d_float_mean (float ***, int, int, int);
extern float  ut_array_3d_float_gmean (float ***, int, int, int);
extern float  ut_array_3d_float_min (float ***, int, int, int);
extern float  ut_array_3d_float_max (float ***, int, int, int);
extern float  ut_array_3d_float_stddev (float ***, float, int, int, int);
extern double ut_array_3d_int_mean (int ***, int, int, int);
extern double ut_array_3d_int_gmean (int ***, int, int, int);
extern int    ut_array_3d_int_min (int ***, int, int, int);
extern int    ut_array_3d_int_max (int ***, int, int, int);
extern double ut_array_3d_int_stddev (int ***, double, int, int, int);
extern double ut_array_3d_uchar_mean (unsigned char ***, int, int, int);
extern double ut_array_3d_uchar_gmean (unsigned char ***, int, int, int);
extern unsigned char ut_array_3d_uchar_min (unsigned char ***, int, int, int);
extern unsigned char ut_array_3d_uchar_max (unsigned char ***, int, int, int);
extern double ut_array_3d_uchar_mean (unsigned char ***, int, int, int);
extern double ut_array_3d_uchar_gmean (unsigned char ***, int, int, int);
extern unsigned short ut_array_3d_ushort_min (unsigned short ***, int, int, int);
extern unsigned short ut_array_3d_ushort_max (unsigned short ***, int, int, int);
extern double ut_array_3d_ushort_stddev (unsigned short ***, double, int, int, int);
extern double ut_array_3d_uchar_stddev (unsigned char ***, double, int, int, int);
extern double ut_array_2d_max (double **, int, int);
extern double ut_array_2d_absmax (double **, int, int);
extern int ut_array_1d_int_max_index (int *, int);
extern double ut_array_1d_sum (double *, int);
extern int ut_array_1d_int_sum (int *, int);
extern double ut_array_1d_abssum (double *, int);
extern int ut_array_1d_int_abssum (int *, int);
extern void ut_array_1d_abs (double *, int);
extern void ut_array_1d_int_abs (int *, int);
extern void ut_array_1d_int_sgn (int *, int, int*);

extern void ut_array_1d_memcpy (double *, int, double *);
extern void ut_array_1d_int_memcpy (int *, int, int *);
extern void ut_array_2d_memcpy (double **, int, int, double **);
extern void ut_array_2d_int_memcpy (int **, int, int, int **);
extern void ut_array_1d_pchar_memcpy (char**, int, char**);
extern void ut_array_3d_memcpy (double ***, int, int, int, double ***);
extern void ut_array_3d_int_memcpy (int ***, int, int, int, int ***);
extern void ut_array_3d_uchar_memcpy (unsigned char ***, int, int, int, unsigned char ***);
extern void ut_array_3d_float_memcpy (float ***, int, int, int, float ***);

extern void ut_array_1d_switch (double *, int, int);
extern void ut_array_1d_int_switch (int *, int, int);
extern void ut_array_1d_sort (double *, int);
extern void ut_array_1d_sort_des (double *, int);
extern void ut_array_1d_int_sort_des (int *, int);
extern void ut_array_1d_sort_index (double *, int, int *);
extern void ut_array_1d_sort_perm (double *, int, int *);
extern void ut_array_1d_sort_index_perm (int*, int, int*);
extern void ut_array_1d_int_sort (int *, int);
extern void ut_array_1d_int_sort_index (int *, int, int *);
extern void ut_array_1d_int_dsort_index (int *, int, int *);
extern void ut_array_1d_pchar_sort (char**, int);
extern void ut_array_1d_abssort (double *, int);
extern void ut_array_1d_abssort_index (double *, int, int *);
extern void ut_array_1d_int_abssort_index (int *, int, int *);
extern int ut_array_1d_int_find (int *, int, int);
extern void ut_array_1d_int_findnreplace (int*, int, int, int);

extern void ut_array_2d_int_sortbycol (int**, int, int, int);
extern void ut_array_2d_int_sortbycol_des (int**, int, int, int);

extern void ut_array_1d_view (double *, int);
extern void ut_array_2d_view (double **, int, int);
extern void ut_array_1d_int_view (int *, int);
extern void ut_array_2d_int_view (int **, int, int);

extern void ut_array_2d_switchlines (double **, int, int, int);
extern void ut_array_2d_int_switchlines_length (int**, int, int, int, int);
extern void ut_array_2d_int_switchlines (int **, int, int, int);

extern void ut_array_1d_int_bubbleelt (int *, int, int);

extern void ut_array_1d_int_rotate (int*, int, int);
extern void ut_array_1d_rotate (double*, int, int);

extern void ut_array_1d_int_reverseelts (int *, int);
extern void ut_array_1d_sort_des_index (double*, int, int*);
extern void ut_array_1d_int_sort_des_index (int*, int, int*);
extern void ut_array_1d_reverseelts (double *, int);
extern int ut_array_1d_max_int_index (int *, int);

extern void ut_array_1d_permutation (double *, int, int *);
extern void ut_array_1d_permutation_int (int *, int, int *);
extern void ut_array_2d_permutation_int (int **, int, int, int *);

extern void ut_array_distribparam (double *, int, double, double *, int *);
extern void ut_array_distrib (double *, int, double, double, int, double *);

extern void ut_array_1d_set (double *, int, double);
extern void ut_array_1d_int_set (int *, int, int);
extern void ut_array_2d_set (double **, int, int, double);
extern void ut_array_2d_int_set (int **, int, int, int);
extern void ut_array_3d_set (double ***, int, int, int, double);
extern void ut_array_3d_int_set (int ***, int, int, int, int);
extern int ut_array_1d_rotpos (int, int, int, int);
extern int ut_array_1d_int_eltpos (int *, int, int);
extern int ut_array_2d_int_linepos (int**, int, int, int*);
extern int ut_array_2dns_int_linepos (int**, int, int*, int*, int);
extern int ut_array_1d_int_eltpos_all (int*, int, int, int*);
extern int ut_array_2d_int_eltpos (int **, int, int, int, int *, int *);
extern int ut_array_1d_string_eltpos (char**, int, char*);

extern double ut_array_1d_valuepos (double*, int, double);
extern void ut_array_1d_int_uniq (int *, int*);
extern void ut_array_1d_int_sort_uniq (int*, int, int*);
extern void ut_array_1d_sub (double*, double*, int, double*);

extern void ut_array_2d_sub (double**, double**, int, int, double**);

extern void ut_array_1d_sample (double*, int, double, double**, int*);
extern void ut_array_2d_sample (double**, int, int, double, double***, int*);

extern int ut_array_1d_fscanf_sample (FILE*, int, double, double**, int*);
extern int ut_array_2d_fscanf_sample (FILE*, int, int, double, double***, int*);

extern int ut_array_1d_int_nbofthisval (int *, int, int);
extern void ut_array_1d_commonelts (int*, int, int*, int, int**, int*);
extern int * ut_array_1d_int_cat (int *, int, int *, int);
extern int ut_array_1d_int_deletencompress (int *, int, int, int);
extern void ut_array_1d_int_memcpy_be (int *, int*, int, int);

extern void ut_array_1d_int_union (int*, int, int*, int, int*, int*);
extern void ut_array_1d_int_minus (int*, int, int*, int, int*, int);

extern void ut_array_1d_int_inter (int*, int, int*, int, int*, int*);
extern int ut_array_1d_int_min_index (int*, int);

extern int ut_array_1d_int_inv (int*, int, int**, int*);

extern void ut_array_1d_int_sort_index2 (int*, int, int*);
extern void ut_array_1d_sort_index2 (double*, int, int*);

extern int ut_array_2dns_int_fprintf (FILE*, int**, int, int*, char*);
extern int ut_array_1d_int_equal (int*, int*, int);
extern int ut_array_1d_equal (double*, double*, int, double);
extern int ut_array_1d_requal (double*, double*, int, double);

extern double ut_array_2d_cov (double**, double, double, int, int, int);
extern double ut_array_2d_linearcorr (double**, int, int, int);

extern void ut_array_2d_transpose (double**, int, int, double***);
extern void ut_array_2d_int_transpose (int**, int, int, int***);

extern int ut_array_1d_char_find (char *, int, char);

extern int ut_array_1d_int_list_addelt (int** parray, int* psize, int val);
extern void ut_array_1d_int_valqty (int* array, int size, int*** parray2, int* parray2qty);

extern int ut_array_1d_int_diff (int*, int, int*, int);

extern int ut_array_1d_int_percent (int* array, int size, int* percent);

#endif /* UT_ARRAY_H */
