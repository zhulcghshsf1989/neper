/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tesl.h"

/* net_tesl lies the polyhedra by suppressing vertices and faces
 * defined several times. That results in the net_tesl structure, which
 * contains all the information about the tessellation.
 */
void
net_tesl (struct GERMSET GermSet, struct POLY *Poly,
              struct TESL *pTesl)
{
/* TVNb and TFNb are arrays containing the relation between the
 * POLY-defined entities (ver & face) and the new entities
 * (TVer ver & TFace face).
 */
  int **TVNb = NULL;
  int **TFNb = NULL;

  // If *pTesl is not empty, free'ing
  neut_tesl_free (pTesl);

  neut_tesl_set_zero (pTesl);

  /* Recording of the tessellation generalities in net_tesl, i.e. number
   * of poly, vertices, faces and edges.
   */
  TessGenDet (GermSet, Poly, pTesl);

  TVNb = ut_alloc_1d_pint ((*pTesl).PolyQty + 1);
  TFNb = ut_alloc_1d_pint ((*pTesl).PolyQty + 1);

  IniTxNb (*pTesl, Poly, TFNb, TVNb);

  TessAlloc (pTesl);

/* ************************************************************************
 * Recording of face properties
 */
  /* For each polyhedron, recording of FacePoly, FaceEq & FaceVerQty
   */
  IniTessFaceOr (pTesl, Poly, TFNb);

  /* For each polyhedron, recording of FaceVerNb
   */
  IniTessFaceBound (pTesl, Poly, TFNb, TVNb);

/* ************************************************************************
 * Recording of vertex properties
 */
  IniTessVer (pTesl, Poly, TVNb);

/* ************************************************************************
 * Recording of polyhedron properties
 */
  /* Recording of the center coordinates and the numbers of face and
   * vertex of the polyhedra
   */
  IniTessPolyCenNXQty (GermSet, pTesl, Poly);

  /* Recording of the face and vertex numbers.
   */
  IniTessPolyXNb (pTesl, Poly, TVNb, TFNb);

/* ************************************************************************
 * Recording of edge properties
 */
  /* Recording of the edges.
   */
  IniTessEdge (pTesl);

  ut_free_2d_int (TVNb, (*pTesl).PolyQty + 1);
  ut_free_2d_int (TFNb, (*pTesl).PolyQty + 1);

/* ************************************************************************
 * Recording of the edge lengths from the edge properties, and of the
 * vertex parent edges.
 */
  IniTessVernEdge (pTesl);

  neut_tesl_init_polytrue (pTesl);
  neut_tesl_init_polybody (pTesl);

  return;
}
