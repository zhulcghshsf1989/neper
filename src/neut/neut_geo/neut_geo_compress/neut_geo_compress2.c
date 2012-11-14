/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_compress_lcl.h"

/* This function deletes the dummy faces in a Geo */
void
neut_geo_compress_faces (struct GEO* pGeo)
{
  int i, pos;
  
  pos = 0;
  for (i = 1; i <= (*pGeo).FaceQty; i++)
    if ((*pGeo).FaceState[i] != -1)
      if (++pos < i)
	neut_geo_compress_moveface (pGeo, i, pos);
  
  // realloc'ing
  (*pGeo).FacePoly = ut_realloc_2d_int_delline ((*pGeo).FacePoly, (*pGeo).FaceQty + 1, pos + 1);
  (*pGeo).FaceEq = ut_realloc_2d_delline ((*pGeo).FaceEq, (*pGeo).FaceQty + 1, pos + 1);
  (*pGeo).FaceVerQty = ut_realloc_1d_int ((*pGeo).FaceVerQty, pos + 1);
  (*pGeo).FaceVerNb = ut_realloc_2d_int_delline ((*pGeo).FaceVerNb, (*pGeo).FaceQty + 1, pos + 1);
  (*pGeo).FaceEdgeNb = ut_realloc_2d_int_delline ((*pGeo).FaceEdgeNb, (*pGeo).FaceQty + 1, pos + 1);
  (*pGeo).FaceEdgeOri = ut_realloc_2d_int_delline ((*pGeo).FaceEdgeOri, (*pGeo).FaceQty + 1, pos + 1);
  (*pGeo).FaceState = ut_realloc_1d_int ((*pGeo).FaceState, pos + 1);
  (*pGeo).FacePt = ut_realloc_1d_int ((*pGeo).FacePt, pos + 1);
  (*pGeo).FacePtCoo = ut_realloc_2d_delline ((*pGeo).FacePtCoo, (*pGeo).FaceQty + 1, pos + 1);
  (*pGeo).FaceFF = ut_realloc_1d ((*pGeo).FaceFF, pos + 1);
  (*pGeo).FaceDom = ut_realloc_2d_int_delline ((*pGeo).FaceDom, (*pGeo).FaceQty + 1, pos + 1);

  (*pGeo).FaceQty = pos;

  return;
}

/* This function deletes the dummy edges in a Geo */
void
neut_geo_compress_edges (struct GEO* pGeo)
{
  int i, pos;
  
  pos = 0;
  for (i = 1; i <= (*pGeo).EdgeQty; i++)
    if ((*pGeo).EdgeState[i] == 0)
      if (++pos < i)
	neut_geo_compress_moveedge (pGeo, i, pos);

  // realloc'ing
  (*pGeo).EdgeVerNb = ut_realloc_2d_int_delline ((*pGeo).EdgeVerNb, (*pGeo).EdgeQty + 1, pos + 1);
  (*pGeo).EdgeFaceQty = ut_realloc_1d_int ((*pGeo).EdgeFaceQty, pos + 1);
  (*pGeo).EdgeFaceNb = ut_realloc_2d_int_delline ((*pGeo).EdgeFaceNb, (*pGeo).EdgeQty + 1, pos + 1);
  (*pGeo).EdgeLength = ut_realloc_1d ((*pGeo).EdgeLength, pos + 1);
  (*pGeo).EdgeState = ut_realloc_1d_int ((*pGeo).EdgeState, pos + 1);
  (*pGeo).EdgeDel = ut_realloc_1d_int ((*pGeo).EdgeDel, pos + 1);
  (*pGeo).EdgeDom = ut_realloc_2d_int_delline ((*pGeo).EdgeDom, (*pGeo).EdgeQty + 1, pos + 1);
  
  (*pGeo).EdgeQty = pos;

  return;
}

/* This function deletes the dummy vertices in a Geo */
void
neut_geo_compress_vers (struct GEO* pGeo)
{
  int i, pos;
  
  pos = 0;
  for (i = 1; i <= (*pGeo).VerQty; i++)
    if ((*pGeo).VerState[i] != -1)
      if (++pos < i)
	neut_geo_compress_movever (pGeo, i, pos);
  
  // realloc'ing
  (*pGeo).VerCoo = ut_realloc_2d_delline ((*pGeo).VerCoo, (*pGeo).VerQty + 1, pos + 1);
  (*pGeo).VerEdgeQty = ut_realloc_1d_int ((*pGeo).VerEdgeQty, pos + 1);
  (*pGeo).VerEdgeNb = ut_realloc_2d_int_delline ((*pGeo).VerEdgeNb, (*pGeo).VerQty + 1, pos + 1);
  (*pGeo).VerDom = ut_realloc_2d_int_delline ((*pGeo).VerDom, (*pGeo).VerQty + 1, pos + 1);
  (*pGeo).VerState = ut_realloc_1d_int ((*pGeo).VerState, pos + 1);

  (*pGeo).VerQty = pos;

  return;
}
