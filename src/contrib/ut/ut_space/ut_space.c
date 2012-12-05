/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"ut_space.h"
#include"ut.h"

#ifdef HAVE_GSL
#include<gsl/gsl_rng.h>
#include<gsl/gsl_linalg.h>
#endif

double
ut_space_dist2d (double *a, double *b)
{
  return sqrt ((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]));
}

double
ut_space_dist (double *a, double *b)
{
  return sqrt ((b[0] - a[0]) * (b[0] - a[0])
	       + (b[1] - a[1]) * (b[1] - a[1])
	       + (b[2] - a[2]) * (b[2] - a[2]));
}

double
ut_space_dist_int (int *a, int *b)
{
  return sqrt ((double) (b[0] - a[0]) * (b[0] - a[0])
	     + (double) (b[1] - a[1]) * (b[1] - a[1])
	     + (double) (b[2] - a[2]) * (b[2] - a[2]));
}

int
ut_space_quad_convex (double *A, double *B, double *C, double *D)
{
  int res;
  double *S1 = ut_alloc_1d (3);
  double *S2 = ut_alloc_1d (3);
  double *S3 = ut_alloc_1d (3);
  double *S4 = ut_alloc_1d (3);
  double sc1, sc2, sc3, sc4;

  S1[0] = B[0] - A[0];
  S1[1] = B[1] - A[1];

  S2[0] = C[0] - B[0];
  S2[1] = C[1] - B[1];

  S3[0] = D[0] - C[0];
  S3[1] = D[1] - C[1];

  S4[0] = A[0] - D[0];
  S4[1] = A[1] - D[1];

  sc1 = S1[0] * S2[1] - S1[1] * S2[0];
  sc2 = S2[0] * S3[1] - S2[1] * S3[0];
  sc3 = S3[0] * S4[1] - S3[1] * S4[0];
  sc4 = S4[0] * S1[1] - S4[1] * S1[0];

  if ((ut_num_sgn (sc1) == ut_num_sgn (sc2))
      && (ut_num_sgn (sc2) == ut_num_sgn (sc3))
      && (ut_num_sgn (sc3) == ut_num_sgn (sc4)))
    res = 1;
  else
    res = 0;

  ut_free_1d (S1);
  ut_free_1d (S2);
  ut_free_1d (S3);
  ut_free_1d (S4);

  return res;
}

int
ut_space_quad_convex_int (int *A, int *B, int *C, int *D)
{
  int res;
  int *S1;
  int *S2;
  int *S3;
  int *S4;
  int sc1, sc2, sc3, sc4;

  /*
     if ( (A[0] == B[0] && A[1] == B[1]) || (A[0] == C[0] && A[1] == C[1])
     || (A[0] == D[0] && A[1] == D[1]) || (B[0] == C[0] && B[1] == C[1])
     || (B[0] == D[0] && B[1] == D[1]) || (C[0] == D[0] && C[1] == D[1]))
     return 1;
     else if ( (A[0] == C[0] && A[1] == C[1])
     ||  (B[0] == D[0] && B[1] == D[1]) )
     return 0;
   */

  S1 = ut_alloc_1d_int (3);
  S2 = ut_alloc_1d_int (3);
  S3 = ut_alloc_1d_int (3);
  S4 = ut_alloc_1d_int (3);

  S1[0] = B[0] - A[0];
  S1[1] = B[1] - A[1];

  S2[0] = C[0] - B[0];
  S2[1] = C[1] - B[1];

  S3[0] = D[0] - C[0];
  S3[1] = D[1] - C[1];

  S4[0] = A[0] - D[0];
  S4[1] = A[1] - D[1];

  sc1 = S1[0] * S2[1] - S1[1] * S2[0];
  sc2 = S2[0] * S3[1] - S2[1] * S3[0];
  sc3 = S3[0] * S4[1] - S3[1] * S4[0];
  sc4 = S4[0] * S1[1] - S4[1] * S1[0];

  if ((ut_num_sgn_int (sc1) == ut_num_sgn_int (sc2))
      && (ut_num_sgn_int (sc2) == ut_num_sgn_int (sc3))
      && (ut_num_sgn_int (sc3) == ut_num_sgn_int (sc4)))
    res = 1;
  else
    res = 0;

  ut_free_1d_int (S1);
  ut_free_1d_int (S2);
  ut_free_1d_int (S3);
  ut_free_1d_int (S4);

  return res;
}

void
ut_space_bary2d (double **P, int qty, double *B)
{
  int i, j;
  double tmp;

  for (i = 0; i < 2; i++)
    {
      tmp = 0;
      for (j = 0; j < qty; j++)
	tmp += P[j][i];

      B[i] = tmp / (double) qty;
    }

  return;
}

void
ut_space_bary2d_int (int **P, int qty, int *B)
{
  int i, j;
  int tmp;

  for (i = 0; i < 2; i++)
    {
      tmp = 0;
      for (j = 0; j < qty; j++)
	tmp += P[j][i];

      B[i] = ut_num_d2ri ((double) tmp / (double) qty);
    }

  return;
}

