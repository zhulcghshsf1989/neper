/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_compress_lcl.h"

void
neut_tess_compress_moveface (struct TESS* pTess, int old, int new)
{
  int i, pos, poly, edge;

  /* moving face parameters */
  ut_array_1d_int_memcpy ((*pTess).FaceDom[new], 2, (*pTess).FaceDom[old]);

  (*pTess).FaceFF[new]    = (*pTess).FaceFF[old];

  (*pTess).FacePt[new]    = (*pTess).FacePt[old];
  
  ut_array_1d_memcpy ((*pTess).FacePtCoo[new], 3, (*pTess).FacePtCoo[old]);

  (*pTess).FaceState[new]   = (*pTess).FaceState[old];
  
  ut_array_1d_memcpy ((*pTess).FaceEq[new], 4, (*pTess).FaceEq[old]);
  
  (*pTess).FaceEdgeOri[new] = ut_realloc_1d_int
    ((*pTess).FaceEdgeOri[new], (*pTess).FaceVerQty[old] + 1);

  ut_array_1d_int_memcpy ((*pTess).FaceEdgeOri[new] + 1,
      (*pTess).FaceVerQty[old], (*pTess).FaceEdgeOri[old] + 1);

  (*pTess).FaceEdgeNb[new] = ut_realloc_1d_int
    ((*pTess).FaceEdgeNb[new], (*pTess).FaceVerQty[old] + 1);

  ut_array_1d_int_memcpy ((*pTess).FaceEdgeNb[new] + 1,
      (*pTess).FaceVerQty[old], (*pTess).FaceEdgeNb[old] + 1);

  (*pTess).FaceVerNb[new] = ut_realloc_1d_int
    ((*pTess).FaceVerNb[new], (*pTess).FaceVerQty[old] + 1);

  ut_array_1d_int_memcpy ((*pTess).FaceVerNb[new] + 1,
      (*pTess).FaceVerQty[old], (*pTess).FaceVerNb[old] + 1);
  
  (*pTess).FaceVerQty[new] = (*pTess).FaceVerQty[old];
  
  ut_array_1d_int_memcpy ((*pTess).FacePoly[new], 2,
                          (*pTess).FacePoly[old]);

  /* correcting poly face parameters */
  for (i = 0; i < 2; i++)
    if ((*pTess).FacePoly[new][i] > 0)
    {
      poly = (*pTess).FacePoly[new][i];

      pos = ut_array_1d_int_eltpos ((*pTess).PolyFaceNb[poly] + 1,
	  (*pTess).PolyFaceQty[poly], old);

      if (pos == -1)
      {
	printf ("PolyFaceNb\n");
        abort ();
      }

      pos++;
      (*pTess).PolyFaceNb[poly][pos] = new;
    }

  /* correcting edge face parameters */
  for (i = 1; i <= (*pTess).FaceVerQty[new]; i++)
  {
    edge = (*pTess).FaceEdgeNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pTess).EdgeFaceNb[edge],
	(*pTess).EdgeFaceQty[edge], old);

    if (pos == -1)
    {
      printf ("EdgeFaceNb\n");
      abort ();
    }

    (*pTess).EdgeFaceNb[edge][pos] = new;
  }

  return;
}

void
neut_tess_compress_moveedge (struct TESS* pTess, int old, int new)
{
  int i, pos, face, ver;

  /* moving edge parameters */
  (*pTess).EdgeDel[new] = (*pTess).EdgeDel[old];
  (*pTess).EdgeState[new] = (*pTess).EdgeState[old];
  (*pTess).EdgeLength[new] = (*pTess).EdgeLength[old];
  (*pTess).EdgeFaceQty[new] = (*pTess).EdgeFaceQty[old];

  ut_array_1d_int_memcpy ((*pTess).EdgeDom[new], 2, (*pTess).EdgeDom[old]);

  (*pTess).EdgeFaceNb[new] = ut_realloc_1d_int ((*pTess).EdgeFaceNb[new], (*pTess).EdgeFaceQty[old]);

  ut_array_1d_int_memcpy ((*pTess).EdgeFaceNb[new],
      (*pTess).EdgeFaceQty[old], (*pTess).EdgeFaceNb[old]);
  
  ut_array_1d_int_memcpy ((*pTess).EdgeVerNb[new], 2,
      (*pTess).EdgeVerNb[old]);

  /* correcting face edge parameters */
  for (i = 0; i < (*pTess).EdgeFaceQty[new]; i++)
  {
    face = (*pTess).EdgeFaceNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pTess).FaceEdgeNb[face] + 1,
	(*pTess).FaceVerQty[face], old);

    if (pos == -1)
    {
      printf ("FaceEdgeNb\n");
      abort ();
    }

    pos++;
    (*pTess).FaceEdgeNb[face][pos] = new;
  }

  /* correcting ver edge parameters */
  for (i = 0; i < 2; i++)
  {
    ver = (*pTess).EdgeVerNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pTess).VerEdgeNb[ver],
	(*pTess).VerEdgeQty[ver], old);

    if (pos == -1)
    {
      printf ("VerEdgeNb\n");
      abort ();
    }

    (*pTess).VerEdgeNb[ver][pos] = new;
  }

  return;
}

void
neut_tess_compress_movever (struct TESS* pTess, int old, int new)
{
  int i, pos, edge, faceqty;
  int* face = NULL; 

  /* moving ver parameters */
  (*pTess).VerState[new] = (*pTess).VerState[old];
  
  ut_array_1d_int_memcpy ((*pTess).VerDom[new], 2, (*pTess).VerDom[old]);
  
  (*pTess).VerEdgeQty[new] = (*pTess).VerEdgeQty[old];

  (*pTess).VerEdgeNb[new] = ut_realloc_1d_int ((*pTess).VerEdgeNb[new], (*pTess).VerEdgeQty[old]);

  ut_array_1d_int_memcpy ((*pTess).VerEdgeNb[new], (*pTess).VerEdgeQty[old], (*pTess).VerEdgeNb[old]);
  
  ut_array_1d_memcpy ((*pTess).VerCoo[new], 3, (*pTess).VerCoo[old]);

  /* correcting edge ver parameters */
  for (i = 0; i < (*pTess).VerEdgeQty[new]; i++)
  {
    edge = (*pTess).VerEdgeNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pTess).EdgeVerNb[edge], 2, old);

    if (pos == -1)
    {
      printf ("EdgeVerNb\n");
      abort ();
    }

    (*pTess).EdgeVerNb[edge][pos] = new;
  }

  neut_tess_ver_faces (*pTess, new, &face, &faceqty);
  for (i = 0; i < faceqty; i++)
  {
    pos = ut_array_1d_int_eltpos ((*pTess).FaceVerNb[face[i]] + 1,
	(*pTess).FaceVerQty[face[i]], old);

    if (pos == -1)
    {
      printf ("FaceVerNb\n");
      abort ();
    }

    pos++;
    (*pTess).FaceVerNb[face[i]][pos] = new;
  }

  ut_free_1d_int (face);

  return;
}
