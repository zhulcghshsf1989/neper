/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_geof_lcl.h"

void
neut_mesh_fprintf_geof_head (FILE* file)
{
  fprintf (file, "***geometry\n");

  return;
}

void
neut_mesh_fprintf_geof_nodes (FILE* out, struct NODES Nodes)
{
  int i, j;

  fprintf (out, "**node\n");
  fprintf (out, "%d 3\n", Nodes.NodeQty);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (out, "%d", i);
    for (j = 0; j < 3; j++)
      fprintf (out, "  %.12f",
	  (fabs (Nodes.NodeCoo[i][j]) < 1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (out, "\n");
  }

  return;
}

void
neut_mesh_fprintf_geof_elts (FILE* out, struct MESH Mesh)
{
  int i, j;

  fprintf (out, "\n**element\n");
  fprintf (out, "%d\n", Mesh.EltQty);

  if (! strcmp (Mesh.EltType, "tri"))
  {
    if (Mesh.Dimension == 3 && Mesh.EltOrder == 1)
      for (i = 1; i <= Mesh.EltQty; i++)
      {
	fprintf (out, "%d c3d4", i);
	for (j = 0; j < 4; j++)
	  fprintf (out, " %d", Mesh.EltNodes[i][j]);
	fprintf (out, "\n");
      }
    else if (Mesh.Dimension == 3 && Mesh.EltOrder == 2)
      for (i = 1; i <= Mesh.EltQty; i++)
      {
	fprintf (out, "%d c3d10", i);
	fprintf (out, " %d", Mesh.EltNodes[i][0]);
	fprintf (out, " %d", Mesh.EltNodes[i][1]);
	fprintf (out, " %d", Mesh.EltNodes[i][2]);
	fprintf (out, " %d", Mesh.EltNodes[i][4]);
	fprintf (out, " %d", Mesh.EltNodes[i][5]);
	fprintf (out, " %d", Mesh.EltNodes[i][6]);
	fprintf (out, " %d", Mesh.EltNodes[i][7]);
	fprintf (out, " %d", Mesh.EltNodes[i][9]);
	fprintf (out, " %d", Mesh.EltNodes[i][8]);
	fprintf (out, " %d", Mesh.EltNodes[i][3]);
	fprintf (out, "\n");
      }
  }
  
  else if (! strcmp (Mesh.EltType, "quad"))
  {
    if (Mesh.Dimension == 3 && Mesh.EltOrder == 1)
    {
      for (i = 1; i <= Mesh.EltQty; i++)
      {
	fprintf (out, "%d c3d8", i);

	fprintf (out, " %d", Mesh.EltNodes[i][0]);
	fprintf (out, " %d", Mesh.EltNodes[i][1]);
	fprintf (out, " %d", Mesh.EltNodes[i][2]);
	fprintf (out, " %d", Mesh.EltNodes[i][3]);
	fprintf (out, " %d", Mesh.EltNodes[i][4]);
	fprintf (out, " %d", Mesh.EltNodes[i][5]);
	fprintf (out, " %d", Mesh.EltNodes[i][6]);
	fprintf (out, " %d", Mesh.EltNodes[i][7]);

	fprintf (out, "\n");
      }
    }
    else
    {
      for (i = 1; i <= Mesh.EltQty; i++)
      {
	fprintf (out, "%d c3d20", i);

	fprintf (out, " %d", Mesh.EltNodes[i][0]);
	fprintf (out, " %d", Mesh.EltNodes[i][8]);
	fprintf (out, " %d", Mesh.EltNodes[i][1]);
	fprintf (out, " %d", Mesh.EltNodes[i][11]);
	fprintf (out, " %d", Mesh.EltNodes[i][2]);
	fprintf (out, " %d", Mesh.EltNodes[i][13]);
	fprintf (out, " %d", Mesh.EltNodes[i][3]);
	fprintf (out, " %d", Mesh.EltNodes[i][9]);
	fprintf (out, " %d", Mesh.EltNodes[i][10]);
	fprintf (out, " %d", Mesh.EltNodes[i][12]);
	fprintf (out, " %d", Mesh.EltNodes[i][14]);
	fprintf (out, " %d", Mesh.EltNodes[i][15]);
	fprintf (out, " %d", Mesh.EltNodes[i][4]);
	fprintf (out, " %d", Mesh.EltNodes[i][16]);
	fprintf (out, " %d", Mesh.EltNodes[i][5]);
	fprintf (out, " %d", Mesh.EltNodes[i][18]);
	fprintf (out, " %d", Mesh.EltNodes[i][6]);
	fprintf (out, " %d", Mesh.EltNodes[i][19]);
	fprintf (out, " %d", Mesh.EltNodes[i][7]);
	fprintf (out, " %d", Mesh.EltNodes[i][17]);

	fprintf (out, "\n");
      }
    }
  }

  return;
}

void
neut_mesh_fprintf_geof_nsets (FILE* file, struct NSET NSet0D, struct NSET NSet1D,
                              struct NSET NSet2D, char* nset)
{
  int i, j, status;
  char** name;
  int qty;

  ut_string_separate (nset, ',', &name, &qty);

  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (! strcmp (name[i], NSet0D.names[j]))
      {
	neut_mesh_fprintf_geof_nset (file, NSet0D.names[j], NSet0D.nodeqty[j], NSet0D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (! strcmp (name[i], NSet1D.names[j]))
      {
	neut_mesh_fprintf_geof_nset (file, NSet1D.names[j], NSet1D.nodeqty[j], NSet1D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (! strcmp (name[i], NSet2D.names[j]))
      {
	neut_mesh_fprintf_geof_nset (file, NSet2D.names[j], NSet2D.nodeqty[j], NSet2D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }
  
  ut_free_2d_char (name, qty);

  return;
}

void
neut_mesh_fprintf_geof_sethead (FILE* file)
{
  fprintf (file, "***group\n");

  return;
}

void
neut_mesh_fprintf_geof_elsets (FILE* file, struct MESH Mesh3D)
{
  int i, j, col;

  for (i = 1; i <= Mesh3D.ElsetQty; i++)
  {
    fprintf (file, "\n**elset poly%d\n", i);
    col = 0;
    for (j = 1; j <= Mesh3D.Elsets[i][0]; j++)
      ut_print_wnc_int (file, Mesh3D.Elsets[i][j], &col, 72);

    fprintf (file, "\n");
  }

  return;
}
  
void
neut_mesh_fprintf_geof_part (FILE* file, struct PART Part)
{
  neut_mesh_fprintf_geof_part_nset  (file, Part);
  neut_mesh_fprintf_geof_part_elset (file, Part);

  return;
}

void
neut_mesh_fprintf_geof_foot (FILE * out)
{
  fprintf (out, "\n***return\n");

  return;
}
