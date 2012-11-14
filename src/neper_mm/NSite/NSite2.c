/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "NSite.h"

void
CalcZoneA (struct IN *pIn, struct GERMSET GermSet,
	   int *pZoneAWidth, int *pZoneAN, double ***pZoneAGermsCoo,
	   int *prngseed, gsl_rng * r)
{
  CalcZoneASize (pIn, GermSet, pZoneAWidth);
  CalcZoneAGerms (GermSet, *pZoneAWidth, pZoneAN, pZoneAGermsCoo, prngseed, r);

  return;
}

void
CalcNSiteNeigh (struct GERMSET GermSet, struct GERMSET *pGermSet2,
		int ZoneAN, double **ZoneAGermsCoo, double nsitesize)
{
  int i, j, id;

  (*pGermSet2).Id = 0;

  /* Recording the size */
  (*pGermSet2).Size = ut_alloc_1d (7);
  (*pGermSet2).Size[1] = (*pGermSet2).Size[3] = (*pGermSet2).Size[5] =
    -nsitesize;
  (*pGermSet2).Size[2] = (*pGermSet2).Size[4] = (*pGermSet2).Size[6] =
    1 + nsitesize;

  /* Recording the germs coo
   * printf("GermSet.N=%d\n",GermSet.N);
   * printf("GermSet.nN=%d\n",GermSet.nN);
   * printf("ZoneAN=%d\n",ZoneAN);
   */

  (*pGermSet2).GermsCoo = ut_alloc_2d (GermSet.N + GermSet.nN + ZoneAN + 1, 4);

  id = 0;

  /* from GermSet domain */
  for (i = 1; i <= GermSet.N; i++)
  {
    id++;

    for (j = 1; j <= 3; j++)
      (*pGermSet2).GermsCoo[id][j] = GermSet.GermsCoo[i][j];
  }

  /* from GermSet neigh */
  for (i = 1; i <= GermSet.nN; i++)
    if (GermSet.nGermsCoo[i][1] > -nsitesize &&
	GermSet.nGermsCoo[i][1] < 1 + nsitesize &&
	GermSet.nGermsCoo[i][2] > -nsitesize &&
	GermSet.nGermsCoo[i][2] < 1 + nsitesize &&
	GermSet.nGermsCoo[i][3] > -nsitesize &&
	GermSet.nGermsCoo[i][3] < 1 + nsitesize)
    {
      id++;

      for (j = 1; j <= 3; j++)
	(*pGermSet2).GermsCoo[id][j] = GermSet.nGermsCoo[i][j];
    }

  /* from zone A */
  for (i = 1; i <= ZoneAN; i++)
    if (ZoneAGermsCoo[i][1] > -nsitesize &&
	ZoneAGermsCoo[i][1] < 1 + nsitesize &&
	ZoneAGermsCoo[i][2] > -nsitesize &&
	ZoneAGermsCoo[i][2] < 1 + nsitesize &&
	ZoneAGermsCoo[i][3] > -nsitesize &&
	ZoneAGermsCoo[i][3] < 1 + nsitesize)
    {
      id++;

      for (j = 1; j <= 3; j++)
	(*pGermSet2).GermsCoo[id][j] = ZoneAGermsCoo[i][j];
    }

  (*pGermSet2).N = id;

  GermSetSize (&(*pGermSet2));

  return;
}


/* Zone B is a cubic zone (integer width) that contains the nsite zone
 * width.
 */
