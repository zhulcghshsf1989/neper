/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesl_tess.h"

void
neut_tesl_tess_general (struct TESL Tess, struct TESS* pTess)
{
  (*pTess).N  = Tess.PolyQty;

  (*pTess).Id = Tess.Id;

  (*pTess).Type = ut_alloc_1d_char (strlen (Tess.Type) + 1);
  strcpy ((*pTess).Type, Tess.Type);

  (*pTess).morpho = ut_alloc_1d_char (strlen (Tess.morpho) + 1);
  strcpy ((*pTess).morpho, Tess.morpho); 

  return;
}

/* neut_tesl_tesstializing the properties of the vertices */
void
neut_tesl_tess_ver (struct TESL Tess, struct TESS *pTess)
{
  (*pTess).VerQty = Tess.VerQty;

  neut_tesl_tessVerCoo (Tess, pTess);
  neut_tesl_tessVerEdge (Tess, pTess);
  neut_tesl_tessVerState (pTess);

  return;
}

/* neut_tesl_tesstializing the properties of the edges */
void
neut_tesl_tess_edge (struct TESL Tess, struct TESS *pTess)
{
  (*pTess).EdgeQty = Tess.EdgeQty;

  neut_tesl_tessEdgeVerNb (Tess, pTess);
  neut_tesl_tessEdgeFace (Tess, pTess);
  neut_tess_init_edgelength (pTess);
  neut_tesl_tessEdgeState (pTess);
  neut_tesl_tessEdgeDel (pTess);

  return;
}

/* neut_tesl_tesstializing the properties of the faces */
void
neut_tesl_tess_face (struct TESL Tess, struct TESS *pTess)
{
  (*pTess).FaceQty = Tess.FaceQty;

  neut_tesl_tessFacePoly (Tess, pTess);
  neut_tesl_tessFaceVer (Tess, pTess);
  neut_tesl_tessFaceEdge (Tess, pTess);
  neut_tesl_tessFaceFF  (pTess); 
  neut_tesl_tessFaceState (pTess); 
  neut_tesl_tessFacePt (pTess); 
  neut_tesl_tessFaceEq (Tess, pTess);

  return;
}

/* neut_tesl_tesstializing the properties of the polyhedra */
void
neut_tesl_tess_poly (struct TESL Tess, struct TESS *pTess)
{
  (*pTess).PolyQty = Tess.PolyQty;
  
  (*pTess).CenterCoo = ut_alloc_2d (Tess.PolyQty + 1, 3);
  (*pTess).PolyTrue  = ut_alloc_1d_int (Tess.PolyQty + 1);
  (*pTess).PolyBody  = ut_alloc_1d_int (Tess.PolyQty + 1);
  
  ut_array_2d_memcpy ((*pTess).CenterCoo, Tess.PolyQty + 1, 3, Tess.CenterCoo);

  ut_array_1d_int_memcpy ((*pTess).PolyTrue, Tess.PolyQty + 1, Tess.PolyTrue);
  ut_array_1d_int_memcpy ((*pTess).PolyBody, Tess.PolyQty + 1, Tess.PolyBody);

  neut_tesl_tessPolyFace (Tess, pTess);
  neut_tesl_tessPolyEdge (Tess, pTess);
  neut_tesl_tessPolyVer (Tess, pTess);

  return;
}
