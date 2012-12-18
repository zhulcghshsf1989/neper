/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_tess_lcl.h"

/* neut_tess_tesstializing the properties of the vertices ******************* */

void
neut_tess_tessVerQty (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).VerQty = TessA.VerQty;

  return;
}

void
neut_tess_tessVerCoo (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).VerCoo = ut_alloc_2d ((*pTessB).VerQty + 1, 3);

  for (i = 1; i <= (*pTessB).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTessB).VerCoo[i][j] = TessA.VerCoo[i][j];

  return;
}

void
neut_tess_tessVerEdge (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).VerEdgeQty = ut_alloc_1d_int ((*pTessB).VerQty + 1);

  (*pTessB).VerEdgeNb  = ut_alloc_1d_pint ((*pTessB).VerQty + 1);

  for (i = 1; i <= (*pTessB).VerQty; i++)
  {
    (*pTessB).VerEdgeQty[i] = TessA.VerEdgeQty[i];

    (*pTessB).VerEdgeNb[i] = ut_alloc_1d_int ((*pTessB).VerEdgeQty[i]);

    for (j = 0; j < (*pTessB).VerEdgeQty[i]; j++)
      (*pTessB).VerEdgeNb[i][j] = TessA.VerEdgeNb[i][j];
  }

  return;
}

void
neut_tess_tessVerBound (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).VerDom = ut_alloc_2d_int ((*pTessB).VerQty + 1, 2);
  ut_array_2d_int_memcpy ((*pTessB).VerDom + 1, (*pTessB).VerQty, 2, TessA.VerDom + 1);

  return;
}

void
neut_tess_tessVerState (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).VerState = ut_alloc_1d_int ((*pTessB).VerQty + 1);

  for (i = 1; i <= (*pTessB).VerQty; i++)
    (*pTessB).VerState[i] = TessA.VerState[i];

  return;
}

/* Initializing the properties of the edges ******************* */

void
neut_tess_tessEdgeQty (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).EdgeQty = TessA.EdgeQty;

  return;
}

void
neut_tess_tessEdgeVerNb (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).EdgeVerNb = ut_alloc_2d_int ((*pTessB).EdgeQty + 1, 2);

  for (i = 1; i <= (*pTessB).EdgeQty; i++)
    for (j = 0; j < 2; j++)
      (*pTessB).EdgeVerNb[i][j] = TessA.EdgeVerNb[i][j];

  return;
}

void
neut_tess_tessEdgeFace (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).EdgeFaceQty = ut_alloc_1d_int ((*pTessB).EdgeQty + 1);
  (*pTessB).EdgeFaceNb  = ut_alloc_1d_pint ((*pTessB).EdgeQty + 1);

  for (i = 1; i <= TessA.EdgeQty; i++)
  {
    (*pTessB).EdgeFaceQty[i] = TessA.EdgeFaceQty[i];

    (*pTessB).EdgeFaceNb[i] = ut_alloc_1d_int ((*pTessB).EdgeFaceQty[i] + 1);

    for (j = 0; j < (*pTessB).EdgeFaceQty[i]; j++)
      (*pTessB).EdgeFaceNb[i][j] = TessA.EdgeFaceNb[i][j];
  }

  return;
}

void
neut_tess_tessEdgeLength (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).EdgeLength = ut_alloc_1d ((*pTessB).EdgeQty + 1);

  for (i = 1; i <= (*pTessB).EdgeQty; i++)
    (*pTessB).EdgeLength[i] = TessA.EdgeLength[i];

  return;
}

void
neut_tess_tessEdgeBound (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).EdgeDom = ut_alloc_2d_int ((*pTessB).EdgeQty + 1, 2);

  ut_array_2d_int_memcpy ((*pTessB).EdgeDom + 1,
			  (*pTessB).EdgeQty, 2, TessA.EdgeDom + 1);

  return;
}

void
neut_tess_tessEdgeDel (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).EdgeDel = ut_alloc_1d_int ((*pTessB).EdgeQty + 1);

  for (i = 1; i <= (*pTessB).EdgeQty; i++)
    (*pTessB).EdgeDel[i] = TessA.EdgeDel[i];

  return;
}

