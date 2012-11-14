/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DeleteEdge.h"

void
DeleteFaceFromItsPoly (struct GEO *pGeo, int face)
{
  int i;
  int k;
  int tmppoly;
  int qty;
  int pos;

  /*ut_print_lineheader(-1); printf("    Deleting face from its parent polyhedra: "); */

  /* it is removed from its one/two parent polyhedron/a. */
  for (i = 0; i <= 1; i++)
  {
    tmppoly = (*pGeo).FacePoly[face][i];
    if (tmppoly >= 1)		/* ie real tmppoly */
    {
      /* printf("%d ",tmppoly); */

      pos = oneDIntEltPos ((*pGeo).PolyFaceNb[tmppoly], 1,
			   (*pGeo).PolyFaceQty[tmppoly], face, 0);

      qty =
	oneDIntDeleteNCompress ((*pGeo).PolyFaceNb[tmppoly], 1,
				(*pGeo).PolyFaceQty[tmppoly], face, 1);
      for (k = pos; k <= (*pGeo).PolyFaceQty[tmppoly] - 1; k++)
	(*pGeo).PolyFaceOri[tmppoly][k] = (*pGeo).PolyFaceOri[tmppoly][k + 1];

      (*pGeo).PolyFaceQty[tmppoly]--;

      if (qty != 1)
      {
	ut_print_lineheader (2);
	printf ("face %d not found in poly %d!\n", face, tmppoly);
	abort ();
      }
    }
  }

  return;
}

void
SearchNewNOldEdges (struct GEO *pGeo, int face, int edge, int *pnew,
		    int *pold)
{
  oneDIntDeleteNCompress ((*pGeo).FaceEdgeNb[face], 1, 3, edge, 1);

  (*pnew) = oneDIntMin ((*pGeo).FaceEdgeNb[face], 1, 2);
  (*pold) = oneDIntMax ((*pGeo).FaceEdgeNb[face], 1, 2);

  return;
}
