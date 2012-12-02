/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing.h"

int
nem_geo_mesh (struct IN In, struct GEOPARA GeoPara, struct GEO Geo, 
              struct NODES* pNodes, struct MESH* pMesh0D,
	      struct MESH* pMesh1D, struct MESH* pMesh2D,
	      struct MESH* pMesh3D)
{
  neut_gmsh_rc ("bak");

  if (GeoPara.dboundcl < 0)
    printf (" (cl = %.3g, pl = %.3g)\n", GeoPara.cl, GeoPara.pcl);
  else
  {
    printf (" (cl = %.3g, pl = %.3g,\n", GeoPara.cl, GeoPara.pcl);
    ut_print_message (0, 1, "             dboundcl = %.3g, dboundpl = %.3g)\n", GeoPara.dboundcl, GeoPara.dboundpcl);
  }

  double** face_eq = ut_alloc_2d (Geo.FaceQty + 1, 4);
  int* face_sim = ut_alloc_1d_int (Geo.FaceQty + 1);
  double*** face_sim_A = ut_alloc_3d (Geo.FaceQty + 1, 3, 3);
  double**  face_sim_B = ut_alloc_2d (Geo.FaceQty + 1, 3);

  if (In.meshdim >= 0)
    Premeshing (Geo, NULL, NULL, NULL,
	face_sim, face_sim_A, face_sim_B, face_eq, NULL, NULL);

  if (In.meshdim >= 0)
    Meshing0D (Geo, GeoPara, pNodes, pMesh0D);

  if (In.meshdim >= 1)
    Meshing1D (Geo, GeoPara, NULL, NULL, NULL, pNodes, pMesh1D);

  if (In.meshdim >= 2)
    Meshing2D (In, GeoPara, Geo, face_sim, face_sim_A, face_sim_B,
	face_eq, NULL, NULL, NULL, *pMesh0D, *pMesh1D, pNodes, pMesh2D);

  ut_free_2d (face_eq, Geo.FaceQty + 1);
  ut_free_1d_int (face_sim);
  ut_free_3d (face_sim_A, Geo.FaceQty + 1, 3);
  ut_free_2d (face_sim_B, Geo.FaceQty + 1);

  if (In.meshdim >= 3)
    Meshing3D (In, GeoPara, Geo, pNodes, *pMesh2D, pMesh3D);

  neut_gmsh_rc ("unbak");
  remove ("debugp.pos");
  remove ("debugr.pos");
  remove ("tmp.geo");
  remove ("tmp.msh");

  return EXIT_SUCCESS;
}
