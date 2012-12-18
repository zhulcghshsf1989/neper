/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesl_tess.h"

/* neut_tesl_tesstializing the properties of the vertices ******************* */

void
neut_tesl_tessVerQty (struct TESL Tess, struct TESS *pTess)
{
  (*pTess).VerQty = Tess.VerQty;

  return;
}

void
neut_tesl_tessVerCoo (struct TESL Tess, struct TESS *pTess)
{
  int i, j;

  (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);

  for (i = 1; i <= (*pTess).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).VerCoo[i][j] = Tess.VerCoo[i][j];

  return;
}

void
neut_tesl_tessVerEdge (struct TESL Tess, struct TESS *pTess)
{
  int i, j;

  (*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
  (*pTess).VerEdgeNb  = ut_alloc_2d_int ((*pTess).VerQty + 1, 4);

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    (*pTess).VerEdgeQty[i] = 4;
    for (j = 0; j < 4; j++)
      (*pTess).VerEdgeNb[i][j] = Tess.VerEdge[i][j];
  }
  for (i = 1; i <= Tess.VerOfDomain[0]; i++)
    (*pTess).VerEdgeQty[Tess.VerOfDomain[i]] = 3;

  return;
}

void
neut_tesl_tessVerState (struct TESS *pTess)
{
  (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);

  return;
}

/* neut_tesl_tesstializing the properties of the edges ******************* */

void
neut_tesl_tessEdgeVerNb (struct TESL Tess, struct TESS *pTess)
{
  int i, j;

  (*pTess).EdgeVerNb = ut_alloc_2d_int ((*pTess).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    for (j = 0; j < 2; j++)
      (*pTess).EdgeVerNb[i][j] = Tess.EdgeVerNb[i][j];

  return;
}

void
neut_tesl_tessEdgeFace (struct TESL Tess, struct TESS *pTess)
{
  int i, j, tmpedge;

  (*pTess).EdgeFaceQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeFaceNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);

  for (i = 1; i <= Tess.FaceQty; i++)
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
    {
      tmpedge = Tess.FaceEdgeNb[i][j];

      (*pTess).EdgeFaceNb[tmpedge] = ut_realloc_1d_int (
	    (*pTess).EdgeFaceNb[tmpedge], ++(*pTess).EdgeFaceQty[tmpedge]);

      (*pTess).EdgeFaceNb[tmpedge][(*pTess).EdgeFaceQty[tmpedge] - 1] = i;
    }

  return;
}

void
neut_tesl_tessEdgeState (struct TESS *pTess)
{
  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  return;
}

void
neut_tesl_tessEdgeDel (struct TESS *pTess)
{
  (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  return;
}

/*
void neut_tesl_tessEdgeDel(struct TESL Tess,struct TESS* pTess)
{
    int i;
    double bound;
    
    double mean=0.4313/pow(Tess.PolyQty,0.33333333333333333333);

    bound=0.5*mean;
    for(i=1;i<=(*pTess).EdgeQty;i++)
	if((*pTess).EdgeLength[i]>bound)
	    (*pTess).EdgeDel[i]=-1;

    return;
}
*/


/* neut_tesl_tesstializing the properties of the faces ******************* */

void
neut_tesl_tessFaceQty (struct TESL Tess, struct TESS *pTess)
{
  (*pTess).FaceQty = Tess.FaceQty;

  return;
}

void
neut_tesl_tessFacePoly (struct TESL Tess, struct TESS *pTess)
{
  int i, j;

  (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 2; j++)
      (*pTess).FacePoly[i][j] = Tess.FacePoly[i][j];

  return;
}

void
neut_tesl_tessFaceVer (struct TESL Tess, struct TESS *pTess)
{
  int i, j;

  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceVerQty[i] = Tess.FaceVerQty[i];
    (*pTess).FaceVerNb[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
      (*pTess).FaceVerNb[i][j] = Tess.FaceVerNb[i][j];
  }

  return;
}

/* a preliminar neut_tesl_tessFaceVer is needed! */
void
neut_tesl_tessFaceEdge (struct TESL Tess, struct TESS *pTess)
{
  int i, j;
    
  (*pTess).FaceEdgeNb  = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceEdgeNb[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);
    (*pTess).FaceEdgeOri[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
    {
      (*pTess).FaceEdgeNb[i][j] = Tess.FaceEdgeNb[i][j];
      (*pTess).FaceEdgeOri[i][j] = Tess.FaceEdgeOri[i][j];
    }
  }

  return;
}

void neut_tesl_tessFaceFF (struct TESS* pTess)
{
  (*pTess).FaceFF = ut_alloc_1d ((*pTess).FaceQty + 1);

  return;
}

void neut_tesl_tessFaceState (struct TESS* pTess)
{
  (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  return;
}

void neut_tesl_tessFacePt (struct TESS* pTess)
{
  (*pTess).FacePt = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FacePtCoo = ut_alloc_2d ((*pTess).FaceQty + 1, 3);

  return;
}

void
neut_tesl_tessFaceEq (struct TESL Tess, struct TESS *pTess)
{
  int i, j;
      
  (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 4; j++)
      (*pTess).FaceEq[i][j] = Tess.FaceEq[i][j];

  return;
}

/* neut_tesl_tesstializing the properties of the faces ******************* */

void
neut_tesl_tessPolyQty (struct TESL Tess, struct TESS *pTess)
{
  (*pTess).PolyQty = Tess.PolyQty;

  return;
}

void
neut_tesl_tessPolyFace (struct TESL Tess, struct TESS *pTess)
{
  int i, j;
    
  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);

  for (i = 1; i <= (*pTess).PolyQty; i++)
  {
    (*pTess).PolyFaceQty[i] = Tess.PolyFaceQty[i];
    (*pTess).PolyFaceNb[i] = ut_alloc_1d_int ((*pTess).PolyFaceQty[i] + 1);
    (*pTess).PolyFaceOri[i] =
      ut_alloc_1d_int ((*pTess).PolyFaceQty[i] + 1);

    for (j = 1; j <= (*pTess).PolyFaceQty[i]; j++)
    {
      (*pTess).PolyFaceNb[i][j] = Tess.PolyFaceNb[i][j];
      (*pTess).PolyFaceOri[i][j] = Tess.PolyFaceOri[i][j];
    }
  }

  return;
}
