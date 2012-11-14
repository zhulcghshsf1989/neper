/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_geo_geo_lcl.h"

/* neut_geo_geotializing the properties of the vertices ******************* */

void
neut_geo_geoVerQty (struct GEO GeoA, struct GEO *pGeoB)
{
  (*pGeoB).VerQty = GeoA.VerQty;

  return;
}

void
neut_geo_geoVerCoo (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).VerCoo = ut_alloc_2d ((*pGeoB).VerQty + 1, 3);

  for (i = 1; i <= (*pGeoB).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pGeoB).VerCoo[i][j] = GeoA.VerCoo[i][j];

  return;
}

void
neut_geo_geoVerEdge (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).VerEdgeQty = ut_alloc_1d_int ((*pGeoB).VerQty + 1);

  (*pGeoB).VerEdgeNb  = ut_alloc_1d_pint ((*pGeoB).VerQty + 1);

  for (i = 1; i <= (*pGeoB).VerQty; i++)
  {
    (*pGeoB).VerEdgeQty[i] = GeoA.VerEdgeQty[i];

    (*pGeoB).VerEdgeNb[i] = ut_alloc_1d_int ((*pGeoB).VerEdgeQty[i]);

    for (j = 0; j < (*pGeoB).VerEdgeQty[i]; j++)
      (*pGeoB).VerEdgeNb[i][j] = GeoA.VerEdgeNb[i][j];
  }

  return;
}

void
neut_geo_geoVerBound (struct GEO GeoA, struct GEO *pGeoB)
{
  (*pGeoB).VerDom = ut_alloc_2d_int ((*pGeoB).VerQty + 1, 2);
  ut_array_2d_int_memcpy ((*pGeoB).VerDom + 1, (*pGeoB).VerQty, 2, GeoA.VerDom + 1);

  return;
}

void
neut_geo_geoVerState (struct GEO GeoA, struct GEO *pGeoB)
{
  int i;

  (*pGeoB).VerState = ut_alloc_1d_int ((*pGeoB).VerQty + 1);

  for (i = 1; i <= (*pGeoB).VerQty; i++)
    (*pGeoB).VerState[i] = GeoA.VerState[i];

  return;
}

/* Initializing the properties of the edges ******************* */

void
neut_geo_geoEdgeQty (struct GEO GeoA, struct GEO *pGeoB)
{
  (*pGeoB).EdgeQty = GeoA.EdgeQty;

  return;
}

void
neut_geo_geoEdgeVerNb (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).EdgeVerNb = ut_alloc_2d_int ((*pGeoB).EdgeQty + 1, 2);

  for (i = 1; i <= (*pGeoB).EdgeQty; i++)
    for (j = 0; j < 2; j++)
      (*pGeoB).EdgeVerNb[i][j] = GeoA.EdgeVerNb[i][j];

  return;
}

void
neut_geo_geoEdgeFace (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).EdgeFaceQty = ut_alloc_1d_int ((*pGeoB).EdgeQty + 1);
  (*pGeoB).EdgeFaceNb  = ut_alloc_1d_pint ((*pGeoB).EdgeQty + 1);

  for (i = 1; i <= GeoA.EdgeQty; i++)
  {
    (*pGeoB).EdgeFaceQty[i] = GeoA.EdgeFaceQty[i];

    (*pGeoB).EdgeFaceNb[i] = ut_alloc_1d_int ((*pGeoB).EdgeFaceQty[i] + 1);

    for (j = 0; j < (*pGeoB).EdgeFaceQty[i]; j++)
      (*pGeoB).EdgeFaceNb[i][j] = GeoA.EdgeFaceNb[i][j];
  }

  return;
}

void
neut_geo_geoEdgeLength (struct GEO GeoA, struct GEO *pGeoB)
{
  int i;

  (*pGeoB).EdgeLength = ut_alloc_1d ((*pGeoB).EdgeQty + 1);

  for (i = 1; i <= (*pGeoB).EdgeQty; i++)
    (*pGeoB).EdgeLength[i] = GeoA.EdgeLength[i];

  return;
}

void
neut_geo_geoEdgeBound (struct GEO GeoA, struct GEO *pGeoB)
{
  (*pGeoB).EdgeDom = ut_alloc_2d_int ((*pGeoB).EdgeQty + 1, 2);

  ut_array_2d_int_memcpy ((*pGeoB).EdgeDom + 1,
			  (*pGeoB).EdgeQty, 2, GeoA.EdgeDom + 1);

  return;
}