void
CalcZoneB (struct GERMSET GermSet, struct GERMSET GermSet2, int ZoneAWidth,
	   int *pZoneBWidth, int *pZoneBN, double ***pZoneBGermsCoo,
	   int *prngseed, gsl_rng * r)
{
  int i;
  double Width;
  int *GermQty;
  double *s = ut_alloc_1d (7);
  double *s2 = ut_alloc_1d (7);
  double Vol;
  int FirstId;

  /* Calculating Width, which is the maximum distance bw Zone A
   * boundary and the domain boundary
   */
  Width = 0;
  for (i = 1; i <= 3; i++)
  {
    Width = ut_num_max (Width, -GermSet2.nSize[2 * i - 1]);
    Width = ut_num_max (Width, GermSet2.nSize[2 * i] - 1);
  }

  /* defining Zone B */

  if (Width <= (double) ZoneAWidth)	/* if nsite zone's neigh belongs to   */
  {				/* zone A, then zone B = zone A       */
    (*pZoneBN) = 0;		/* not bigger ...                     */
    (*pZoneBWidth) = ZoneAWidth;
    (*pZoneBGermsCoo) = NULL;
    /*printf("Zone B not needed: %lf<=%d\n",Width,ZoneAWidth); */
  }
  else				/* zone B is bigger than zone A */
  {
    (*pZoneBWidth) = ceil (Width);

    /* Determining germ qties */
    GermQty = ut_alloc_1d_int ((*pZoneBWidth) - ZoneAWidth + 1);

    (*pZoneBN) = 0;
    for (i = 1; i <= (*pZoneBWidth) - ZoneAWidth; i++)
    {
      Vol = pow ((double) (1 + 2 * (ZoneAWidth + i)), 3)
	- pow ((double) (1 + 2 * (ZoneAWidth + i - 1)), 3);

      GermQty[i] =
	neut_rand_poisson (Vol * GermSet.NDensity, GermSet.Random, *prngseed);
      (*prngseed)++;

      (*pZoneBN) += GermQty[i];
    }

    /*printf("Ntotal=%d\n",(*pZoneBN)); */

    (*pZoneBGermsCoo) = ut_alloc_2d ((*pZoneBN) + 1, 4);

    FirstId = 0;
    for (i = 1; i <= (*pZoneBWidth) - ZoneAWidth; i++)
    {
      /*printf("i=%d/%d\n",i,(*pZoneBWidth)-ZoneAWidth); */
      s2[1] = s2[3] = s2[5] = -(double) (ZoneAWidth + i - 1);
      s2[2] = s2[4] = s2[6] = 1 + (double) (ZoneAWidth + i - 1);
      s[1] = s[3] = s[5] = -(double) (ZoneAWidth + i);
      s[2] = s[4] = s[6] = 1 + (double) (ZoneAWidth + i);

      DistribPoints (s, s2, GermQty[i], (*pZoneBGermsCoo) + FirstId, r);
      FirstId += GermQty[i];
    }

    ut_free_1d_int (GermQty);
  }

  /*
     for(i=1;i<=(*pZoneBN);i++)
     printf("%lf %lf %lf\n",(*pZoneBGermsCoo)[i][1],(*pZoneBGermsCoo)[i][2],(*pZoneBGermsCoo)[i][3]);

     abort();
   */
  ut_free_1d (s);
  ut_free_1d (s2);

  return;
}

void
CreateNSitenNeighTess (struct GERMSET GermSet, struct GERMSET GermSet2,
		       int ZoneAN, double **ZoneAGermsCoo,
		       int ZoneBN, double **ZoneBGermsCoo, struct TESS *pTess)
{
  struct GERMSET GermSet0;
  int i, j, id;

  neut_germset_set_zero (&GermSet0);
  GermSet0.morpho = ut_alloc_1d_char (10);
  sprintf (GermSet0.morpho, "poisson");

  /* Building the tessellation of the nsite + nsite's neigh zone
   * Allocation with the maximal value.
   */
  GermSet0.GermsCoo =
    ut_alloc_2d (GermSet.N + GermSet.nN + ZoneAN + ZoneBN + 1, 4);

  id = 0;
  for (i = 1; i <= GermSet.N; i++)
  {
    id++;
    /*printf("1 id=%d/%d\n",id,GermSet.N); */
    for (j = 1; j <= 3; j++)
      GermSet0.GermsCoo[id][j] = GermSet.GermsCoo[i][j];
  }

  for (i = 1; i <= GermSet.nN; i++)
  {
    id++;
    /*printf("2 id=%d/%d\n",id,GermSet.N); */
    for (j = 1; j <= 3; j++)
      GermSet0.GermsCoo[id][j] = GermSet.nGermsCoo[i][j];
  }

  for (i = 1; i <= ZoneAN; i++)
    if (ZoneAGermsCoo[i][1] > GermSet2.nSize[1] &&
	ZoneAGermsCoo[i][1] < GermSet2.nSize[2] &&
	ZoneAGermsCoo[i][2] > GermSet2.nSize[3] &&
	ZoneAGermsCoo[i][2] < GermSet2.nSize[4] &&
	ZoneAGermsCoo[i][3] > GermSet2.nSize[5] &&
	ZoneAGermsCoo[i][3] < GermSet2.nSize[6])
    {
      id++;
      /*printf("3 id=%d/%d\n",id,GermSet.N); */
      for (j = 1; j <= 3; j++)
	GermSet0.GermsCoo[id][j] = ZoneAGermsCoo[i][j];
    }

  for (i = 1; i <= ZoneBN; i++)
    if (ZoneBGermsCoo[i][1] > GermSet2.nSize[1] &&
	ZoneBGermsCoo[i][1] < GermSet2.nSize[2] &&
	ZoneBGermsCoo[i][2] > GermSet2.nSize[3] &&
	ZoneBGermsCoo[i][2] < GermSet2.nSize[4] &&
	ZoneBGermsCoo[i][3] > GermSet2.nSize[5] &&
	ZoneBGermsCoo[i][3] < GermSet2.nSize[6])
    {
      id++;
      /*printf("4 id=%d/%d\n",id,GermSet.N); */
      for (j = 1; j <= 3; j++)
	GermSet0.GermsCoo[id][j] = ZoneBGermsCoo[i][j];
    }

  GermSet0.N = id;

  /*
     printf("tess: %d germs\n",GermSet.N);
     printf("size: %lf %lf\n",s[1],s[2]);
     printf("size: %lf %lf\n",s[3],s[4]);
     printf("size: %lf %lf\n",s[5],s[6]);

     for(i=1;i<=GermSet.N;i++)
     printf("%lf %lf %lf\n",GermSet.GermsCoo[i][1],
     GermSet.GermsCoo[i][2],GermSet.GermsCoo[i][3]);
   */

  CreateTess (GermSet0, GermSet2.nSize, pTess);

  return;
}

