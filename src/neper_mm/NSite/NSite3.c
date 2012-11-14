/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "NSite.h"

/* CalcZoneASize calculates the internal zone size
 * The internal zone is taken as a cube (just one parameter).
 * The minimum width is the max width of the neigh size (MaxNeighWidth)
 * and the maximum value of MaxNeighWidth and nsitesize.
 * Finally, ceil(value) is taken for the subsequent germ distribution. 
 * It's finally an integer value.
 */
void
CalcZoneASize (struct IN *pIn, struct GERMSET GermSet, int *pZoneAWidth)
{
  int i;
  double Width = 0;

  /* Calculation of the max `width' of the (real, previously-defined)
   * neighbouring zone.
   */

  for (i = 1; i <= 3; i++)
  {
    Width = ut_num_max (Width, -GermSet.nSize[2 * i - 1]);
    Width = ut_num_max (Width, GermSet.nSize[2 * i] - 1);
  }

  /* Initializing nsitesize if not prescribed by the user. */
  if ((*pIn).nsitesize < 0)
    (*pIn).nsitesize = 2 / pow (GermSet.NDensity, 0.33);	/* set to the domain neigh' size. */

  Width = ut_num_max (Width, (*pIn).nsitesize);

  (*pZoneAWidth) = ceil (Width);
  /* printf("ZoneAWidth=%d\n",(*pZoneAWidth)); */

  return;
}

void
CalcZoneAGerms (struct GERMSET GermSet, int ZoneAWidth,
		int *pZoneAN, double ***pZoneAGermsCoo,
		int *prngseed, gsl_rng * r)
{
  int i;
  double *s = ut_alloc_1d (7);
  double *s2 = ut_alloc_1d (7);
  double tmp;
  int nWidthUp;
  double Vol;
  int *GermQty;
  int FirstId;

  /* DISTRIBUTE PROPERLY THE GERMS IN ZONE A */

  tmp = 0;
  for (i = 1; i <= 3; i++)
  {
    tmp = ut_num_max (tmp, -GermSet.nSize[2 * i - 1]);
    tmp = ut_num_max (tmp, GermSet.nSize[2 * i] - 1);
  }

  nWidthUp = ceil (tmp);
  /*printf("nWidthUp=%d\n",nWidthUp); */

  GermQty = ut_alloc_1d_int (1 + ZoneAWidth - nWidthUp);

  Vol = pow ((double) (1 + 2 * nWidthUp), 3) - (1 + GermSet.nVol);
  GermQty[0] = neut_rand_poisson (Vol * GermSet.NDensity, GermSet.Random, *prngseed);
  (*prngseed)++;

  for (i = 1; i <= ZoneAWidth - nWidthUp; i++)
  {
    Vol =
      pow ((double) (1 + 2 * (nWidthUp + i)),
	   3) - pow ((double) (1 + 2 * (nWidthUp + i - 1)), 3);
    /* printf("Vol=%lf\n",Vol); */
    GermQty[i] = neut_rand_poisson (Vol * GermSet.NDensity, GermSet.Random, *prngseed);
    (*prngseed)++;
  }

  (*pZoneAN) = 0;
  for (i = 0; i <= ZoneAWidth - nWidthUp; i++)
    (*pZoneAN) += GermQty[i];

  (*pZoneAGermsCoo) = ut_alloc_2d ((*pZoneAN) + 1, 4);

  s[1] = s[3] = s[5] = -nWidthUp;
  s[2] = s[4] = s[6] = 1 + nWidthUp;
  DistribPoints (s, GermSet.nSize, GermQty[0], (*pZoneAGermsCoo), r);

  /*printf("DISTRIB from %lf to %lf: %d germs\n",(double)(nWidthUp),(double)(nWidthUp+1),GermQty[0]); */

  FirstId = GermQty[0];
  for (i = 1; i <= ZoneAWidth - nWidthUp; i++)
  {
    /* printf("Zone A, layer %d/%d\n",i,ZoneAWidth-nWidthUp); */
    s2[1] = s2[3] = s2[5] = -(double) (nWidthUp + i - 1);
    s2[2] = s2[4] = s2[6] = 1 + (double) (nWidthUp + i - 1);
    s[1] = s[3] = s[5] = -(double) (nWidthUp + i);
    s[2] = s[4] = s[6] = 1 + (double) (nWidthUp + i);

    DistribPoints (s, s2, GermQty[i], (*pZoneAGermsCoo) + FirstId, r);
    FirstId += GermQty[i];
  }

  /*
     for(i=1;i<=(*pZoneAN);i++)
     printf("%lf %lf %lf\n",(*pZoneAGermsCoo)[i][1],(*pZoneAGermsCoo)[i][2],(*pZoneAGermsCoo)[i][3]);
     abort();
   */
  ut_free_1d (s);
  ut_free_1d (s2);

  return;
}

