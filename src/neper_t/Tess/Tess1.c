/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Tess.h"

/* Tessellation lies the polyhedra by suppressing vertices and faces
 * defined several times. That results in the Tess structure, which
 * contains all the information about the tessellation.
 */
void
Tessellation (struct GERMSET GermSet, struct POLY *Poly,
              struct TESS *pTess)
{
/* TVNb and TFNb are arrays containing the relation between the
 * POLY-defined entities (ver & face) and the new entities
 * (TVer ver & TFace face).
 */
  int **TVNb = NULL;
  int **TFNb = NULL;

  neut_tess_set_zero (pTess);

  /* Recording of the tessellation generalities in Tess, i.e. number
   * of poly, vertices, faces and edges.
   */
  TessGenDet (GermSet, Poly, pTess);

  TVNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
  TFNb = ut_alloc_1d_pint ((*pTess).PolyQty + 1);

  IniTxNb (*pTess, Poly, TFNb, TVNb);

  TessAlloc (pTess);

/* ************************************************************************
 * Recording of face properties
 */
  /* For each polyhedron, recording of FacePoly, FaceEq & FaceVerQty
   */
  IniTessFaceOr (pTess, Poly, TFNb);

  /* For each polyhedron, recording of FaceVerNb
   */
  IniTessFaceBound (pTess, Poly, TFNb, TVNb);

/* ************************************************************************
 * Recording of vertex properties
 */
  IniTessVer (pTess, Poly, TVNb);

/* ************************************************************************
 * Recording of polyhedron properties
 */
  /* Recording of the center coordinates and the numbers of face and
   * vertex of the polyhedra
   */
  IniTessPolyCenNXQty (GermSet, pTess, Poly);

  /* Recording of the face and vertex numbers.
   */
  IniTessPolyXNb (pTess, Poly, TVNb, TFNb);

/* ************************************************************************
 * Recording of edge properties
 */
  /* Recording of the edges.
   */
  IniTessEdge (pTess);

  ut_free_2d_int (TVNb, (*pTess).PolyQty + 1);
  ut_free_2d_int (TFNb, (*pTess).PolyQty + 1);

/* ************************************************************************
 * Recording of the edge lengths from the edge properties, and of the
 * vertex parent edges.
 */
  IniTessVernEdge (pTess);

  neut_tess_init_polytrue (pTess);
  neut_tess_init_polybody (pTess);

  return;
}
