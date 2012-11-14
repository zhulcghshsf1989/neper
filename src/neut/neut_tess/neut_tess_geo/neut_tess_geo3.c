/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_geo.h"

/* neut_tess_geotializing the properties of the vertices ******************* */

void
neut_tess_geoVerQty (struct TESS Tess, struct GEO *pGeo)
{
  (*pGeo).VerQty = Tess.VerQty;

  return;
}

void
neut_tess_geoVerCoo (struct TESS Tess, struct GEO *pGeo)
{
  int i, j;

  (*pGeo).VerCoo = ut_alloc_2d ((*pGeo).VerQty + 1, 3);

  for (i = 1; i <= (*pGeo).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pGeo).VerCoo[i][j] = Tess.VerCoo[i][j];

  return;
}

void
neut_tess_geoVerEdge (struct TESS Tess, struct GEO *pGeo)
{
  int i, j;

  (*pGeo).VerEdgeQty = ut_alloc_1d_int ((*pGeo).VerQty + 1);
  (*pGeo).VerEdgeNb  = ut_alloc_2d_int ((*pGeo).VerQty + 1, 4);

  for (i = 1; i <= (*pGeo).VerQty; i++)
  {
    (*pGeo).VerEdgeQty[i] = 4;
    for (j = 0; j < 4; j++)
      (*pGeo).VerEdgeNb[i][j] = Tess.VerEdge[i][j];
  }
  for (i = 1; i <= Tess.VerOfDomain[0]; i++)
    (*pGeo).VerEdgeQty[Tess.VerOfDomain[i]] = 3;

  return;
}

void
neut_tess_geoVerState (struct GEO *pGeo)
{
  (*pGeo).VerState = ut_alloc_1d_int ((*pGeo).VerQty + 1);

  return;
}

/* neut_tess_geotializing the properties of the edges ******************* */

void
neut_tess_geoEdgeVerNb (struct TESS Tess, struct GEO *pGeo)
{
  int i, j;

  (*pGeo).EdgeVerNb = ut_alloc_2d_int ((*pGeo).EdgeQty + 1, 2);

  for (i = 1; i <= (*pGeo).EdgeQty; i++)
    for (j = 0; j < 2; j++)
      (*pGeo).EdgeVerNb[i][j] = Tess.EdgeVerNb[i][j];

  return;
}

void
neut_tess_geoEdgeFace (struct TESS Tess, struct GEO *pGeo)
{
  int i, j, tmpedge;

  (*pGeo).EdgeFaceQty = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);
  (*pGeo).EdgeFaceNb = ut_alloc_1d_pint ((*pGeo).EdgeQty + 1);

  for (i = 1; i <= Tess.FaceQty; i++)
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
    {
      tmpedge = Tess.FaceEdgeNb[i][j];

      (*pGeo).EdgeFaceNb[tmpedge] = ut_realloc_1d_int (
	    (*pGeo).EdgeFaceNb[tmpedge], ++(*pGeo).EdgeFaceQty[tmpedge]);

      (*pGeo).EdgeFaceNb[tmpedge][(*pGeo).EdgeFaceQty[tmpedge] - 1] = i;
    }

  return;
}

void
neut_tess_geoEdgeState (struct GEO *pGeo)
{
  (*pGeo).EdgeState = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);

  return;
}

void
neut_tess_geoEdgeDel (struct GEO *pGeo)
{
  (*pGeo).EdgeDel = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);

  return;
}

/*
void neut_tess_geoEdgeDel(struct TESS Tess,struct GEO* pGeo)
{
    int i;
    double bound;
    
    double mean=0.4313/pow(Tess.PolyQty,0.33333333333333333333);

    bound=0.5*mean;
    for(i=1;i<=(*pGeo).EdgeQty;i++)
	if((*pGeo).EdgeLength[i]>bound)
	    (*pGeo).EdgeDel[i]=-1;

    return;
}
*/


/* neut_tess_geotializing the properties of the faces ******************* */

void
neut_tess_geoFaceQty (struct TESS Tess, struct GEO *pGeo)
{
  (*pGeo).FaceQty = Tess.FaceQty;

  return;
}

