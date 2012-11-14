/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Connectivity.h"

void
PrintConnectivity (struct IN In, struct TESS Tess)
{
  FILE* file = NULL;
  int i, j, k, face, neighpoly;
  double area;
  double* eq = ut_alloc_1d (4);
  
  file = ut_file_open (In.neigh, "w");

  for (i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, "%d %d ", i, Tess.PolyFaceQty[i]);
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
    {
      face = Tess.PolyFaceNb[i][j]; 
      neighpoly = (Tess.FacePoly[face][0] != i) ?
	           Tess.FacePoly[face][0] : Tess.FacePoly[face][1];
      fprintf (file, "%d ", neighpoly);
    }
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
    {
      face = Tess.PolyFaceNb[i][j]; 
      neut_tess_face_area (Tess, face, &area);
      fprintf (file, "%.12f ", area);
    }
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
    {
      face = Tess.PolyFaceNb[i][j]; 
      ut_array_1d_memcpy (eq, 4, Tess.FaceEq[face]);
      if (Tess.PolyFaceOri[i][j] == -1)
      ut_array_1d_scale (eq, 4, -1);
      for (k = 0; k < 4; k++)
	fprintf (file, "%.12f ", eq[k]);
    }
    fprintf (file, "\n");
  }

  ut_file_close (file, In.neigh, "w");
  
  ut_free_1d (eq);

  return;
}