int
ut_space_indomain2d (double **P, int qty, double x, double y)
{
  int i, interqty, res;
  double *bary = ut_alloc_1d (2);
  double *A = ut_alloc_1d (2);

  A[0] = x;
  A[1] = y;

  ut_space_bary2d (P, qty, bary);

  interqty = 0;
  for (i = 0; i < qty - 1; i++)
    interqty += ut_space_quad_convex (P[i], bary, P[i + 1], A);
  interqty += ut_space_quad_convex (P[qty - 1], bary, P[0], A);

  if (ut_num_even (interqty))
    res = 1;
  else
    res = 0;

  ut_free_1d (bary);
  ut_free_1d (A);

  return res;
}

int
ut_space_indomain2d_int (int **P, int qty, int *A)
{
  int i, interqty, res;
  int *bary = ut_alloc_1d_int (2);

  ut_space_bary2d_int (P, qty, bary);

  interqty = 0;
  for (i = 0; i < qty - 1; i++)
    interqty += ut_space_quad_convex_int (P[i], bary, P[i + 1], A);
  interqty += ut_space_quad_convex_int (P[qty - 1], bary, P[0], A);

  if (ut_num_even (interqty))
    res = 1;
  else
    res = 0;

  ut_free_1d_int (bary);

  return res;
}

/* indicates the half-space whose a specified point belongs to. */
int
ut_space_planeside (double *P, double *C)
{
  if (P[1] * C[1] + P[2] * C[2] + P[3] * C[3] - P[0] < 0)
    return -1;
  else
    return 1;
}

/* Side indicates the half-space whose a specified point belongs to. */
int
ut_space_planeside_tol (double *P, double *C, double eps)
{
  if (P[1] * C[1] + P[2] * C[2] + P[3] * C[3] - P[0] < eps)
    return -1;
  else
    return 1;
}

/* Side2d indicates the half-space whose a specified point belongs to (2d). */
int
ut_space_planeside_2d (double *P, double *C)
{
  if (P[1] * C[1] + P[2] * C[2] - P[0] < 0)
    return -1;
  else
    return 1;
}


/* ut_space_trianglearea area returns the area of the triangle
 * whose vertex coordinates are specified as input data:
 * vertex i: coo[i][1,2,3] (i=1,2,3)
 * The used formula is:
 * A = || (P2-P1) vect. (P3-P1) || /2
 */
double
ut_space_trianglearea (double *p1, double *p2, double *p3)
{
  int i;
  double res;
  double *tmp1 = ut_alloc_1d (3);
  double *tmp2 = ut_alloc_1d (3);
  double *tmp3 = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    {
      tmp1[i] = p2[i] - p1[i];
      tmp2[i] = p3[i] - p1[i];
    }
  ut_vector_vectprod (tmp1, tmp2, tmp3);

  res = 0.5 * ut_vector_norm (tmp3);

  ut_free_1d (tmp1);
  ut_free_1d (tmp2);
  ut_free_1d (tmp3);

  return res;
}

void
ut_space_trianglenormal (double *p1, double *p2, double *p3, double *n)
{
  int i;
  double *a = ut_alloc_1d (4);
  double *b = ut_alloc_1d (4);
  double norm;

  for (i = 0; i < 3; i++)
    {
      a[i] = p2[i] - p1[i];
      b[i] = p3[i] - p2[i];
    }
  ut_vector_vectprod (a, b, n);
  norm = ut_vector_norm (n);

  for (i = 0; i < 3; i++)
    n[i] /= norm;

  ut_free_1d (a);
  ut_free_1d (b);

  return;
}

int
ut_space_pointaligned (double *p1, double *p2, double *p3, double eps)
{
  int i, res;
  double angle;
  double *v1 = NULL;
  double *v2 = NULL;

  if (ut_array_1d_requal (p1, p2, 3, 1e-6) == 1
   || ut_array_1d_requal (p2, p3, 3, 1e-6) == 1
   || ut_array_1d_requal (p1, p3, 3, 1e-6) == 1)
    return 1;

  v1 = ut_alloc_1d (3);
  v2 = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    v1[i] = p2[i] - p1[i];

  for (i = 0; i < 3; i++)
    v2[i] = p3[i] - p1[i];

  angle = ut_vector_angle (v1, v2);

  res = (angle < eps) ? 1 : 0;

  ut_free_1d (v1);
  ut_free_1d (v2);

  return res;
}

