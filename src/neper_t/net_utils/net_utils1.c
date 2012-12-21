/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_utils.h"

int
net_poly_clip (struct POLY* pPoly, double* eq)
{
  int i, status;
  struct POLYMOD Polymod;

  PolymodAlloc (&Polymod);

  neut_poly_polymod ((*pPoly), &Polymod);

  int* BadVer = ut_alloc_1d_int (Polymod.VerQty + 1);
  
  for (i = 1; i <= Polymod.VerQty; i++)
    if (Polymod.VerUse[i] == 1)
      if (ut_space_planeside (eq, Polymod.VerCoo[i] - 1) == 1)
	BadVer[++BadVer[0]] = i;

  status = 0;
  if (BadVer[0] != 0)
  {
    PolyhedronModification (-1, eq, &Polymod, BadVer);
    status = 1;
  }

  ut_free_1d_int (BadVer);

  neut_poly_free (pPoly);

  PolyhedronRecording (0, Polymod, pPoly);

  status = (status == 0) ? 0 : (*pPoly).FaceQty;
  
  PolymodFree (&Polymod);

  return status;
}

void
CreateTesl (struct GERMSET GermSet, double *Size, struct TESL *pTesl)
{
  struct POLY Domain;
  struct POLY* Poly = NULL;

  net_domain_cube (Size, &Domain);
  net_polycomp (Domain, GermSet, &Poly, 0);
  net_tesl (GermSet, Poly, pTesl);

  return;
}

void
net_tess_poly (struct TESS Tess, int poly, struct POLY* pPoly)
{
  int i, j, ver, face, verqty, faceqty, ori;
  int* vers = NULL;
  int* faces = NULL;
  int verfaceqty;
  int* vers_inv = NULL;
  int vers_invqty;
  int* faces_inv = NULL;
  int faces_invqty;

  neut_poly_free (pPoly);

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);
  (*pPoly).VerQty = verqty;

  (*pPoly).VerFace = ut_alloc_1d_pint ((*pPoly).VerQty + 1);
  (*pPoly).VerCoo  = ut_alloc_2d      ((*pPoly).VerQty + 1, 3);

  for (i = 1; i <= (*pPoly).VerQty; i++)
  {
    ver = vers[i - 1];

    neut_tess_ver_poly_faces (Tess, ver, poly, &((*pPoly).VerFace[i]), &verfaceqty);

    if (verfaceqty != 3)
    {
      printf ("ver = %d verfaceqty = %d != 3\n", ver, verfaceqty);
      abort ();
    }

    ut_array_1d_memcpy ((*pPoly).VerCoo[i], 3, Tess.VerCoo[ver]);
  }

  (*pPoly).FaceQty = Tess.PolyFaceQty[poly];

  (*pPoly).FacePoly = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
  (*pPoly).FaceEq   = ut_alloc_2d ((*pPoly).FaceQty + 1, 4);
  (*pPoly).FaceVerQty = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
  (*pPoly).FaceVerNb  = ut_alloc_1d_pint ((*pPoly).FaceQty + 1);

  faceqty = (*pPoly).FaceQty; 
  for (i = 1; i <= (*pPoly).FaceQty; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    (*pPoly).FacePoly[i] = (Tess.FacePoly[face][0] == poly) ?
                            Tess.FacePoly[face][1] :
			    Tess.FacePoly[face][0];

    (*pPoly).FaceVerQty[i] = Tess.FaceVerQty[face];
    (*pPoly).FaceVerNb[i]  = ut_alloc_1d_int ((*pPoly).FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pPoly).FaceVerNb[i] + 1,
			    (*pPoly).FaceVerQty[i],
			    Tess.FaceVerNb[face] + 1);

    ut_array_1d_memcpy ((*pPoly).FaceEq[i], 4, Tess.FaceEq[face]);

    // reversing face to get it pointing outwards, if necessary
    neut_tess_poly_face_ori (Tess, poly, face, &ori);
    
    if (ori == -1)
    {
      ut_array_1d_scale ((*pPoly).FaceEq[i], 4, -1);
      ut_array_1d_int_reverseelts ((*pPoly).FaceVerNb[i] + 1,
				   (*pPoly).FaceVerQty[i]);
    }
  }

  // renumbering vers
  int vermin = ut_array_1d_int_min (vers, verqty);
  ut_array_1d_int_addval (vers, verqty, -vermin, vers);

  ut_array_1d_int_inv (vers, verqty, &vers_inv, &vers_invqty);

  for (i = 1; i <= (*pPoly).FaceQty; i++)
    for (j = 1; j <= (*pPoly).FaceVerQty[i]; j++)
    {
      ver = (*pPoly).FaceVerNb[i][j];
      (*pPoly).FaceVerNb[i][j] = vers_inv[ver - vermin] + 1;
    }

  // renumbering faces
  faces = ut_alloc_1d_int (faceqty);
  ut_array_1d_int_memcpy (faces, faceqty, Tess.PolyFaceNb[poly] + 1);

  int facemin = ut_array_1d_int_min (faces, faceqty);
  ut_array_1d_int_addval (faces, faceqty, -facemin, faces);

  ut_array_1d_int_inv (faces, faceqty, &faces_inv, &faces_invqty);

  for (i = 1; i <= (*pPoly).VerQty; i++)
    for (j = 0; j < 3; j++)
    {
      face = (*pPoly).VerFace[i][j];
	(*pPoly).VerFace[i][j] = faces_inv[face - facemin] + 1;
    }

  for (i = 1; i <= (*pPoly).FaceQty; i++)
  (*pPoly).FacePoly[i] = -i;

  ut_free_1d_int (faces);
  ut_free_1d_int (vers);
  ut_free_1d_int (faces_inv);
  ut_free_1d_int (vers_inv);

  return;
}

void
net_poly_tesl (struct POLY Poly, struct TESL* pTess)
{
  struct GERMSET GermSet; 
  struct POLY* PolyArray = (struct POLY*) calloc (2, sizeof (struct POLY));
  PolyArray[1] = Poly;

  neut_tesl_set_zero (pTess);

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

  net_tesl (GermSet, PolyArray, pTess);

  free (PolyArray);

  neut_germset_free (GermSet);

  return;
}

void
net_poly_tess (struct POLY Poly, struct TESS* pTess)
{
  neut_tess_set_zero (pTess);

  struct TESL Tess;
  neut_tesl_set_zero (&Tess);

  net_poly_tesl (Poly, &Tess);
  neut_tesl_tess (Tess, pTess);

  neut_tesl_free (&Tess);

  return;
}
