/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"FaceFF.h"

// FFBaryFace builds the interpolation of the face by
// searching the point used (barycenter or a vertex) and calculating the
// corresponding flatness fault.
double
FFBaryFace (struct TESS *pTess, int face)
{
  int i, j, k;
  int ver1, ver2, ver3;
  double ff = 180;
  double minff = 180;
  double thisff = 180;
  double** eq   = ut_alloc_2d ((*pTess).FaceVerQty[face] + 1, 4);
  double* angle = ut_alloc_1d ((*pTess).FaceVerQty[face] + 1);

  /* for face point on each ver of the face, the fff is calculated:
   * fff = maximum angle between 2 triangular parts of the face (see
   * the method of interpolation of the face).
   */
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    thisff = 0;

    /* ver i is the face point. */
    ver1 = (*pTess).FaceVerNb[face][i];

    for (j = 1; j <= (*pTess).FaceVerQty[face] - 2; j++)
    {
      ver2 =
	(*pTess).FaceVerNb[face][ut_num_rotpos
				(1, (*pTess).FaceVerQty[face], i, j)];
      ver3 =
	(*pTess).FaceVerNb[face][ut_num_rotpos
				(1, (*pTess).FaceVerQty[face], i, j + 1)];

      ut_space_points_plane ((*pTess).VerCoo[ver1], (*pTess).VerCoo[ver2],
		 (*pTess).VerCoo[ver3], eq[j]);
    }
    for (j = 1; j <= (*pTess).FaceVerQty[face] - 2; j++)
      for (k = j + 1; k <= (*pTess).FaceVerQty[face] - 2; k++)
      {
	ff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
	thisff = ut_num_max (thisff, ff);
      }

    if (thisff < minff)
    {
      minff = thisff;
      (*pTess).FacePt[face] = i;
    }
  }

  // case of the barycenter 
  
  // calculating coo
  ut_array_1d_zero ((*pTess).FacePtCoo[face], 3);
  for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
  {
    ver1 = (*pTess).FaceVerNb[face][i];
    ut_array_1d_add ((*pTess).FacePtCoo[face], (*pTess).VerCoo[ver1], 3, (*pTess).FacePtCoo[face]);
  }
  ut_array_1d_scale ((*pTess).FacePtCoo[face], 3, 1./(*pTess).FaceVerQty[face]);

  thisff = 0;
  for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
  {
    ver2 =
      (*pTess).FaceVerNb[face][ut_num_rotpos (1, (*pTess).FaceVerQty[face], i, j)];
    ver3 =
      (*pTess).FaceVerNb[face][ut_num_rotpos
			      (1, (*pTess).FaceVerQty[face], i, j + 1)];

    ut_space_points_plane ((*pTess).FacePtCoo[face], (*pTess).VerCoo[ver2],
	       (*pTess).VerCoo[ver3], eq[j]);
  }
  for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
    for (k = j + 1; k <= (*pTess).FaceVerQty[face] - 2; k++)
    {
      ff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
      thisff = ut_num_max (thisff, ff);
    }

  if (thisff < minff)
  {
    (*pTess).FacePt[face] = 0;
    minff = thisff;
  }
  else
  {
    ver1 = (*pTess).FaceVerNb[face][(*pTess).FacePt[face]];

    ut_array_1d_memcpy ((*pTess).FacePtCoo[face], 3, (*pTess).VerCoo[ver1]);
  }

  (*pTess).FaceFF[face] = minff;

  ut_free_2d (eq, (*pTess).FaceVerQty[face] + 1);
  ut_free_1d (angle);

  return (*pTess).FaceFF[face];
}
