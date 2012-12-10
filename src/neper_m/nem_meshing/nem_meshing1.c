/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing.h"

// This function applies meshing to a tessellation whose topology is
// described in the GEO structure. The edge and face morphologies (if
// not straight and flat, respectively) can be provided using with the
// pRNodes, pRMesh1D and pRMesh2D structures.  This is useful for
// remeshing.
int
nem_meshing (struct IN In, struct GEOPARA GeoPara, struct GEO Geo, 
             struct NODES RNodes, struct MESH RMesh1D, struct MESH RMesh2D,
             struct NODES* pNodes, struct MESH* pMesh0D,
	     struct MESH* pMesh1D, struct MESH* pMesh2D,
	     struct MESH* pMesh3D)
{
  double** face_eq = ut_alloc_2d (Geo.FaceQty + 1, 4);
  int* face_op = ut_alloc_1d_int (Geo.FaceQty + 1);
  int* edge_op = ut_alloc_1d_int (Geo.EdgeQty + 1);

  if (GeoPara.dboundcl < 0)
    printf (" (cl = %.3g, pl = %.3g)\n", GeoPara.cl, GeoPara.pcl);
  else
  {
    printf (" (cl = %.3g, pl = %.3g,\n", GeoPara.cl, GeoPara.pcl);
    ut_print_message (0, 1, "             dboundcl = %.3g, dboundpl = %.3g)\n", GeoPara.dboundcl, GeoPara.dboundpcl);
  }

  if (In.meshdim >= 0)
    nem_meshing_prep (Geo, RNodes, RMesh1D, RMesh2D, face_eq, edge_op, face_op);

  if (In.meshdim >= 0)
    nem_meshing_0D (Geo, GeoPara, pNodes, pMesh0D);

  if (In.meshdim >= 1)
    nem_meshing_1D (Geo, GeoPara, RNodes, RMesh1D, edge_op, pNodes, pMesh1D);

  if (In.meshdim >= 2)
  {
    nem_meshing_2D (In, GeoPara, Geo, face_eq, face_op, RNodes, RMesh2D, *pMesh1D, pNodes, pMesh2D);
    
    if (In.remesh)
      nem_mesh_pinching (Geo, face_eq, *pMesh1D, *pNodes, pMesh2D);
  }

  if (In.meshdim >= 3)
    nem_meshing_3D (In, GeoPara, Geo, pNodes, *pMesh2D, pMesh3D);

  ut_free_2d (face_eq, Geo.FaceQty + 1);
  ut_free_1d_int (face_op);
  ut_free_1d_int (edge_op);

  return EXIT_SUCCESS;
}
