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
