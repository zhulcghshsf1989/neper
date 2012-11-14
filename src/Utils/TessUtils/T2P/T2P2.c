/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"T2P.h"

/* Recording of the vertex quantity */
void
T2PVerQty (struct TESS Tess, int nb, struct POLY *pPoly)
{
  (*pPoly).VerQty = Tess.PolyVerQty[nb];

  return;
}

/* Recording of the face quantity */
void
T2PFaceQty (struct TESS Tess, int nb, struct POLY *pPoly)
{
  (*pPoly).FaceQty = Tess.PolyFaceQty[nb];

  return;
}

/* Recording of the face properties */
void
T2PFaceProp (struct TESS Tess, int nb, struct POLY *pPoly, int *TV2PV)
{
  /* Recording of the face germs */
  T2PFacePoly (Tess, nb, pPoly);
  /* Recording of the face equation parameters */
  T2PFaceEq (Tess, nb, pPoly);
  /* Recording of the face vertex quantities */
  T2PFaceVerQty (Tess, nb, pPoly);
  /* Recording of the face vertex numbers */
  T2PFaceVerNb (Tess, nb, pPoly, TV2PV);

  return;
}