void
ut_space_arrangepoints (double **pts, int qty, double **box, int size,
			int *sequence)
{
  int i, x, y, z;
  double *cl = ut_alloc_1d (3);
  double *BoxCentre = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    cl[i] = (box[i][1] - box[i][0]) / size;

  /* for every cubic box, looking for the elset in it */
  for (z = 0; z < size; z++)
    {
      BoxCentre[2] = box[2][0] + (box[2][1] - box[2][0]) * (z + 0.5) / size;

      for (y = 0; y < size; y++)
	{
	  BoxCentre[1] = box[1][0] +
	    (box[1][1] - box[1][0]) * (y + 0.5) / size;

	  for (x = 0; x < size; x++)
	    {
	      BoxCentre[0] = box[0][0] +
		(box[0][1] - box[0][0]) * (x + 0.5) / size;

	      for (i = 0; i < qty; i++)
		{
		  if (ut_array_1d_int_eltpos (sequence + 1, sequence[0], i) ==
		      -1)
		    if (fabs (pts[i][0] - BoxCentre[0]) <= cl[0]
			&& fabs (pts[i][1] - BoxCentre[1]) <= cl[1]
			&& fabs (pts[i][2] - BoxCentre[2]) <= cl[2])
		      sequence[++sequence[0]] = i;
		}
	    }
	}
    }

  if (sequence[0] != qty)
    {
      printf ("Bad grain sequence for renumbering");
      printf ("sequence[0] = %d != qty = %d\n", sequence[0], qty);
      abort ();
    }

  ut_free_1d (cl);
  ut_free_1d (BoxCentre);

  return;
}

double
ut_space_triangle_area (double *p1, double *p2, double *p3)
{
  int i;
  double res;
  double *tmp1 = ut_alloc_1d (3);
  double *tmp2 = ut_alloc_1d (3);
  double *tmp3 = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    {
      tmp1[i] = p2[i] - p1[i];
      tmp2[i] = p3[i] - p1[i];
    }
  ut_vector_vectprod (tmp1, tmp2, tmp3);

  res = 0.5 * ut_vector_norm (tmp3);

  ut_free_1d (tmp1);
  ut_free_1d (tmp2);
  ut_free_1d (tmp3);

  return res;
}

int
ut_space_tet_centre (double *p1, double *p2, double *p3, double *p4, double* c)
{
  int i;

  for (i = 0; i < 3; i++)
    c[i] = (p1[i] + p2[i] + p3[i] + p4[i]) / 4;

  return 0;
}

/* V = norm ( (p1 - p4) . [(p2 - p4) x (p3 - p4)] )/ 6 */
double
ut_space_tet_volume (double *p1, double *p2, double *p3, double *p4)
{
  int i;
  double V;
  double *a = ut_alloc_1d (3);
  double *b = ut_alloc_1d (3);
  double *c = ut_alloc_1d (3);
  double *v = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
    {
      a[i] = p1[i] - p4[i];
      b[i] = p2[i] - p4[i];
      c[i] = p3[i] - p4[i];
    }

  ut_vector_vectprod (b, c, v);
  V = 0.166666666666666666666666667 * ut_vector_scalprod (a, v);

  ut_free_1d (a);
  ut_free_1d (b);
  ut_free_1d (c);
  ut_free_1d (v);

  return (V >= 0) ? V : -V;
}

double
ut_space_hexa_volume (double *p0, double *p1, double *p2, double *p3,
                      double *p4, double *p5, double *p6, double *p7)
{
  double V;
  double* center = ut_alloc_1d (3);

  ut_space_hexa_center (p0, p1, p2, p3, p4, p5, p6, p7, center);

  V = 0;

  V += ut_space_tet_volume (center, p0, p5, p1);
  V += ut_space_tet_volume (center, p0, p5, p4);
  V += ut_space_tet_volume (center, p3, p6, p2);
  V += ut_space_tet_volume (center, p3, p6, p7);

  V += ut_space_tet_volume (center, p1, p6, p2);
  V += ut_space_tet_volume (center, p1, p6, p5);
  V += ut_space_tet_volume (center, p3, p4, p0);
  V += ut_space_tet_volume (center, p3, p4, p7);

  V += ut_space_tet_volume (center, p4, p6, p5);
  V += ut_space_tet_volume (center, p4, p6, p7);
  V += ut_space_tet_volume (center, p0, p2, p1);
  V += ut_space_tet_volume (center, p0, p2, p3);

  ut_free_1d (center);

  return (V >= 0) ? V : -V;
}

int
ut_space_hexa_center (double *p0, double *p1, double *p2, double *p3,
                      double *p4, double *p5, double *p6, double *p7,
		      double* center)
{
  int i;

  for (i = 0; i < 3; i++)
    center[i] = p0[i] + p1[i] + p2[i] + p3[i]
              + p4[i] + p5[i] + p6[i] + p7[i];

  ut_array_1d_scale (center, 3, 0.125);

  return 0;
}

