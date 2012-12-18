/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Tesl.h"

/* PrevFace returns a face number of polyhedron PrevP which contains
 * the studied vertex. The chosen face is that whose facepoly=PNb.
 */
int
PrevFace (struct POLY *Poly, int PNb, int PrevP)
{
  if (oneDIntEltPos (Poly[PrevP].FacePoly, 1, Poly[PrevP].FaceQty, PNb, 0) ==
      -1)
    ut_error_reportbug ();

  return oneDIntEltPos (Poly[PrevP].FacePoly, 1, Poly[PrevP].FaceQty, PNb, 0);
}

/* The parent germs of the 3 parent faces of the vertex are
 * recorded.
*/
void
ParFacePoly (struct POLY *Poly, int PNb, int VNb, int PrevP, int *PFG)
{
  int i;			/* Mute variable */
  int pos;

  /* First, we search the parent germs that the vertex of poly PrevP
   * must have. They are the same than the studied poly PNb vertex,
   * except for the value PrevP which is replaced by PNb.
   * The values are written in PFG.
   */

  /* The VNb parent face germ are copied.
   */
  for (i = 0; i <= 2; i++)
    PFG[i] = Poly[PNb].FacePoly[Poly[PNb].VerFace[VNb][i]];

  /* PrevP is replaced by PNb
   */
  pos = oneDIntEltPos (PFG, 0, 2, PrevP, 0);
  PFG[pos] = PNb;

  return;
}


/* We search the vertex in the found PrevP poly
 */
int
SearchInPrevP (struct POLY *Poly, int PrevP, int *PFG)
{
  int i, j;
  int *ParentFace = ut_alloc_1d_int (3);
  int ver = 0;			/* Value to return */

  /* Every vertex of the polyhedron is checked. If it has the same parent
   * face parent germs than the studied vertex, except for germ PNb which
   * is replaced by PrevP, that is a common vertex with that we study.
   */
  for (i = 1; i <= Poly[PrevP].VerQty; i++)
  {
    /* For clarity reasons, the vertex parent faces are recorded.
     */
    for (j = 0; j <= 2; j++)
      ParentFace[j] = Poly[PrevP].VerFace[i][j];

    if (oneDIntEltPos (PFG, 0, 2, Poly[PrevP].FacePoly[ParentFace[0]], 0) !=
	-1
	&& oneDIntEltPos (PFG, 0, 2, Poly[PrevP].FacePoly[ParentFace[1]],
			  0) != -1
	&& oneDIntEltPos (PFG, 0, 2, Poly[PrevP].FacePoly[ParentFace[2]],
			  0) != -1)
    {
      ver = i;
      break;
    }
  }

  ut_free_1d_int (ParentFace);

  return ver;
}
