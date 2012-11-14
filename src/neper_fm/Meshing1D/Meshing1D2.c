/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Meshing1D.h"

void
EdgeMeshing (struct GEO Geo, int edge, double cl, double pcl,
             struct NODES Nodes, struct NODES* pN, struct MESH* pM)
{
  int i, j, node1, node2, seg, segqty, ptqty;
  int seg1ptqty = -1;
  double nextcoo, nextcooseg1;

  double **Seg = ut_alloc_2d (4, 3);
  double x, x1, x2, X;
  double *Coo, *Cl;

  double nearone = 0.95;
  
  /* pcl is the factor between 2 successive segments of an edge; */
  /* PCL is the maximum absolute slope of the curve cl=f(x).     */
  /* PCL = pcl-1 > 0                                             */
  double PCL = ut_num_max (0.001, pcl - 1);

  double l = Geo.EdgeLength[edge];
  double cl1 = Nodes.NodeCl[Geo.EdgeVerNb[edge][0]];
  double cl2 = Nodes.NodeCl[Geo.EdgeVerNb[edge][1]];

  if (cl <= 0 || pcl < 1)
  {
    ut_print_message (2, 0, "Bad values of cl or pcl: cl = %f pcl = %f\n", cl, pcl);
    abort ();
  }

  cl1 = Nodes.NodeCl[Geo.EdgeVerNb[edge][0]];
  cl2 = Nodes.NodeCl[Geo.EdgeVerNb[edge][1]];
  
  cl1 /= l;
  cl2 /= l;
  cl  /= l;

  /* 10 for security */
  Coo = ut_alloc_1d (10 + (int) (1 / (ut_num_min (cl1, cl2))));
  Cl = ut_alloc_1d (10 + (int) (1 / (ut_num_min (cl1, cl2))));

  if (cl1 > 1.0000001 * cl || cl2 > 1.0000001 * cl || PCL < 0)
  {
    ut_print_message (2, 0, "EdgeMeshing: Bad values of cl1, cl2 & cl\n");
    printf ("cl1 = %f cl2 = %f cl = %f\n", cl1, cl2, cl);
    printf ("(these are relative values, divided by l) l = %f\n", l);

    int ver1 = Geo.EdgeVerNb[edge][0];
    int ver2 = Geo.EdgeVerNb[edge][1];
    printf ("edge = %d of vers = %d %d\n", edge, ver1, ver2);

    printf ("edgepolylevelmax = %d\n", neut_geo_edge_polytruelevelmax (Geo, Geo.PolyTrue, edge));
    printf ("ver1polylevelmax = %d\n", neut_geo_ver_polytruelevelmax (Geo, Geo.PolyTrue, ver1));
    printf ("ver2polylevelmax = %d\n", neut_geo_ver_polytruelevelmax (Geo, Geo.PolyTrue, ver2));

    ut_error_reportbug ();
  }

  if (cl1 / cl > nearone && cl2 / cl > nearone)
  {
    segqty = 1;
    Seg[1][0] = 1;
    Seg[1][1] = 0;
    Seg[1][2] = cl;
  }
  else if (cl1 / cl > nearone)
  {
    x = 1 - (cl - cl2) / PCL;

    if (x > 0)
    {
      segqty = 2;
      Seg[1][0] = x;
      Seg[1][1] = 0;
      Seg[1][2] = cl;
      Seg[2][0] = 1;
      Seg[2][1] = -PCL;
      Seg[2][2] = cl2 + PCL;
    }
    else
    {
      segqty = 1;
      Seg[1][0] = 1;
      Seg[1][1] = cl2 - cl;
      Seg[1][2] = cl;
    }
  }
  else if (cl2 / cl > nearone)
  {
    x = (cl - cl1) / PCL;

    if (x < 1)
    {
      segqty = 2;
      Seg[1][0] = x;
      Seg[1][1] = PCL;
      Seg[1][2] = cl1;
      Seg[2][0] = 1;
      Seg[2][1] = 0;
      Seg[2][2] = cl;
    }
    else
    {
      segqty = 1;
      Seg[1][0] = 1;
      Seg[1][1] = cl - cl1;
      Seg[1][2] = cl1;
    }
  }
  else /* cl1 & cl2 are different from cl: 1, 2 or 3 parts? */
  {
    x = 0.5 + (cl2 - cl1) / (2 * PCL);

    if (x < 0 || x > 1)
    {
      /* PCL=cl2-cl1 is used to go from cl1 to cl2 */
      segqty = 1;

      Seg[1][0] = 1;
      Seg[1][1] = cl2 - cl1;
      Seg[1][2] = cl1;
    }
    else if (PCL * x + cl1 < cl)
    {
      segqty = 2;

      /* cl = PCL*x + cl1 */
      Seg[1][0] = x;
      Seg[1][1] = PCL;
      Seg[1][2] = cl1;
      /* cl = PCL*(1-x) + cl2 = cl2+PCL -PCL*x */
      Seg[2][0] = 1;
      Seg[2][1] = -PCL;
      Seg[2][2] = cl2 + PCL;
    }
    else			/* 3 parts */
    {
      segqty = 3;

      x1 = (cl - cl1) / PCL;
      x2 = (cl2 + PCL - cl) / PCL;

      if (x1 > x || x2 < x)
      {
	ut_error_reportbug ();
	printf ("Bad x1, x, x2\n");
	abort ();
      }

      /* cl = PCL*x + cl1 */
      Seg[1][0] = x1;
      Seg[1][1] = PCL;
      Seg[1][2] = cl1;
      /* cl = cl */
      Seg[2][0] = x2;
      Seg[2][1] = 0;
      Seg[2][2] = cl;
      /* cl = PCL*(1-x) + cl2 = cl2+PCL -PCL*x */
      Seg[3][0] = 1;
      Seg[3][1] = -PCL;
      Seg[3][2] = cl2 + PCL;
    }
  }