#ifdef HAVE_GSL
/* Projection of a point along a vector onto a plane. */
void
ut_space_projpoint_alongonto (double *coo, double *n, double *plane)
{
  int i;
  int s;
  gsl_matrix *A = gsl_matrix_alloc (4, 4);
  gsl_vector *B = gsl_vector_alloc (4);
  gsl_vector *X = gsl_vector_alloc (4);
  gsl_permutation *p = gsl_permutation_alloc (4);

  /* Projection of any point M (X,Y,Z) along n(a',b',c') onto
   * plane P (ax+by+cz-d=0), denoted as M' (X',Y',Z') is given
   * by the following matrix system:
   * A.X=B with
   * ~ - -
   * A = | 1 0 0 a' |; X = |X'|; B = |X|. (k is a distance parameter.)
   *     | 0 1 0 b' |      |Y'|      |Y|  the 3 first lines means M'M
   *     | 0 0 1 c' |      |Z'|      |Z|  the 4th one means M' belongs to P.
   *     | a b c 0  |      |k |      |d|  

   * recording A, which does no depend upon ver, but which is modified by
   * gsl_linalg_LU_... . */
  gsl_matrix_set (A, 0, 0, 1);
  gsl_matrix_set (A, 0, 1, 0);
  gsl_matrix_set (A, 0, 2, 0);
  gsl_matrix_set (A, 0, 3, n[0]);
  gsl_vector_set (B, 0, coo[0]);

  gsl_matrix_set (A, 1, 0, 0);
  gsl_matrix_set (A, 1, 1, 1);
  gsl_matrix_set (A, 1, 2, 0);
  gsl_matrix_set (A, 1, 3, n[1]);
  gsl_vector_set (B, 1, coo[1]);

  gsl_matrix_set (A, 2, 0, 0);
  gsl_matrix_set (A, 2, 1, 0);
  gsl_matrix_set (A, 2, 2, 1);
  gsl_matrix_set (A, 2, 3, n[2]);
  gsl_vector_set (B, 2, coo[2]);

  gsl_matrix_set (A, 3, 3, 0);
  gsl_matrix_set (A, 3, 0, plane[1]);
  gsl_matrix_set (A, 3, 1, plane[2]);
  gsl_matrix_set (A, 3, 2, plane[3]);
  gsl_vector_set (B, 3, plane[0]);

  gsl_linalg_LU_decomp (A, p, &s);
  if (fabs (gsl_linalg_LU_det (A, s)) < 1e-15)
    {
      printf ("det=%.20f\n", gsl_linalg_LU_det (A, s));
      printf ("ProjectPointAlongOnto: det=0\n");
      abort ();
    }

  gsl_linalg_LU_solve (A, p, B, X);

  for (i = 0; i <= 2; i++)
    coo[i] = gsl_vector_get (X, i);

  gsl_matrix_free (A);
  gsl_vector_free (B);
  gsl_vector_free (X);
  gsl_permutation_free (p);

  return;
}
#endif

void
ut_space_points_plane (double *p1, double *p2, double *p3, double *eq)
{
  int k;
  double norm;
  double *u = ut_alloc_1d (3);
  double *v = ut_alloc_1d (3);

  /* determining normal */
  for (k = 0; k < 3; k++)
    {
      u[k] = p2[k] - p1[k];
      v[k] = p3[k] - p2[k];
    }
  ut_vector_vectprod (u, v, eq + 1);
  norm = ut_vector_norm (eq + 1);
  ut_array_1d_scale (eq + 1, 3, 1. / norm);

  eq[0] = 0;
  /* determining constant */
  for (k = 1; k <= 3; k++)
    eq[0] += eq[k] * p1[k - 1];

  ut_free_1d (u);
  ut_free_1d (v);

  return;
}

void
ut_space_points_invect_plane (double *p1, double *p2, double *invect,
			      double *eq)
{
  int i;
  double *p3 = ut_alloc_1d (3);
  double dist = ut_space_dist (p1, p2);
  // dist to make the distance between points quite the same

  for (i = 0; i < 3; i++)
    p3[i] = p2[i] + invect[i] * dist;

  ut_space_points_plane (p1, p2, p3, eq);

  ut_free_1d (p3);

  return;
}

