/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTPOLY_H
#define STRUCTPOLY_H

/* DEFINITION OF STRUCTURE POLY
 * It contains the exhaustive description of one polyhedron.
 * POLY is a compacted form of structure POLYMOD.
 */

struct POLY
{
  /* All entities of POLY structure are the same than those of POLYMOD
   * structure. VerUse and FaceUse do not exist in POLY.
   */

  /* GENERAL INFORMATION ***************************************************
   */
  int VerQty;			/* Polyhedron quantities of vertex and face */
  int FaceQty;

  /* VERTEX INFORMATION ****************************************************
   */
  /* For vertex i (i=1...VerQty): */
  int **VerFace;		/* VerFace[i][j] (j=0...2) are the numbers of the 3 */
  /* parent faces  */
  double **VerCoo;		/* VerCoo[i][j] (j=0...2) are the 3 coordinates. */

  /* FACE INFORMATION ******************************************************
   */
  /* For face i (i=1...FaceQty): */
  int *FacePoly;		/* FacePoly[i] is the number of the 2nd parent germ */
  /* (other than the polyhedron center). */
  double **FaceEq;		/* FaceEq[i][j] (j=0...3) are the four equation */
  /* parameters: */
  /* FaceEq[i][1]*X1+FaceEq[i][2]*X2+FaceEq[i][3]*X3=FaceEq[i][0] */
  int *FaceVerQty;		/* FaceVerQty[i] is the quantity of face vertices */
  int **FaceVerNb;		/* FaceVerNb[i][j] (j=1...FaceVerQty[i]) are the */
  /* numbers of the vertices. */
};
typedef struct POLY POLY;

#endif /* STRUCTPOLY_H */
