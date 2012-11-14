/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyCalc.h"

/* PlanePara calculates the equation of the bissecting plane of segment
 * which lies the center to the neighbour.
 */
void
PlanePara (int center, int nei, struct GERMSET GermSet, double *plane)
{
  double norm;

  /* The equation of the bissecting plane of segment [AB] with 
   * A(x1,y1,z1) and B(x2,y2,z2) is:
   * 2(x2-x1) X+ 2(y2-y1) Y+ 2(z2-z1) Z= x2^2-x1^2 + y2^2-y1^2 + z2^2-z1^2
   * it is recorded as:
   * plane[1] X+ plane[2] Y+ plane[3] Z= plane[0]
   */

  /* mute variable */
  int i;

  for (i = 1; i <= 3; i++)
    plane[i] = 2 * (GermSet.GermsCoo[nei][i] - GermSet.GermsCoo[center][i]);

  plane[0] = 0;
  for (i = 1; i <= 3; i++)
    plane[0] +=
      (pow (GermSet.GermsCoo[nei][i], 2) -
       pow (GermSet.GermsCoo[center][i], 2));

  /* The equation is normed so as to get:
   * plane[1]*plane[1]+plane[2]*plane[2]+plane[3]*plane[3]=1;
   */
  norm = VectorNorm (plane, 1);
  for (i = 0; i <= 3; i++)
    plane[i] /= norm;

  return;
}

/* BadVerDet searches the vertices that are on the bad side of the
 * bissecting plane, i.e. that of the neighbour
 */
void
BadVerDet (int Center, double *plane, GERMSET GermSet, POLYMOD Polymod,
	   int **pBadVer)
{
  /* Mute variables */
  int i;

  /* CenterSide & VerSide are integers identifying the side of the center
   * and the considered vertex, respectively.
   */
  int CenterSide, VerSide;

  /* Side returns a number indicating the side where a point is situated
   * relatively to a given plane. If it i situated in the "down" side
   * ...X+...Y+...Z < second member, the program returns 1 ; else it
   * returns 2. If it is on the plane, it returns the value
   * specified as last argument of the function
   */
  CenterSide = Side (plane, GermSet.GermsCoo[Center]);

  /* For every real vertex of the polyhedron, we check if it is
   * in the bad side or not.
   */
  for (i = 1; i <= Polymod.VerQty; i++)
    if (Polymod.VerUse[i] == 1)
    {
      VerSide = Side (plane, Polymod.VerCoo[i] - 1);
      /* the term -1 is added to take into account the fact that Side
       * read coordinates in [1]...[3] while they are stored in 
       * [0]...[2] in VerCoo[i]
       *printf("Ver Side=%d\n",VerSide);

       * if the vertex is in the bad side, its existence is taken
       * into account: BadVer[0]++; and its number (i) is stored.
       */
      if (VerSide != CenterSide)
      {
	(*pBadVer)[0]++;
	(*pBadVer) = ut_realloc_1d_int ((*pBadVer), (*pBadVer)[0] + 2);
	(*pBadVer)[(*pBadVer)[0]] = i;
      }
    }

  return;
}


/* PolyhedronModification modifies the polyhedron to take into account
 * the new neighbour.
 */
void
PolyhedronModification (int Nei, double *plane, struct POLYMOD *pPolymod,
			int *BadVer)
{
  int *FnLFaces;		/* first & last face to be modified         */
  int Face;			/* current face to be modified              */
  int pFace;			/* previous face that has been modified     */
  int nFace;			/* next face to modifiy                     */
  int firstVer;			/* first created vertex                     */

  FnLFaces = ut_alloc_1d_int (2);

  /* VerUseUpdating modifies pPolymod.VerUse: VerUse[i]<--0 if the
   * ith vertex is a bad vertex.
   */
  VerUseUpdating (pPolymod, BadVer);

  /* FaceUseUpdating modifies pPolymod.FaceUse: FaceUse[i]<--0 if all
   * the vertices of face i are bad vertices.
   */
  FaceUseUpdating (pPolymod, BadVer);

  /* NewFace adds the new face by increasing FaceQty and
   * initializing FaceUse, FacePoly & FaceEq.
   */
  NewFace (Nei, pPolymod, plane);

  /* FnLFacesDet determines the first and last faces to be modified.
   * These are two neighbor faces cut by the new plane, and are
   * recorded as FnLFaces[0] and FnLFaces[1].
   */
  FnLFacesDet (*pPolymod, BadVer, FnLFaces);

  /* NewVer adds the vertex which is the intersection between faces
   * first, second and third, i.e. the first and last face to modify and
   * the new face: VerQty is increased, VerUse, VerFace and VerCoo are
   * initiated.. The number of the created vertex is returned.
   */
  firstVer = NewVer (pPolymod, FnLFaces[0], FnLFaces[1], (*pPolymod).FaceQty);

  /* NewFaceAddVer adds the new vertex to the new face.
   */
  NewFaceAddVer (pPolymod);

  nFace = FnLFaces[0];
  pFace = FnLFaces[1];

  while (nFace != FnLFaces[1])
  {
    Face = nFace;

    /* FaceModif modifies Face to take into account the new
     * face.
     */
    nFace = FaceModif (pFace, Face, pPolymod, BadVer, 0);
    /* The new face is updated by adding the last created vertex.
     */
    NewFaceAddVer (pPolymod);
    pFace = Face;
  }

  FaceModif (pFace, FnLFaces[1], pPolymod, BadVer, firstVer);

  ut_free_1d_int (FnLFaces);

  return;
}