/*
void
ut_space_points_invect_plane (double *p1, double *p2, double *invect, double *eq)
{
  // int i, j;
  double norm;

  int s;
  gsl_matrix *A = gsl_matrix_alloc (4, 4);
  gsl_vector *B = gsl_vector_alloc (4);
  gsl_vector *X = gsl_vector_alloc (4);
  gsl_permutation *p = gsl_permutation_alloc (4);

  gsl_matrix_set (A, 0, 0, invect[0]);
  gsl_matrix_set (A, 0, 1, invect[1]);
  gsl_matrix_set (A, 0, 2, invect[2]);
  gsl_matrix_set (A, 0, 3, 0);
  gsl_vector_set (B, 0, 0);

  gsl_matrix_set (A, 1, 0, p1[0]);
  gsl_matrix_set (A, 1, 1, p1[1]);
  gsl_matrix_set (A, 1, 2, p1[2]);
  gsl_matrix_set (A, 1, 3, -1);
  gsl_vector_set (B, 1, 0);

  gsl_matrix_set (A, 2, 0, p2[0]);
  gsl_matrix_set (A, 2, 1, p2[1]);
  gsl_matrix_set (A, 2, 2, p2[2]);
  gsl_matrix_set (A, 2, 3, -1);
  gsl_vector_set (B, 2, 0);

  gsl_matrix_set (A, 3, 0, 1);
  gsl_matrix_set (A, 3, 1, 1);
  gsl_matrix_set (A, 3, 2, 1);
  gsl_matrix_set (A, 3, 3, 0);
  gsl_vector_set (B, 3, 1);

  gsl_linalg_LU_decomp (A, p, &s);

  if (fabs (gsl_linalg_LU_det (A, s)) < 1e-15)
  {
    gsl_matrix_set (A, 3, 0, 1);
    gsl_matrix_set (A, 3, 1, 0);
    gsl_matrix_set (A, 3, 2, 0);
    gsl_linalg_LU_decomp (A, p, &s);
    if (fabs (gsl_linalg_LU_det (A, s)) < 1e-15)
    {
      gsl_matrix_set (A, 3, 0, 0);
      gsl_matrix_set (A, 3, 1, 1);
      gsl_matrix_set (A, 3, 2, 0);
      gsl_linalg_LU_decomp (A, p, &s);
      if (fabs (gsl_linalg_LU_det (A, s)) < 1e-15)
      {
	gsl_matrix_set (A, 3, 0, 0);
	gsl_matrix_set (A, 3, 1, 0);
	gsl_matrix_set (A, 3, 2, 1);
	gsl_linalg_LU_decomp (A, p, &s);
	if (fabs (gsl_linalg_LU_det (A, s)) < 1e-15)
	{
	  abort ();
	}
      }
    }
  }

  gsl_linalg_LU_solve (A, p, B, X);

  norm = sqrt (pow (gsl_vector_get (X, 0), 2) +
	       pow (gsl_vector_get (X, 1), 2) +
	       pow (gsl_vector_get (X, 2), 2));

  gsl_vector_scale (X, 1 / norm);

  eq[0] = gsl_vector_get (X, 3);
  eq[1] = gsl_vector_get (X, 0);
  eq[2] = gsl_vector_get (X, 1);
  eq[3] = gsl_vector_get (X, 2);

  gsl_matrix_free (A);
  gsl_vector_free (B);
  gsl_vector_free (X);
  gsl_permutation_free (p);

  return;
}
*/

void
ut_space_points_invect_plane_new (double *p1, double *p2, double *invect,
				  double *eq)
{
  int i;
  double norm;
  double *v = ut_alloc_1d (3);

  // determining plane normal by cross product of invect and the vector
  // based on p1 and p2
  for (i = 0; i < 3; i++)
    v[i] = p2[i] - p1[i];

  ut_vector_vectprod (invect, v, eq + 1);
  norm = ut_vector_norm (eq + 1);
  ut_array_1d_scale (eq + 1, 3, norm);

  // determining constant by using the first point
  eq[0] = -(p1[0] * eq[0] + p1[1] * eq[1] + p1[2] * eq[2]);

  ut_free_1d (v);

  return;
}

// returns 1 if P is in the triangle based on p1, p2 and p3
// all points must be in the same plane.
int
ut_space_triangle_point_in (double *p1, double *p2, double *p3, double *P)
{
  double res, eps;
  double **eq = ut_alloc_2d (4, 4);

  ut_space_points_plane (p1, p2, p3, eq[0]);
  eps = ut_space_triangle_area (p1, p2, p3);
  eps = 1e-6 * sqrt (eps);

  // calculating planes normal to the triangle passing throught the
  // triangle edge, w outgoing normal
  ut_space_points_invect_plane (p1, p2, eq[0] + 1, eq[1]);
  if (ut_space_planeside (eq[1], p3 - 1) == 1)
    ut_array_1d_scale (eq[1], 4, -1);

  // printf ("eq1: ");
  // ut_array_1d_fprintf (stdout, eq[1], 4, "%f");

  ut_space_points_invect_plane (p2, p3, eq[0] + 1, eq[2]);
  if (ut_space_planeside (eq[2], p1 - 1) == 1)
    ut_array_1d_scale (eq[2], 4, -1);
  // printf ("eq2: ");
  // ut_array_1d_fprintf (stdout, eq[2], 4, "%f");

  ut_space_points_invect_plane (p3, p1, eq[0] + 1, eq[3]);
  if (ut_space_planeside (eq[3], p2 - 1) == 1)
    ut_array_1d_scale (eq[3], 4, -1);

  // printf ("eq3: ");
  // ut_array_1d_fprintf (stdout, eq[3], 4, "%f");

  // printf ("side1 = %d\n", ut_space_planeside_tol (eq[1], P - 1, eps));
  // printf ("side2 = %d\n", ut_space_planeside_tol (eq[2], P - 1, eps));
  // printf ("side3 = %d\n", ut_space_planeside_tol (eq[3], P - 1, eps));

  res = 0;
  if (ut_space_planeside_tol (eq[1], P - 1, eps) == -1
      && ut_space_planeside_tol (eq[2], P - 1, eps) == -1
      && ut_space_planeside_tol (eq[3], P - 1, eps) == -1)
    res = 1;

  ut_free_2d (eq, 4);

  return res;
}

