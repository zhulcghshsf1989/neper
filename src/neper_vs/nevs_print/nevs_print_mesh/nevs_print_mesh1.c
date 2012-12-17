/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_print_mesh.h"

void
nevs_print_mesh (FILE* file, struct PRINT Print,
		 struct NODES Nodes, struct MESH Mesh0D,
		 struct MESH Mesh1D, struct MESH Mesh2D,
		 struct MESH Mesh3D, struct MESHDATA MeshData)
{
  int i, j, k, elt3d, elt2dqty; //, eltinv;
  double *pos = ut_alloc_1d (3);
  int* elts = NULL;
  int elt_qty = 0;
  int* elt2delts3d = ut_alloc_1d_int (2);
  int* eltsinv = NULL;
  int elt_max;
  int* nodes = NULL;
  int* elts2d = NULL;
  int* elts3d = NULL;
  int elt3dqty;
  int* printelt2d = ut_alloc_1d_int (Mesh2D.EltQty + 1);
  int* printelt3d = ut_alloc_1d_int (Mesh3D.EltQty + 1);
  int* tmp = ut_alloc_1d_int (10);
  int elt2dnodeqty;
  int elt3dfaceqty;
  int** printelt3dface = NULL;
  int** seq = NULL;
  int** seq2 = NULL;
  double ambient = (Print.showshadow == 1) ? 0.6 : 1;
  char* texture = ut_alloc_1d_char (100);
  double reduc;

  int elt3d_id, elt1d_id, elt0d_id, node_id, elt3dedge_id, node2elt_id;

  neut_meshdata_entity_id ("elt3d"    , &elt3d_id    );
  neut_meshdata_entity_id ("elt1d"    , &elt1d_id    );
  neut_meshdata_entity_id ("elt0d"    , &elt0d_id    );
  neut_meshdata_entity_id ("node"     , &node_id     );
  neut_meshdata_entity_id ("elt3dedge", &elt3dedge_id);
  neut_meshdata_entity_id ("node2elt",  &node2elt_id );

  if (strcmp (Mesh3D.EltType, "tri") == 0)
  {
    elt2dnodeqty = 3;
    elt3dfaceqty = 4;
  }
  else if (strcmp (Mesh3D.EltType, "quad") == 0)
  {
    elt2dnodeqty = 4;
    elt3dfaceqty = 6;
  }
  else
    abort ();

  printelt3dface = ut_alloc_2d_int (Mesh3D.EltQty + 1, elt3dfaceqty);

  // tri
  seq2 = ut_alloc_2d_int (elt2dnodeqty, 2);
  if (strcmp (Mesh3D.EltType, "tri") == 0)
  {
    seq2[0][0] = 0; 
    seq2[0][1] = 1; 
    seq2[1][0] = 1; 
    seq2[1][1] = 2; 
    seq2[2][0] = 0; 
    seq2[2][1] = 2; 
  }
  else if (strcmp (Mesh3D.EltType, "quad") == 0)
  {
    seq2[0][0] = 0; 
    seq2[0][1] = 1; 
    seq2[1][0] = 1; 
    seq2[1][1] = 2; 
    seq2[2][0] = 2; 
    seq2[2][1] = 3; 
    seq2[3][0] = 3; 
    seq2[3][1] = 0; 
  }

  seq = ut_alloc_2d_int (elt3dfaceqty, elt2dnodeqty);
  if (strcmp (Mesh3D.EltType, "tri") == 0)
  {
    seq[0][0] = 0; 
    seq[0][1] = 1; 
    seq[0][2] = 2; 
    seq[1][0] = 1; 
    seq[1][1] = 2; 
    seq[1][2] = 3; 
    seq[2][0] = 2; 
    seq[2][1] = 3; 
    seq[2][2] = 0; 
    seq[3][0] = 0; 
    seq[3][1] = 1; 
    seq[3][2] = 3; 
  }
  else if (strcmp (Mesh3D.EltType, "quad") == 0)
  {
    seq[0][0] = 0; 
    seq[0][1] = 1; 
    seq[0][2] = 2; 
    seq[0][3] = 3; 
    seq[1][0] = 4; 
    seq[1][1] = 5; 
    seq[1][2] = 6; 
    seq[1][3] = 7; 
    seq[2][0] = 0; 
    seq[2][1] = 1; 
    seq[2][2] = 5; 
    seq[2][3] = 4; 
    seq[3][0] = 3; 
    seq[3][1] = 2; 
    seq[3][2] = 6; 
    seq[3][3] = 7; 
    seq[4][0] = 1; 
    seq[4][1] = 2; 
    seq[4][2] = 6; 
    seq[4][3] = 5; 
    seq[5][0] = 0; 
    seq[5][1] = 3; 
    seq[5][2] = 7; 
    seq[5][3] = 4; 
  }
  else
    abort ();

