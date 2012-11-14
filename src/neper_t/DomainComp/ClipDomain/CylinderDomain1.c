/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"CylinderDomain.h"

void
CylinderDomain (double *size, struct POLY *pDomain)
{
  int i, j;
  double* cubesize = ut_alloc_1d (7);
  double* plane = ut_alloc_1d (4);
  struct POLYMOD Polymod;

  double rad = size[4] / 2;
  double height = size[2];
  int clipqty = size[6];

  cubesize[1] = 0.5 - 3 * rad;
  cubesize[2] = 0.5 + 3 * rad;
  cubesize[3] = 0.5 - 3 * rad;
  cubesize[4] = 0.5 + 3 * rad;
  cubesize[5] = 0;
  cubesize[6] = height;

  CubeDomain (cubesize, pDomain);

  PolymodAlloc (&Polymod);

  P2Pm ((*pDomain), &Polymod);
  
  // PolymodZeros (IntePara.MaxPolyVerQty, IntePara.MaxPolyFaceQty, &Polymod);

  int* BadVer = NULL;

  double* n0 = ut_alloc_1d (3);
  n0[0] = -1;

  double* r = ol_r_alloc ();
  ol_r_set_this (r, 0, 0, -1);

  double theta;
  double** g = ol_g_alloc ();
  for (i = 0; i < clipqty; i++)
  {
    theta = 360 * ((double) i) / clipqty;
    ol_rtheta_g (r, theta, g);
    ol_g_vect_vect (g, n0, plane + 1);
    plane[0] = 0.4 + (plane[1] * 0.5 + plane[2] * 0.5 + plane[3] * 0.5);

    BadVer = ut_alloc_1d_int (Polymod.VerQty + 1);
    
    for (j = 1; j <= Polymod.VerQty; j++)
      if (Polymod.VerUse[j] == 1)
	if (ut_space_planeside (plane, Polymod.VerCoo[j] - 1) == 1)
	  BadVer[++BadVer[0]] = j;

    if (BadVer[0] != 0)
      PolyhedronModification (-7 - i, plane, &Polymod, BadVer);

    ut_free_1d_int (BadVer);
  }

  // neut_debug_polymod (stdout, Polymod);

  // PolyFree (pDomain - 1, 1);

  // neut_debug_polymod (stdout, Polymod);
  PolyhedronRecording (0, Polymod, pDomain);
  // neut_debug_poly (stdout, *pDomain);

  PolymodFree (&Polymod);

  ut_free_1d (cubesize);
  ut_free_1d (plane);

  return;
}