// returns 1 if P is in the tet based on p1, p2, p3 and p4
// all points must be in the same plane.
int
ut_space_tet_point_in (double *p1, double *p2, double *p3, double *p4,
		       double *P)
{
  double res;
  double *bary = ut_alloc_1d (3);
  double **eq = ut_alloc_2d (4, 4);

  res = 0;

  // testing vertices
  if (ut_array_1d_requal (p1, P, 3, 1e-6) == 1
   || ut_array_1d_requal (p2, P, 3, 1e-6) == 1
   || ut_array_1d_requal (p3, P, 3, 1e-6) == 1
   || ut_array_1d_requal (p4, P, 3, 1e-6) == 1)
    res = 1;
  // testing edges
  else if (ut_space_segment_point_in (p1, p2, P) == 1
        || ut_space_segment_point_in (p2, p3, P) == 1
        || ut_space_segment_point_in (p3, p4, P) == 1
        || ut_space_segment_point_in (p1, p4, P) == 1)
    res = 1;
  // testing body
  else
  {
    // calculating plane eqs and bary
    ut_space_points_plane (p1, p2, p3, eq[0]);
    ut_space_points_plane (p2, p1, p4, eq[1]);
    ut_space_points_plane (p3, p4, p2, eq[2]);
    ut_space_points_plane (p4, p3, p1, eq[3]);

    ut_array_1d_zero (bary, 3);
    ut_array_1d_add (bary, p1, 3, bary);
    ut_array_1d_add (bary, p2, 3, bary);
    ut_array_1d_add (bary, p3, 3, bary);
    ut_array_1d_add (bary, p4, 3, bary);
    ut_array_1d_scale (bary, 3, 0.25);

    // checking if the bary and the point are of the same side of each
    // plane
    if ((ut_space_planeside (eq[0], P - 1) ==
	 ut_space_planeside (eq[0], bary - 1))
	&& (ut_space_planeside (eq[1], P - 1) ==
	    ut_space_planeside (eq[1], bary - 1))
	&& (ut_space_planeside (eq[2], P - 1) ==
	    ut_space_planeside (eq[2], bary - 1))
	&& (ut_space_planeside (eq[3], P - 1) ==
	    ut_space_planeside (eq[3], bary - 1)))
      res = 1;
  }

  ut_free_2d (eq, 4);
  ut_free_1d (bary);

  return res;
}

// returns 1 if [AB] and [CD] intersect, 0 otherwise.  the 4 points must
// be in the same plane.
int
ut_space_segments_intersect (double* A, double* B, double* C, double* D)
{
  int res;
  double* P  = ut_alloc_1d (4);
  double* P2 = ut_alloc_1d (4);

  res = -1;

  // Checking for C and/or D in [AB]
  if (ut_space_segment_point_in (A, B, C) == 1
   || ut_space_segment_point_in (A, B, D) == 1)
    res = 1;

  // Checking for A and/or B in [CD] (does not duplicate the previous one)
  else if (ut_space_segment_point_in (C, D, A) == 1
   || ut_space_segment_point_in (C, D, B) == 1)
    res = 1;

  // Checking for C aligned with [AB] (but not in it, due to previous)
  else if (ut_space_points_aligned (A, B, C))
    res = 0;

  // Checking for D aligned with [AB] (but not in it, due to previous)
  else if (ut_space_points_aligned (A, B, D))
    res = 0;


  if (res == -1)
  {
    // At this point, we know the points are not aligned in any way...
  
    // Checking that the A B C D are coplanar.
    ut_space_points_plane (A, B, C, P);
    ut_space_points_plane (A, B, D, P2);

    if (ut_space_planes_equal (P, P2) == 0)
    {
      printf ("Points are not in the same plane!\n");
      ut_array_1d_fprintf (stdout, A, 3, "%f");
      ut_array_1d_fprintf (stdout, B, 3, "%f");
      ut_array_1d_fprintf (stdout, C, 3, "%f");
      ut_array_1d_fprintf (stdout, D, 3, "%f");
      ut_array_1d_fprintf (stdout, P, 4, "%.12f");
      ut_array_1d_fprintf (stdout, P2, 4, "%.12f");
      abort ();
    }

    // Now, general case.
    // Considering the half spaces build on [AB].  Checking if C
    // and D are in the same. If yes, [AB] and [CD] are not
    // intersecting.  Else, considering the half spaces build on [CD].
    // Checking if A and B are in the same.  If yes 
    //
    ut_space_points_invect_plane (A, B, P + 1, P2);
    if (ut_space_planeside (P2, C - 1) ==  ut_space_planeside (P2, D - 1))
      res = 0;
    else
    {
      ut_space_points_invect_plane (C, D, P + 1, P2);
      if (ut_space_planeside (P2, A - 1) ==  ut_space_planeside (P2, B - 1))
	res = 0;
      else
	res = 1;
    }
  }

  if (res == -1)
  {
    printf ("res should not be -1\n");
    abort ();
  }

  ut_free_1d (P);
  ut_free_1d (P2);

  return res;
}

