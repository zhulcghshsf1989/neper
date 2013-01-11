/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_tess_lcl.h"

void
neut_tess_tessGen (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tessVerQty (TessA, pTessB);
  neut_tess_tessEdgeQty (TessA, pTessB);
  neut_tess_tessFaceQty (TessA, pTessB);
  neut_tess_tessPolyQty (TessA, pTessB);

  (*pTessB).N = TessA.N;
  (*pTessB).Id = TessA.Id;

  (*pTessB).morpho = ut_alloc_1d_char (strlen (TessA.morpho) + 1);
  strcpy ((*pTessB).morpho, TessA.morpho);

  (*pTessB).Type = ut_alloc_1d_char (strlen (TessA.Type) + 1);
  strcpy ((*pTessB).Type, TessA.Type);

  if (TessA.PolyId != NULL)
  {
    (*pTessB).PolyId = ut_alloc_1d_int (TessA.PolyQty + 1);
    ut_array_1d_int_memcpy ((*pTessB).PolyId + 1, TessA.PolyQty,
	TessA.PolyId + 1);
  }

  (*pTessB).sel = TessA.sel;
  (*pTessB).maxff = TessA.maxff;
  if (TessA.dbound != NULL)
  {
    (*pTessB).dbound = ut_alloc_1d_char (strlen (TessA.dbound) + 1);
    strcpy ((*pTessB).dbound, TessA.dbound);
    (*pTessB).dboundsel = TessA.dboundsel;
  }

  return;
}

/* neut_tess_tesstializing the properties of the vertices */
void
neut_tess_tessVer (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tessVerCoo (TessA, pTessB);
  neut_tess_tessVerEdge (TessA, pTessB);
  neut_tess_tessVerBound (TessA, pTessB);
  neut_tess_tessVerState (TessA, pTessB);

  return;
}

/* neut_tess_tesstializing the properties of the edges */
void
neut_tess_tessEdge (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tessEdgeVerNb (TessA, pTessB);
  neut_tess_tessEdgeFace (TessA, pTessB);
  neut_tess_tessEdgeLength (TessA, pTessB);
  neut_tess_tessEdgeDel (TessA, pTessB);
  neut_tess_tessEdgeBound (TessA, pTessB);
  neut_tess_tessEdgeState (TessA, pTessB);

  return;
}

/* neut_tess_tesstializing the properties of the faces */
void
neut_tess_tessFace (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tessFacePoly (TessA, pTessB);
  neut_tess_tessFaceVer (TessA, pTessB);
  neut_tess_tessFaceEdge (TessA, pTessB);
  neut_tess_tessFaceFF (TessA, pTessB);
  neut_tess_tessFaceEq (TessA, pTessB);
  neut_tess_tessFaceBound (TessA, pTessB);
  neut_tess_tessFaceState (TessA, pTessB);
  neut_tess_tessFacePt (TessA, pTessB);

  return;
}

/* neut_tess_tesstializing the properties of the polyhedra */
void
neut_tess_tessPoly (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tessPolyFace (TessA, pTessB);
  neut_tess_tessPolyCenter (TessA, pTessB);
  neut_tess_tessPolyTrue (TessA, pTessB);
  neut_tess_tessPolyBody (TessA, pTessB);

  return;
}

/* neut_tess_tesstializing the properties of the domain */
void
neut_tess_tessDomain (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_tessDomainData (TessA, pTessB);

  return;
}
