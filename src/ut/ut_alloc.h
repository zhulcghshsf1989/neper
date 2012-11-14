/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
#ifndef UT_ALLOC_H
#define UT_ALLOC_H

extern int *ut_alloc_1d_int (unsigned int);
extern unsigned short *ut_alloc_1d_ushort (unsigned int);
extern unsigned char *ut_alloc_1d_uchar (unsigned int);
extern unsigned int *ut_alloc_1d_uint (unsigned int);
extern size_t *ut_alloc_1d_sizet (unsigned int);
extern float *ut_alloc_1d_float (unsigned int);
extern double *ut_alloc_1d_double (unsigned int);
extern char *ut_alloc_1d_char (unsigned int);
extern char *ut_realloc_1d_char (char*, unsigned int);
extern double *ut_alloc_1d (unsigned int);
extern FILE **ut_alloc_1d_pfile (unsigned int);
extern int **ut_alloc_1d_pint (unsigned int);
extern unsigned short **ut_alloc_1d_pushort (unsigned int);
extern unsigned char **ut_alloc_1d_puchar (unsigned int);
extern int **ut_realloc_1d_pint (int**, unsigned int);
extern unsigned short **ut_realloc_1d_pushort (unsigned short**, unsigned int);
extern unsigned char **ut_realloc_1d_puchar (unsigned char**, unsigned int);
extern double **ut_realloc_1d_pdouble (double**, unsigned int);
extern double ***ut_realloc_1d_ppdouble (double***, unsigned int);
extern char **ut_realloc_1d_pchar (char**, unsigned int);
extern char **ut_alloc_1d_pchar (unsigned int);
extern double **ut_alloc_1d_pdouble (unsigned int);
extern int ***ut_alloc_1d_ppint (unsigned int);
extern unsigned short ***ut_alloc_1d_ppushort (unsigned int);
extern unsigned char ***ut_alloc_1d_ppuchar (unsigned int);
extern char ***ut_alloc_1d_ppchar (unsigned char);
extern double ***ut_alloc_1d_ppdouble (unsigned int);
extern int **ut_alloc_2d_int (unsigned int, unsigned int);
extern unsigned short **ut_alloc_2d_ushort (unsigned int, unsigned int);
extern unsigned char **ut_alloc_2d_uchar (unsigned int, unsigned int);
extern char ***ut_alloc_2d_pchar (unsigned int, unsigned int);
extern unsigned int **ut_alloc_2d_uint (unsigned int, unsigned int);
extern float **ut_alloc_2d_float (unsigned int, unsigned int);
extern double **ut_alloc_2d_double (unsigned int, unsigned int);
extern char **ut_alloc_2d_char (unsigned int, unsigned int);
extern double **ut_alloc_2d (unsigned int, unsigned int);
extern int ***ut_alloc_3d_int (unsigned int, unsigned int, unsigned int);
extern unsigned short ***ut_alloc_3d_ushort (unsigned int, unsigned int, unsigned int);
extern unsigned char ***ut_alloc_3d_uchar (unsigned int, unsigned int, unsigned int);
extern unsigned int ***ut_alloc_3d_uint (unsigned int, unsigned int, unsigned int);
extern double ***ut_alloc_3d_double (unsigned int, unsigned int,
				     unsigned int);
extern float ***ut_alloc_3d_float (unsigned int, unsigned int,
				     unsigned int);
extern double ***ut_alloc_3d (unsigned int, unsigned int, unsigned int);
extern char ***ut_alloc_3d_char (unsigned int, unsigned int, unsigned int);

extern double ****ut_alloc_4d (unsigned int, unsigned int,
			       unsigned int, unsigned int);
extern double ****ut_alloc_4d_double (unsigned int, unsigned int,
				      unsigned int, unsigned int);

extern int *ut_realloc_1d_int (int*, unsigned int);
extern unsigned short *ut_realloc_1d_ushort (unsigned short*, unsigned int);
extern unsigned char *ut_realloc_1d_uchar (unsigned char*, unsigned int);
extern double *ut_realloc_1d (double*, unsigned int);
extern int** ut_realloc_2d_int_addline (int**, int, int);
extern double** ut_realloc_2d_addline (double**, int, int);
extern double*** ut_realloc_3d_addarray (double***, int, int, int);
extern char** ut_realloc_2d_char_addline (char**, int, int);
extern int** ut_realloc_2d_int_delline (int**, int, int);
extern double** ut_realloc_2d_delline (double**, int, int);

#endif /* UT_ALLOC_H */
