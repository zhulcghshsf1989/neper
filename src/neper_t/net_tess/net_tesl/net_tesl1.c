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
              struct TESL *pnet_tesl)
{
/* TVNb and TFNb are arrays containing the relation between the
 * POLY-defined entities (ver & face) and the new entities
 * (TVer ver & TFace face).
 */
  int **TVNb = NULL;
  int **TFNb = NULL;

  // If *pnet_tesl is not empty, free'ing
  neut_tesl_free (pnet_tesl);

  neut_tesl_set_zero (pnet_tesl);

  /* Recording of the tessellation generalities in net_tesl, i.e. number
   * of poly, vertices, faces and edges.
   */
  TessGenDet (GermSet, Poly, pnet_tesl);

  TVNb = ut_alloc_1d_pint ((*pnet_tesl).PolyQty + 1);
  TFNb = ut_alloc_1d_pint ((*pnet_tesl).PolyQty + 1);

  IniTxNb (*pnet_tesl, Poly, TFNb, TVNb);

  TessAlloc (pnet_tesl);

/* ************************************************************************
 * Recording of face properties
 */
  /* For each polyhedron, recording of FacePoly, FaceEq & FaceVerQty
   */
  IniTessFaceOr (pnet_tesl, Poly, TFNb);

  /* For each polyhedron, recording of FaceVerNb
   */
  IniTessFaceBound (pnet_tesl, Poly, TFNb, TVNb);

/* ************************************************************************
 * Recording of vertex properties
 */
  IniTessVer (pnet_tesl, Poly, TVNb);

/* ************************************************************************
 * Recording of polyhedron properties
 */
  /* Recording of the center coordinates and the numbers of face and
   * vertex of the polyhedra
   */
  IniTessPolyCenNXQty (GermSet, pnet_tesl, Poly);

  /* Recording of the face and vertex numbers.
   */
  IniTessPolyXNb (pnet_tesl, Poly, TVNb, TFNb);

/* ************************************************************************
 * Recording of edge properties
 */
  /* Recording of the edges.
   */
  IniTessEdge (pnet_tesl);

  ut_free_2d_int (TVNb, (*pnet_tesl).PolyQty + 1);
  ut_free_2d_int (TFNb, (*pnet_tesl).PolyQty + 1);

/* ************************************************************************
 * Recording of the edge lengths from the edge properties, and of the
 * vertex parent edges.
 */
  IniTessVernEdge (pnet_tesl);

  neut_tesl_init_polytrue (pnet_tesl);
  neut_tesl_init_polybody (pnet_tesl);

  return;
}