  Mesh0D.EltQty = Mesh0D.EltQty;
      
  ut_print_message (0, 3, "Preparing mesh data ...\n");
  fflush (stdout);

  elt_qty = ut_array_1d_int_sum (Print.showelt + 1, Mesh3D.EltQty);

  elts = ut_alloc_1d_int (elt_qty);

  elt_qty = 0;
  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    if (Print.showelt[i] == 1)
    {
      elts[elt_qty] = i;
      elt_qty++;
    }
  }

  if (elt_qty > 0)
    ut_array_1d_int_inv (elts, elt_qty, &eltsinv, &elt_max);

  // Compressing input data --------------------------------------------
  
  ut_print_message (0, 3, "Reducing data ...\n");
  fflush (stdout);

  int* leftelts = NULL;
  int lefteltqty = 0;
  int* elset_full = ut_alloc_1d_int (Mesh3D.ElsetQty + 1);

  int elt_id;
  neut_meshdata_entity_id ("elt", &elt_id);

  if (Print.datareduction == 1)
  {
    ut_array_1d_int_set (elset_full + 1, Mesh3D.ElsetQty, 1);
    for (i = 1; i <= Mesh3D.ElsetQty; i++)
      for (j = 1; j <= Mesh3D.Elsets[i][0]; j++)
	if (Print.showelt[Mesh3D.Elsets[i][j]] == 0)
	{
	  elset_full[i] = 0;
	  break;
	}

    if (elt_qty > 0)
	neut_mesh_elts_boundelts (Mesh3D, elts, elt_qty, &leftelts, &lefteltqty);
    else
      lefteltqty = 0;

    for (i = 0; i < lefteltqty; i++)
      printelt3d[leftelts[i]] = 1;

    if (elt_qty == 0)
     reduc = 0;
    else 
      reduc = 100 * (double) (elt_qty - lefteltqty) / (double) elt_qty;
    ut_print_message (0, 4,
	"Number of 3D elts    reduced by %3.0f\%% (to %d).\n", reduc,
							lefteltqty);
  }
  else
  {
    lefteltqty = elt_qty;
    leftelts = elts;
  }
    
  // Writing pov file --------------------------------------------------
  // nevs_print_mesh_header (file, Print);

  int* elt2delt3d = ut_alloc_1d_int (1);
  struct NODES N;
  struct MESH M2D; 

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M2D);
  M2D.EltType = ut_alloc_1d_char (strlen (Mesh3D.EltType) + 1);
  M2D.Dimension = 2;
  M2D.EltOrder = 1;
  sprintf (M2D.EltType, "%s", Mesh3D.EltType);

  nodes = ut_alloc_1d_int (elt2dnodeqty);

  if (lefteltqty > 0)
  {
    for (i = 0; i < lefteltqty; i++)
    {
      elt3d = leftelts[i];
      // eltinv = eltsinv[elt3d];

      for (j = 0; j < elt3dfaceqty; j++)
      {
	for (k = 0; k < elt2dnodeqty; k++)
	  nodes[k] = Mesh3D.EltNodes[elt3d][seq[j][k]];

	neut_mesh_nodes_comelts (Mesh2D, nodes, elt2dnodeqty, &elts2d, &elt2dqty);
	neut_mesh_nodes_comelts (Mesh3D, nodes, elt2dnodeqty, &elts3d, &elt3dqty);

	int print;

	print = 0;

	if (elt3dqty == 1)
	  print = 1;

	else if (elt3dqty == 2)
	{
	  // keep the first condition, or combine w the second
	  // (because of the `else if')
	  if ((Mesh3D.EltElset[elts3d[0]] == Mesh3D.EltElset[elts3d[1]]))
	  {
	    if (elset_full[Mesh3D.EltElset[elts3d[0]]] == 1)
	      print = 0;
	    else if (printelt3d[elts3d[0]] == 0
		  || printelt3d[elts3d[1]] == 0)
	      print = 1;
	  }
	  else
	  {
	    if (elset_full[Mesh3D.EltElset[elts3d[0]]] == 1
	     && elset_full[Mesh3D.EltElset[elts3d[1]]] == 1)
	      print = 0;
	    else
	      print = 1;
	  }
	}

	if (print)
	{
	  printelt3dface[elt3d][j] = 1;
	  neut_mesh_addelt (&M2D, nodes);
	  elt2delt3d = ut_realloc_1d_int (elt2delt3d, M2D.EltQty + 1);
	  elt2delt3d[M2D.EltQty] = elt3d;

	  if (elt2dqty == 1)
	    printelt2d[elts2d[0]] = 1;
	}
      }
    }

    ut_print_message (0, 4, "Number of elt faces  reduced by %3.0f\%% (to %d).\n", \
  100 * (double) (lefteltqty * elt3dfaceqty - M2D.EltQty) / (double) (lefteltqty * elt3dfaceqty),
    M2D.EltQty);
  }

  // Printing element faces
  
  int** rgb = NULL;

  int* node_status = ut_alloc_1d_int (Nodes.NodeQty + 1);
  for (i = 1; i <= M2D.EltQty; i++)
    for (j = 0; j < elt2dnodeqty; j++)
      node_status[M2D.EltNodes[i][j]] = 1;

  int max = 0;
  for (i = 1; i <= Nodes.NodeQty; i++)
    if (node_status[i] == 1)
      max = i;

  int* nodes_old_new = ut_alloc_1d_int (max + 1);
  int* nodes_new_old = ut_alloc_1d_int (Nodes.NodeQty + 1);
  int id = 0;

  for (i = 1; i <= max; i++)
    if (node_status[i] == 1)
    {
      nodes_old_new[i] = ++id;
      nodes_new_old[id] = i;
      neut_nodes_addnode (&N, MeshData.coo[node_id][i], 0);
    }

  neut_mesh_switch (&M2D, nodes_old_new, NULL, NULL);
  neut_mesh_init_nodeelts (&M2D, Nodes.NodeQty);

  if (MeshData.col[elt3d_id] != NULL)
  {
    rgb = ut_alloc_2d_int (M2D.EltQty + 1, 3);

    for (i = 1; i <= M2D.EltQty; i++)
    {
      elt3d = elt2delt3d[i];
      ut_array_1d_int_memcpy (rgb[i], 3, MeshData.col[elt3d_id][elt3d]);
    }
    
    nevs_print_mesh2d (file, N, M2D, Print, rgb, "elt");

    ut_free_2d_int (rgb, M2D.EltQty + 1);
  }

  if (MeshData.col[node2elt_id] != NULL)
  {
    rgb = ut_alloc_2d_int (N.NodeQty + 1, 3);

    for (i = 1; i <= N.NodeQty; i++)
    {
      int node = nodes_new_old[i];
      ut_array_1d_int_memcpy (rgb[i], 3, MeshData.col[node2elt_id][node]);
    }

    nevs_print_mesh2d (file, N, M2D, Print, rgb, "node");

    ut_free_2d_int (rgb, N.NodeQty + 1);
  }
  /*
  else
  {
    rgb = ut_alloc_2d_int (M2D.EltQty + 1, 3);
    ut_array_2d_int_set (rgb, M2D.EltQty + 1, 3, 255);

    nevs_print_mesh2d (file, N, M2D, Print, rgb, "elt");
  }
  */

  ut_free_1d_int (nodes_old_new);
  ut_free_1d_int (nodes_new_old);
  ut_free_1d_int (node_status);
  
  int edge_qty = 0;
  int** edges = ut_alloc_1d_pint (1);
  int* elts_tmp = NULL;
  int eltqty;
  // Element edges
  if (M2D.EltQty > 0)
    for (i = 1; i <= M2D.EltQty; i++)
      if (Print.showeltedge[elt2delt3d[i]] == 1)
      for (j = 0; j < elt2dnodeqty; j++)
      {
	for (k = 0; k < 2; k++)
	  tmp[k] = M2D.EltNodes[i][seq2[j][k]];

	neut_mesh_nodes_comelts (M2D, tmp, 2, &elts_tmp, &eltqty);
	if (ut_array_1d_int_min (elts_tmp, eltqty) == i)
	{
	  edge_qty++;
	  edges = ut_realloc_2d_int_addline (edges, edge_qty, 2);
	  ut_array_1d_int_memcpy (edges[edge_qty - 1], 2, tmp);
	}
      }
  ut_free_1d_int (elts_tmp);

  if (M2D.EltQty > 0)
    ut_print_message (0, 4, "Number of face edges reduced by %3.0f\%% (to %d).\n", \
  100 * (double) (M2D.EltQty * elt2dnodeqty - edge_qty)
  / (double) (M2D.EltQty * elt2dnodeqty), edge_qty);

  char* string = ut_alloc_1d_char (100);
    
  fprintf (file,
     "#declare elt3dedge =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
      MeshData.col[elt3dedge_id][1][0] / 255.,
      MeshData.col[elt3dedge_id][1][1] / 255.,
      MeshData.col[elt3dedge_id][1][2] / 255.,
      ambient);
    
  sprintf (texture, "elt3dedge");
  sprintf (string, "%.12f", MeshData.rad[elt3dedge_id][1]);

  for (i = 0; i < edge_qty; i++)
    nevs_print_segment (file, N.NodeCoo[edges[i][0]], N.NodeCoo[edges[i][1]],
			     string, texture);

  ut_free_2d_int (edges, edge_qty);

  // Element 1D
  //
  int elt1d_qty;
  elt1d_qty = ut_array_1d_int_sum (Print.showelt1d + 1, Mesh1D.EltQty);
  
  if (elt1d_qty > 0)
  {
    int printelt1d_qty = 0;

    int texture_unique = 1;
    int* col = ut_alloc_1d_int (3);
    for (i = 1; i <= Mesh1D.EltQty; i++)
      if (Print.showelt1d[i] == 1)
      {
	if (col == NULL)
	{
	  col = ut_alloc_1d_int (3);
	  ut_array_1d_int_memcpy (col, 3, MeshData.col[elt1d_id][i]);
	}
	else
	  if (ut_array_1d_int_diff (MeshData.col[elt1d_id][i], 3, col, 3))
	  {
	    texture_unique = 0;
	    break;
	  }
      }

    if (texture_unique)
    {
      fprintf (file,
   "#declare elt1d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
	col[0] / 255., col[1] / 255., col[2] / 255., ambient);
  
	sprintf (texture, "elt1d");
    }

    for (i = 1; i <= Mesh1D.EltQty; i++)
      if (Print.showelt1d[i] == 1)
      {
	if (! texture_unique)
	{
	  fprintf (file,
       "#declare elt1d%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
	    i,
	    MeshData.col[elt1d_id][i][0] / 255., \
	    MeshData.col[elt1d_id][i][1] / 255., \
	    MeshData.col[elt1d_id][i][2] / 255., \
	    ambient);
    
	  sprintf (texture, "elt1d%d", i);
	}

	neut_mesh_nodes_comelts (Mesh3D, Mesh1D.EltNodes[i], 2,
	                       &elts3d, &elt3dqty);

	int l, print = 0;
	for (j = 0; j < elt3dqty; j++)
	{
	  elt3d = elts3d[j];

	  for (k = 0; k < elt3dfaceqty; k++)
	    if (printelt3dface[elt3d][k] == 1)
	    {
	      for (l = 0; l < elt2dnodeqty; l++)
		tmp[l] = Mesh3D.EltNodes[elt3d][seq[k][l]];

	      if (ut_array_1d_int_eltpos (tmp, elt2dnodeqty, 
		                 Mesh1D.EltNodes[i][0]) != -1
	       && ut_array_1d_int_eltpos (tmp, elt2dnodeqty, 
		                 Mesh1D.EltNodes[i][1]) != -1)
	      {
		print = 1;
		break;
	      }
	    }
	}
	/*
        l = 1;
	print = l;
	*/

	if (print == 1)
	{
	  char* string = ut_alloc_1d_char (100);
	  sprintf (string, "%.12f", MeshData.rad[elt1d_id][i]);
	  nevs_print_segment (file,
	      MeshData.coo[node_id][Mesh1D.EltNodes[i][0]],
	      MeshData.coo[node_id][Mesh1D.EltNodes[i][1]],
	      string, texture);

	  printelt1d_qty++;
	}
      }

    ut_print_message (0, 4, "Number of 1D elts    reduced by %3.0f\%% (to %d).\n", \
    100 * (double) (elt1d_qty - printelt1d_qty) / (double) elt1d_qty, printelt1d_qty);
  }

  // Element 0D
  //
  int elt0d_qty;

  elt0d_qty = ut_array_1d_int_sum (Print.showelt0d + 1, Mesh0D.EltQty);
  
  if (elt0d_qty > 0)
  {
    int printelt0d_qty = 0;

    for (i = 1; i <= Mesh0D.EltQty; i++)
      if (Print.showelt0d[i] == 1)
      {
	fprintf (file,
     "#declare elt0d%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
	  i,
	  MeshData.col[elt0d_id][i][0] / 255., \
	  MeshData.col[elt0d_id][i][1] / 255., \
	  MeshData.col[elt0d_id][i][2] / 255., \
	  ambient);
    
	sprintf (texture, "elt0d%d", i);

	neut_mesh_nodes_comelts (Mesh3D, Mesh0D.EltNodes[i], 1,
	                       &elts3d, &elt3dqty);

	int l, print = 0;
	/*
	for (j = 0; j < elt3dqty; j++)
	{
	  elt3d = elts3d[j];

	  for (k = 0; k < elt3dfaceqty; k++)
	    if (printelt3dface[elt3d][k] == 1)
	    {
	      for (l = 0; l < elt2dnodeqty; l++)
		tmp[l] = Mesh3D.EltNodes[elt3d][seq[k][l]];

	      if (ut_array_0d_int_eltpos (tmp, elt2dnodeqty, 
		                 Mesh0D.EltNodes[i][0]) != -1
	       && ut_array_0d_int_eltpos (tmp, elt2dnodeqty, 
		                 Mesh0D.EltNodes[i][1]) != -1)
	      {
		print = 1;
		break;
	      }
	    }
	}
	*/
	l = 1;
	print = l;

	if (print == 1)
	{
	  char* string = ut_alloc_1d_char (100);
	  sprintf (string, "%.12f", MeshData.rad[elt0d_id][i]);
	  nevs_print_sphere (file,
	      MeshData.coo[node_id][Mesh0D.EltNodes[i][0]],
	      string, texture);

	  printelt0d_qty++;
	}
      }

    ut_print_message (0, 4, "Number of 0D elts    reduced by %3.0f\%% (to %d).\n", \
    100 * (double) (elt0d_qty - printelt0d_qty) / (double) elt0d_qty, printelt0d_qty);
  }

  // Nodes
  
  int node_qty;
  node_qty = ut_array_1d_int_sum (Print.shownode + 1, Nodes.NodeQty);
  
  if (node_qty > 0)
  {
    int printnode_qty = 0;

    for (i = 1; i <= Nodes.NodeQty; i++)
      if (Print.shownode[i] == 1)
      {
	fprintf (file,
     "#declare node%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
	  i,
	  MeshData.col[node_id][i][0] / 255., \
	  MeshData.col[node_id][i][1] / 255., \
	  MeshData.col[node_id][i][2] / 255., \
	  ambient);
    
	sprintf (texture, "node%d", i);

	int print = 0;
	for (j = 0; j < Mesh3D.NodeElts[i][0]; j++)
	{
	  elt3d = Mesh3D.NodeElts[i][j];

	  for (k = 0; k < elt3dfaceqty; k++)
	    if (printelt3dface[elt3d][k] == 1)
	    {
	      print = 1;
	      break;
	      /*
	      for (l = 0; l < elt2dnodeqty; l++)
		tmp[l] = Mesh3D.EltNodes[elt3d][seq[k][l]];

	      if (ut_array_1d_int_eltpos (tmp, elt2dnodeqty, 
		                 Mesh1D.EltNodes[i][0]) != -1
	       && ut_array_1d_int_eltpos (tmp, elt2dnodeqty, 
		                 Mesh1D.EltNodes[i][1]) != -1)
	      {
		print = 1;
		break;
	      }
	      */
	    }
	}

	if (print == 1)
	{
	  char* string = ut_alloc_1d_char (100);
	  sprintf (string, "%.12f", MeshData.rad[node_id][i]);
	  nevs_print_sphere (file, MeshData.coo[node_id][i], string, texture);

	  printnode_qty++;
	}
      }

    ut_print_message (0, 4, "Number of nodes      reduced by %3.0f\%% (to %d).\n", \
    100 * (double) (node_qty - printnode_qty) / (double) node_qty, printnode_qty);
  }


  ut_free_1d (pos);
  ut_free_1d_int (elts);
  ut_free_1d_int (elt2delts3d);
  ut_free_1d_int (nodes);
  ut_free_1d_int (elts2d);
  ut_free_1d_int (elts3d);
  ut_free_1d_int (printelt2d);
  ut_free_1d_int (printelt3d);
  ut_free_2d_int (printelt3dface, Mesh3D.EltQty + 1);
  ut_free_2d_int (seq2, elt2dnodeqty);
  ut_free_2d_int (seq, elt3dfaceqty);
  ut_free_1d_int (tmp);
  ut_free_1d_int (eltsinv);
  ut_free_1d_int (elt2delt3d);
  ut_free_1d_int (leftelts);
  ut_free_1d_char (string);
  ut_free_1d_char (texture);

  neut_mesh_free (&M2D);
  neut_nodes_free (&N);

  return;
}