// returns 1 if A B C are aligned, 0 else.
int
ut_space_points_aligned (double* A, double* B, double* C)
{
  if (ut_space_points_aligned2 (A, B, C) != 0)
    return 1;
  else
    return 0;
}

// returns 1 if A B C are aligned and A>B>C, -1 if aligned otherwise, 0 else.
int
ut_space_points_aligned2 (double* A, double* B, double* C)
{
  int i, res;
  double dist, angle, scal;
  double* ab = ut_alloc_1d (3);
  double* ac = ut_alloc_1d (3);

  dist = ut_space_dist (A, B);
  for (i = 0; i < 3; i++)
    ab[i] = (B[i] - A[i]) / dist;

  for (i = 0; i < 3; i++)
    ac[i] = (C[i] - A[i]) / dist;

  angle = ut_vector_angle (ab, ac);
  if (ut_num_equal (angle,   0, 1e-3) != 1
   && ut_num_equal (angle, 180, 1e-3) != 1)
    res = 0;
  else
  {
    scal = ut_vector_scalprod (ab, ac);
    if (scal > 1)
      res = 1;
    else
      res = -1;
  }

  ut_free_1d (ab);
  ut_free_1d (ac);

  return res;
}

// returns 1 if C is in [AB]
int
ut_space_segment_point_in (double* A, double* B, double* C)
{
  int i, res;
  double dist, angle, scal;
  double* ab = ut_alloc_1d (3);
  double* ac = ut_alloc_1d (3);

  dist = ut_space_dist (A, B);

  for (i = 0; i < 3; i++)
    ab[i] = (B[i] - A[i]) / dist;

  for (i = 0; i < 3; i++)
    ac[i] = (C[i] - A[i]) / dist;

  angle = ut_vector_angle (ab, ac);
  if (ut_num_equal (angle,   0, 1e-6) != 1
   && ut_num_equal (angle, 180, 1e-6) != 1)
    res = 0;
  else
  {
    scal = ut_vector_scalprod (ab, ac);
    if (scal >= 0 && scal <= 1)
      res = 1;
    else
      res = 0;
  }

  ut_free_1d (ab);
  ut_free_1d (ac);

  return res;
}

int
ut_space_planes_equal (double* P1, double* P2)
{
  int res;
  double angle, norm;
  double* P1cpy = ut_alloc_1d (4);
  double* P2cpy = ut_alloc_1d (4);

  ut_array_1d_memcpy (P1cpy, 4, P1);
  norm = ut_array_1d_norm (P1cpy, 4);
  ut_array_1d_scale (P1cpy, 4, 1. / norm);

  ut_array_1d_memcpy (P2cpy, 4, P2);
  norm = ut_array_1d_norm (P2cpy, 4);
  ut_array_1d_scale (P2cpy, 4, 1. / norm);

  angle = ut_vector_angle (P1cpy + 1, P2cpy + 1);
  if (ut_num_equal (angle, 0, 1e-3))
  {
    if (ut_num_equal (P1cpy[0], P2cpy[0], 1e-6))
      res = 1;
    else
      res = 0;
  }
  else if (ut_num_equal (angle, 180, 1e-3))
  {
    if (ut_num_equal (P1cpy[0], -P2cpy[0], 1e-6))
      res = 1;
    else
      res = 0;
  }
  else
    res = 0;

  ut_free_1d (P1cpy);
  ut_free_1d (P2cpy);

  return res;
}

/*
int
ut_space_planes_equal (double* P1, double* P2)
{
  int res;
  int idx;
  double norm;
  double* P1cpy = ut_alloc_1d (4);
  double* P2cpy = ut_alloc_1d (4);

  ut_array_1d_memcpy (P1cpy, 4, P1);
  norm = ut_array_1d_norm (P1cpy, 4);
  ut_array_1d_scale (P1cpy, 4, 1. / norm);
  idx = ut_array_1d_absmax (P1cpy + 1, 3);
  if (P1cpy[idx + 1] < 0)
    ut_array_1d_scale (P1cpy, 4, -1);

  ut_array_1d_memcpy (P2cpy, 4, P2);
  norm = ut_array_1d_norm (P2cpy, 4);
  ut_array_1d_scale (P2cpy, 4, 1. / norm);
  idx = ut_array_1d_absmax (P2cpy + 1, 3);
  if (P2cpy[idx + 1] < 0)
    ut_array_1d_scale (P2cpy, 4, -1);
    
  res = ut_array_1d_requal (P1cpy, P2cpy, 4, 1e-6);
  printf ("P1 = %2.0f %2.0f %2.0f %2.0f ", P1[0], P1[1], P1[2], P1[3]);
  printf ("P2 = %2.0f %2.0f %2.0f %2.0f ", P2[0], P2[1], P2[2], P2[3]);
  if (res == 0)
    printf ("are not equal\n");
  else
    printf ("are equal\n");

  ut_free_1d (P1cpy);
  ut_free_1d (P2cpy);

  return res;
}
*/

