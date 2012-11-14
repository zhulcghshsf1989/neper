/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"FaceFF.h"

// FFBaryFace builds the geometrical interpolation of the face by
// searching the point used (barycenter or a vertex) and calculating the
// corresponding flatness fault.
double
FFBaryFace (struct GEO *pGeo, int face)
{
  int i, j, k;
  int ver1, ver2, ver3;
  double ff = 180;
  double minff = 180;
  double thisff = 180;
  double** eq   = ut_alloc_2d ((*pGeo).FaceVerQty[face] + 1, 4);
  double* angle = ut_alloc_1d ((*pGeo).FaceVerQty[face] + 1);

  /* for face point on each ver of the face, the fff is calculated:
   * fff = maximum angle between 2 triangular parts of the face (see
   * the method of interpolation of the face).
   */
  for (i = 1; i <= (*pGeo).FaceVerQty[face]; i++)
  {
    thisff = 0;

    /* ver i is the face point. */
    ver1 = (*pGeo).FaceVerNb[face][i];

    for (j = 1; j <= (*pGeo).FaceVerQty[face] - 2; j++)
    {
      ver2 =
	(*pGeo).FaceVerNb[face][ut_num_rotpos
				(1, (*pGeo).FaceVerQty[face], i, j)];
      ver3 =
	(*pGeo).FaceVerNb[face][ut_num_rotpos
				(1, (*pGeo).FaceVerQty[face], i, j + 1)];

      ut_space_points_plane ((*pGeo).VerCoo[ver1], (*pGeo).VerCoo[ver2],
		 (*pGeo).VerCoo[ver3], eq[j]);
    }
    for (j = 1; j <= (*pGeo).FaceVerQty[face] - 2; j++)
      for (k = j + 1; k <= (*pGeo).FaceVerQty[face] - 2; k++)
      {
	ff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
	thisff = ut_num_max (thisff, ff);
      }

    if (thisff < minff)
    {
      minff = thisff;
      (*pGeo).FacePt[face] = i;
    }
  }

  // case of the barycenter 
  
  // calculating coo
  ut_array_1d_zero ((*pGeo).FacePtCoo[face], 3);
  for (i = 1; i <= (*pGeo).FaceVerQty[face]; i++)
  {
    ver1 = (*pGeo).FaceVerNb[face][i];
    ut_array_1d_add ((*pGeo).FacePtCoo[face], (*pGeo).VerCoo[ver1], 3, (*pGeo).FacePtCoo[face]);
  }
  ut_array_1d_scale ((*pGeo).FacePtCoo[face], 3, 1./(*pGeo).FaceVerQty[face]);

  thisff = 0;
  for (j = 1; j <= (*pGeo).FaceVerQty[face]; j++)
  {
    ver2 =
      (*pGeo).FaceVerNb[face][ut_num_rotpos (1, (*pGeo).FaceVerQty[face], i, j)];
    ver3 =
      (*pGeo).FaceVerNb[face][ut_num_rotpos
			      (1, (*pGeo).FaceVerQty[face], i, j + 1)];

    ut_space_points_plane ((*pGeo).FacePtCoo[face], (*pGeo).VerCoo[ver2],
	       (*pGeo).VerCoo[ver3], eq[j]);
  }
  for (j = 1; j <= (*pGeo).FaceVerQty[face]; j++)
    for (k = j + 1; k <= (*pGeo).FaceVerQty[face] - 2; k++)
    {
      ff = ut_vector_angle (eq[j] + 1, eq[k] + 1);
      thisff = ut_num_max (thisff, ff);
    }

  if (thisff < minff)
  {
    (*pGeo).FacePt[face] = 0;
    minff = thisff;
  }
  else
  {
    ver1 = (*pGeo).FaceVerNb[face][(*pGeo).FacePt[face]];

    ut_array_1d_memcpy ((*pGeo).FacePtCoo[face], 3, (*pGeo).VerCoo[ver1]);
  }

  (*pGeo).FaceFF[face] = minff;

  ut_free_2d (eq, (*pGeo).FaceVerQty[face] + 1);
  ut_free_1d (angle);

  return (*pGeo).FaceFF[face];
}
