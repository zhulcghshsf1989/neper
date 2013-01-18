/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WriteMeshAbq.h"

void
WriteMeshAbq (FILE* file, char* dim, struct NODES Nodes,
    struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
    struct MESH Mesh3D, struct NSET NSet0D, struct NSET NSet1D,
    struct NSET NSet2D, char* nset)
{
  int i, j, eltnodeqty2D, eltnodeqty3D;
  int seq2d[6]  = {0,1,2,3,4,5};
  int seq3d[10] = {0,1,2,3,4,5,6,7,9,8};
  int col;

  Mesh1D.EltQty = Mesh1D.EltQty;

  eltnodeqty2D = neut_elt_nodeqty ("tri", Mesh2D.Dimension, Mesh2D.EltOrder);
  eltnodeqty3D = neut_elt_nodeqty ("tri", Mesh3D.Dimension, Mesh3D.EltOrder);
  i = Mesh0D.Dimension;

/*----------------------------------------------------------------------
 * header */

  fprintf (file, "*Part, name=tess\n");

/*----------------------------------------------------------------------
 * nodes */

  fprintf (file, "*Node\n");
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d, ", i);
    for (j = 0; j < 2; j++)
      fprintf (file, "%.12f, ", Nodes.NodeCoo[i][j]);
    fprintf (file, "%.12f\n", Nodes.NodeCoo[i][2]);
  }

/*----------------------------------------------------------------------
 * elements */

  // 0D mesh
  /*
  if (ut_string_inlist (dim, ',', "0"))
  {
    if (Mesh0D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh0D, NULL);
    for (i = 1; i <= Mesh0D.EltQty; i++)
    {
      fprintf (file, "%d %d 3 0 %d 0 ", i, elt_type0D, Mesh0D.EltElset[i]);
      ut_array_1d_int_fprintf (file, Mesh0D.EltNodes[i], 1, "%d");
    }
  }

  // 1D mesh
  if (ut_string_inlist (dim, ',', "1"))
  {
    if (Mesh1D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh1D, NULL);

    for (i = 1; i <= Mesh1D.EltQty; i++)
    {
      fprintf (file, "%d %d 3 0 %d 0 ", i, elt_type1D, Mesh1D.EltElset[i]);
      ut_array_1d_int_fprintf (file, Mesh1D.EltNodes[i], eltnodeqty1D, "%d");
    }
  }

  // 2D mesh
  if (ut_string_inlist (dim, ',', "2"))
  {
    if (Mesh2D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh2D, NULL);

    for (i = 1; i <= Mesh2D.EltQty; i++)
    {
      fprintf (file, "%d %d 3 0 %d 0 ", i, elt_type2D, Mesh2D.EltElset[i]);
      ut_array_1d_int_fprintf (file, Mesh2D.EltNodes[i], eltnodeqty2D, "%d");
    }
  }
  */

// 2D elts -------------------------------------------------------------

  if (ut_string_inlist (dim, ',', "2"))
  {
    fprintf (file, "\n*Element, type=");
    if (Mesh2D.EltOrder == 1)
      fprintf (file, "CPE3\n");
    else if (Mesh2D.EltOrder == 2)
      fprintf (file, "CPE6\n");

    for (i = 1; i <= Mesh2D.EltQty; i++)
    {
      fprintf (file, "%d, ", i);
      for (j = 0; j < eltnodeqty2D - 1; j++)
	fprintf (file, "%d, ", Mesh2D.EltNodes[i][seq2d[j]]);
      fprintf (file, "%d\n", Mesh2D.EltNodes[i][seq2d[eltnodeqty2D - 1]]);
    }
  }

// 3D elts -------------------------------------------------------------

  int id_shift = ut_string_inlist (dim, ',', "2") ? Mesh2D.EltQty : 0;

  if (ut_string_inlist (dim, ',', "3"))
  {
    fprintf (file, "\n*Element, type=");
    if (Mesh3D.EltOrder == 1)
      fprintf (file, "C3D4\n");
    else if (Mesh3D.EltOrder == 2)
      fprintf (file, "C3D10\n");

    for (i = 1; i <= Mesh3D.EltQty; i++)
    {
      fprintf (file, "%d, ", i + id_shift);
      for (j = 0; j < eltnodeqty3D - 1; j++)
	fprintf (file, "%d, ", Mesh3D.EltNodes[i][seq3d[j]]);
      fprintf (file, "%d\n", Mesh3D.EltNodes[i][seq3d[eltnodeqty3D - 1]]);
    }
  }

// 2D elsets -----------------------------------------------------------

  if (ut_string_inlist (dim, ',', "2"))
  {
    for (i = 1; i <= Mesh2D.ElsetQty; i++)
    {
      fprintf (file, "\n*Elset, elset=face%d\n", i);
      col = 0;
      for (j = 1; j < Mesh2D.Elsets[i][0]; j++)
	ut_print_wnc (file, &col, 72, "%d, ", Mesh2D.Elsets[i][j]);
      ut_print_wnc (file, &col, 72, "%d\n", Mesh2D.Elsets[i][Mesh2D.Elsets[i][0]]);
    }
  }

// 3D elsets -----------------------------------------------------------

  if (ut_string_inlist (dim, ',', "3"))
  {
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
    {
      fprintf (file, "\n*Elset, elset=poly%d\n", i);
      col = 0;
      for (j = 1; j < Mesh3D.Elsets[i][0]; j++)
	ut_print_wnc (file, &col, 72, "%d, ", Mesh3D.Elsets[i][j] + id_shift);
      ut_print_wnc (file, &col, 72, "%d\n", Mesh3D.Elsets[i][Mesh3D.Elsets[i][0]] + id_shift);
    }
  }

// node sets -----------------------------------------------------------

  if (nset != NULL)
    neut_mesh_fprintf_abq_nsets (file, NSet0D, NSet1D, NSet2D, nset);

// foot ----------------------------------------------------------------

  fprintf (file, "*End Part\n");
  
  return;
}
