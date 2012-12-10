/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transport.h"
void
nem_transport (struct IN In, struct GEO Geo, struct NODES RNodes, struct
    MESH RMesh2D, struct MESH RMesh3D, struct NODES* pNodes, struct
    MESH* pMesh2D, struct MESH* pMesh3D)
{
  int i, j;
  int* oldelt = ut_alloc_1d_int ((*pMesh3D).EltQty + 1);
  FILE* file;
  char* name = NULL;
  int dim;

  ut_print_message (0, 2, "nem_transporting element data ...\n");
  
  nem_transport_mesh3d_parelt (Geo, RNodes, RMesh2D, RMesh3D, *pNodes, *pMesh2D, *pMesh3D, oldelt);

  for (i = 0; i < In.remap; i++)
  {
    ut_print_message (0, 3, "nem_transporting `%s' ...\n", In.remapspec[i][2]);

    if (strcmp (In.remapspec[i][0], "elt") != 0)
    {
      ut_print_message (2, 0, "Unknown type of data\n");
      printf ("data %d: %s %s %s\n", i + 1, In.remapspec[i][0],
			In.remapspec[i][1], In.remapspec[i][2]);
      ut_print_message (2, 0, "(must be `elt')\n");
      continue;
    }

    if (sscanf (In.remapspec[i][1], "real%d", &dim) == 1)
    {
      double** data = ut_alloc_2d (RMesh3D.EltQty + 1, dim);

      file = ut_file_open (In.remapspec[i][2], "r");
      for (j = 1; j <= RMesh3D.EltQty; j++)
	ut_array_1d_fscanf (file, data[j], dim);
      ut_file_close (file, In.remapspec[i][2], "r");

      name = ut_alloc_1d_char (strlen (In.remapspec[i][2]) + 5);
      sprintf (name, "%s.rem", In.remapspec[i][2]);

      file = ut_file_open (name, "w");
      for (j = 1; j <= (*pMesh3D).EltQty; j++)
	ut_array_1d_fprintf (file, data[oldelt[j]], dim, "%11.6f");
      ut_file_close (file, name, "w");
      ut_free_1d_char (name);

      ut_free_2d (data, RMesh3D.EltQty + 1);
    }
    else if (sscanf (In.remapspec[i][1], "int%d", &dim) == 1)
    {
      int** data = ut_alloc_2d_int (RMesh3D.EltQty + 1, dim);

      file = ut_file_open (In.remapspec[i][2], "r");
      for (j = 1; j <= RMesh3D.EltQty; j++)
	ut_array_1d_int_fscanf (file, data[j], dim);
      ut_file_close (file, In.remapspec[i][2], "r");

      name = ut_alloc_1d_char (strlen (In.remapspec[i][2]) + 5);
      sprintf (name, "%s.rem", In.remapspec[i][2]);

      file = ut_file_open (name, "w");
      for (j = 1; j <= (*pMesh3D).EltQty; j++)
	ut_array_1d_int_fprintf (file, data[oldelt[j]], dim, "%11.6f");
      ut_file_close (file, name, "w");
      ut_free_1d_char (name);

      ut_free_2d_int (data, RMesh3D.EltQty + 1);
    }
  }

  ut_free_1d_int (oldelt);

  return;
}