void
neut_tess_geoFacePoly (struct TESS Tess, struct GEO *pGeo)
{
  int i, j;

  (*pGeo).FacePoly = ut_alloc_2d_int ((*pGeo).FaceQty + 1, 2);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
    for (j = 0; j < 2; j++)
      (*pGeo).FacePoly[i][j] = Tess.FacePoly[i][j];

  return;
}

void
neut_tess_geoFaceVer (struct TESS Tess, struct GEO *pGeo)
{
  int i, j;

  (*pGeo).FaceVerQty = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  (*pGeo).FaceVerNb = ut_alloc_1d_pint ((*pGeo).FaceQty + 1);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
  {
    (*pGeo).FaceVerQty[i] = Tess.FaceVerQty[i];
    (*pGeo).FaceVerNb[i] = ut_alloc_1d_int ((*pGeo).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pGeo).FaceVerQty[i]; j++)
      (*pGeo).FaceVerNb[i][j] = Tess.FaceVerNb[i][j];
  }

  return;
}

/* a preliminar neut_tess_geoFaceVer is needed! */
void
neut_tess_geoFaceEdge (struct TESS Tess, struct GEO *pGeo)
{
  int i, j;
    
  (*pGeo).FaceEdgeNb  = ut_alloc_1d_pint ((*pGeo).FaceQty + 1);
  (*pGeo).FaceEdgeOri = ut_alloc_1d_pint ((*pGeo).FaceQty + 1);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
  {
    (*pGeo).FaceEdgeNb[i] = ut_alloc_1d_int ((*pGeo).FaceVerQty[i] + 1);
    (*pGeo).FaceEdgeOri[i] = ut_alloc_1d_int ((*pGeo).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pGeo).FaceVerQty[i]; j++)
    {
      (*pGeo).FaceEdgeNb[i][j] = Tess.FaceEdgeNb[i][j];
      (*pGeo).FaceEdgeOri[i][j] = Tess.FaceEdgeOri[i][j];
    }
  }

  return;
}

void neut_tess_geoFaceFF (struct GEO* pGeo)
{
  (*pGeo).FaceFF = ut_alloc_1d ((*pGeo).FaceQty + 1);

  return;
}

void neut_tess_geoFaceState (struct GEO* pGeo)
{
  (*pGeo).FaceState = ut_alloc_1d_int ((*pGeo).FaceQty + 1);

  return;
}

void neut_tess_geoFacePt (struct GEO* pGeo)
{
  (*pGeo).FacePt = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  (*pGeo).FacePtCoo = ut_alloc_2d ((*pGeo).FaceQty + 1, 3);

  return;
}

void
neut_tess_geoFaceEq (struct TESS Tess, struct GEO *pGeo)
{
  int i, j;
      
  (*pGeo).FaceEq = ut_alloc_2d ((*pGeo).FaceQty + 1, 4);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
    for (j = 0; j < 4; j++)
      (*pGeo).FaceEq[i][j] = Tess.FaceEq[i][j];

  return;
}

/* neut_tess_geotializing the properties of the faces ******************* */

void
neut_tess_geoPolyQty (struct TESS Tess, struct GEO *pGeo)
{
  (*pGeo).PolyQty = Tess.PolyQty;

  return;
}

void
neut_tess_geoPolyFace (struct TESS Tess, struct GEO *pGeo)
{
  int i, j;
    
  (*pGeo).PolyFaceQty = ut_alloc_1d_int ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceNb = ut_alloc_1d_pint ((*pGeo).PolyQty + 1);
  (*pGeo).PolyFaceOri = ut_alloc_1d_pint ((*pGeo).PolyQty + 1);

  for (i = 1; i <= (*pGeo).PolyQty; i++)
  {
    (*pGeo).PolyFaceQty[i] = Tess.PolyFaceQty[i];
    (*pGeo).PolyFaceNb[i] = ut_alloc_1d_int ((*pGeo).PolyFaceQty[i] + 1);
    (*pGeo).PolyFaceOri[i] =
      ut_alloc_1d_int ((*pGeo).PolyFaceQty[i] + 1);

    for (j = 1; j <= (*pGeo).PolyFaceQty[i]; j++)
    {
      (*pGeo).PolyFaceNb[i][j] = Tess.PolyFaceNb[i][j];
      (*pGeo).PolyFaceOri[i][j] = Tess.PolyFaceOri[i][j];
    }
  }

  return;
}