void
neut_tess_tessEdgeState (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).EdgeState = ut_alloc_1d_int ((*pTessB).EdgeQty + 1);

  for (i = 1; i <= (*pTessB).EdgeQty; i++)
    (*pTessB).EdgeState[i] = TessA.EdgeState[i];

  return;
}

/* neut_tess_tesstializing the properties of the faces ******************* */

void
neut_tess_tessFaceQty (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).FaceQty = TessA.FaceQty;

  return;
}

void
neut_tess_tessFacePoly (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FacePoly = ut_alloc_2d_int ((*pTessB).FaceQty + 1, 2);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
    for (j = 0; j < 2; j++)
      (*pTessB).FacePoly[i][j] = TessA.FacePoly[i][j];

  return;
}

void
neut_tess_tessFaceVer (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FaceVerQty = ut_alloc_1d_int ((*pTessB).FaceQty + 1);
  (*pTessB).FaceVerNb = ut_alloc_1d_pint ((*pTessB).FaceQty + 1);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
  {
    (*pTessB).FaceVerQty[i] = TessA.FaceVerQty[i];

    (*pTessB).FaceVerNb[i] =
      ut_alloc_1d_int ((*pTessB).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTessB).FaceVerQty[i]; j++)
      (*pTessB).FaceVerNb[i][j] = TessA.FaceVerNb[i][j];
  }

  return;
}

/* a preliminar neut_tess_tessFaceVer is needed! */
void
neut_tess_tessFaceEdge (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FaceEdgeNb = ut_alloc_1d_pint ((*pTessB).FaceQty + 1);
  (*pTessB).FaceEdgeOri = ut_alloc_1d_pint ((*pTessB).FaceQty + 1);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
  {
    (*pTessB).FaceEdgeNb[i] =
      ut_alloc_1d_int ((*pTessB).FaceVerQty[i] + 1);
    (*pTessB).FaceEdgeOri[i] =
      ut_alloc_1d_int ((*pTessB).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTessB).FaceVerQty[i]; j++)
    {
      (*pTessB).FaceEdgeNb[i][j] = TessA.FaceEdgeNb[i][j];
      (*pTessB).FaceEdgeOri[i][j] = TessA.FaceEdgeOri[i][j];
    }
  }

  return;
}

void
neut_tess_tessFaceFF (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).FaceFF = ut_alloc_1d ((*pTessB).FaceQty + 1);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
    (*pTessB).FaceFF[i] = TessA.FaceFF[i];

  return;
}

void
neut_tess_tessFaceEq (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FaceEq = ut_alloc_2d ((*pTessB).FaceQty + 1, 4);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
    for (j = 0; j < 4; j++)
      (*pTessB).FaceEq[i][j] = TessA.FaceEq[i][j];

  return;
}

void
neut_tess_tessFaceBound (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).FaceDom = ut_alloc_2d_int ((*pTessB).FaceQty + 1, 2);

  ut_array_2d_int_memcpy ((*pTessB).FaceDom + 1,
			  (*pTessB).FaceQty, 2, TessA.FaceDom + 1);

  return;
}

void
neut_tess_tessFaceState (struct TESS TessA, struct TESS *pTessB)
{
  int i;
  
  (*pTessB).FaceState = ut_alloc_1d_int ((*pTessB).FaceQty + 1);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
    (*pTessB).FaceState[i] = TessA.FaceState[i];

  return;
}

void
neut_tess_tessFacePt (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).FacePt = ut_alloc_1d_int ((*pTessB).FaceQty + 1);
  (*pTessB).FacePtCoo = ut_alloc_2d ((*pTessB).FaceQty + 1, 3);

  for (i = 1; i <= (*pTessB).FaceQty; i++)
  {
    (*pTessB).FacePt[i] = TessA.FacePt[i];
    for (j = 0; j < 3; j++)
      (*pTessB).FacePtCoo[i][j] = TessA.FacePtCoo[i][j];
  }

  return;
}

/* neut_tess_tesstializing the properties of the faces ******************* */

void
neut_tess_tessPolyQty (struct TESS TessA, struct TESS *pTessB)
{
  (*pTessB).PolyQty = TessA.PolyQty;

  return;
}