void
RecNSiteArray (struct IN In, struct GEO Geo, int MeshSize, int *pSiteQty,
	       double ***pSiteCoo, int ***pSiteType)
{
  struct NODES Nodes;
  struct MESH Mesh;
  int NSiteMeshSize;
  int *EQty = ut_alloc_1d_int (3);
  int **trash = NULL;
  double *GCoo = ut_alloc_1d (7);
  int *NodePoly;		/* NodePoly[i] = poly in which node i is. */

  /* Building mesh of the nsite zone */
  if (In.verbosity >= 1)
    ut_print_message (0, 3, "Creating nucleation sites ...\n");

  /* Calc the mesh size */
  NSiteMeshSize = ceil (In.nsitesize * MeshSize);

  GCoo[1] = GCoo[3] = GCoo[5] = -(double) NSiteMeshSize / (double) MeshSize;
  GCoo[2] = GCoo[4] = GCoo[6] =
    1 + (double) NSiteMeshSize / (double) MeshSize;

  /* Elt qty along one direction */
  EQty[0] = MeshSize + 2 * NSiteMeshSize;
  EQty[1] = EQty[0];
  EQty[2] = EQty[0];
  /*printf("EQty=%d\n",EQty); */

  BuildMMesh3D (EQty, 1, &Nodes, &Mesh, &trash);
  neut_nodes_scale (&Nodes, In.nsitesize, In.nsitesize, In.nsitesize);
  neut_nodes_shift (&Nodes, (1 - In.nsitesize) / 2,
		    (1 - In.nsitesize) / 2, (1 - In.nsitesize) / 2);

  if (In.verbosity >= 1)
    ut_print_message (0, 3, "Searching nucleation site types (step 1/2) ...\n");

  /* Searching node polyhedra. */
  NodePoly = ut_alloc_1d_int (Nodes.NodeQty + 1);

  SearchNodePoly (Geo, Nodes, &NodePoly);

  /* Searching site types */
  (*pSiteQty) = Mesh.EltQty;
  (*pSiteCoo) = ut_alloc_2d ((*pSiteQty) + 1, 3);
  (*pSiteType) = ut_alloc_2d_int ((*pSiteQty) + 1, 2);

  if (In.verbosity >= 1)
    ut_print_message (0, 3, "Searching nucleation site types (step 2/2) ...\n");

  SearchSiteType (In, Geo, Nodes, Mesh, NSiteMeshSize, NodePoly, pSiteCoo,
		  pSiteType, MeshSize);


  ut_free_1d_int (EQty);
  ut_free_1d (GCoo);
  ut_free_1d_int (NodePoly);

  return;
}


void
RecNSiteGeom (struct IN In, struct GEO Geo, int *pSiteQty,
	      double ***pSiteCoo, int ***pSiteType)
{
  int i, j;

  (*pSiteCoo) = ut_alloc_2d (Geo.VerQty + 1, 3);
  (*pSiteType) = ut_alloc_2d_int (Geo.VerQty + 1, 2);

  (*pSiteQty) = 0;
  for (i = 1; i <= Geo.VerQty; i++)
    if (Geo.VerCoo[i][0] > -In.nsitesize
	&& Geo.VerCoo[i][0] < 1 + In.nsitesize
	&& Geo.VerCoo[i][1] > -In.nsitesize
	&& Geo.VerCoo[i][1] < 1 + In.nsitesize
	&& Geo.VerCoo[i][2] > -In.nsitesize
	&& Geo.VerCoo[i][2] < 1 + In.nsitesize)
    {
      (*pSiteQty)++;
      for (j = 0; j <= 2; j++)
	(*pSiteCoo)[(*pSiteQty)][j] = Geo.VerCoo[i][j];
      (*pSiteType)[(*pSiteQty)][0] = 4;
      (*pSiteType)[(*pSiteQty)][1] = 1;
    }

  return;
}
