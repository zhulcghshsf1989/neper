/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tesl.h"

/* We search the number of a polyhedron in which the studied
 * vertex has been recorded previously.
 */
int
PrevPoly (struct POLY *Poly, int PNb, int VNb)
{
  int i;			/* mute variable                                */
  int ParentFace;		/* mute variable representing a parent face of  */
  /* the vertex                                   */
  int PrevP;			/* value to return                              */

  /* in Poly PNb and for ver VNb:                                     */
  /* the lower (but positive) parent germ of the parent faces         */
  /* of the ver is searched, and is registered as PrevP. This         */
  /* will be used next.                                               */

  /* for each parent face of the vertex in poly.                      */
  for (i = 0; i < 3; i++)
  {
    ParentFace = Poly[PNb].VerFace[VNb][i];
    PrevP = Poly[PNb].FacePoly[ParentFace];

    /* if this PrevP is lower than PNb and positive, it is
     * kept.
     */
    if (PrevP < PNb && PrevP > 0)
      break;
  }

  return PrevP;
}

/* We search now the number of the studied vertex in
 * this polyhedron. To do that, we first search the
 * face whose FacePoly==i, called PrevF.
 */
int
PrevVer (struct POLY *Poly, int PNb, int VNb, int PrevP)
{
  int PrevS;			/* Value to return
				 */
  int *PFG = ut_alloc_1d_int (3);

  /* The searched vertex of Poly PrevP -which also belongs to Poly
   * PNb- is such that:
   * the germs of its 3 parent faces are the same that those of
   * the Poly PNb vertex, except for PrevP which is replaced by PNb.
   */
  /* The parent germs of the 3 parent faces of the vertex are
   * recorded.
   */
  ParFacePoly (Poly, PNb, VNb, PrevP, PFG);

  /* We search the vertex in the found PrevP poly
   */
  PrevS = SearchInPrevP (Poly, PrevP, PFG);

  ut_free_1d_int (PFG);

  return PrevS;
}

/* The new ver corresponding of that of poly PNb and ver VNb
 * takes the value of that from PrevP and PrevS.
 */
void
UpdTVNb (int **TVNb, int PNb, int VNb, int PrevP, int PrevS)
{
  TVNb[PNb][VNb] = TVNb[PrevP][PrevS];

  return;
}

/* The face edges that are not recorded as polyhedron
 * edges yet as so recorded.
 */
int
FaceEdges2PolyEdges (struct TESL *pnet_tesl, int PNb, int FNb, int qty)
{
  int i;
  int ENb;

  /* for every of the face edges
   */
  for (i = 1; i <= (*pnet_tesl).FaceVerQty[FNb]; i++)
  {
    /* The edge number is recorded
     */
    ENb = (*pnet_tesl).FaceEdgeNb[FNb][i];
    /* If the edge has not been recorded as belonging to the
     * polyhedron yet, it is recorded.
     */
    if (oneDIntEltPos ((*pnet_tesl).PolyEdgeNb[PNb], 1, qty, ENb, 0) == -1)
    {
      /* the qty of polyedron faces is incremented.
       */
      qty++;
      (*pnet_tesl).PolyEdgeNb[PNb] = ut_realloc_1d_int ((*pnet_tesl).PolyEdgeNb[PNb], qty + 1);
      /* The polyhedron new edge number is recorded.
       */
      (*pnet_tesl).PolyEdgeNb[PNb][qty] = ENb;
    }
  }

  return qty;
}