/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RMeshingFNId.h"

void
NIdSearchEltPoly (int* msize, struct GERMSET *pGermSet, struct MESH* pMesh)
{
  int i, elt;
  int Germ;
  double dist;

  int EltQty = msize[0] * msize[1] * msize[2];

  double* GermDist = ut_alloc_1d ((*pGermSet).N + (*pGermSet).nN + 1);

  char* progress = ut_alloc_1d_char (100);

  ut_print_progress (stdout, 0, (*pMesh).EltQty, "%3.0f%%", progress);

  GermDist[0] = 1e30;	/* This is to avoid id 0 in the */
  /* minimum searching below.     */

  /* for every elt, the poly of which it belongs is searched out. */
  for (elt = 1; elt <= EltQty; elt++)
  {
    /* calculation of the distances bw the elt and the germs
     * (GermsCoo...+1 for memory skip)
     */
    for (i = 1; i <= (*pGermSet).N; i++)
    {
      dist = ut_space_dist ((*pMesh).EltCoo[elt], (*pGermSet).GermsCoo[i] + 1);
      GermDist[i] = dist;
    }
    for (i = 1; i <= (*pGermSet).nN; i++)
    {
      dist = ut_space_dist ((*pMesh).EltCoo[elt], (*pGermSet).nGermsCoo[i] + 1);
      GermDist[(*pGermSet).N + i] = dist;
    }

    /* recording of the id of the nearest seed  */
    Germ = ut_array_1d_min_index (GermDist, (*pGermSet).N + (*pGermSet).nN + 1);

    (*pMesh).EltElset[elt] = Germ;

    ut_print_progress (stdout, elt, (*pMesh).EltQty, "%3.0f%%", progress);
  }

  ut_free_1d (GermDist);

  /* If the tessellation is periodic */
  /* applying GermtoGerm on EltPoly */
  if (! strcmp ((*pGermSet).ttype, "periodic"))
    for (elt = 1; elt <= EltQty; elt++)
    {
      Germ = (*pMesh).EltElset[elt];

      if (Germ >= (*pGermSet).N + 1)	/* modifying Germ */
      {
	Germ = (*pGermSet).GermToGerm[Germ - (*pGermSet).N];
	(*pMesh).EltElset[elt] = Germ;
      }
    }

  return;
}
