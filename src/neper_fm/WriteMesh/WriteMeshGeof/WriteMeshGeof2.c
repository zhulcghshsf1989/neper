/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WriteMeshGeof.h"

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
      fprintf (out, "  %.12f", Nodes.NodeCoo[i][j]);
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

/*
void
neut_mesh_fprintf_geof_lisets (struct GEO Geo, int **FoD, struct NODES Nodes,
	       struct MESH Mesh2D, FILE * out)
{
  int i, j, k, qty, edge, face;
  int face1, face2, face1nodeqty, face2nodeqty;
  int *edge_list;
  int *face1nodes;
  int *face2nodes;
  int *node_list;
  int nodeqty;
  char **LisetNames = ut_alloc_2d_char (7, 100);
  double *ref = ut_alloc_1d (3);
  double *uvect = ut_alloc_1d (3);
  double *vect = ut_alloc_1d (3);
  double *coo;
  int *index;

  sprintf (LisetNames[1], "x0");
  sprintf (LisetNames[2], "x1");
  sprintf (LisetNames[3], "y0");
  sprintf (LisetNames[4], "y1");
  sprintf (LisetNames[5], "z0");
  sprintf (LisetNames[6], "z1");

  for (i = 1; i <= 6; i++)	// for every face of the domain 
  {
    fprintf (out, "\n**liset %s\n", LisetNames[i]);

    // all edges of the faces must be plotted; recording them 
    qty = 0;
    for (j = 1; j <= FoD[i][0]; j++)
      qty += Geo.FaceVerQty[FoD[i][j]];

    edge_list = ut_alloc_1d_int (qty + 1);

    for (j = 1; j <= FoD[i][0]; j++)
    {
      face = FoD[i][j];

      for (k = 1; k <= Geo.FaceVerQty[face]; k++)
      {
	edge = Geo.FaceEdgeNb[face][k];

	if (ut_array_1d_int_eltpos (edge_list + 1, edge_list[0], edge) == -1)
	  edge_list[++edge_list[0]] = edge;
      }
    }

    // for every edge, plotting liset 
    for (j = 1; j <= edge_list[0]; j++)
    {
      edge = edge_list[j];
      face1 = Geo.EdgeFaceNb[edge][0];
      face2 = Geo.EdgeFaceNb[edge][1];

      // building the list of nodes 
      neut_mesh_elset_nodes (Mesh2D, face1, &face1nodes, &face1nodeqty);
      neut_mesh_elset_nodes (Mesh2D, face2, &face2nodes, &face2nodeqty);

      node_list = ut_alloc_1d_int (ut_num_max (face1nodeqty, face2nodeqty));
      ut_array_1d_int_inter (face1nodes, face1nodeqty,
			 face2nodes, face2nodeqty, node_list, &nodeqty);
      ut_free_1d_int (face1nodes);
      ut_free_1d_int (face2nodes);

      ut_array_1d_memcpy (ref, 3, Nodes.NodeCoo[node_list[0]]);
      ut_array_1d_sub (ref, Nodes.NodeCoo[node_list[1]], 3, uvect);

      coo = ut_alloc_1d (nodeqty);
      for (k = 0; k < nodeqty; k++)
      {
	ut_array_1d_sub (ref, Nodes.NodeCoo[node_list[k]], 3, vect);
	coo[k] = ut_vector_scalprod (uvect, vect);
      }

      index = ut_alloc_1d_int (nodeqty);
      ut_array_1d_sort_index (coo, nodeqty, index);
      ut_array_1d_permutation_int (node_list, nodeqty, index);

      if (Mesh2D.EltOrder == 1)
	for (k = 0; k < nodeqty - 1; k++)
	  fprintf (out, "line %d %d\n", node_list[k], node_list[k + 1]);
      else if (Mesh2D.EltOrder == 2)
	for (k = 0; k < nodeqty - 2; k += 2)
	  fprintf (out, "quad %d %d %d\n", node_list[k], node_list[k + 1],
		   node_list[k + 2]);
      else
	abort ();

      ut_free_1d (coo);
      ut_free_1d_int (index);
      ut_free_1d_int (node_list);
    }

    ut_free_1d_int (edge_list);
  }

  ut_free_2d_char (LisetNames, 7);
  ut_free_1d (uvect);
  ut_free_1d (vect);

  return;
}
*/

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
