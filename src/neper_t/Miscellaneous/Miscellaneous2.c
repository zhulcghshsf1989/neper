/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Miscellaneous.h"

void
GermSetSize (struct GERMSET *pGermSet)
{
  int i, j, k;
  int ver, poly;
  double *tmp;
  int **DFaceVer;
  double *s = (*pGermSet).Size;

  struct GERMSET GermSet;
  struct TESS Tess;
  neut_tess_set_zero (&Tess);
  neut_germset_set_zero (&GermSet);
  GermSet.morpho = ut_alloc_1d_char (8);
  sprintf (GermSet.morpho, "poisson");

  if ((*pGermSet).N <= 0)
  {
    ut_print_messagewnc (2, 72, "The domain has no germ.");
    abort ();
  }

  if (((*pGermSet).Size[1] >= (*pGermSet).Size[2])
      || ((*pGermSet).Size[3] >= (*pGermSet).Size[4])
      || ((*pGermSet).Size[5] >= (*pGermSet).Size[6]))
  {
    ut_print_messagewnc (2, 72, "Bad domain size.");
    abort ();
  }

  /* Building GermSet */
  GermSet.N = (*pGermSet).N;
  /* GermSet.Id=(*pGermSet).Id; */
  GermSet.GermsCoo = ut_alloc_2d (GermSet.N + 1, 4);
  for (i = 1; i <= GermSet.N; i++)
  {
    for (j = 1; j <= 3; j++)
      GermSet.GermsCoo[i][j] = (*pGermSet).GermsCoo[i][j];
  }

  /* Building tessellation to calculate the neighbourhood zone. */
  CreateTess (GermSet, (*pGermSet).Size, &Tess);

  DFaceVer = ut_alloc_2d_int (7, Tess.VerQty + 1);
  neut_tess_domainfacever (Tess, DFaceVer);

  (*pGermSet).nSize = ut_alloc_1d (7);

  for (i = 1; i <= 6; i++)
  {
    /* tmp[j] is the distance bw ver j and its poly centres. */
    tmp = ut_alloc_1d (DFaceVer[i][0] + 1);

    for (j = 1; j <= DFaceVer[i][0]; j++)
    {
      ver = DFaceVer[i][j];

      for (k = 0; k <= 3; k++)
      {
	poly = Tess.VerGerm[ver][k];

	if (poly >= 1)
	{
	  tmp[j] =
	    ut_space_dist (Tess.VerCoo[ver], Tess.CenterCoo[poly]);
	  break;
	}
      }
    }

    (*pGermSet).nSize[i] =
      s[i] + pow (-1, i) * ut_array_1d_max (tmp + 1, DFaceVer[i][0]);

    ut_free_1d (tmp);
  }

  /* Calculating the volume */
  CalcNeighVol (pGermSet);

  return;
}


/* GermSetGermsPer defines:
 * nGermsCoo, nGermQty, GermToGerm
 */