  /* Calculation of Coo */
  Coo[0] = 0;
  ptqty = 0;
  for (i = 1; i <= segqty; i++)
  {
    nextcoo = NextCoo (Coo[ptqty], Seg[i][1], Seg[i][2]);

    if (ptqty == seg1ptqty)
    {
      nextcooseg1 = NextCoo (Coo[ptqty], Seg[1][1], Seg[1][2]);
      if (nextcooseg1 < nextcoo)
	nextcoo = nextcooseg1;
    }

    while (nextcoo < Seg[i][0])
    {
      ptqty++;
      Coo[ptqty] = nextcoo;

      nextcoo = NextCoo (Coo[ptqty], Seg[i][1], Seg[i][2]);
    }

    if (i == 1)
      seg1ptqty = ptqty;
  }

  if (ptqty == seg1ptqty && Seg[1][1] < 0)
    X = Coo[ptqty] + Seg[1][1] * Coo[ptqty] + Seg[1][2];
  else
    X = Coo[ptqty] + cl2;

  for (i = 1; i <= ptqty; i++)
    Coo[i] /= X;

  /* Calculation of Cl */
  seg = 1;
  for (i = 1; i <= ptqty; i++)
    if (Coo[i] < Seg[seg][0])
      Cl[i] = Seg[seg][1] * Coo[i] + Seg[seg][2];
    else
    {
      seg++;
      i--;
    }

/***********************************************************************
 * recording N */

  neut_nodes_set_zero (pN);

  node1 = Geo.EdgeVerNb[edge][0];
  node2 = Geo.EdgeVerNb[edge][1];

  (*pN).NodeQty = 2 + ptqty;
  (*pN).NodeCoo = ut_alloc_2d (3 + ptqty, 3);
  (*pN).NodeCl  = ut_alloc_1d (3 + ptqty);

  // recording coo of the boundary nodes (which are actually recorded as
  // 0D-mesh nodes)
  ut_array_1d_memcpy ((*pN).NodeCoo[1], 3, Nodes.NodeCoo[node1]);
  ut_array_1d_memcpy ((*pN).NodeCoo[ptqty + 2], 3, Nodes.NodeCoo[node2]);
    
  /* recording properties for the body nodes (coo + cl) */
  for (i = 1; i <= ptqty; i++)
  {
    /* i'st body node has pos i + 1 */
    for (j = 0; j < 3; j++)
      (*pN).NodeCoo[i + 1][j] =
	  (1 - Coo[i]) * Nodes.NodeCoo[node1][j] 
	      + Coo[i] * Nodes.NodeCoo[node2][j];

    (*pN).NodeCl[i + 1] = Cl[i] * l;
  }

/*********************************************************************** 
 * recording M */

  neut_mesh_set_zero (pM);

  (*pM).Dimension = 1;
  (*pM).EltOrder = 1;
  (*pM).EltQty = ptqty + 1;

  (*pM).EltNodes = ut_alloc_2d_int ((*pM).EltQty + 1, 2);
  for (i = 1; i <= (*pM).EltQty; i++)
  {
    (*pM).EltNodes[i][0] = i;
    (*pM).EltNodes[i][1] = i + 1;
  }

  (*pM).ElsetQty = 1;
  (*pM).Elsets = ut_alloc_2d_int ((*pM).ElsetQty + 1, (*pM).EltQty + 1);
  (*pM).Elsets[1][0] = (*pM).EltQty;
  for (i = 1; i <= (*pM).EltQty; i++)
    (*pM).Elsets[1][i] = i;

  ut_free_1d (Coo);
  ut_free_1d (Cl);

  ut_free_2d (Seg, 4);

  return;
}
