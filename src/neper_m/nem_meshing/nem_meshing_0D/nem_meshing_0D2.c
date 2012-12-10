/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_0D.h"

void
VerMeshing (struct GEO Geo, int ver, struct GEOPARA GeoPara,
            struct NODES* pNodes, struct MESH* pMesh)
{
  int     i, edge;
  double  mindist;
  double  cl;
  double  pcl;
  double  clmin  = GeoPara.clmin;
  double* dist   = NULL;

  neut_nodes_free (pNodes);
  neut_mesh_free (pMesh);

/***********************************************************************
 * Initializing nodes and mesh */

  /* nodes */
  (*pNodes).NodeQty = 1;
  (*pNodes).NodeCoo = ut_alloc_2d (2, 3);
  (*pNodes).NodeCl  = ut_alloc_1d (2);

  ut_array_1d_memcpy ((*pNodes).NodeCoo[1], 3, Geo.VerCoo[ver]);

  /* mesh */
  (*pMesh).Dimension = 0;
  (*pMesh).EltOrder = 1;
  (*pMesh).EltQty = 1;
  (*pMesh).EltNodes = ut_alloc_2d_int (2, 1);
  (*pMesh).EltNodes[1][0] = 1;
  (*pMesh).ElsetQty = 1;
  (*pMesh).Elsets = ut_alloc_2d_int (2, 2);
  (*pMesh).Elsets[1][0] = 1;
  (*pMesh).Elsets[1][1] = 1;
  
/*********************************************************************** 
 * Nodes: if dbound ver & dboundvercl initialized, using it */

  if (GeoPara.dboundcl < 0)
  {
    cl  = GeoPara.cl;
    pcl  = GeoPara.pcl;
  }
  else 
  {
    int var_qty = 2;
    char** vars  = ut_alloc_2d_char (var_qty, 15);
    double* vals = ut_alloc_1d      (var_qty);
    int status = -1;
    double dbound;

    sprintf (vars[0], "body");
    sprintf (vars[1], "true");
    vals[0] = neut_geo_ver_polybodylevelmax (Geo, Geo.PolyBody, ver);
    vals[1] = neut_geo_ver_polytruelevelmax (Geo, Geo.PolyTrue, ver);

    status = ut_math_eval (GeoPara.dbound, var_qty, vars, vals, &dbound);

    if (status == -1)
      abort ();

    if (dbound == 1)
    {
      cl  = GeoPara.dboundcl;
      pcl = GeoPara.dboundpcl;
    }
    else
    {
      cl  = GeoPara.cl;
      pcl = GeoPara.pcl;
    }

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

/***********************************************************************
 * Nodes: calculating characteristic length from cl, pcl & clmin (&
 * mindist) */

  dist = ut_alloc_1d (Geo.VerEdgeQty[ver]);

  for (i = 0; i < Geo.VerEdgeQty[ver]; i++)
  {
    edge = Geo.VerEdgeNb[ver][i];
    dist[i] = Geo.EdgeLength[edge];
  }
  mindist = ut_array_1d_min (dist, Geo.VerEdgeQty[ver]);

  if (mindist * (1 + pcl) > cl)
    (*pNodes).NodeCl[1] = cl;
  else
    (*pNodes).NodeCl[1] = mindist * pcl;

  (*pNodes).NodeCl[1] = ut_num_max ((*pNodes).NodeCl[1], clmin);

  ut_free_1d (dist);

  return;
}