extern void
GermSetGermsPer (struct GERMSET *pGermSet)
{
  int i, j, id;
  int *per = ut_alloc_1d_int (4);
  double *coo = ut_alloc_1d (4);
  /* shortcuts: */
  double *s = (*pGermSet).Size;	/* domain size          */
  double *ns = (*pGermSet).nSize;	/* domain neigh size    */

  /* Distributing the germs that are into the sd+neigh zone
   * these are the max sizes.
   */
  (*pGermSet).nGermsCoo = ut_alloc_2d (26 * (*pGermSet).N + 1, 4);
  (*pGermSet).GermToGerm = ut_alloc_1d_int (26 * (*pGermSet).N + 1);

  id = 0;
  for (per[3] = -1; per[3] <= 1; per[3]++)	/* Z-periodicity */
    for (per[2] = -1; per[2] <= 1; per[2]++)	/* Y-periodicity */
      for (per[1] = -1; per[1] <= 1; per[1]++)	/* X-periodicity */
	if (per[3] == 0 && per[2] == 0 && per[1] == 0)
	  continue;
	else
	  for (i = 1; i <= (*pGermSet).N; i++)
	  {
	    for (j = 1; j <= 3; j++)
	      coo[j] = (*pGermSet).GermsCoo[i][j] +
		per[j] * (s[2 * j] - s[2 * j - 1]);

	    if ((coo[1] > ns[1] && coo[1] < s[1])
		|| (coo[1] < ns[2] && coo[1] > s[2])
		|| (coo[2] > ns[3] && coo[2] < s[3])
		|| (coo[2] < ns[4] && coo[2] > s[4])
		|| (coo[3] > ns[5] && coo[3] < s[5])
		|| (coo[3] < ns[6] && coo[3] > s[6]))
	    {
	      id++;
	      for (j = 1; j <= 3; j++)
		(*pGermSet).nGermsCoo[id][j] = coo[j];

	      (*pGermSet).GermToGerm[id] = i;
	    }
	  }

  (*pGermSet).nN = id;

  ut_free_1d_int (per);
  ut_free_1d (coo);

  return;
}

extern void
GermSetGermsSD (struct GERMSET *pGermSet)
{
  int i, j, id;
  double *coo = ut_alloc_1d (4);
  int RandId;

  double *s = (*pGermSet).Size;
  double *ns = (*pGermSet).nSize;
  gsl_rng *r;


  /* Calculation of the number of germs into the neighbourhood zone
   * the mean is NeighRealVol*(*pGermSet).NDensity.
   * the rand seed is the same than for the inside germ, and
   * we take the second value (the first was for the inside germs).
   */
  (*pGermSet).nN = neut_rand_poisson ((*pGermSet).nVol * (*pGermSet).NDensity,
			     (*pGermSet).Random, 2);

  (*pGermSet).nGermsCoo = ut_alloc_2d ((*pGermSet).nN + 1, 4);
  (*pGermSet).nGermRandId = ut_alloc_1d_int ((*pGermSet).nN + 1);

  /* Calculating the neighbourhood germs coo */

  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, (*pGermSet).Random);

  /* skipping the 10*GermSet.NDensity first values:
   * near GermSet.NDensity are used for the inside germs
   * we skip 2*GermSet.NDensity more to avoid correllation
   * bw the germs positions of:
   * -MM -n 10 -id 1 -ttype 0 (case 1)
   * -MM -n 10 -id 1 -ttype 2 (case 2)
   * indeed, assuming that the second case has 7 inside germs, 
   * and if we do not skip 10*10 but 10, the position of the 8th
   * germ of case 1 would be correlated to the 1st neighbourhood
   * germ of case 2 
   * ((one more condition is that the 1st germ is
   * in the neigh zone (ie outside the domain))).
   * ... let's keep everything perfect :-) ...
   */

  for (i = 1; i <= 10 * (*pGermSet).NDensity; i++)
  {
    if (gsl_rng_uniform (r) > 10) // this is to trash
      r = NULL;
  }

  id = 0;
  RandId = 10 * (*pGermSet).NDensity;
  while (id != (*pGermSet).nN)
  {
    /* calculating tmp coo */
    for (j = 1; j <= 3; j++)
    {
      coo[j] =
	ns[2 * j - 1] + gsl_rng_uniform (r) * (ns[2 * j] - ns[2 * j - 1]);
      RandId++;
    }

    /* if the point is not within the domain */
    if ((coo[1] < s[1] || coo[1] > s[2])	/* actually: */
	|| (coo[2] < s[3] || coo[2] > s[4])	/* s[2i-1]=0 */
	|| (coo[3] < s[5] || coo[3] > s[6]))	/* s[2i]  =1 */
    {
      id++;			/* registering! */
      for (j = 1; j <= 3; j++)
	(*pGermSet).nGermsCoo[id][j] = coo[j];

      (*pGermSet).nGermRandId[id] = RandId - 2;
    }
  }

  gsl_rng_free (r);
  ut_free_1d (coo);

  return;
}

