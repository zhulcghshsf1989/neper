/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_prep.h"

void
nem_meshing_prep (struct TESS Tess, struct NODES RNodes,
            struct MESH RMesh1D, struct MESH RMesh2D,
	    double** face_proj, int* edge_op, int* face_op)
{
  int i, j, status;
  int edge_cpy;
  int face_cpy;
  char* message = ut_alloc_1d_char (8);

  ut_print_message (0, 2, "Preparing  ... ");

  /* face projection vector ***************** */

  if (face_op != NULL)
  {
    ut_array_1d_int_set (face_op + 1, Tess.FaceQty, 0);
    if (edge_op == NULL)
      ut_error_reportbug ();

    ut_array_1d_int_set (edge_op + 1, Tess.EdgeQty, 0);
  }

  ut_print_progress (stdout, 0, Tess.FaceQty, "%3.0f%%", message);
  for (i = 1; i <= Tess.FaceQty; i++)
  {
    status = nem_mesh2d_face_nproj (Tess, RNodes, RMesh1D, RMesh2D,
	                            i, face_proj[i]);

    if (status != 0)
    {
      if (face_op != NULL)
	face_op[i] = 1;
      else
	ut_error_reportbug ();
    }

    ut_print_progress (stdout, i, Tess.FaceQty, "%3.0f%%", message);
    fflush (stdout);
  }

  if (face_op != NULL)
  {
    for (i = 1; i <= Tess.FaceQty; i++)
      if (face_op[i] == 1)
	for (j = 1; j <= Tess.FaceVerQty[i]; j++)
	  edge_op[Tess.FaceEdgeNb[i][j]] = 1;

    edge_cpy = ut_array_1d_int_sum (edge_op + 1, Tess.EdgeQty);
    face_cpy = ut_array_1d_int_sum (face_op + 1, Tess.FaceQty);

    if (edge_cpy != 0 || face_cpy != 0)
    {
      ut_print_message (0, 3, "%d face%s (%d edge%s) cannot be meshed (will be copied).\n",
      face_cpy, (face_cpy > 1) ? "s": "", edge_cpy, (edge_cpy > 1) ? "s": "");
    }
  }

  ut_free_1d_char (message);

  return;
}
