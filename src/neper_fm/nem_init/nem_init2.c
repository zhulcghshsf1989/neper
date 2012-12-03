/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

/* This file is ext of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_init.h"

void
nem_init_remeshingmapping_geo_updating (struct GEO* pGeo, struct NODES
    RNodes, struct MESH RMesh0D, struct MESH RMesh1D, struct MESH
    RMesh2D, struct MESH RMesh3D)
{
  int i, j, elt, node;
  double length, norm;
  double* eq = ut_alloc_1d (4);
  double* eqe = ut_alloc_1d (4);

  ut_print_message (0, 2, "Retrieving geometry properties ...\n");

  // reinitializing geo properties from nodes / mesh
  // ver coo
  for (i = 1; i <= (*pGeo).VerQty; i++)
  {
    node = RMesh0D.EltNodes[i][0];
    ut_array_1d_memcpy ((*pGeo).VerCoo[i], 3, RNodes.NodeCoo[node]);
  }
  
  // edge length (sum over the elements to account for curvature)
  for (i = 1; i <= (*pGeo).EdgeQty; i++)
  {
    length = 0;
    for (j = 1; j <= RMesh1D.Elsets[i][0]; j++)
    {
      elt = RMesh1D.Elsets[i][j];
      length += ut_space_dist (RNodes.NodeCoo[RMesh1D.EltNodes[elt][0]],
                               RNodes.NodeCoo[RMesh1D.EltNodes[elt][1]]);
    }
    (*pGeo).EdgeLength[i] = length;
  }
  
  // face equations
  // the elt normals are averaged, so as the constant for the equation
  // the constant does not really matter because it does not change the
  // projection (which is done along the normal).

  for (i = 1; i <= (*pGeo).FaceQty; i++)
  {
    (*pGeo).FaceState[i] = 1;

    ut_array_1d_zero (eq, 4);
    for (j = 1; j <= RMesh2D.Elsets[i][0]; j++)
    {
      elt = RMesh2D.Elsets[i][j];
      neut_mesh_elteq (RMesh2D, RNodes, elt, eqe);
      ut_array_1d_add (eqe, eq, 4, eq);
    }

    norm = ut_vector_norm (eq + 1);
    ut_array_1d_scale (eq, 4, 1./norm);

    ut_array_1d_memcpy ((*pGeo).FaceEq[i], 4, eq);
  }

  // poly centre: centre of mass of the elsets
  for (i = 1; i <= (*pGeo).PolyQty; i++)
    neut_mesh_elset_centre (RNodes, RMesh3D, i, (*pGeo).CenterCoo[i]);

  ut_free_1d (eqe);
  ut_free_1d (eq);
  
  return;
}
