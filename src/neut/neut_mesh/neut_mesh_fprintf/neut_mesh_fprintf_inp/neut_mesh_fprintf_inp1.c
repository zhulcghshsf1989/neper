/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_inp.h"

void
neut_mesh_fprintf_inp (FILE* file, char* dim, struct NODES Nodes,
    struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
    struct MESH Mesh3D, struct NSET NSet0D, struct NSET NSet1D,
    struct NSET NSet2D, char* nset)
{
  int i, j, eltnodeqty2D, eltnodeqty3D;
  int seq2d[6]  = {0,1,2,3,4,5};
  int seq3d[10] = {0,1,2,3,4,5,6,7,9,8};
  int col, call;

  Mesh1D.EltQty = Mesh1D.EltQty;

  eltnodeqty2D = neut_elt_nodeqty ("tri", Mesh2D.Dimension, Mesh2D.EltOrder);
  eltnodeqty3D = neut_elt_nodeqty ("tri", Mesh3D.Dimension, Mesh3D.EltOrder);
  i = Mesh0D.Dimension;

/*----------------------------------------------------------------------
 * header */

/*----------------------------------------------------------------------
 * nodes */

  fprintf (file, "*Node\n");
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 0; j < 3; j++)
      fprintf (file, ", %.12f",
	  (fabs (Nodes.NodeCoo[i][j]) < 1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }
  fprintf (file, "\n");

/*----------------------------------------------------------------------
 * elements */

// 2D elts -------------------------------------------------------------

  if (ut_string_inlist (dim, ',', "2"))
  {
    if (! strcmp (Mesh3D.EltType, "tri"))
    {
      fprintf (file, "*Element, type=");
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

    fprintf (file, "\n");
  }

// 3D elts -------------------------------------------------------------

  if (ut_string_inlist (dim, ',', "3"))
  {
    fprintf (file, "*Element, type=");

    if (! strcmp (Mesh3D.EltType, "tri"))
    {
      if (Mesh3D.EltOrder == 1)
	fprintf (file, "C3D4\n");
      else if (Mesh3D.EltOrder == 2)
	fprintf (file, "C3D10\n");

      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
	fprintf (file, "%d, ", i);
	for (j = 0; j < eltnodeqty3D - 1; j++)
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][seq3d[j]]);
	fprintf (file, "%d\n", Mesh3D.EltNodes[i][seq3d[eltnodeqty3D - 1]]);
      }
    }

    else if (! strcmp (Mesh3D.EltType, "quad"))
    {
      if (Mesh3D.EltOrder == 1)
      {
	fprintf (file, "C3D8\n");

	for (i = 1; i <= Mesh3D.EltQty; i++)
	{
	  fprintf (file, "%d,", i);

	  fprintf (file, " %d," , Mesh3D.EltNodes[i][0]);
	  fprintf (file, " %d," , Mesh3D.EltNodes[i][1]);
	  fprintf (file, " %d," , Mesh3D.EltNodes[i][2]);
	  fprintf (file, " %d," , Mesh3D.EltNodes[i][3]);
	  fprintf (file, " %d," , Mesh3D.EltNodes[i][4]);
	  fprintf (file, " %d," , Mesh3D.EltNodes[i][5]);
	  fprintf (file, " %d," , Mesh3D.EltNodes[i][6]);
	  fprintf (file, " %d\n", Mesh3D.EltNodes[i][7]);
	}
      }
      else
      {
	fprintf (file, "C3D20\n");

	for (i = 1; i <= Mesh3D.EltQty; i++)
	{
	  fprintf (file, "%d, ", i);

	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][0]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][1]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][2]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][3]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][4]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][5]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][6]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][7]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][8]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][11]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][13]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][9]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][16]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][18]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][19]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][17]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][10]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][12]);
	  fprintf (file, "%d, ", Mesh3D.EltNodes[i][14]);
	  fprintf (file, "%d\n", Mesh3D.EltNodes[i][15]);
	}
      }
    }

    else 
      ut_error_reportbug ();

    fprintf (file, "\n");
  }

// 2D elsets -----------------------------------------------------------

  if (ut_string_inlist (dim, ',', "2"))
  {
    for (i = 1; i <= Mesh2D.ElsetQty; i++)
    {
      fprintf (file, "*Elset, elset=face%d\n", i);
      col = 0;
      call = 0;
      for (j = 1; j <= Mesh2D.Elsets[i][0]; j++)
	ut_print_wnc_wncall (file, &col, 72, &call, 16, 
	    (j < Mesh2D.Elsets[i][0]) ? "%d," : "%d\n", Mesh2D.Elsets[i][j]);
      fprintf (file, "\n");
    }
  }

// 3D elsets -----------------------------------------------------------

  if (ut_string_inlist (dim, ',', "3"))
  {
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
    {
      fprintf (file, "*Elset, elset=poly%d\n", i);
      col = 0;
      call = 0;
      for (j = 1; j <= Mesh3D.Elsets[i][0]; j++)
	ut_print_wnc_wncall (file, &col, 72, &call, 16, 
	    (j < Mesh3D.Elsets[i][0]) ? "%d," : "%d\n", Mesh3D.Elsets[i][j]);
      fprintf (file, "\n");
    }
  }

// node sets -----------------------------------------------------------

  if (nset != NULL)
    neut_mesh_fprintf_inp_nsets (file, NSet0D, NSet1D, NSet2D, nset);
  
  return;
}