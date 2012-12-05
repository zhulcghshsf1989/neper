/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"ut_vector.h"
#include"ut.h"

double
ut_vector_norm (double *a)
{
  return sqrt (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

double
ut_vector_int_norm (int *a)
{
  return sqrt ((double) a[0] * (double) a[0] + (double) a[1] * (double) a[1] +
	       (double) a[2] * (double) a[2]);
}

double
ut_vector_scalprod (double *a, double *b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void
ut_vector_vectprod (double *a, double *b, double *c)
{
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];

  return;
}

double
ut_vector_angle_rad (double *a, double *b)
{
  double angle;

  angle = ut_vector_scalprod (a, b);
  angle /= ut_vector_norm (a);
  angle /= ut_vector_norm (b);
  angle = ut_num_acos (angle);

  return angle;
}

double
ut_vector_angle (double *a, double *b)
{
  return (180 / M_PI) * ut_vector_angle_rad (a, b);
}

double
ut_vector_int_angle_rad (int *a, int *b)
{
  int i;
  double angle;
  double *a2 = ut_alloc_1d (3);
  double *b2 = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    a2[i] = (double) a[i];

  for (i = 0; i < 3; i++)
    b2[i] = (double) b[i];

  angle = ut_vector_scalprod (a2, b2);
  angle /= ut_vector_norm (a2);
  angle /= ut_vector_norm (b2);
  angle = acos (angle);
  if (angle != angle) // NaN
    angle = 0;

  ut_free_1d (a2);
  ut_free_1d (b2);

  return angle;
}

double
ut_vector_int_angle (int *a, int *b)
{
  return (180 / M_PI) * ut_vector_int_angle_rad (a, b);
}

void
ut_vector_covar (double** R, int qty, double **evect, double *eval)
{
  int i, j, k;
  int maxid;
  int n;

  double **ERR = ut_alloc_2d (3, 3);
  double *ER = ut_alloc_1d (3);
  double **Sb = ut_alloc_2d (3, 3);

  ut_array_2d_zero (evect, 3, 3);
  ut_array_1d_zero (eval , 3);

  if (qty == 0)
  {
    ut_print_message (2, 0, "ut_vector_covar: set of size 0");
    abort ();
  }
  else if (qty == 1)
  {
    evect[0][0] = evect[1][1] = evect[2][2] = 1;
    ut_array_1d_set (eval, 3, 0);
  }
  else
  {
    n = qty;
    /* calculation of the covariant matrix S */

    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
      {
	for (k = 0; k < n; k++)
	  ERR[i][j] += R[k][i] * R[k][j];

	ERR[i][j] /= (double) n;
      }

    for (i = 0; i < 3; i++)
    {
      for (k = 0; k < n; k++)
	ER[i] += R[k][i];

      ER[i] /= (double) n;
    }

    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
	Sb[i][j] = ERR[i][j] - ER[i] * ER[j];

    /* calculation of the eigen values and vectors of S */

    ut_mat_eigen (3, Sb, eval, evect);

    /* the max coo of v0 and v1 are made positive, and v2
     * is (re)calculated as v0 'vect prod' v1 to get a right-handed
     * csys. */

    // ut_mat_transpose (3, evect, evect);

    for (i = 0; i < 3; i++)
    {
      maxid = ut_array_1d_absmax_index (evect[i], 3);
      if (evect[i][maxid] < 0)
	ut_array_1d_scale (evect[i], 3, -1);
    }

    ut_vector_vectprod (evect[0], evect[1], evect[2]);

    // ut_mat_transpose (3, evect, evect);
  }

  return;
}

void
ut_vector_uvect (double* v, double* uvect)
{
  ut_array_1d_memcpy (uvect, 3, v);
  ut_array_1d_scale (uvect, 3, 1./ut_array_1d_norm (uvect, 3));

  return;
}
