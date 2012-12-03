/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Premeshing.h"

void
Premeshing (struct GEO Geo,
	    struct NODES* pRNodes, struct MESH* pRMesh1D, 
	    struct MESH* pRMesh2D,
	    int* face_sim, double*** face_sim_A, double** face_sim_B,
	    double** face_proj, int* edge_op, int* face_op)
{
  int i, j, status;
  int edge_cpy;
  int face_cpy;
  face_sim_A = face_sim_A;
  face_sim_B = face_sim_B;
  char* message = ut_alloc_1d_char (8);

  if (face_sim != NULL)
  {
    ut_array_1d_int_zero (face_sim, Geo.FaceQty + 1);
    // nefm_geo_face_sim (Geo, face_sim, face_sim_A, face_sim_B);
  }
  
  ut_print_message (0, 2, "Preparing  ... ");

  /* face projection vector ***************** */

  if (face_op != NULL)
  {
    ut_array_1d_int_set (face_op + 1, Geo.FaceQty, 0);
    if (edge_op == NULL)
      ut_error_reportbug ();

    ut_array_1d_int_set (edge_op + 1, Geo.EdgeQty, 0);
  }

  ut_print_progress (stdout, 0, Geo.FaceQty, "%3.0f%%", message);
  for (i = 1; i <= Geo.FaceQty; i++)
  {
    status = nefm_mesh2d_face_nproj (Geo,
	pRNodes, pRMesh1D, pRMesh2D,
	i, face_proj[i]);

    /*
    printf ("face %d status = %d\n", i, status);
    if (status != 0)
      abort ();
    */

    if (status != 0)
    {
      if (face_op != NULL)
	face_op[i] = 1;
      else
	ut_error_reportbug ();
    }

    ut_print_progress (stdout, i, Geo.FaceQty, "%3.0f%%", message);
    fflush (stdout);
  }

  if (face_op != NULL)
  {
    for (i = 1; i <= Geo.FaceQty; i++)
      if (face_op[i] == 1)
	for (j = 1; j <= Geo.FaceVerQty[i]; j++)
	  edge_op[Geo.FaceEdgeNb[i][j]] = 1;

    edge_cpy = ut_array_1d_int_sum (edge_op + 1, Geo.EdgeQty);
    face_cpy = ut_array_1d_int_sum (face_op + 1, Geo.FaceQty);

    if (edge_cpy != 0 || face_cpy != 0)
    {
      ut_print_message (0, 3, "%d face%s (%d edge%s) cannot be remeshed (will be copied).\n",
      face_cpy, (face_cpy > 1) ? "s": "", edge_cpy, (edge_cpy > 1) ? "s": "");
    }
  }

  ut_free_1d_char (message);

  return;
}
