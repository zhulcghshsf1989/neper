/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_poly.h"

void
neut_poly_set_zero (struct POLY* pPoly)
{
  (*pPoly).VerQty = 0;
  (*pPoly).FaceQty = 0;

  (*pPoly).VerFace = NULL;
  (*pPoly).VerCoo = NULL;

  (*pPoly).FacePoly = NULL;
  (*pPoly).FaceEq = NULL;
  (*pPoly).FaceVerQty = NULL;
  (*pPoly).FaceVerNb = NULL;

  return;
}

void
neut_poly_free (struct POLY* pPoly)
{
  ut_free_2d_int    ((*pPoly).VerFace  , (*pPoly).VerQty + 1);
  ut_free_2d ((*pPoly).VerCoo   , (*pPoly).VerQty + 1);

  ut_free_1d_int    ((*pPoly).FacePoly);
  ut_free_2d ((*pPoly).FaceEq    , (*pPoly).FaceQty + 1);
  ut_free_1d_int    ((*pPoly).FaceVerQty);
  ut_free_2d_int    ((*pPoly).FaceVerNb , (*pPoly).FaceQty + 1);

  return;
}

void
neut_poly_bbox (struct POLY Poly, double* size)
{
  int i, j;

  size[1] = size[3] = size[5] =  DBL_MAX;
  size[2] = size[4] = size[6] = -DBL_MAX;

  for (j = 0; j < 3; j++)
    for (i = 1; i <= Poly.VerQty; i++)
    {
      size[2 * j + 1] = ut_num_min (size[2 * j + 1], Poly.VerCoo[i][j]);
      size[2 * j + 2] = ut_num_max (size[2 * j + 2], Poly.VerCoo[i][j]);
    }

  return;
}

int
neut_poly_point_in (struct POLY Poly, double* coo)
{
  int i, j;
  int status = 0;
  double* C = ut_alloc_1d (3);

  // rough centre
  for (j = 0; j < 3; j++)
  {
    for (i = 1; i <= Poly.VerQty; i++)
      C[j] += Poly.VerCoo[i][j];

    C[j] /= Poly.VerQty;
  }

  status = 1;
  for (i = 1; i <= Poly.FaceQty; i++)
    if (ut_space_planeside (Poly.FaceEq[i], coo - 1)
     != ut_space_planeside (Poly.FaceEq[i], C - 1))
    {
      status = 0;
      break;
    }

  ut_free_1d (C);

  return status;
}

void
neut_poly_tess (struct POLY Poly, struct TESS* pTess)
{
  struct GERMSET GermSet; 
  struct POLY* PolyArray = (struct POLY*) calloc (2, sizeof (struct POLY));
  PolyArray[1] = Poly;

  neut_tess_set_zero (pTess);

  neut_germset_set_zero (&GermSet);

  GermSet.N = 1;
  GermSet.N1d = 1;
  GermSet.Id = 1;

  GermSet.morpho = ut_alloc_1d_char (10);
  sprintf (GermSet.morpho, "poly");

  GermSet.GermsCoo = ut_alloc_2d (2, 4);
  GermSet.GermsCoo[1][1] = 1;
  GermSet.GermsCoo[1][2] = 2;
  GermSet.GermsCoo[1][3] = 3;

  GermSet.NDensity = 1;

  Tessellation (GermSet, PolyArray, pTess);

  free (PolyArray);

  neut_germset_free (GermSet);

  return;
}

void
neut_poly_geo (struct POLY Poly, struct GEO* pGeo)
{
  neut_geo_set_zero (pGeo);

  struct TESS Tess;
  neut_tess_set_zero (&Tess);

  neut_poly_tess (Poly, &Tess);
  neut_tess_geo (Tess, pGeo);

  neut_tess_free (&Tess);

  return;
}

