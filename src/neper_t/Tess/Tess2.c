/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Tess.h"

/* Recording of the tessellation generalities in Tess, i.e. number
 * of poly, vertices, faces and edges.
 */
void
TessGenDet (struct GERMSET GermSet, struct POLY *Poly, struct TESS *pTess)
{
  (*pTess).Type = ut_alloc_1d_char (10);
  strcpy ((*pTess).Type, "standard"); 

  (*pTess).Id = GermSet.Id;
  (*pTess).morpho = ut_realloc_1d_char ((*pTess).morpho, 10);

  strcpy ((*pTess).morpho, GermSet.morpho);

  (*pTess).PolyQty = TPolyQty (GermSet);
  (*pTess).FaceQty = TFaceQty (GermSet, Poly);
  (*pTess).VerQty = TVerQty (GermSet, Poly);
  (*pTess).EdgeQty = TEdgeQty (GermSet, Poly);

  return;
}

void
TessAlloc (struct TESS *pTess)
{
  (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
  (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);
  (*pTess).FaceVerQty = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  (*pTess).FaceVerNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeNb = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).FaceEdgeOri = ut_alloc_1d_pint ((*pTess).FaceQty + 1);
  (*pTess).VerGerm = ut_alloc_2d_int ((*pTess).VerQty + 1, 4);
  (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
  (*pTess).CenterCoo = ut_alloc_2d ((*pTess).PolyQty + 1, 3);
  (*pTess).PolyVerQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyVerNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).PolyEdgeQty = ut_alloc_1d_int ((*pTess).PolyQty + 1);
  (*pTess).PolyEdgeNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  (*pTess).EdgeVerNb = ut_alloc_1d_pint (1);
  (*pTess).EdgeVerNb[0] = NULL;

  return;
}

/* Recording of TFNb & TVNb */
void
IniTxNb (struct TESS Tess, struct POLY *Poly, int **TFNb, int **TVNb)
{
  /* Recording of TFNb */
  IniTFNb (Tess, Poly, TFNb);
  /* Recording of TVNb */
  IniTVNb (Tess, Poly, TVNb);
  
  return;
}

/* For each polyhedron, recording of FacePoly, FaceEq & FaceVerQty */
void
IniTessFaceOr (struct TESS *pTess, struct POLY *Poly, int **TFNb)
{
  /* Recording of Face germ */
  IniTessFacePoly (pTess, Poly, TFNb);
  /* Recording of Face eq */
  IniTessFaceEq (pTess, Poly, TFNb);
  /* Recording of Face ver qty */
  IniTessFaceVerQty (pTess, Poly, TFNb);

  return;
}

/* For each polyhedron, recording of FaceVerNb */
void
IniTessFaceBound (struct TESS *pTess, struct POLY *Poly, int **TFNb,
		  int **TVNb)
{
  int i, j, k;
  int FNb;

  /* for every face of every polyhedron */
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      /* if it has not been treated previously */
      if (i < Poly[i].FacePoly[j] || Poly[i].FacePoly[j] < 0)
      {
	/* the face tess number is recorded */
	FNb = TFNb[i][j];

	(*pTess).FaceVerNb[FNb] =
	  ut_alloc_1d_int ((*pTess).FaceVerQty[FNb] + 1);

	for (k = 1; k <= (*pTess).FaceVerQty[FNb]; k++)
	  (*pTess).FaceVerNb[FNb][k] = TVNb[i][Poly[i].FaceVerNb[j][k]];
      }

  return;
}

/* Recording of vertex properties */
void
IniTessVer (struct TESS *pTess, struct POLY *Poly, int **TVNb)
{
  /* Recording of the vertex coordinates */
  IniTessVerCoo (pTess, Poly, TVNb);
  /* Recording of the numbers of the ver parent germs */
  IniTessVerGerm (pTess, Poly, TVNb);

  return;
}

/* Recording of the numbers of face and vertex of the polyhedra */
void
IniTessPolyCenNXQty (struct GERMSET GermSet, struct TESS *pTess,
		     struct POLY *Poly)
{
  int num;
  int i;
  
  for (num = 1; num <= (*pTess).PolyQty; num++)
  {
    for (i = 0; i < 3; i++)
      (*pTess).CenterCoo[num][i] = GermSet.GermsCoo[num][i + 1];
    (*pTess).PolyFaceQty[num] = Poly[num].FaceQty;
    (*pTess).PolyVerQty[num] = Poly[num].VerQty;
  }

  return;
}

