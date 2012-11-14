/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"T2P.h"

/* Recording of the vertices of the faces. This step is carried
 * out at last because it needs the previously-defined data.
 */
void
T2PVerFace (struct TESS Tess, int nb, struct POLY *pPoly, int *TV2PV)
{
  int i, j;			/* mute variables */
  int TV;			/* vertex nb, as described into Tess */
  int rec;			/* nb of recorded verface(s) */
  int TVG;			/* nb of the vertex germ, as recorded into Tess */

  /* The memory is allocated */
  (*pPoly).VerFace = twoDIntDAlloc ((*pPoly).VerQty + 1, 3);


  for (i = 1; i <= (*pPoly).VerQty; i++)	/* for every vertex */
  {
    TV = TV2PV[i];		/* TV is recorded */
    rec = 0;			/* nb of recorded verface(s) */
    for (j = 0; j <= 3; j++)	/* for every of the 4 vertex parent germs */
    {
      TVG = Tess.VerGerm[TV][j];	/* TGP is recorded */
      if (TVG != nb)		/* if it is not nb */
      {				/* the nb of the face, as recorded into poly, */
	/* is recorded */
	(*pPoly).VerFace[i][rec] =
	  oneDIntEltPos ((*pPoly).FacePoly, 1, (*pPoly).FaceQty, TVG, 0);
	rec++;			/* and rec is incremented */
      }
    }
  }

  return;
}

/* Recording of the vertex coordinates */
void
T2PVerCoo (struct TESS Tess, int nb, struct POLY *pPoly, int *TV2PV)
{
  int i, j;
  int TV;			/* tessellation vertex number */

  (*pPoly).VerCoo = twoDDoubleDAlloc ((*pPoly).VerQty + 1, 3);

  for (i = 1; i <= (*pPoly).VerQty; i++)
  {
    TV = Tess.PolyVerNb[nb][i];	/* recording of the vertex into the */
    /* tessellation description */
    for (j = 0; j <= 2; j++)
      (*pPoly).VerCoo[i][j] = Tess.VerCoo[TV][j];

    TV2PV[i] = TV;		/* rellation poly vertex <--> tess vertex */
  }

  return;
}

/* Recording of the face germs */
void
T2PFacePoly (struct TESS Tess, int nb, struct POLY *pPoly)
{
  int i;			/* mute variable */
  int TF;			/* face number, as recorded into the tessellation */

  /* FacePoly is allocated */
  (*pPoly).FacePoly = ut_alloc_1d_int ((*pPoly).FaceQty + 1);

  for (i = 1; i <= (*pPoly).FaceQty; i++)	/* for every face */
  {
    TF = Tess.PolyFaceNb[nb][i];	/* recording of TF */

    if (Tess.FacePoly[TF][0] != nb)	/* if the 1st germ is not nb */
      (*pPoly).FacePoly[i] = Tess.FacePoly[TF][0];	/* recording */
    else			/* else */
      (*pPoly).FacePoly[i] = Tess.FacePoly[TF][1];	/* recording of [1] */
  }

  return;
}

/* Recording of the face equation parameters */
void
T2PFaceEq (struct TESS Tess, int nb, struct POLY *pPoly)
{
  int i, j;			/* mute variables */
  int TF;			/* face number, as recorded into the tessellation   */

  /* FaceEq is allocated */
  (*pPoly).FaceEq = twoDDoubleDAlloc ((*pPoly).FaceQty + 1, 4);

  for (i = 1; i <= (*pPoly).FaceQty; i++)	/* for every face */
  {
    TF = Tess.PolyFaceNb[nb][i];	/* recording of TF */

    for (j = 0; j <= 3; j++)	/* for every para */
      (*pPoly).FaceEq[i][j] = Tess.FaceEq[TF][j];	/* recording */
  }

  return;
}

/* Recording of the face vertex quantities */
void
T2PFaceVerQty (struct TESS Tess, int nb, struct POLY *pPoly)
{
  int i;			/* mute variable */
  int TF;			/* tessellation face number */

  /* FaceVerQty is allocated */
  (*pPoly).FaceVerQty = ut_alloc_1d_int ((*pPoly).FaceQty + 1);

  for (i = 1; i <= (*pPoly).FaceQty; i++)	/* for every face */
  {
    TF = Tess.PolyFaceNb[nb][i];	/* recording of TF */
    (*pPoly).FaceVerQty[i] = Tess.FaceVerQty[TF];	/* recording */
  }

  return;
}

/* Recording of the face vertex numbers */
void
T2PFaceVerNb (struct TESS Tess, int nb, struct POLY *pPoly, int *TV2PV)
{
  int i, j;			/* mute variables */
  int TF;			/* tessellation face number */
  int TV;			/* tessellation vertex number */
  int PV;			/* polyhedron  vertex number */

  /* FaceVerNb is allocated as a pInt 1-dimension array  */
  (*pPoly).FaceVerNb = oneDpIntDAlloc ((*pPoly).FaceQty + 1);

  for (i = 1; i <= (*pPoly).FaceQty; i++)
  {
    TF = Tess.PolyFaceNb[nb][i];

    (*pPoly).FaceVerNb[i] = ut_alloc_1d_int ((*pPoly).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pPoly).FaceVerQty[i]; j++)
    {
      TV = Tess.FaceVerNb[TF][j];
      PV = oneDIntEltPos (TV2PV, 1, (*pPoly).VerQty, TV, 0);
      (*pPoly).FaceVerNb[i][j] = PV;
    }
  }

  return;
}
