/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Premeshing.h"

void
nefm_geo_face_sim (struct GEO Geo, int* face_sim, double*** face_sim_A,
    double** face_sim_B)

{
  int i, j, ver;
  
  // for each face, looking for a face with the same properties
  
  double** face_center = ut_alloc_2d (Geo.FaceQty + 1, 3);
  double*** face_coo = ut_alloc_1d_ppdouble (Geo.FaceQty + 1);
  for (i = 1; i <= Geo.FaceQty; i++)
  {
    printf ("--- face %d --------------------------- \n", i);

    face_coo[i] = ut_alloc_2d (Geo.FaceVerQty[i], 3);
    for (j = 1; j <= Geo.FaceVerQty[i]; j++)
    {
      ver = Geo.FaceVerNb[i][j];
      ut_array_1d_memcpy (face_coo[i][j - 1], 3, Geo.VerCoo[ver]);
      ut_array_1d_add (face_center[i], Geo.VerCoo[ver], 3, face_center[i]);
    }
    ut_array_1d_scale (face_center[i], 3, 1. / Geo.FaceVerQty[i]);

    ut_array_1d_scale (face_center[i], 3, -1);
    for (j = 1; j <= Geo.FaceVerQty[i]; j++)
      ut_array_1d_add (face_coo[i][j - 1], face_center[i], 3, face_coo[i][j - 1]);
    ut_array_1d_scale (face_center[i], 3, -1);

    // for face i, comparing to the previous surfaces
    int k, l, master, verqty;
    int match = 0;
    int ver_match = 0;
    for (master = 1; master < i; master++)
    {
      printf ("  master = %d\n", master);

      match = 1;
      if (Geo.FaceVerQty[master] != Geo.FaceVerQty[i])
      {
	printf ("    verqty does not match\n");
	match = 0;
	continue;
      }
      
      verqty = Geo.FaceVerQty[master];

      for (k = 0; k < verqty; k++)
      {
	ver_match = 0;
	for (l = 0; l < verqty; l++)
	  if (ut_space_dist (face_coo[i][k], face_coo[master][l]) < 1e-9)
	  {
	    ver_match = 1;
	    break;
	  }
	if (ver_match == 0)
	{
	  printf ("    ver_match = %d\n", ver_match);
	  match = 0;
	  break;
	}
      }
      
      if (match == 1)
	break;
    }
    if (match == 1)
    {
      face_sim[i] = master;
      ol_g_set_id (face_sim_A[i]);
      if (ut_vector_scalprod (Geo.FaceEq[i] + 1, Geo.FaceEq[master] + 1) < 0)
	face_sim[i] *= -1;
      for (j = 0; j < 3; j++)
	face_sim_B[i][j] = face_center[i][j] - face_center[master][j];
    }
  }
  ut_free_2d (face_center, Geo.FaceQty + 1);
  // ut_free_3d (face_coo, Geo.FaceQty + 1, Geo.FaceVerQty[i]);

  return;
}


int
nefm_mesh2d_face_nproj (struct GEO Geo,
                        struct NODES* pRNodes, struct MESH* pRMesh1D,
			struct MESH* pRMesh2D,
			int face, double* face_proj)
{
  int i;
  int status, iter, res;
  int nodeqty = 0;
  int* nodes = NULL;
  double** coo = NULL;
  double theta;
  double* r = ol_r_alloc ();
  double** g = ol_g_alloc ();
  int itermax = 1000;
  double* n = ut_alloc_1d (3);

  // Fixing self-intersecting faces ------------------------------------
  srand48 (face);

  ut_array_1d_memcpy (face_proj, 3, Geo.FaceEq[face] + 1);

  if (pRNodes != NULL)
    neut_mesh_face_boundnodes (Geo, face, *pRMesh1D, &nodes, &nodeqty);

  coo = ut_alloc_2d (nodeqty, 3);
  
  res = 0;
  status = 0;
  iter = 1;
  do
  {
    // copying / projecting node coos
    for (i = 0; i < nodeqty; i++)
    {
      ut_array_1d_memcpy (coo[i], 3, (*pRNodes).NodeCoo[nodes[i]]);
      ut_space_projpoint_alongonto (coo[i], face_proj, Geo.FaceEq[face]);
    }

    status = ut_space_contour_intersect (coo, nodeqty);

    if (status == 0 && pRNodes != NULL)
    {
      for (i = 1; i <= (*pRMesh2D).Elsets[face][0]; i++)
      {
	neut_mesh_eltnormal (*pRMesh2D, *pRNodes, (*pRMesh2D).Elsets[face][i], n);
	if (ut_vector_scalprod (n, face_proj) < 0)
	{
	  status = 1;
	  // elt = (*pRMesh2D).Elsets[face][i];
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
