/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp.h"

void
NeiSearching (struct GERMSET GermSet, int germ, int *nbs)
{
  int i;
  double* dists = ut_alloc_1d (GermSet.N + 1);

  for (i = 1; i <= GermSet.N; i++)
    dists[i] = ut_space_dist (GermSet.GermCoo[germ],
				GermSet.GermCoo[i]);

  ut_array_1d_sort_index (dists + 1, GermSet.N, nbs + 1);
  ut_array_1d_int_addval (nbs + 1, GermSet.N, 1, nbs + 1);

  ut_free_1d (dists);

  return;
}

int
CurrentPolyTest (struct POLYMOD Polymod, double* coo, double* coo2)
{
  int i;
  double maxidist, neidist;

  neidist = ut_space_dist (coo, coo2);

  // max distance between the poly germ and its further vertex.
  maxidist = 0;
  for (i = 1; i <= Polymod.VerQty; i++)
    if (Polymod.VerUse[i] == 1)
    {
      maxidist
	= ut_num_max (maxidist, ut_space_dist (coo, Polymod.VerCoo[i]));

      if (neidist < 2 * maxidist)
	return 1;
    }

  return 0;
}

/* NewPolyhedron modifies the current polyhedron to take into
 * account neighbour Nei.
 */
void
NewPolyhedron (struct GERMSET GermSet, int PolyId, int NeiId, struct POLYMOD * pPolymod)
{
  int CenterSide;

  /* plane contains the parameters of the bissecting plane of segment    */
  /* which lies the center to the considered neighbor, i.e. the new face.*/
  double *plane = ut_alloc_1d (4);

  /* BadVer contains the number of the polyhedron vertices situated      */
  /* in the bad side of the bissecting plane.                            */
  /* BadVer[0] is the quantity of vertices situated in the bad side.     */
  /* BadVer[i] are the number these vertices.                            */
  int *BadVer = NULL;

  /* A new neighbour will be considered. The first step is to search out
     the bissecting plane between the center and the neighbour.
     Then, we look at the polyhedron vertices which are on the "bad"
     hand side of the plane -that of the neighbour-, i.e. vertices
     that are nearer from the neighbour than from the center. If they
     exist, the polyhedron must be modified.                             */

  /* PlanePara calculates the equation of the bissecting plane of segment*/
  /* which lies the center to the considered neighbour.                  */
  PlanePara (PolyId, NeiId, GermSet, plane);

  /* ut_space_planeside returns a number indicating the side where a
   * point is situated relatively to a given plane. If it i situated in
   * the "down" side ...X+...Y+...Z < second member, the program returns
   * -1 ; else it returns 1. If it is on the plane, it returns the value
   * specified as last argument of the function
   */
  CenterSide = ut_space_planeside (plane, GermSet.GermCoo[PolyId] - 1);

  /* BadVerDet searches the vertices that are on the bad side of the     */
  /* bissecting plane -the bad vertices-, which are written in BadVer.   */
  BadVerDet (plane, CenterSide, *pPolymod, &BadVer);

  /* If there are vertices to suppress, the polyhedron is modified       */
  if (BadVer[0] != 0)
    /* PolyhedronModification modifies the polyhedron to take into account
     * the new neighbour.
     */
    PolyhedronModification (NeiId, plane, pPolymod, BadVer);

  /* The memory of the temporary arrays plane and vertices is unallocated.*/
  ut_free_1d (plane);
  ut_free_1d_int (BadVer);

  return;
}

// NewPolyhedron_plane is a variant of function NewPolyhedron, which
// takes the cutting plane and the CenterSide as argument. 
void
NewPolyhedron_plane (int NeiId, int CenterSide, double* plane, struct POLYMOD * pPolymod)
{
  int *BadVer = NULL;

  BadVerDet (plane, CenterSide, *pPolymod, &BadVer);

  if (BadVer[0] != 0)
    PolyhedronModification (NeiId, plane, pPolymod, BadVer);

  ut_free_1d_int (BadVer);

  return;
}
