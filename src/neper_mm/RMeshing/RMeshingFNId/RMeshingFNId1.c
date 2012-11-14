/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RMeshingFNId.h"

void
RMeshingFNId (struct IN In, struct MESH* pMesh, struct GERMSET *pGermSet)
{
  int i, j;
  struct GERMSET GermSet;
  double* size = ut_alloc_1d (7);
  size[2] = size[4] = size[6] = 1;
  double** dsize = ut_alloc_2d (3, 2);
  dsize[0][1] = In.domainparms[0] * In.F[0];
  dsize[1][1] = In.domainparms[1] * In.F[1];
  dsize[2][1] = In.domainparms[2] * In.F[2];
  
  struct POLY Domain; 
  Domain.VerQty = 0;
  Domain.FaceQty = 0;

  // Setting germs coo
  (*pGermSet).Random = neut_rand_nnid2rand ((*pGermSet).NDensity, (*pGermSet).Id);

  // N
  if (! strcmp ((*pGermSet).ttype, "subdomain") == 0)
  {
    (*pGermSet).N = (*pGermSet).NDensity;
  }
  else
    (*pGermSet).N =
      neut_rand_poisson ((*pGermSet).NDensity, (int) (*pGermSet).Random, 1);

  // GermsCoo inside the domain
  (*pGermSet).GermsCoo = ut_alloc_2d ((*pGermSet).N + 1, 4);

  if (strcmp ((*pGermSet).morpho, "poisson") == 0)
    IniGermsCoo ((*pGermSet).morpho, (*pGermSet).N, (*pGermSet).Random,
		 (*pGermSet).Size, Domain, (*pGermSet).GermsCoo);
  else if (strcmp ((*pGermSet).morpho, "custom") == 0)
    DistribFromFile (In.centrecoo, (*pGermSet).N, &((*pGermSet).GermsCoo));
  else 
  {
    GermSet.N1d = (*pGermSet).N1d;

    if (strcmp ((*pGermSet).morpho, "cube") == 0)
      CubeDistrib (&GermSet, size);
    else if (strcmp ((*pGermSet).morpho, "tocta") == 0)
      TruOctaDistrib (&GermSet, size);
    else
      abort ();

    (*pGermSet).GermsCoo = ut_alloc_2d ((*pGermSet).N + 1, 4);
    for (i = 1; i <= (*pGermSet).N; i++)
      for (j = 1; j <= 3; j++)
        (*pGermSet).GermsCoo[i][j] = GermSet.GermsCoo[i][j];

    ut_free_2d (GermSet.GermsCoo, (*pGermSet).N + 1);
  }

  DefGermSet (pGermSet);

  // Searching elt poly
  int* msize = ut_alloc_1d_int (3);
  nemm_in_msize (In, dsize, msize);
  NIdSearchEltPoly (msize, pGermSet, pMesh);

  ut_free_1d (size);
  ut_free_1d_int (msize);
  ut_free_2d (dsize, 3);

  return;
}
