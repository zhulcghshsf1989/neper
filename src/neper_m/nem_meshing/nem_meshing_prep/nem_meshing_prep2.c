/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_prep.h"

int
nem_mesh2d_face_nproj (struct GEO Geo, struct NODES RNodes, struct MESH RMesh1D,
			struct MESH RMesh2D, int face, double* face_proj)
{
  int i;
  int status, iter, res;
  int nodeqty;
  int* nodes = NULL;
  double** coo = NULL;
  double theta;
  double* r = ol_r_alloc ();
  double** g = ol_g_alloc ();
  int itermax = 1000;
  double* n = ut_alloc_1d (3);

  int mesh_defined = (RNodes.NodeQty > 0) ? 1 : 0;

  // Fixing self-intersecting faces ------------------------------------
  srand48 (face);

  ut_array_1d_memcpy (face_proj, 3, Geo.FaceEq[face] + 1);

  nodeqty = 0;
  if (mesh_defined)
    neut_mesh_face_boundnodes (Geo, face, RMesh1D, &nodes, &nodeqty);

  coo = ut_alloc_2d (nodeqty, 3);
  
  res = 0;
  status = 0;
  iter = 1;
  do
  {
    // copying / projecting node coos
    for (i = 0; i < nodeqty; i++)
    {
      ut_array_1d_memcpy (coo[i], 3, RNodes.NodeCoo[nodes[i]]);
      ut_space_projpoint_alongonto (coo[i], face_proj, Geo.FaceEq[face]);
    }

    status = ut_space_contour_intersect (coo, nodeqty);

    if (status == 0 && mesh_defined)
    {
      for (i = 1; i <= RMesh2D.Elsets[face][0]; i++)
      {
	neut_mesh_eltnormal (RMesh2D, RNodes, RMesh2D.Elsets[face][i], n);
	if (ut_vector_scalprod (n, face_proj) < 0)
	{
	  status = 1;
	  // elt = RMesh2D.Elsets[face][i];
	  abort ();
	  break;
	}
      }
    }
    
    theta = 2 + 88 * pow ((double) iter / (double) itermax, 2);
    if (status == 1)
    {
      ol_nb_r (drand48 (), drand48 (), r);
      ol_rtheta_g (r, 10, g);
      ol_g_vect_vect (g, Geo.FaceEq[face] + 1, face_proj);
    }

    if (status != 0 && iter == 1)
      res = 1;

    if (status == 0 && iter > 1)
      printf ("face %d ok with angle %f\n", face, theta);
  }
  while (status == 1 && ++iter <= itermax);

  if (iter > itermax)
    res = -1;

  ut_free_1d_int (nodes);
  ol_r_free (r);
  ol_g_free (g);
  ut_free_1d (n);
  ut_free_2d (coo, nodeqty);

  return res;
}
