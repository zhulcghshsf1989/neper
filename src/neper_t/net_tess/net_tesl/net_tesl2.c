/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tesl.h"

/* Recording of the tessellation generalities in net_tesl, i.e. number
 * of poly, vertices, faces and edges.
 */
void
TessGenDet (struct GERMSET GermSet, struct POLY *Poly, struct TESL *pnet_tesl)
{
  (*pnet_tesl).Type = ut_alloc_1d_char (10);
  strcpy ((*pnet_tesl).Type, "standard"); 

  (*pnet_tesl).Id = GermSet.Id;
  (*pnet_tesl).morpho = ut_realloc_1d_char ((*pnet_tesl).morpho, 10);

  strcpy ((*pnet_tesl).morpho, GermSet.morpho);

  (*pnet_tesl).PolyQty = TPolyQty (GermSet);
  (*pnet_tesl).FaceQty = TFaceQty (GermSet, Poly);
  (*pnet_tesl).VerQty = TVerQty (GermSet, Poly);
  (*pnet_tesl).EdgeQty = TEdgeQty (GermSet, Poly);

  return;
}

void
TessAlloc (struct TESL *pnet_tesl)
{
  (*pnet_tesl).FacePoly = ut_alloc_2d_int ((*pnet_tesl).FaceQty + 1, 2);
  (*pnet_tesl).FaceEq = ut_alloc_2d ((*pnet_tesl).FaceQty + 1, 4);
  (*pnet_tesl).FaceVerQty = ut_alloc_1d_int ((*pnet_tesl).FaceQty + 1);
  (*pnet_tesl).FaceVerNb = ut_alloc_1d_pint ((*pnet_tesl).FaceQty + 1);
  (*pnet_tesl).FaceEdgeNb = ut_alloc_1d_pint ((*pnet_tesl).FaceQty + 1);
  (*pnet_tesl).FaceEdgeOri = ut_alloc_1d_pint ((*pnet_tesl).FaceQty + 1);
  (*pnet_tesl).VerGerm = ut_alloc_2d_int ((*pnet_tesl).VerQty + 1, 4);
  (*pnet_tesl).VerCoo = ut_alloc_2d ((*pnet_tesl).VerQty + 1, 3);
  (*pnet_tesl).CenterCoo = ut_alloc_2d ((*pnet_tesl).PolyQty + 1, 3);
  (*pnet_tesl).PolyVerQty = ut_alloc_1d_int ((*pnet_tesl).PolyQty + 1);
  (*pnet_tesl).PolyFaceQty = ut_alloc_1d_int ((*pnet_tesl).PolyQty + 1);
  (*pnet_tesl).PolyVerNb = ut_alloc_1d_pint ((*pnet_tesl).PolyQty + 1);
  (*pnet_tesl).PolyFaceNb = ut_alloc_1d_pint ((*pnet_tesl).PolyQty + 1);
  (*pnet_tesl).PolyFaceOri = ut_alloc_1d_pint ((*pnet_tesl).PolyQty + 1);
  (*pnet_tesl).PolyEdgeQty = ut_alloc_1d_int ((*pnet_tesl).PolyQty + 1);
  (*pnet_tesl).PolyEdgeNb = ut_alloc_1d_pint ((*pnet_tesl).PolyQty + 1);
  (*pnet_tesl).EdgeVerNb = ut_alloc_1d_pint (1);
  (*pnet_tesl).EdgeVerNb[0] = NULL;

  return;
}

/* Recording of TFNb & TVNb */
void
IniTxNb (struct TESL net_tesl, struct POLY *Poly, int **TFNb, int **TVNb)
{
  /* Recording of TFNb */
  IniTFNb (net_tesl, Poly, TFNb);
  /* Recording of TVNb */
  IniTVNb (net_tesl, Poly, TVNb);
  
  return;
}

/* For each polyhedron, recording of FacePoly, FaceEq & FaceVerQty */
void
IniTessFaceOr (struct TESL *pnet_tesl, struct POLY *Poly, int **TFNb)
{
  /* Recording of Face germ */
  IniTessFacePoly (pnet_tesl, Poly, TFNb);
  /* Recording of Face eq */
  IniTessFaceEq (pnet_tesl, Poly, TFNb);
  /* Recording of Face ver qty */
  IniTessFaceVerQty (pnet_tesl, Poly, TFNb);

  return;
}