void
ut_space_points_covarmatrix (double** P, double* w, int dim, int size, double** evect, double* eval)
{
  int i, j, k;
  double sumw;
  double** ERR = ut_alloc_2d (dim, dim);
  double*  ER  = ut_alloc_1d (dim);
  double** S   = ut_alloc_2d (dim, dim);

  if (w != NULL)
  {
    sumw = ut_array_1d_sum (w, size);
    for (i = 0; i < dim; i++)
      for (j = 0; j < dim; j++)
      {
	for (k = 0; k < size; k++)
	  ERR[i][j] += w[k] * P[k][i] * P[k][j];

	ERR[i][j] /= sumw;
      }

    for (i = 0; i < dim; i++)
    {
      for (k = 0; k < size; k++)
	ER[i] += w[k] * P[k][i];

      ER[i] /= sumw;
    }
  }
  else
  {
    for (i = 0; i < dim; i++)
      for (j = 0; j < dim; j++)
      {
	for (k = 0; k < size; k++)
	  ERR[i][j] += P[k][i] * P[k][j];

	ERR[i][j] /= (double) size;
      }

    for (i = 0; i < dim; i++)
    {
      for (k = 0; k < size; k++)
	ER[i] += P[k][i];

      ER[i] /= (double) size;
    }
  }

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      S[i][j] = ERR[i][j] - ER[i] * ER[j];

  /* calculation of the eigen values and vectors of S */

  ut_mat_eigen (dim, S, eval, evect);

  ut_free_2d (ERR, dim);
  ut_free_1d (ER);
  ut_free_2d (S, dim);

  return;
}

int
ut_space_contour_intersect (double** coo, int ptqty)
{
  int i, j, res, last, I, J;

  // Consider there are 6 elements.
  // elt 1: checking 3-4-5 (i + 2)
  // elt 2: checking 4-5-6
  // elt 3: checking 5-6
  // elt 4: checking 6
  // elt 5-6: none (2 lasts)
  
  res = 0;
  for (i = 0; i < ptqty - 2; i++)
  {
    last = (i == 0) ? ptqty - 1 : ptqty;
    for (j = i + 2; j < last; j++)
    {
      I = (i + 1 != ptqty)? i + 1 : 0;
      J = (j + 1 != ptqty)? j + 1 : 0;

      res = ut_space_segments_intersect (coo[i], coo[I],
	                                 coo[j], coo[J]);

      if (res == 1)
	break;
    }

    if (res == 1)
      break;
  }

  return res;
}

int
ut_space_points_uvect (double* a, double* b, double* uvect)
{
  int i;

  for (i = 0; i < 3; i++)
    uvect[i] = b[i] - a[i];

  ut_array_1d_scale (uvect, 3, 1./ut_array_1d_norm (uvect, 3));

  return 0;
}

#ifdef HAVE_GSL
int
ut_space_point_plane_dist (double* C, double* P, double* pdist)
{
  double* Cproj = ut_alloc_1d (3);
  
  ut_array_1d_memcpy (Cproj, 3, C);
  ut_space_projpoint_alongonto (Cproj, P + 1, P);
  (*pdist) = ut_space_dist (C, Cproj);

  ut_free_1d (Cproj);

  return 0;
}

// Provides the intersection point between segment [AB] and plane P.
// The results is recorded in I.
int
ut_space_segment_plane_intersect (double* A, double* B, double* P,
                                  double* I)
{
  int i;
  double* AB = ut_alloc_1d (3);

  ut_array_1d_memcpy (I, 3, A);

  for (i = 0; i < 3; i++)
    AB[i] = B[i] - A[i];

  ut_space_projpoint_alongonto (I, AB, P);

  ut_free_1d (AB);

  return 0;
}
#endif

void
ut_space_string_plane (char* string, double* plane)
{
  ut_array_1d_zero (plane, 4);

  if (string[0] == 'x')
    plane[1] = 1;
  else if (string[0] == 'y')
    plane[2] = 1;
  else if (string[0] == 'z')
    plane[3] = 1;
  else
    abort ();

  if (string[1] != '=')
    abort ();

  if (sscanf (string + 2, "%lf", &(plane[0])) != 1)
    abort ();

  return;
}