void
SearchNodePoly (struct GEO Geo, struct NODES Nodes, int **pNodePoly)
{
  int i, j;
  double *coo = ut_alloc_1d (4);

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    for (j = 1; j <= 3; j++)
      coo[j] = Nodes.NodeCoo[i][j - 1];

    for (j = 1; j <= Geo.PolyQty; j++)
      if (neut_geo_point_inpoly (Geo, coo, j) == 1)
      {
	(*pNodePoly)[i] = j;
	break;
      }

    /*printf("node %d: coo= %lf %lf %lf: site=%d\n",i,coo[1],coo[2],coo[3],(*pNodePoly)[i]); */

    if ((*pNodePoly)[i] == 0)
    {
      printf ("Node %d has no poly!\n", i);
      abort ();
    }
  }

  ut_free_1d (coo);

  return;
}


void
SearchSiteType (struct IN In, struct GEO Geo, struct NODES Nodes,
		struct MESH Mesh, int NSiteMeshSize, int *NodePoly,
		double ***pSiteCoo, int ***pSiteType, int MeshSize)
{
  int i, j;
  int *PolyList = ut_alloc_1d_int (8);
  int *EltPos = ut_alloc_1d_int (3);
  int EltId;

  /* Searching the number of polyhedra of every element */
  for (i = 1; i <= Mesh.EltQty; i++)
  {
    /* Building Site coo from the one of its node 1 */
    for (j = 0; j <= 2; j++)
      (*pSiteCoo)[i][j] = Nodes.NodeCoo[Mesh.EltNodes[i][1]][j]
	+ 0.5 / (double) MeshSize;

    for (j = 1; j <= 8; j++)
      PolyList[j - 1] = NodePoly[Mesh.EltNodes[i][j]];

    ut_array_1d_int_sort_uniq (PolyList, 8, &((*pSiteType)[i][0]));

    /*printf("i=%d/%d: type=%d\n",i,Mesh.EltQty,SiteType[i][0]); */
  }


  /* searching the number of (geometrically defined) tessellation vertices */
  /* that are inside the element */
  for (i = 1; i <= Geo.VerQty; i++)
  {
    /* if the poly ver is into the nsite zone */
    if (Geo.VerCoo[i][0] >= -In.nsitesize
	&& Geo.VerCoo[i][0] <= 1 + In.nsitesize
	&& Geo.VerCoo[i][1] >= -In.nsitesize
	&& Geo.VerCoo[i][1] <= 1 + In.nsitesize
	&& Geo.VerCoo[i][2] >= -In.nsitesize
	&& Geo.VerCoo[i][2] <= 1 + In.nsitesize)
    {

      for (j = 0; j <= 2; j++)
	EltPos[j] =
	  ceil ((Geo.VerCoo[i][j] +
		 (double) NSiteMeshSize / (double) MeshSize) *
		(double) MeshSize);

      EltId =
	EltCoo2Id (EltPos[0], EltPos[1], EltPos[2],
		   MeshSize + 2 * NSiteMeshSize,
		   MeshSize + 2 * NSiteMeshSize);

      if ((*pSiteType)[EltId][0] != 4)
	(*pSiteType)[EltId][0] = 4;
      (*pSiteType)[EltId][1]++;
    }

  }

  /* if an element is type>=4 and has no ((geometrically defined) tessellation vertex
   * inside it, it is said to be of type 3 (only element that have a tess vertex inside
   * them are type 4)
   */
  for (i = 1; i <= Mesh.EltQty; i++)
    if ((*pSiteType)[i][0] >= 4 && (*pSiteType)[i][1] == 0)
      (*pSiteType)[i][0] = 3;

  ut_free_1d_int (PolyList);
  ut_free_1d_int (EltPos);

  return;
}
