/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"T2P.h"

/* T2P extracts polyhedron nb from the Tess tessellation
 * description.
 */
void
T2P (struct TESS Tess, int nb, struct POLY *pPoly)
{
  int *TV2PV;			/* TV2PV for "Tessellation Vertices to Polyhedron
				 * Vertices" is such that:
				 * TV2PV[i] (i: nb of the vertex into Poly
				 * =1...(*pPoly).VerQty) is the number 
				 * of the vertex recorded into Tess
				 */

  /* Recording of the vertex quantity */
  T2PVerQty (Tess, nb, pPoly);

  /* Recording of the face quantity */
  T2PFaceQty (Tess, nb, pPoly);

  TV2PV = ut_alloc_1d_int ((*pPoly).VerQty + 1);

  /* Recording of the vertex coordinates */
  T2PVerCoo (Tess, nb, pPoly, TV2PV);

  /* Recording of the face properties */
  T2PFaceProp (Tess, nb, pPoly, TV2PV);

  /* Recording of the vertices of the faces. This step is carried
   * out at last because it needs the previously-defined data.
   */
  T2PVerFace (Tess, nb, pPoly, TV2PV);

  oneDIntFree (TV2PV);

  return;
}
