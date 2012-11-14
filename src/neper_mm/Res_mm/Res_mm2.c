/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Res_mm.h"

void
CalcPolyEltQty (struct MESH Mesh, struct GERMSET GermSet,
		int** pPolyEltQty)
{
  int i;

  GermSet.N = GermSet.N;

  (*pPolyEltQty) = ut_alloc_1d_int (Mesh.ElsetQty + 1);

  (*pPolyEltQty)[0] = 0;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    (*pPolyEltQty)[i] = Mesh.Elsets[i][0];
    if ((*pPolyEltQty)[i] > 0)
      (*pPolyEltQty)[0]++;
  }

  return;
}

/* WriteMesh writes the mesh: nodes, elts, nsets. */
void
WriteMapMeshGeof (struct IN In, char *nsetlist,
               char* faset, struct NODES Nodes, struct MESH Mesh,
	       int **FoDNodes, FILE* file)
{
  int i, j, x, y, z, elt;
  int col;

  fprintf (file, "***geometry\n");

  /* Writing nodes */
  fprintf (file, "**node\n");
  fprintf (file, "%d 3\n", Nodes.NodeQty);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 0; j <= 2; j++)
      fprintf (file, " %.12f", Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }

  /* Writing elements */
  fprintf (file, "**element\n");
  fprintf (file, "%d\n", Mesh.EltQty);
  if (In.morder == 1)
  {
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d c3d8", i);

      fprintf (file, " %d", Mesh.EltNodes[i][0]);
      fprintf (file, " %d", Mesh.EltNodes[i][1]);
      fprintf (file, " %d", Mesh.EltNodes[i][2]);
      fprintf (file, " %d", Mesh.EltNodes[i][3]);
      fprintf (file, " %d", Mesh.EltNodes[i][4]);
      fprintf (file, " %d", Mesh.EltNodes[i][5]);
      fprintf (file, " %d", Mesh.EltNodes[i][6]);
      fprintf (file, " %d", Mesh.EltNodes[i][7]);

      fprintf (file, "\n");
    }
  }
  else
  {
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d c3d20", i);

      fprintf (file, " %d", Mesh.EltNodes[i][0]);
      fprintf (file, " %d", Mesh.EltNodes[i][8]);
      fprintf (file, " %d", Mesh.EltNodes[i][1]);
      fprintf (file, " %d", Mesh.EltNodes[i][11]);
      fprintf (file, " %d", Mesh.EltNodes[i][2]);
      fprintf (file, " %d", Mesh.EltNodes[i][13]);
      fprintf (file, " %d", Mesh.EltNodes[i][3]);
      fprintf (file, " %d", Mesh.EltNodes[i][9]);
      fprintf (file, " %d", Mesh.EltNodes[i][10]);
      fprintf (file, " %d", Mesh.EltNodes[i][12]);
      fprintf (file, " %d", Mesh.EltNodes[i][14]);
      fprintf (file, " %d", Mesh.EltNodes[i][15]);
      fprintf (file, " %d", Mesh.EltNodes[i][4]);
      fprintf (file, " %d", Mesh.EltNodes[i][16]);
      fprintf (file, " %d", Mesh.EltNodes[i][5]);
      fprintf (file, " %d", Mesh.EltNodes[i][18]);
      fprintf (file, " %d", Mesh.EltNodes[i][6]);
      fprintf (file, " %d", Mesh.EltNodes[i][19]);
      fprintf (file, " %d", Mesh.EltNodes[i][7]);
      fprintf (file, " %d", Mesh.EltNodes[i][17]);

      fprintf (file, "\n");
    }
  }

  fprintf (file, "\n***group\n");

  /* Writing nsets */
  WriteNSets (4, nsetlist, FoDNodes, "geof", file);

  if (In.loadmesh != NULL && strlen (faset) > 0)
  {
    ut_print_message (1, 1, "-faset with -loadmesh has not been implemented yet.\n");
    ut_print_message (1, 1, "Skipping it.  Ask for it!\n");
    faset[0] = '\0';
  }

  /* write fasets */
  if (ut_string_inlist (faset, ',', "x0"))
  {
    fprintf (file, "\n**faset x0\n");
    x = 1;

    if (In.morder == 1)
      for (y = 1; y <= Mesh.msize[1]; y++)
	for (z = 1; z <= Mesh.msize[2]; z++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q4");
	  fprintf (file, " %d", Mesh.EltNodes[elt][0]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][4]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][7]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][3]);
	  fprintf (file, "\n");
	}
    else
      for (y = 1; y <= Mesh.msize[1]; y++)
	for (z = 1; z <= Mesh.msize[2]; z++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q8");
	  fprintf (file, " %d", Mesh.EltNodes[elt][0]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][10]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][4]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][17]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][7]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][15]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][3]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][9]);
	  fprintf (file, "\n");
	}
    }

  if (ut_string_inlist (faset, ',', "x1"))
  {
    fprintf (file, "\n**faset x1\n");
    x = Mesh.msize[0];

    if (In.morder == 1)
      for (y = 1; y <= Mesh.msize[1]; y++)
	for (z = 1; z <= Mesh.msize[2]; z++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q4");
	  fprintf (file, " %d", Mesh.EltNodes[elt][1]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][2]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][6]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][5]);
	  fprintf (file, "\n");
	}
    else
      for (y = 1; y <= Mesh.msize[1]; y++)
	for (z = 1; z <= Mesh.msize[2]; z++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q8");
	  fprintf (file, " %d", Mesh.EltNodes[elt][1]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][11]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][2]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][14]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][6]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][18]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][5]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][12]);
	  fprintf (file, "\n");
	}
  }

  if (ut_string_inlist (faset, ',', "y0"))
  {
    fprintf (file, "\n**faset y0\n");
    y = 1;

    if (In.morder == 1)
      for (x = 1; x <= Mesh.msize[0]; x++)
	for (z = 1; z <= Mesh.msize[2]; z++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q4");
	  fprintf (file, " %d", Mesh.EltNodes[elt][0]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][1]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][5]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][4]);
	  fprintf (file, "\n");
	}
    else
      for (x = 1; x <= Mesh.msize[0]; x++)
	for (z = 1; z <= Mesh.msize[2]; z++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q8");
	  fprintf (file, " %d", Mesh.EltNodes[elt][0]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][8]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][1]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][12]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][5]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][16]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][4]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][10]);
	  fprintf (file, "\n");
	}
  }

  if (ut_string_inlist (faset, ',', "y1"))
  {
    fprintf (file, "\n**faset y1\n");
    y = Mesh.msize[1];

    if (In.morder == 1)
      for (x = 1; x <= Mesh.msize[0]; x++)
	for (z = 1; z <= Mesh.msize[2]; z++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q4");
	  fprintf (file, " %d", Mesh.EltNodes[elt][3]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][7]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][6]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][2]);
	  fprintf (file, "\n");
	}
    else
      for (x = 1; x <= Mesh.msize[0]; x++)
	for (z = 1; z <= Mesh.msize[2]; z++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q8");
	  fprintf (file, " %d", Mesh.EltNodes[elt][3]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][15]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][7]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][19]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][6]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][14]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][2]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][13]);
	  fprintf (file, "\n");
	}
  }

  if (ut_string_inlist (faset, ',', "z0"))
  {
    fprintf (file, "\n**faset z0\n");
    z = 1;

    if (In.morder == 1)
      for (x = 1; x <= Mesh.msize[0]; x++)
	for (y = 1; y <= Mesh.msize[1]; y++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q4");
	  fprintf (file, " %d", Mesh.EltNodes[elt][0]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][3]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][2]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][1]);
	  fprintf (file, "\n");
	}
    else
      for (x = 1; x <= Mesh.msize[0]; x++)
	for (y = 1; y <= Mesh.msize[1]; y++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q8");
	  fprintf (file, " %d", Mesh.EltNodes[elt][0]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][9]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][3]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][13]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][2]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][11]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][1]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][8]);
	  fprintf (file, "\n");
	}
  }

  if (ut_string_inlist (faset, ',', "z1"))
  {
    fprintf (file, "\n**faset z1\n");
    z = Mesh.msize[2];

    if (In.morder == 1)
      for (x = 1; x <= Mesh.msize[0]; x++)
	for (y = 1; y <= Mesh.msize[1]; y++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q4");
	  fprintf (file, " %d", Mesh.EltNodes[elt][4]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][5]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][6]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][7]);
	  fprintf (file, "\n");
	}
    else
      for (x = 1; x <= Mesh.msize[0]; x++)
	for (y = 1; y <= Mesh.msize[1]; y++)
	{
	  elt = EltCoo2Id (x, y, z, Mesh.msize[0], Mesh.msize[1]);
	  fprintf (file, "q8");
	  fprintf (file, " %d", Mesh.EltNodes[elt][4]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][16]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][5]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][18]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][6]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][19]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][7]);
	  fprintf (file, " %d", Mesh.EltNodes[elt][17]);
	  fprintf (file, "\n");
	}
  }

  int realelset = 0;
  for (i = 1; i <= Mesh.ElsetQty; i++)
    if (Mesh.Elsets[i][0] > 0)
    {
      fprintf (file, "\n**elset poly%d\n", ++realelset);
      col = 0;
      for (j = 1; j <= Mesh.Elsets[i][0]; j++)
	FPrintfWNC (file, Mesh.Elsets[i][j], &col, 72);

      fprintf (file, "\n");
    }

  // if (ut_string_inlist (In.format, ',', "geof"))
  fprintf (file, "\n***return\n");

  return;
}