void
neut_geo_geoEdgeDel (struct GEO GeoA, struct GEO *pGeoB)
{
  int i;

  (*pGeoB).EdgeDel = ut_alloc_1d_int ((*pGeoB).EdgeQty + 1);

  for (i = 1; i <= (*pGeoB).EdgeQty; i++)
    (*pGeoB).EdgeDel[i] = GeoA.EdgeDel[i];

  return;
}

void
neut_geo_geoEdgeState (struct GEO GeoA, struct GEO *pGeoB)
{
  int i;

  (*pGeoB).EdgeState = ut_alloc_1d_int ((*pGeoB).EdgeQty + 1);

  for (i = 1; i <= (*pGeoB).EdgeQty; i++)
    (*pGeoB).EdgeState[i] = GeoA.EdgeState[i];

  return;
}

/* neut_geo_geotializing the properties of the faces ******************* */

void
neut_geo_geoFaceQty (struct GEO GeoA, struct GEO *pGeoB)
{
  (*pGeoB).FaceQty = GeoA.FaceQty;

  return;
}

void
neut_geo_geoFacePoly (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).FacePoly = ut_alloc_2d_int ((*pGeoB).FaceQty + 1, 2);

  for (i = 1; i <= (*pGeoB).FaceQty; i++)
    for (j = 0; j < 2; j++)
      (*pGeoB).FacePoly[i][j] = GeoA.FacePoly[i][j];

  return;
}

void
neut_geo_geoFaceVer (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).FaceVerQty = ut_alloc_1d_int ((*pGeoB).FaceQty + 1);
  (*pGeoB).FaceVerNb = ut_alloc_1d_pint ((*pGeoB).FaceQty + 1);

  for (i = 1; i <= (*pGeoB).FaceQty; i++)
  {
    (*pGeoB).FaceVerQty[i] = GeoA.FaceVerQty[i];

    (*pGeoB).FaceVerNb[i] =
      ut_alloc_1d_int ((*pGeoB).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pGeoB).FaceVerQty[i]; j++)
      (*pGeoB).FaceVerNb[i][j] = GeoA.FaceVerNb[i][j];
  }

  return;
}

/* a preliminar neut_geo_geoFaceVer is needed! */
void
neut_geo_geoFaceEdge (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).FaceEdgeNb = ut_alloc_1d_pint ((*pGeoB).FaceQty + 1);
  (*pGeoB).FaceEdgeOri = ut_alloc_1d_pint ((*pGeoB).FaceQty + 1);

  for (i = 1; i <= (*pGeoB).FaceQty; i++)
  {
    (*pGeoB).FaceEdgeNb[i] =
      ut_alloc_1d_int ((*pGeoB).FaceVerQty[i] + 1);
    (*pGeoB).FaceEdgeOri[i] =
      ut_alloc_1d_int ((*pGeoB).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pGeoB).FaceVerQty[i]; j++)
    {
      (*pGeoB).FaceEdgeNb[i][j] = GeoA.FaceEdgeNb[i][j];
      (*pGeoB).FaceEdgeOri[i][j] = GeoA.FaceEdgeOri[i][j];
    }
  }

  return;
}

void
neut_geo_geoFaceFF (struct GEO GeoA, struct GEO *pGeoB)
{
  int i;

  (*pGeoB).FaceFF = ut_alloc_1d ((*pGeoB).FaceQty + 1);

  for (i = 1; i <= (*pGeoB).FaceQty; i++)
    (*pGeoB).FaceFF[i] = GeoA.FaceFF[i];

  return;
}

void
neut_geo_geoFaceEq (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).FaceEq = ut_alloc_2d ((*pGeoB).FaceQty + 1, 4);

  for (i = 1; i <= (*pGeoB).FaceQty; i++)
    for (j = 0; j < 4; j++)
      (*pGeoB).FaceEq[i][j] = GeoA.FaceEq[i][j];

  return;
}

void
neut_geo_geoFaceBound (struct GEO GeoA, struct GEO *pGeoB)
{
  (*pGeoB).FaceDom = ut_alloc_2d_int ((*pGeoB).FaceQty + 1, 2);

  ut_array_2d_int_memcpy ((*pGeoB).FaceDom + 1,
			  (*pGeoB).FaceQty, 2, GeoA.FaceDom + 1);

  return;
}

void
neut_geo_geoFaceState (struct GEO GeoA, struct GEO *pGeoB)
{
  int i;
  
  (*pGeoB).FaceState = ut_alloc_1d_int ((*pGeoB).FaceQty + 1);

  for (i = 1; i <= (*pGeoB).FaceQty; i++)
    (*pGeoB).FaceState[i] = GeoA.FaceState[i];

  return;
}

