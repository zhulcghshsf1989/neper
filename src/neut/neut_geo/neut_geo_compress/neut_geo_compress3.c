/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_compress_lcl.h"

void
neut_geo_compress_moveface (struct GEO* pGeo, int old, int new)
{
  int i, pos, poly, edge;

  /* moving face parameters */
  ut_array_1d_int_memcpy ((*pGeo).FaceDom[new], 2, (*pGeo).FaceDom[old]);

  (*pGeo).FaceFF[new]    = (*pGeo).FaceFF[old];

  (*pGeo).FacePt[new]    = (*pGeo).FacePt[old];
  
  ut_array_1d_memcpy ((*pGeo).FacePtCoo[new], 3, (*pGeo).FacePtCoo[old]);

  (*pGeo).FaceState[new]   = (*pGeo).FaceState[old];
  
  ut_array_1d_memcpy ((*pGeo).FaceEq[new], 4, (*pGeo).FaceEq[old]);
  
  (*pGeo).FaceEdgeOri[new] = ut_realloc_1d_int
    ((*pGeo).FaceEdgeOri[new], (*pGeo).FaceVerQty[old] + 1);

  ut_array_1d_int_memcpy ((*pGeo).FaceEdgeOri[new] + 1,
      (*pGeo).FaceVerQty[old], (*pGeo).FaceEdgeOri[old] + 1);

  (*pGeo).FaceEdgeNb[new] = ut_realloc_1d_int
    ((*pGeo).FaceEdgeNb[new], (*pGeo).FaceVerQty[old] + 1);

  ut_array_1d_int_memcpy ((*pGeo).FaceEdgeNb[new] + 1,
      (*pGeo).FaceVerQty[old], (*pGeo).FaceEdgeNb[old] + 1);

  (*pGeo).FaceVerNb[new] = ut_realloc_1d_int
    ((*pGeo).FaceVerNb[new], (*pGeo).FaceVerQty[old] + 1);

  ut_array_1d_int_memcpy ((*pGeo).FaceVerNb[new] + 1,
      (*pGeo).FaceVerQty[old], (*pGeo).FaceVerNb[old] + 1);
  
  (*pGeo).FaceVerQty[new] = (*pGeo).FaceVerQty[old];
  
  ut_array_1d_int_memcpy ((*pGeo).FacePoly[new], 2,
                          (*pGeo).FacePoly[old]);

  /* correcting poly face parameters */
  for (i = 0; i < 2; i++)
    if ((*pGeo).FacePoly[new][i] > 0)
    {
      poly = (*pGeo).FacePoly[new][i];

      pos = ut_array_1d_int_eltpos ((*pGeo).PolyFaceNb[poly] + 1,
	  (*pGeo).PolyFaceQty[poly], old);

      if (pos == -1)
      {
	printf ("PolyFaceNb\n");
        abort ();
      }

      pos++;
      (*pGeo).PolyFaceNb[poly][pos] = new;
    }

  /* correcting edge face parameters */
  for (i = 1; i <= (*pGeo).FaceVerQty[new]; i++)
  {
    edge = (*pGeo).FaceEdgeNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pGeo).EdgeFaceNb[edge],
	(*pGeo).EdgeFaceQty[edge], old);

    if (pos == -1)
    {
      printf ("EdgeFaceNb\n");
      abort ();
    }

    (*pGeo).EdgeFaceNb[edge][pos] = new;
  }

  return;
}

void
neut_geo_compress_moveedge (struct GEO* pGeo, int old, int new)
{
  int i, pos, face, ver;

  /* moving edge parameters */
  (*pGeo).EdgeDel[new] = (*pGeo).EdgeDel[old];
  (*pGeo).EdgeState[new] = (*pGeo).EdgeState[old];
  (*pGeo).EdgeLength[new] = (*pGeo).EdgeLength[old];
  (*pGeo).EdgeFaceQty[new] = (*pGeo).EdgeFaceQty[old];

  ut_array_1d_int_memcpy ((*pGeo).EdgeDom[new], 2, (*pGeo).EdgeDom[old]);

  (*pGeo).EdgeFaceNb[new] = ut_realloc_1d_int ((*pGeo).EdgeFaceNb[new], (*pGeo).EdgeFaceQty[old]);

  ut_array_1d_int_memcpy ((*pGeo).EdgeFaceNb[new],
      (*pGeo).EdgeFaceQty[old], (*pGeo).EdgeFaceNb[old]);
  
  ut_array_1d_int_memcpy ((*pGeo).EdgeVerNb[new], 2,
      (*pGeo).EdgeVerNb[old]);

  /* correcting face edge parameters */
  for (i = 0; i < (*pGeo).EdgeFaceQty[new]; i++)
  {
    face = (*pGeo).EdgeFaceNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pGeo).FaceEdgeNb[face] + 1,
	(*pGeo).FaceVerQty[face], old);

    if (pos == -1)
    {
      printf ("FaceEdgeNb\n");
      abort ();
    }

    pos++;
    (*pGeo).FaceEdgeNb[face][pos] = new;
  }

  /* correcting ver edge parameters */
  for (i = 0; i < 2; i++)
  {
    ver = (*pGeo).EdgeVerNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pGeo).VerEdgeNb[ver],
	(*pGeo).VerEdgeQty[ver], old);

    if (pos == -1)
    {
      printf ("VerEdgeNb\n");
      abort ();
    }

    (*pGeo).VerEdgeNb[ver][pos] = new;
  }

  return;
}

void
neut_geo_compress_movever (struct GEO* pGeo, int old, int new)
{
  int i, pos, edge, faceqty;
  int* face = NULL; 

  /* moving ver parameters */
  (*pGeo).VerState[new] = (*pGeo).VerState[old];
  
  ut_array_1d_int_memcpy ((*pGeo).VerDom[new], 2, (*pGeo).VerDom[old]);
  
  (*pGeo).VerEdgeQty[new] = (*pGeo).VerEdgeQty[old];

  (*pGeo).VerEdgeNb[new] = ut_realloc_1d_int ((*pGeo).VerEdgeNb[new], (*pGeo).VerEdgeQty[old]);

  ut_array_1d_int_memcpy ((*pGeo).VerEdgeNb[new], (*pGeo).VerEdgeQty[old], (*pGeo).VerEdgeNb[old]);
  
  ut_array_1d_memcpy ((*pGeo).VerCoo[new], 3, (*pGeo).VerCoo[old]);

  /* correcting edge ver parameters */
  for (i = 0; i < (*pGeo).VerEdgeQty[new]; i++)
  {
    edge = (*pGeo).VerEdgeNb[new][i];

    pos = ut_array_1d_int_eltpos ((*pGeo).EdgeVerNb[edge], 2, old);

    if (pos == -1)
    {
      printf ("EdgeVerNb\n");
      abort ();
    }

    (*pGeo).EdgeVerNb[edge][pos] = new;
  }

  neut_geo_ver_faces (*pGeo, new, &face, &faceqty);
  for (i = 0; i < faceqty; i++)
  {
    pos = ut_array_1d_int_eltpos ((*pGeo).FaceVerNb[face[i]] + 1,
	(*pGeo).FaceVerQty[face[i]], old);

    if (pos == -1)
    {
      printf ("FaceVerNb\n");
      abort ();
    }

    pos++;
    (*pGeo).FaceVerNb[face[i]][pos] = new;
  }

  ut_free_1d_int (face);

  return;
}