/* GermSet4EmpD defines the neighbouhood for an empty domain (no germ inside
 * it):
 * nSize, nN, nGermsCoo.
 */
void
GermSet4EmpD (struct GERMSET *pGermSet)
{
  int i, j, id;
  int tmpnN = 0, tmpnN2 = 0, RandId;
  double tmpnVol, tmpnVol2;
  double **tmpGermsCoo;
  int *tmpRandId;
  double dist;
  double *VerToGermDist = ut_alloc_1d (9);
  double *coo = ut_alloc_1d (4);
  double *tmpnSize = ut_alloc_1d (7);
  double **VerCoo = ut_alloc_2d (9, 3);

  double *s = (*pGermSet).Size;	/* MUST be [0,1]**3 */
  gsl_rng *r;

  VerCoo[1][0] = 0;
  VerCoo[1][1] = 0;
  VerCoo[1][2] = 0;
  VerCoo[2][0] = 1;
  VerCoo[2][1] = 0;
  VerCoo[2][2] = 0;
  VerCoo[3][0] = 1;
  VerCoo[3][1] = 1;
  VerCoo[3][2] = 0;
  VerCoo[4][0] = 0;
  VerCoo[4][1] = 1;
  VerCoo[4][2] = 0;
  VerCoo[5][0] = 0;
  VerCoo[5][1] = 0;
  VerCoo[5][2] = 1;
  VerCoo[6][0] = 1;
  VerCoo[6][1] = 0;
  VerCoo[6][2] = 1;
  VerCoo[7][0] = 1;
  VerCoo[7][1] = 1;
  VerCoo[7][2] = 1;
  VerCoo[8][0] = 0;
  VerCoo[8][1] = 1;
  VerCoo[8][2] = 1;
  tmpnSize[1] = tmpnSize[3] = tmpnSize[5] = 0;
  tmpnSize[2] = tmpnSize[4] = tmpnSize[6] = 1;

  for (i = 1; i <= 8; i++)
    VerToGermDist[i] = 1e32;

  /* Calculating the neighbourhood germs coo */

  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, (*pGermSet).Random);

  for (i = 1; i <= 10 * (*pGermSet).NDensity; i++)
    if (gsl_rng_uniform (r) > 10) // this is to trash
      r = NULL; 
  RandId = 10 * (*pGermSet).NDensity;

  /* The neigh size is increased until there is at least one germ
   * inside it. */
  while (tmpnN == 0)
  {
    for (i = 1; i <= 6; i++)
      tmpnSize[i] += pow (-1, i);

    tmpnVol = (tmpnSize[2] - tmpnSize[1]) *
      (tmpnSize[4] - tmpnSize[3]) * (tmpnSize[6] - tmpnSize[5]) - 1;

    tmpnN = neut_rand_poisson (tmpnVol * (*pGermSet).NDensity,
		       (*pGermSet).Random, 2);
  }

  tmpGermsCoo = ut_alloc_2d (tmpnN + 1, 4);
  tmpRandId = ut_alloc_1d_int (tmpnN + 1);

  /* distributing germs */
  id = 0;
  while (id != tmpnN)
  {
    /* calculating tmp coo */
    for (j = 1; j <= 3; j++)
    {
      coo[j] = tmpnSize[2 * j - 1] +
	gsl_rng_uniform (r) * (tmpnSize[2 * j] - tmpnSize[2 * j - 1]);
      RandId++;
    }

    /* if the point is not within the domain */
    if ((coo[1] < s[1] || coo[1] > s[2])
	|| (coo[2] < s[3] || coo[2] > s[4])
	|| (coo[3] < s[5] || coo[3] > s[6]))
    {
      id++;			/* registering! */
      for (j = 1; j <= 3; j++)
	tmpGermsCoo[id][j] = coo[j];

      tmpRandId[id] = RandId - 2;
    }
  }

  /* Calculating the minimum distances bw the vertices and the germs. 
   * to determine the real neighbouhood size 
   */
  for (i = 1; i <= 8; i++)
    for (j = 1; j <= tmpnN; j++)
    {
      dist = ut_space_dist (VerCoo[i], tmpGermsCoo[j] + 1);
      VerToGermDist[i] = ut_num_min (VerToGermDist[i], dist);
    }

  (*pGermSet).nSize = ut_alloc_1d (7);

  /* Calculating the real neighbouhood size from tmpnSize, the size of the
   * domain, and the germs distribution.
   */
  for (i = 1; i <= 3; i++)
  {
    /* lower bounds */
    (*pGermSet).nSize[2 * i - 1] = tmpnSize[2 * i - 1];

    for (j = 1; j <= 8; j++)
      (*pGermSet).nSize[2 * i - 1] =
	ut_num_min ((*pGermSet).nSize[2 * i - 1],
		   VerCoo[j][i] - VerToGermDist[j]);

    /* upper bounds */
    (*pGermSet).nSize[2 * i] = tmpnSize[2 * i];

    for (j = 1; j <= 8; j++)
      (*pGermSet).nSize[2 * i] =
	ut_num_max ((*pGermSet).nSize[2 * i], VerCoo[j][i] + VerToGermDist[j]);
  }
  CalcNeighVol (pGermSet);

  /* parameters for the extra neighbouring zone: vol and N */
  tmpnVol2 = ((*pGermSet).nSize[2] - (*pGermSet).nSize[1]) *
    ((*pGermSet).nSize[4] - (*pGermSet).nSize[3]) *
    ((*pGermSet).nSize[6] - (*pGermSet).nSize[5]) - tmpnVol - 1;

  tmpnN2 =
    neut_rand_poisson (tmpnVol2 * (*pGermSet).NDensity, (*pGermSet).Random, 3);

  /* Recording the real nN, nGermsCoo and nGermRandId */
  (*pGermSet).nN = tmpnN + tmpnN2;
  (*pGermSet).nGermsCoo = ut_alloc_2d ((*pGermSet).nN + 1, 4);
  (*pGermSet).nGermRandId = ut_alloc_1d_int ((*pGermSet).nN + 1);

  for (i = 1; i <= tmpnN; i++)
  {
    for (j = 1; j <= 3; j++)
      (*pGermSet).nGermsCoo[i][j] = tmpGermsCoo[i][j];

    (*pGermSet).nGermRandId[i] = tmpRandId[i];
  }

  id = tmpnN;
  while (id != (*pGermSet).nN)
  {
    /* calculating tmp coo */
    for (j = 1; j <= 3; j++)
    {
      coo[j] = (*pGermSet).nSize[2 * j - 1] +
	gsl_rng_uniform (r) * ((*pGermSet).nSize[2 * j] -
			       (*pGermSet).nSize[2 * j - 1]);
      RandId++;
    }

    /* if the point is not within the domain */
    if ((coo[1] < tmpnSize[1] || coo[1] > tmpnSize[2])
	|| (coo[2] < tmpnSize[3] || coo[2] > tmpnSize[4])
	|| (coo[3] < tmpnSize[5] || coo[3] > tmpnSize[6]))
    {
      id++;			/* registering! */
      for (j = 1; j <= 3; j++)
	(*pGermSet).nGermsCoo[id][j] = coo[j];

      (*pGermSet).nGermRandId[id] = RandId - 2;
    }
  }

  gsl_rng_free (r);

  ut_free_1d_int (tmpRandId);
  ut_free_2d (tmpGermsCoo, tmpnN + 1);

  ut_free_1d (VerToGermDist);
  ut_free_1d (coo);
  ut_free_1d (tmpnSize);
  ut_free_2d (VerCoo, 9);

  return;
}