void
neut_geo_geoFacePt (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).FacePt = ut_alloc_1d_int ((*pGeoB).FaceQty + 1);
  (*pGeoB).FacePtCoo = ut_alloc_2d ((*pGeoB).FaceQty + 1, 3);

  for (i = 1; i <= (*pGeoB).FaceQty; i++)
  {
    (*pGeoB).FacePt[i] = GeoA.FacePt[i];
    for (j = 0; j < 3; j++)
      (*pGeoB).FacePtCoo[i][j] = GeoA.FacePtCoo[i][j];
  }

  return;
}

/* neut_geo_geotializing the properties of the faces ******************* */

void
neut_geo_geoPolyQty (struct GEO GeoA, struct GEO *pGeoB)
{
  (*pGeoB).PolyQty = GeoA.PolyQty;

  return;
}

void
neut_geo_geoPolyFace (struct GEO GeoA, struct GEO *pGeoB)
{
  int i, j;

  (*pGeoB).PolyFaceQty = ut_alloc_1d_int ((*pGeoB).PolyQty + 1);
  (*pGeoB).PolyFaceNb = ut_alloc_1d_pint ((*pGeoB).PolyQty + 1);
  (*pGeoB).PolyFaceOri = ut_alloc_1d_pint ((*pGeoB).PolyQty + 1);

  for (i = 1; i <= (*pGeoB).PolyQty; i++)
  {
    (*pGeoB).PolyFaceQty[i] = GeoA.PolyFaceQty[i];
    
    (*pGeoB).PolyFaceNb[i] =
      ut_alloc_1d_int ((*pGeoB).PolyFaceQty[i] + 1);
    (*pGeoB).PolyFaceOri[i] =
      ut_alloc_1d_int ((*pGeoB).PolyFaceQty[i] + 1);

    for (j = 1; j <= (*pGeoB).PolyFaceQty[i]; j++)
    {
      (*pGeoB).PolyFaceNb[i][j] = GeoA.PolyFaceNb[i][j];
      (*pGeoB).PolyFaceOri[i][j] = GeoA.PolyFaceOri[i][j];
    }
  }

  return;
}

void
neut_geo_geoPolyCenter (struct GEO GeoA, struct GEO *pGeoB)
{
  int i;

  if (!GeoA.CenterCoo)
    return;
  
  (*pGeoB).CenterCoo = ut_alloc_2d (GeoA.PolyQty + 1, 3);
  for (i = 1; i <= GeoA.PolyQty; i++)
    ut_array_1d_memcpy ((*pGeoB).CenterCoo[i], 3, GeoA.CenterCoo[i]);

  return;
}

void
neut_geo_geoPolyTrue (struct GEO GeoA, struct GEO *pGeoB)
{
  if (!GeoA.PolyTrue)
    return;
  
  (*pGeoB).PolyTrue = ut_alloc_1d_int (GeoA.PolyQty + 1);
  ut_array_1d_int_memcpy ((*pGeoB).PolyTrue + 1, GeoA.PolyQty, GeoA.PolyTrue + 1);

  return;
}

void
neut_geo_geoPolyBody (struct GEO GeoA, struct GEO *pGeoB)
{
  if (!GeoA.PolyBody)
    return;
  
  (*pGeoB).PolyBody = ut_alloc_1d_int (GeoA.PolyQty + 1);
  ut_array_1d_int_memcpy ((*pGeoB).PolyBody + 1, GeoA.PolyQty, GeoA.PolyBody + 1);

  return;
}

