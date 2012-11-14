/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DomainComp.h"

void
net_domain_cylinder_planes (double h, double rad, int qty, double** eq)
{
  int i;
  double* n = ut_alloc_1d (3);
  double* r = ol_r_alloc ();
  double theta;
  double** g = ol_g_alloc ();

  eq[0][1] =  0;
  eq[0][2] =  0;
  eq[0][3] = -1;
  eq[0][0] =  0;

  eq[1][1] =  0;
  eq[1][2] =  0;
  eq[1][3] =  1;
  eq[1][0] =  h;

  rad *= cos (M_PI / qty);

  n[0] = -1;
  ol_r_set_this (r, 0, 0, -1);

  for (i = 0; i < qty; i++)
  {
    theta = 360 * ((double) i) / qty;
    ol_rtheta_g (r, theta, g);
    ol_g_vect_vect (g, n, eq[i + 2] + 1);
    eq[i + 2][0] = rad + (eq[i + 2][1] * rad + eq[i + 2][2] * rad + eq[i + 2][3] * rad);
  }

  ol_r_free (n);
  ol_r_free (r);
  ol_g_free (g);

  return;
}

void
net_domain_tesspoly_planes (struct GEO Geo, int id, int* pqty, double** eq)
{
  int i, face;

  (*pqty) = Geo.PolyFaceQty[id]; 
  for (i = 1; i <= Geo.PolyFaceQty[id]; i++)
  {
    face = Geo.PolyFaceNb[id][i];
    ut_array_1d_memcpy (eq[i - 1], 4, Geo.FaceEq[face]);
    if (Geo.PolyFaceOri[id][i] == -1)
      ut_array_1d_scale (eq[i - 1], 4, -1);
  }

  return;
}

void
net_domain_clip (struct POLY* pDomain, double** eq, int qty)
{
  int i, j;
  double* cubesize = ut_alloc_1d (7);
  struct POLYMOD Polymod;

  cubesize[1] = -1e6;
  cubesize[2] =  1e6;
  cubesize[3] = -1e6;
  cubesize[4] =  1e6;
  cubesize[5] = -1e6;
  cubesize[6] =  1e6;

  CubeDomain (cubesize, pDomain);

  for (i = 1; i <= 6; i++)
    (*pDomain).FacePoly[i] = -qty - 1;

  PolymodAlloc (&Polymod);

  P2Pm ((*pDomain), &Polymod);

  int* BadVer = NULL;

  for (i = 0; i < qty; i++)
  {
    BadVer = ut_alloc_1d_int (Polymod.VerQty + 1);
    
    for (j = 1; j <= Polymod.VerQty; j++)
      if (Polymod.VerUse[j] == 1)
	if (ut_space_planeside (eq[i], Polymod.VerCoo[j] - 1) == 1)
	  BadVer[++BadVer[0]] = j;

    if (BadVer[0] != 0)
      PolyhedronModification (-(i + 1), eq[i], &Polymod, BadVer);

    ut_free_1d_int (BadVer);
  }

  // neut_debug_polymod (stdout, Polymod);

  neut_poly_free (pDomain);

  // neut_debug_polymod (stdout, Polymod);
  PolyhedronRecording (0, Polymod, pDomain);
  // neut_debug_poly (stdout, *pDomain);
  
  PolymodFree (&Polymod);

  ut_free_1d (cubesize);

  return;
}
