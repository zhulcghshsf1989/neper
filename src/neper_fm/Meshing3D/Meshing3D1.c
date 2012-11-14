/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Meshing3D.h"

void
Meshing3D (struct IN In, struct GEOPARA GeoPara, struct GEO Geo,
	   struct NODES *pNodes, struct MESH Mesh2D, struct MESH *pMesh3D)
{
  int i;
  double allowed_t, max_elapsed_t;
  int a;
  char* format = ut_alloc_1d_char (128);
  char* message = ut_alloc_1d_char (128);
  char* tmpstring = ut_alloc_1d_char (128);
  struct MULTIM Multim;

  neut_multim_set_zero (&Multim);

  (*pMesh3D).Dimension = 3;
  (*pMesh3D).EltOrder = 1;
  (*pMesh3D).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh3D).EltType, "tri");

  allowed_t = In.mesh3dmaxtime;
  max_elapsed_t = 0;

  if (In.mesh3dreport == 1)
    if (system ("mkdir -p neper-report") == -1)
      abort ();

  // breaking algo string into mesher / optimizer
  
  char* mesh3dalgo = ut_alloc_1d_char (100);
  // retrieving list of algorithms to use
  if (strcmp (In.mesh3dalgo, "default") == 0)
    sprintf (mesh3dalgo, "netg/gmsh,netg/gmne");
  else if (strcmp (In.mesh3dalgo, "qualmax") == 0)
    sprintf (mesh3dalgo, "netg/gmsh,netg/netg,netg/gmne");
  else
    sprintf (mesh3dalgo, "%s", In.mesh3dalgo);

  neut_multim_fromlist (mesh3dalgo, &Multim);
  ut_free_1d_char (mesh3dalgo);

  ut_print_message (0, 2, "3D meshing ... ");

  double cl;

  int* meshpoly = ut_alloc_1d_int (Geo.PolyQty + 1);
  ut_array_1d_int_set (meshpoly + 1, Geo.PolyQty, 1);
  meshpoly[0] = Geo.PolyQty;

  if (In.meshpoly != NULL)
    neut_geo_expr_polytab (Geo, In.meshpoly, meshpoly);
  
  ut_print_progress (stdout, 0, Geo.PolyQty, "%3.0f%%", message);

  for (i = 1; i <= Geo.PolyQty; i++)
  {
    if (meshpoly[i] == 0)
      continue;

    // Setting parameter (cl) ------------------------------------------
    
    Meshing3D_poly_cl (GeoPara, Geo, i, &cl);

    // Meshing poly ----------------------------------------------------
    
    Meshing3D_poly (In, cl, &Multim, allowed_t,
	  &max_elapsed_t, Geo, pNodes, Mesh2D, pMesh3D, i);

    // Printing message ------------------------------------------------

    sprintf (format, "%%3.0f%%%% (%.2g|%.2g/",
		     ut_array_1d_min  (Multim.O + 1, i),
		     ut_array_1d_mean (Multim.O + 1, i));

    int* pct = ut_alloc_1d_int (Multim.algoqty);

    ut_array_1d_int_percent (Multim.algohit, Multim.algoqty, pct);

    for (a = 0; a < Multim.algoqty; a++)
    {
      sprintf (tmpstring, "%s%2d%%%c", format, pct[a], (a < Multim.algoqty - 1)? '|':')');
      sprintf (format, "%s", tmpstring);
    }

    ut_free_1d_int (pct);

    ut_print_progress (stdout, i, Geo.PolyQty, format, message);

    if (In.mesh3dreport == 1)
    {
      for (a = 0; a < Multim.algoqty; a++)
      {
	sprintf (tmpstring, "neper-report/poly%d-a%d-prop", i, a);
	FILE* file = ut_file_open (tmpstring, "W");

	fprintf (file, "%f ", Multim.mOdis[i][a]);
	fprintf (file, "%f ", Multim.mOsize[i][a]);
	fprintf (file, "%f\n", Multim.mO[i][a]);
	
	ut_file_close (file, tmpstring, "W");
      }

      sprintf (tmpstring, "neper-report/poly%d-prop", i);
      FILE* file = ut_file_open (tmpstring, "W");

      fprintf (file, "%d ", Multim.Oalgo[i]);
      fprintf (file, "%f ", Multim.Odis[i]);
      fprintf (file, "%f ", Multim.Osize[i]);
      fprintf (file, "%f\n", Multim.O[i]);
      
      ut_file_close (file, tmpstring, "W");
    }
  }

  ut_free_1d_int (meshpoly);

  neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);
  neut_multim_free (&Multim, Geo.PolyQty);
  ut_free_1d_char (format);
  ut_free_1d_char (message);
  ut_free_1d_char (tmpstring);

  return;
}