void
neut_tess_tessPolyFace (struct TESS TessA, struct TESS *pTessB)
{
  int i, j;

  (*pTessB).PolyFaceQty = ut_alloc_1d_int ((*pTessB).PolyQty + 1);
  (*pTessB).PolyFaceNb = ut_alloc_1d_pint ((*pTessB).PolyQty + 1);
  (*pTessB).PolyFaceOri = ut_alloc_1d_pint ((*pTessB).PolyQty + 1);

  for (i = 1; i <= (*pTessB).PolyQty; i++)
  {
    (*pTessB).PolyFaceQty[i] = TessA.PolyFaceQty[i];
    
    (*pTessB).PolyFaceNb[i] =
      ut_alloc_1d_int ((*pTessB).PolyFaceQty[i] + 1);
    (*pTessB).PolyFaceOri[i] =
      ut_alloc_1d_int ((*pTessB).PolyFaceQty[i] + 1);

    for (j = 1; j <= (*pTessB).PolyFaceQty[i]; j++)
    {
      (*pTessB).PolyFaceNb[i][j] = TessA.PolyFaceNb[i][j];
      (*pTessB).PolyFaceOri[i][j] = TessA.PolyFaceOri[i][j];
    }
  }

  return;
}

void
neut_tess_tessPolyCenter (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  if (!TessA.CenterCoo)
    return;
  
  (*pTessB).CenterCoo = ut_alloc_2d (TessA.PolyQty + 1, 3);
  for (i = 1; i <= TessA.PolyQty; i++)
    ut_array_1d_memcpy ((*pTessB).CenterCoo[i], 3, TessA.CenterCoo[i]);

  return;
}

void
neut_tess_tessPolyTrue (struct TESS TessA, struct TESS *pTessB)
{
  if (!TessA.PolyTrue)
    return;
  
  (*pTessB).PolyTrue = ut_alloc_1d_int (TessA.PolyQty + 1);
  ut_array_1d_int_memcpy ((*pTessB).PolyTrue + 1, TessA.PolyQty, TessA.PolyTrue + 1);

  return;
}

void
neut_tess_tessPolyBody (struct TESS TessA, struct TESS *pTessB)
{
  if (!TessA.PolyBody)
    return;
  
  (*pTessB).PolyBody = ut_alloc_1d_int (TessA.PolyQty + 1);
  ut_array_1d_int_memcpy ((*pTessB).PolyBody + 1, TessA.PolyQty, TessA.PolyBody + 1);

  return;
}