void
neut_geo_geoDomainData (struct GEO GeoA, struct GEO *pGeoB)
{
  int i;

  (*pGeoB).DomType = ut_alloc_1d_char (strlen (GeoA.DomType) + 1);
  strcpy ((*pGeoB).DomType, GeoA.DomType);

  // ver
 
  (*pGeoB).DomVerQty = GeoA.DomVerQty;

  (*pGeoB).DomTessVerNb = ut_alloc_1d_int ((*pGeoB).DomVerQty + 1);
  ut_array_1d_int_memcpy ((*pGeoB).DomTessVerNb + 1, (*pGeoB).DomVerQty,
                              GeoA.DomTessVerNb + 1);

  (*pGeoB).DomVerEdgeQty = ut_alloc_1d_int ((*pGeoB).DomVerQty + 1);
  (*pGeoB).DomVerEdgeNb  = ut_alloc_1d_pint ((*pGeoB).DomVerQty + 1);

  ut_array_1d_int_memcpy ((*pGeoB).DomVerEdgeQty + 1, (*pGeoB).DomVerQty,
                              GeoA.DomVerEdgeQty + 1);

  for (i = 1; i <= (*pGeoB).DomVerQty; i++)
  {
    (*pGeoB).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pGeoB).DomVerEdgeQty[i]);
    ut_array_1d_int_memcpy ((*pGeoB).DomVerEdgeNb[i], (*pGeoB).DomVerEdgeQty[i], 
	                        GeoA.DomVerEdgeNb[i]);
  }

  // edge
  
  (*pGeoB).DomEdgeQty = GeoA.DomEdgeQty;

  (*pGeoB).DomEdgeVerNb = ut_alloc_2d_int ((*pGeoB).DomEdgeQty + 1, 2);
  ut_array_2d_int_memcpy ((*pGeoB).DomEdgeVerNb + 1, (*pGeoB).DomEdgeQty, 2,
                              GeoA.DomEdgeVerNb + 1);

  (*pGeoB).DomEdgeFaceNb = ut_alloc_2d_int ((*pGeoB).DomEdgeQty + 1, 2);
  ut_array_2d_int_memcpy ((*pGeoB).DomEdgeFaceNb + 1, (*pGeoB).DomEdgeQty, 2,
                              GeoA.DomEdgeFaceNb + 1);

  (*pGeoB).DomTessEdgeQty = ut_alloc_1d_int ((*pGeoB).DomEdgeQty + 1);
  (*pGeoB).DomTessEdgeNb = ut_alloc_1d_pint ((*pGeoB).DomEdgeQty + 1);

  ut_array_1d_int_memcpy ((*pGeoB).DomTessEdgeQty + 1, (*pGeoB).DomEdgeQty,
			      GeoA.DomTessEdgeQty + 1);

  for (i = 1; i <= (*pGeoB).DomEdgeQty; i++)
  {
    (*pGeoB).DomTessEdgeNb[i] = ut_alloc_1d_int ((*pGeoB).DomTessEdgeQty[i] + 1);
    ut_array_1d_int_memcpy ((*pGeoB).DomTessEdgeNb[i] + 1, (*pGeoB).DomTessEdgeQty[i],
	                        GeoA.DomTessEdgeNb[i] + 1);
  }

  // face
  
  (*pGeoB).DomFaceQty = GeoA.DomFaceQty;

  (*pGeoB).DomFaceEq = ut_alloc_2d ((*pGeoB).DomFaceQty + 1, 4);
  ut_array_2d_memcpy ((*pGeoB).DomFaceEq + 1, (*pGeoB).DomFaceQty, 4,
                          GeoA.DomFaceEq + 1);

  (*pGeoB).DomTessFaceQty = ut_alloc_1d_int ((*pGeoB).DomFaceQty + 1);
  ut_array_1d_int_memcpy ((*pGeoB).DomTessFaceQty + 1, (*pGeoB).DomFaceQty,
                              GeoA.DomTessFaceQty + 1);

  (*pGeoB).DomTessFaceNb = ut_alloc_1d_pint ((*pGeoB).DomFaceQty + 1);

  for (i = 1; i <= (*pGeoB).DomFaceQty; i++)
  {
    (*pGeoB).DomTessFaceNb[i] = ut_alloc_1d_int ((*pGeoB).DomTessFaceQty[i] + 1);
    ut_array_1d_int_memcpy ((*pGeoB).DomTessFaceNb[i] + 1, (*pGeoB).DomTessFaceQty[i],
				GeoA.DomTessFaceNb[i] + 1);
  }

  (*pGeoB).DomFaceVerQty = ut_alloc_1d_int ((*pGeoB).DomFaceQty + 1);
  (*pGeoB).DomFaceVerNb = ut_alloc_1d_pint ((*pGeoB).DomFaceQty + 1);
  (*pGeoB).DomFaceEdgeNb = ut_alloc_1d_pint ((*pGeoB).DomFaceQty + 1);

  ut_array_1d_int_memcpy ((*pGeoB).DomFaceVerQty + 1, (*pGeoB).DomFaceQty,
                              GeoA.DomFaceVerQty + 1);

  for (i = 1; i <= (*pGeoB).DomFaceQty; i++)
  {
    (*pGeoB).DomFaceVerNb[i] = ut_alloc_1d_int ((*pGeoB).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pGeoB).DomFaceVerNb[i] + 1,
	                    (*pGeoB).DomFaceVerQty[i],
	                    GeoA.DomFaceVerNb[i] + 1);

    (*pGeoB).DomFaceEdgeNb[i] = ut_alloc_1d_int ((*pGeoB).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pGeoB).DomFaceEdgeNb[i] + 1,
	                    (*pGeoB).DomFaceVerQty[i],
	                    GeoA.DomFaceEdgeNb[i] + 1);
  }

  return;
}
