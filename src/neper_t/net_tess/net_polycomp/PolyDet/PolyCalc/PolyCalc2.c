/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyCalc.h"
#include<gsl/gsl_sort.h>
#include<gsl/gsl_sort_vector.h>

void
NeiSearching (struct GERMSET GermSet, int ToSort, int germ, int *nbs,
	      double *dists)
{
  int i;
  size_t *TmpNb = (size_t *) calloc (GermSet.N, sizeof (size_t));
  double *TmpDist = ut_alloc_1d (GermSet.N);

  for (i = 0; i < GermSet.N; i++)
    TmpDist[i] = ut_space_dist (GermSet.GermsCoo[germ] + 1,
				GermSet.GermsCoo[i + 1] + 1);

  gsl_sort_index (TmpNb, TmpDist, 1, GermSet.N);

  for (i = 0; i < ToSort; i++)
  {
    nbs[i] = (int) TmpNb[i] + 1;
    dists[i] = TmpDist[TmpNb[i]];
  }

  ut_free_1d_sizet (TmpNb);

  ut_free_1d (TmpDist);

  return;
}

double
MaxiDist (int Center, GERMSET GermSet, POLYMOD pPolymod)
{
  double maxidist = 0, tmpdist;
  int i;

  for (i = 1; i <= pPolymod.VerQty; i++)
    if (pPolymod.VerUse[i] == 1)
    {
      tmpdist = ut_space_dist (GermSet.GermsCoo[Center] + 1,
				  pPolymod.VerCoo[i]);

      if (tmpdist > maxidist)
	maxidist = tmpdist;
    }

  return maxidist;
}

int
CurrentPolyTest (double maxidist, double neidist)
{
  if (neidist >= 2 * maxidist)
    return 0;
  else
    return 1;
}

/* NewPolyhedron modifies the current polyhedron to take into
 * account neighbour Nei.
 */
void
NewPolyhedron (int Center, int Nei, GERMSET GermSet, POLYMOD * pPolymod)
{
  /* plane contains the parameters of the bissecting plane of segment    */
  /* which lies the center to the considered neighbor, i.e. the new face.*/
  double *plane = ut_alloc_1d (4);

  /* BadVer contains the number of the polyhedron vertices situated      */
  /* in the bad side of the bissecting plane.                            */
  /* BadVer[0] is the quantity of vertices situated in the bad side.     */
  /* BadVer[i] are the number these vertices.                            */
  int *BadVer = ut_alloc_1d_int (1);

  /* A new neighbour will be considered. The first step is to search out
     the bissecting plane between the center and the neighbour.
     Then, we look at the polyhedron vertices which are on the "bad"
     hand side of the plane -that of the neighbour-, i.e. vertices
     that are nearer from the neighbour than from the center. If they
     exist, the polyhedron must be modified.                             */

  /* PlanePara calculates the equation of the bissecting plane of segment*/
  /* which lies the center to the considered neighbour.                  */
  PlanePara (Center, Nei, GermSet, plane);

  /* BadVerDet searches the vertices that are on the bad side of the     */
  /* bissecting plane -the bad vertices-, which are written in BadVer.   */
  BadVerDet (Center, plane, GermSet, *pPolymod, &BadVer);

  /* If there are vertices to suppress, the polyhedron is modified       */
  if (BadVer[0] != 0)
    /* PolyhedronModification modifies the polyhedron to take into account
     * the new neighbour.
     */
    PolyhedronModification (Nei, plane, pPolymod, BadVer);

  /* The memory of the temporary arrays plane and vertices is unallocated.*/
  ut_free_1d (plane);
  ut_free_1d_int (BadVer);

  return;
}