/* For each polyhedron, recording of FaceVerNb */
void
IniTessFaceBound (struct TESL *pnet_tesl, struct POLY *Poly, int **TFNb,
		  int **TVNb)
{
  int i, j, k;
  int FNb;

  /* for every face of every polyhedron */
  for (i = 1; i <= (*pnet_tesl).PolyQty; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      /* if it has not been treated previously */
      if (i < Poly[i].FacePoly[j] || Poly[i].FacePoly[j] < 0)
      {
	/* the face tesl number is recorded */
	FNb = TFNb[i][j];

	(*pnet_tesl).FaceVerNb[FNb] =
	  ut_alloc_1d_int ((*pnet_tesl).FaceVerQty[FNb] + 1);

	for (k = 1; k <= (*pnet_tesl).FaceVerQty[FNb]; k++)
	  (*pnet_tesl).FaceVerNb[FNb][k] = TVNb[i][Poly[i].FaceVerNb[j][k]];
      }

  return;
}

/* Recording of vertex properties */
void
IniTessVer (struct TESL *pnet_tesl, struct POLY *Poly, int **TVNb)
{
  /* Recording of the vertex coordinates */
  IniTessVerCoo (pnet_tesl, Poly, TVNb);
  /* Recording of the numbers of the ver parent germs */
  IniTessVerGerm (pnet_tesl, Poly, TVNb);

  return;
}

/* Recording of the numbers of face and vertex of the polyhedra */
void
IniTessPolyCenNXQty (struct GERMSET GermSet, struct TESL *pnet_tesl,
		     struct POLY *Poly)
{
  int num;
  int i;
  
  for (num = 1; num <= (*pnet_tesl).PolyQty; num++)
  {
    for (i = 0; i < 3; i++)
      (*pnet_tesl).CenterCoo[num][i] = GermSet.GermsCoo[num][i + 1];
    (*pnet_tesl).PolyFaceQty[num] = Poly[num].FaceQty;
    (*pnet_tesl).PolyVerQty[num] = Poly[num].VerQty;
  }

  return;
}

/* Recording of the face and vertex numbers */
void
IniTessPolyXNb (struct TESL *pnet_tesl, struct POLY *Poly, int **TVNb, int **TFNb)
{
  int num, i;

  for (num = 1; num <= (*pnet_tesl).PolyQty; num++)
  {
    (*pnet_tesl).PolyVerNb[num] = ut_alloc_1d_int (Poly[num].VerQty + 1);
    for (i = 1; i <= Poly[num].VerQty; i++)
      (*pnet_tesl).PolyVerNb[num][i] = TVNb[num][i];

    (*pnet_tesl).PolyFaceNb[num] = ut_alloc_1d_int (Poly[num].FaceQty + 1);
    for (i = 1; i <= Poly[num].FaceQty; i++)
      (*pnet_tesl).PolyFaceNb[num][i] = TFNb[num][i];

    (*pnet_tesl).PolyFaceOri[num] = ut_alloc_1d_int (Poly[num].FaceQty + 1);
    for (i = 1; i <= Poly[num].FaceQty; i++)
      (*pnet_tesl).PolyFaceOri[num][i] = IniPolyFaceOri (pnet_tesl, num, i);
  }

  return;
}

/* Recording of the edges */
void
IniTessEdge (struct TESL *pnet_tesl)
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

  for (i = 1; i <= (*pnet_tesl).FaceQty; i++)
  {
    (*pnet_tesl).FaceEdgeNb[i] = ut_alloc_1d_int ((*pnet_tesl).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pnet_tesl).FaceVerQty[i]; j++)
    {
      /* Recording of the 2 vertex numbers */
      S1 = (*pnet_tesl).FaceVerNb[i][j];
      if (j != (*pnet_tesl).FaceVerQty[i])
	S2 = (*pnet_tesl).FaceVerNb[i][j + 1];
      else
	S2 = (*pnet_tesl).FaceVerNb[i][1];

      /* TreatFaceVer returns a (strictly) positive value if the
       * ver been recorded previously, 0 else.
       * If 0, the vertex number has to be created, else it has
       * to be searched among the edges of face whose number has
       * been returned.
       */
      PrevF = TreatFaceVer (pnet_tesl, i, S1, S2);
      if (PrevF == 0)
	/* Creation of new edge nb through the incrementation of nb
	 * and the writing of:
	 * EdgeVerNb[nb][0,1]
	 * FaceEdgeNb[i][S1]
	 */
	nb = AddEdge (pnet_tesl, i, j, S1, S2, nb);
      else
	/* Search of the previously-given number of the edge
	 * to record it in FaceEdgeNb.
	 */
	SearchEdge (pnet_tesl, i, PrevF, S1, S2, j);
    }
  }

  /* Recording of FaceEdgeOri */
  IniTessFaceEdgeOri (pnet_tesl);

  /* Recording of net_tesl.PolyEdgeX */
  IniTessPolyEdge (pnet_tesl);

  /* Correction of (*pnet_tesl).EdgeQty */
  (*pnet_tesl).EdgeQty = nb;

  return;
}

/* ************************************************************************
 * Recording of the edge lengths from the edge properties, and of the
 * vertex parent edges.
 */
void
IniTessVernEdge (struct TESL *pnet_tesl)
{
  int i, j;			/* mute variables */
  int ver;			/* tmp variable = ver nb */
  int *nb;			/* tmp variable = id of the ver parent edge to be recorded */

  (*pnet_tesl).VerOfDomain = ut_alloc_1d_int (1);
  (*pnet_tesl).VerEdge = ut_alloc_2d_int ((*pnet_tesl).VerQty + 1, 4);

  nb = ut_alloc_1d_int ((*pnet_tesl).VerQty + 1);

  /* for every edge of the tessellation, its number (id) is assigned to
   * its two vertices.
   */
  for (i = 1; i <= (*pnet_tesl).EdgeQty; i++)
    for (j = 0; j <= 1; j++)
    {
      ver = (*pnet_tesl).EdgeVerNb[i][j];
      (*pnet_tesl).VerEdge[ver][nb[ver]] = i;
      nb[ver]++;
    }

  /* The number of the vertices which are vertex of the cubic domain are
   * recorded.
   */
  for (i = 1; i <= (*pnet_tesl).VerQty; i++)
  {
    /*printf("%d: %d %d %d %d\n",i,(*pnet_tesl).VerEdge[i][0],(*pnet_tesl).VerEdge[i][1],(*pnet_tesl).VerEdge[i][2],(*pnet_tesl).VerEdge[i][3]); */
    if (nb[i] == 3)
    {
      (*pnet_tesl).VerOfDomain[0]++;
      (*pnet_tesl).VerOfDomain = ut_realloc_1d_int ((*pnet_tesl).VerOfDomain, (*pnet_tesl).VerOfDomain[0] + 1);
      (*pnet_tesl).VerOfDomain[(*pnet_tesl).VerOfDomain[0]] = i;
    }
  }

  ut_free_1d_int (nb);

  return;
}