void
WriteMapMeshAbq (struct IN In, char *nsetlist,
               char* faset, struct NODES Nodes, struct MESH Mesh,
	       int **FoDNodes, FILE* file)
{
  int i, j, col, call;

  /* Writing nodes */
  fprintf (file, "*Node\n");
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d, ", i);
    for (j = 0; j <= 2; j++)
      fprintf (file, "%.12f%s", Nodes.NodeCoo[i][j],
	  (j < 2) ? ", " : "\n");
  }

  fprintf (file, "\n");

  /* Writing elements */
  if (In.morder == 1)
  {
    fprintf (file, "*Element, type=C3D8\n");

    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d,", i);

      fprintf (file, " %d," , Mesh.EltNodes[i][0]);
      fprintf (file, " %d," , Mesh.EltNodes[i][1]);
      fprintf (file, " %d," , Mesh.EltNodes[i][2]);
      fprintf (file, " %d," , Mesh.EltNodes[i][3]);
      fprintf (file, " %d," , Mesh.EltNodes[i][4]);
      fprintf (file, " %d," , Mesh.EltNodes[i][5]);
      fprintf (file, " %d," , Mesh.EltNodes[i][6]);
      fprintf (file, " %d\n", Mesh.EltNodes[i][7]);
    }
  }
  else
  {
    fprintf (file, "*Element, type=C3D20\n");

    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d, ", i);

      fprintf (file, "%d, ", Mesh.EltNodes[i][0]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][1]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][2]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][3]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][4]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][5]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][6]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][7]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][8]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][11]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][13]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][9]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][16]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][18]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][19]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][17]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][10]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][12]);
      fprintf (file, "%d, ", Mesh.EltNodes[i][14]);
      fprintf (file, "%d\n", Mesh.EltNodes[i][15]);
    }
  }

  /* Writing nsets */
  WriteNSets (4, nsetlist, FoDNodes, "abq", file);

  /* write fasets */
  if (faset == NULL) // for accepting faset
    i = i;

  /* write elsets */

  col = 1;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    fprintf (file, "\n*Elset, elset=poly%d\n", i);
    col = 0;
    call = 0;
    for (j = 1; j < Mesh.Elsets[i][0]; j++)
      ut_print_wnc_wncall (file, &col, 72, &call, 16, "%d,", Mesh.Elsets[i][j]);
    ut_print_wnc_wncall (file, &col, 72, &call, 16, "%d\n",
	                 Mesh.Elsets[i][Mesh.Elsets[i][0]]);
  }

  return;
}

void
WriteMapMeshGmsh (char* dim, char *nset, char* faset,
               struct NODES Nodes,   struct MESH* pMesh0D,
	       struct MESH* pMesh1D, struct MESH* pMesh2D,
	       struct MESH* pMesh3D, int **FoDNodes, FILE* file)
{
  struct PART Part;

  neut_part_set_zero (&Part);

  // using the general routine for printing the mesh, but not the
  // nsets (generalization NEEDED!)
  neut_mesh_fprintf_gmsh (file, dim, Nodes, *pMesh0D, *pMesh1D, *pMesh2D,
                          *pMesh3D, Part);

  // Writing nsets
  WriteNSets (4, nset, FoDNodes, "msh", file);

  // write fasets
  if (faset == NULL) // for accepting faset
  {
  }

  neut_part_free (Part);

  return;
}