/* Recording of the face and vertex numbers */
void
IniTessPolyXNb (struct TESS *pTess, struct POLY *Poly, int **TVNb, int **TFNb)
{
  int num, i;

  for (num = 1; num <= (*pTess).PolyQty; num++)
  {
    (*pTess).PolyVerNb[num] = ut_alloc_1d_int (Poly[num].VerQty + 1);
    for (i = 1; i <= Poly[num].VerQty; i++)
      (*pTess).PolyVerNb[num][i] = TVNb[num][i];

    (*pTess).PolyFaceNb[num] = ut_alloc_1d_int (Poly[num].FaceQty + 1);
    for (i = 1; i <= Poly[num].FaceQty; i++)
      (*pTess).PolyFaceNb[num][i] = TFNb[num][i];

    (*pTess).PolyFaceOri[num] = ut_alloc_1d_int (Poly[num].FaceQty + 1);
    for (i = 1; i <= Poly[num].FaceQty; i++)
      (*pTess).PolyFaceOri[num][i] = IniPolyFaceOri (pTess, num, i);
  }

  return;
}

/* Recording of the edges */
void
IniTessEdge (struct TESS *pTess)
{
  int i, j;			/* mute variable.                                           */
  int nb = 0;			/* nb of the currently added edge.                          */
  int S1, S2;			/* numbers of the two vertices on which studied segment is  */
  /* based.                                                   */
  int PrevF;			/* number of the previous face, in which the studied        */
  /* edge has been recorded.                                  */

  /* For every face, for every couple of neighbor vertices, we search
   * if the edge has already been treated.
   * We first search the three common germs of these vertices.
   * These 3 neighboring germs have generated 3 faces whose studied
   * edge belongs to boundary.
   * If the number of the current studied face is the lowest, a new edge
   * has to be recorded. Else, we must found the number of the
   * previously-defined edge which corresponds to this one.
   * To do that, we should consider the lowest number of the 3 faces.
   * A first approach would be to search in VerNb the number of the edge
   * whose extremities are the two considered vertices.
   * Another one would be to write (VerNei &) VerEdge and to access these
   * information more rapidly
   */

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceEdgeNb[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
    {
      /* Recording of the 2 vertex numbers */
      S1 = (*pTess).FaceVerNb[i][j];
      if (j != (*pTess).FaceVerQty[i])
	S2 = (*pTess).FaceVerNb[i][j + 1];
      else
	S2 = (*pTess).FaceVerNb[i][1];

      /* TreatFaceVer returns a (strictly) positive value if the
       * ver been recorded previously, 0 else.
       * If 0, the vertex number has to be created, else it has
       * to be searched among the edges of face whose number has
       * been returned.
       */
      PrevF = TreatFaceVer (pTess, i, S1, S2);
      if (PrevF == 0)
	/* Creation of new edge nb through the incrementation of nb
	 * and the writing of:
	 * EdgeVerNb[nb][0,1]
	 * FaceEdgeNb[i][S1]
	 */
	nb = AddEdge (pTess, i, j, S1, S2, nb);
      else
	/* Search of the previously-given number of the edge
	 * to record it in FaceEdgeNb.
	 */
	SearchEdge (pTess, i, PrevF, S1, S2, j);
    }
  }

  /* Recording of FaceEdgeOri */
  IniTessFaceEdgeOri (pTess);

  /* Recording of Tess.PolyEdgeX */
  IniTessPolyEdge (pTess);

  /* Correction of (*pTess).EdgeQty */
  (*pTess).EdgeQty = nb;

  return;
}

/* ************************************************************************
 * Recording of the edge lengths from the edge properties, and of the
 * vertex parent edges.
 */
void
IniTessVernEdge (struct TESS *pTess)
{
  int i, j;			/* mute variables */
  int ver;			/* tmp variable = ver nb */
  int *nb;			/* tmp variable = id of the ver parent edge to be recorded */

  (*pTess).VerOfDomain = ut_alloc_1d_int (1);
  (*pTess).VerEdge = ut_alloc_2d_int ((*pTess).VerQty + 1, 4);

  nb = ut_alloc_1d_int ((*pTess).VerQty + 1);

  /* for every edge of the tessellation, its number (id) is assigned to
   * its two vertices.
   */
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    for (j = 0; j <= 1; j++)
    {
      ver = (*pTess).EdgeVerNb[i][j];
      (*pTess).VerEdge[ver][nb[ver]] = i;
      nb[ver]++;
    }

  /* The number of the vertices which are vertex of the cubic domain are
   * recorded.
   */
  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    /*printf("%d: %d %d %d %d\n",i,(*pTess).VerEdge[i][0],(*pTess).VerEdge[i][1],(*pTess).VerEdge[i][2],(*pTess).VerEdge[i][3]); */
    if (nb[i] == 3)
    {
      (*pTess).VerOfDomain[0]++;
      (*pTess).VerOfDomain = ut_realloc_1d_int ((*pTess).VerOfDomain, (*pTess).VerOfDomain[0] + 1);
      (*pTess).VerOfDomain[(*pTess).VerOfDomain[0]] = i;
    }
  }

  ut_free_1d_int (nb);

  return;
}