void
neut_tess_tessDomainData (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).DomType = ut_alloc_1d_char (strlen (TessA.DomType) + 1);
  strcpy ((*pTessB).DomType, TessA.DomType);

  // ver
 
  (*pTessB).DomVerQty = TessA.DomVerQty;

  (*pTessB).DomTessVerNb = ut_alloc_1d_int ((*pTessB).DomVerQty + 1);
  ut_array_1d_int_memcpy ((*pTessB).DomTessVerNb + 1, (*pTessB).DomVerQty,
                              TessA.DomTessVerNb + 1);

  (*pTessB).DomVerEdgeQty = ut_alloc_1d_int ((*pTessB).DomVerQty + 1);
  (*pTessB).DomVerEdgeNb  = ut_alloc_1d_pint ((*pTessB).DomVerQty + 1);

  ut_array_1d_int_memcpy ((*pTessB).DomVerEdgeQty + 1, (*pTessB).DomVerQty,
                              TessA.DomVerEdgeQty + 1);

  for (i = 1; i <= (*pTessB).DomVerQty; i++)
  {
    (*pTessB).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pTessB).DomVerEdgeQty[i]);
    ut_array_1d_int_memcpy ((*pTessB).DomVerEdgeNb[i], (*pTessB).DomVerEdgeQty[i], 
	                        TessA.DomVerEdgeNb[i]);
  }

  // edge
  
  (*pTessB).DomEdgeQty = TessA.DomEdgeQty;

  (*pTessB).DomEdgeVerNb = ut_alloc_2d_int ((*pTessB).DomEdgeQty + 1, 2);
  ut_array_2d_int_memcpy ((*pTessB).DomEdgeVerNb + 1, (*pTessB).DomEdgeQty, 2,
                              TessA.DomEdgeVerNb + 1);

  (*pTessB).DomEdgeFaceNb = ut_alloc_2d_int ((*pTessB).DomEdgeQty + 1, 2);
  ut_array_2d_int_memcpy ((*pTessB).DomEdgeFaceNb + 1, (*pTessB).DomEdgeQty, 2,
                              TessA.DomEdgeFaceNb + 1);

  (*pTessB).DomTessEdgeQty = ut_alloc_1d_int ((*pTessB).DomEdgeQty + 1);
  (*pTessB).DomTessEdgeNb = ut_alloc_1d_pint ((*pTessB).DomEdgeQty + 1);

  ut_array_1d_int_memcpy ((*pTessB).DomTessEdgeQty + 1, (*pTessB).DomEdgeQty,
			      TessA.DomTessEdgeQty + 1);

  for (i = 1; i <= (*pTessB).DomEdgeQty; i++)
  {
    (*pTessB).DomTessEdgeNb[i] = ut_alloc_1d_int ((*pTessB).DomTessEdgeQty[i] + 1);
    ut_array_1d_int_memcpy ((*pTessB).DomTessEdgeNb[i] + 1, (*pTessB).DomTessEdgeQty[i],
	                        TessA.DomTessEdgeNb[i] + 1);
  }

  // face
  
  (*pTessB).DomFaceQty = TessA.DomFaceQty;

  (*pTessB).DomFaceEq = ut_alloc_2d ((*pTessB).DomFaceQty + 1, 4);
  ut_array_2d_memcpy ((*pTessB).DomFaceEq + 1, (*pTessB).DomFaceQty, 4,
                          TessA.DomFaceEq + 1);
  (*pTessB).DomFaceLabel = ut_alloc_1d_pchar ((*pTessB).DomFaceQty + 1);
  ut_array_1d_pchar_memcpy ((*pTessB).DomFaceLabel + 1, (*pTessB).DomFaceQty,
                                TessA.DomFaceLabel + 1);


  (*pTessB).DomTessFaceQty = ut_alloc_1d_int ((*pTessB).DomFaceQty + 1);
  ut_array_1d_int_memcpy ((*pTessB).DomTessFaceQty + 1, (*pTessB).DomFaceQty,
                              TessA.DomTessFaceQty + 1);

  (*pTessB).DomTessFaceNb = ut_alloc_1d_pint ((*pTessB).DomFaceQty + 1);

  for (i = 1; i <= (*pTessB).DomFaceQty; i++)
  {
    (*pTessB).DomTessFaceNb[i] = ut_alloc_1d_int ((*pTessB).DomTessFaceQty[i] + 1);
    ut_array_1d_int_memcpy ((*pTessB).DomTessFaceNb[i] + 1, (*pTessB).DomTessFaceQty[i],
				TessA.DomTessFaceNb[i] + 1);
  }

  (*pTessB).DomFaceVerQty = ut_alloc_1d_int ((*pTessB).DomFaceQty + 1);
  (*pTessB).DomFaceVerNb = ut_alloc_1d_pint ((*pTessB).DomFaceQty + 1);
  (*pTessB).DomFaceEdgeNb = ut_alloc_1d_pint ((*pTessB).DomFaceQty + 1);

  ut_array_1d_int_memcpy ((*pTessB).DomFaceVerQty + 1, (*pTessB).DomFaceQty,
                              TessA.DomFaceVerQty + 1);

  for (i = 1; i <= (*pTessB).DomFaceQty; i++)
  {
    (*pTessB).DomFaceVerNb[i] = ut_alloc_1d_int ((*pTessB).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pTessB).DomFaceVerNb[i] + 1,
	                    (*pTessB).DomFaceVerQty[i],
	                    TessA.DomFaceVerNb[i] + 1);

    (*pTessB).DomFaceEdgeNb[i] = ut_alloc_1d_int ((*pTessB).DomFaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pTessB).DomFaceEdgeNb[i] + 1,
	                    (*pTessB).DomFaceVerQty[i],
	                    TessA.DomFaceEdgeNb[i] + 1);
  }

  return;
}
