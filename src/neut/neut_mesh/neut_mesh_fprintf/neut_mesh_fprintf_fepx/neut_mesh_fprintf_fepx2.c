/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_fepx.h"

void
neut_mesh_fprintf_fepx_parms (FILE* file, struct NODES Nodes, struct MESH Mesh)
{
  /* fepx1 = nb of elts and nodes */
  fprintf (file, "%d %d 9\n", Mesh.EltQty, Nodes.NodeQty);
}

void
neut_mesh_fprintf_fepx_nodes (FILE * file, struct NODES Nodes)
{
  int i, j;

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d", i - 1);
    for (j = 0; j < 3; j++)
      fprintf (file, "  %.12f", 
	  (fabs (Nodes.NodeCoo[i][j]) < 1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }

  return;
}

void
neut_mesh_fprintf_fepx_elts (FILE * file, struct GEO Geo,
                            struct NSET NSet2D, struct MESH Mesh)
{
  int i, j, fod;

  if (Mesh.Dimension == 2 && Mesh.EltOrder == 1)
  {
    fprintf (file, "1\n      %d\n", Mesh.EltQty);
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i - 1);
      for (j = 0; j < 3; j++)
	fprintf (file, " %d", Mesh.EltNodes[i][j] - 1);

      fod = - ut_array_1d_int_min (Geo.FacePoly[Mesh.EltElset[i]], 2);

      fprintf (file, " %s\n", NSet2D.names[fod]);
    }
  }
  else if (Mesh.Dimension == 2 && Mesh.EltOrder == 2)
  {
    fprintf (file, "1\n%8d\n", Mesh.EltQty);
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][0] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][3] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][1] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][4] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][2] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][5] - 1);

      fod = - ut_array_1d_int_min (Geo.FacePoly[Mesh.EltElset[i]], 2);

      fprintf (file, " %s\n", NSet2D.names[fod]);
    }
  }
  else if (Mesh.Dimension == 3 && Mesh.EltOrder == 1)
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i - 1);
      for (j = 0; j < 4; j++)
	fprintf (file, " %d", Mesh.EltNodes[i][j] - 1);
      fprintf (file, "\n");
    }
  else if (Mesh.Dimension == 3 && Mesh.EltOrder == 2)
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][0] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][4] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][1] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][5] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][2] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][6] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][7] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][9] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][8] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][3] - 1);
      fprintf (file, "\n");
    }

  return;
}

void
neut_mesh_fprintf_fepx_skinelts (FILE * file, struct GEO Geo, struct MESH Mesh2D,
				struct MESH Mesh3D, struct NODES Nodes,
				struct NSET NSet2D, char* surflist)
{
  double res;
  int i, j, k, l, eltnb, eltqty, elt3dqty;
  int* elt3d = ut_alloc_1d_int (2);
  double *n  = ut_alloc_1d (3);
  double *n0 = ut_alloc_1d (3);
  int seqo1[3] = { 0, 1, 2 };
  int seqo2[6] = { 0, 3, 1, 4, 2, 5 };
  char** name = NULL;
  int qty;
  
  ut_string_separate (surflist, ',', &name, &qty);

  fprintf (file, "%d\n", qty);

  for (i = 1; i <= Geo.DomFaceQty; i++)
  {
    if (ut_string_inlist (surflist, ',', NSet2D.names[i]) == 0)
      continue;

    // calculating number of elements
    eltqty = 0;
    for (j = 1; j <= Geo.DomTessFaceQty[i]; j++)
      eltqty += Mesh2D.Elsets[Geo.DomTessFaceNb[i][j]][0];
  
    fprintf (file, "%d\n", eltqty);

    ut_array_1d_memcpy (n0, 3, Geo.DomFaceEq[i] + 1);
    ut_array_1d_scale (n0, 3, -1);

    for (j = 1; j <= Geo.DomTessFaceQty[i]; j++)
    {
      for (k = 1; k <= Mesh2D.Elsets[Geo.DomTessFaceNb[i][j]][0]; k++)
      {
	eltnb = Mesh2D.Elsets[Geo.DomTessFaceNb[i][j]][k];
	neut_mesh_eltnormal (Mesh2D, Nodes, eltnb, n);

	neut_mesh_elt2delts3d (Mesh2D, eltnb, Mesh3D, elt3d, &elt3dqty);

	if (elt3dqty != 1)
	{
	  printf ("Nodes:\n");
	  neut_debug_nodes (stdout, Nodes);
	  printf ("Mesh2D:\n");
	  neut_debug_mesh  (stdout, Mesh2D);
	  printf ("Mesh3D:\n");
	  neut_debug_mesh  (stdout, Mesh3D);

	  printf ("elt = %d\n", eltnb);
	  printf ("elt3dqty != 1 ( = %d)\n", elt3dqty);
	  ut_array_1d_int_fprintf (stdout, elt3d, elt3dqty, "%d");
	  abort ();
	}

	fprintf (file, "%d", elt3d[0] - 1);

	res = ut_vector_scalprod (n0, n);
	if (Mesh2D.EltOrder == 1)
	{
	  if (res > 0)
	    for (l = 0; l <= 2; l++)
	      fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo1[l]] - 1);
	  else
	    for (l = 2; l >= 0; l--)
	      fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo1[l]] - 1);
	}
	else if (Mesh2D.EltOrder == 2)
	{
	  if (res > 0)
	  {
	    for (l = 0; l < 6; l++)
	      fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo2[l]] - 1);
	  }
	  else
	  {
	    /* reverse order from 0 (must be from a 1st order node) */
	    fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo2[0]] - 1);
	    for (l = 5; l >= 1; l--)
	      fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo2[l]] - 1);
	  }
	}

	fprintf (file, " \n");
      }
    }
  }

  ut_free_1d (n);
  ut_free_1d (n0);
  ut_free_1d_int (elt3d);
  ut_free_2d_char (name, qty);

  return;
}

void
neut_mesh_fprintf_fepx_elsets (FILE * file, struct MESH Mesh3D)
{
  int i;

  fprintf (file, "grain-input\n");
  fprintf (file, "%d %d\n", Mesh3D.EltQty, Mesh3D.ElsetQty);
  
  for (i = 1; i <= Mesh3D.EltQty; i++)
    fprintf (file, "%d 1\n", Mesh3D.EltElset[i]); /* 1 if for the phase */

  fprintf (file, "end-options\n");

  return;
}


void
neut_mesh_fprintf_fepx_nsets (FILE* file, struct NSET NSet0D, struct NSET NSet1D,
                             struct NSET NSet2D, char* nset)
{
  int i, j, status;
  char** name;
  int qty;

  ut_string_separate (nset, ',', &name, &qty);
  
  /* first pass, for calculating total number of bcs */
  int bcqty = 0;
  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (! strcmp (name[i], NSet0D.names[j]))
      {
	bcqty += NSet0D.nodeqty[j];
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (! strcmp (name[i], NSet1D.names[j]))
      {
	bcqty += NSet1D.nodeqty[j];
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (! strcmp (name[i], NSet2D.names[j]))
      {
	bcqty += NSet2D.nodeqty[j];
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }

  /* header */
  fprintf (file, "0\n%d\n", bcqty);

  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (! strcmp (name[i], NSet0D.names[j]))
      {
	neut_mesh_fprintf_fepx_nset (file, NSet0D.names[j], NSet0D.nodeqty[j], NSet0D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (! strcmp (name[i], NSet1D.names[j]))
      {
	neut_mesh_fprintf_fepx_nset (file, NSet1D.names[j], NSet1D.nodeqty[j], NSet1D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (! strcmp (name[i], NSet2D.names[j]))
      {
	neut_mesh_fprintf_fepx_nset (file, NSet2D.names[j], NSet2D.nodeqty[j], NSet2D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }
  
  /* foot */
  fprintf (file, "0\n");

  return;
}

/*
void
neut_mesh_bcsfile_update (FILE* filein, FILE* file, struct PART Part)
{
  int i, j, id, newid, tmp, qty, colqty;
  char* string = ut_alloc_1d_char (1000);

  colqty = ut_file_nbwords_pointer (filein) - 3;
  colqty /= tmp;
  colqty--;

  fscanf (filein, "%d\n", &tmp);
  fprintf (file, "%d\n", tmp);
  fscanf (filein, "%d\n", &qty);
  fprintf (file, "%d\n", qty);

  for (i = 0; i < qty; i++)
  {
    fscanf (filein, "%d", &id);
    id++;
    if (Part.qty > 0)
      newid = Part.node_old_new[id];
    else
      newid = id;

    fprintf (file, "%d", newid - 1);

    for (j = 0; j < colqty; j++)
    {
      fscanf (filein, "%s", string);
      fprintf (file, " %s", string);
    }

    fprintf (file, "\n");
  }
  
  fscanf (filein, "%d\n", &tmp);
  fprintf (file, "%d\n", tmp);

  ut_free_1d_char (string);

  return;
}
*/

/*
void
neut_mesh_surffile_update (FILE* filein, FILE* file, struct PART Part)
{
  int i, j, st, eltnodeqty, id, newid, tmp, qty;
  char* string = ut_alloc_1d_char (1000);

  eltnodeqty = ut_file_nbwords_pointer (filein) - 2; 

  fscanf (filein, "%d", &tmp);
  fprintf (file, "%d\n", tmp);

  fscanf (filein, "%d", &qty);
  fprintf (file, "%d\n", qty);

  eltnodeqty /= qty;

  st = 0;
  switch (eltnodeqty)
  {
  case 8:
    st = 1;
    eltnodeqty = 6;
    break;
  case 7:
    st = 0;
    eltnodeqty = 6;
    break;
  case 5:
    st = 1;
    eltnodeqty = 3;
    break;
  case 4:
    st = 0;
    eltnodeqty = 3;
    break;
  default:
    {
      ut_print_message (2, 0, "surf file has a bad format.\n");
      abort ();
    }
  }

  for (i = 0; i < qty; i++)
  {
    fscanf (filein, "%d", &id);
    id++;
    if (Part.qty > 0)
      newid = Part.elts_old_new[id];
    else
      newid = id;

    fprintf (file, "%d", newid - 1); 

    for (j = 0; j < eltnodeqty; j++)
    {
      fscanf (filein, "%d", &id);
      id++;
      if (Part.qty > 0)
	newid = Part.nodes_old_new[id];
      else
	newid = id;
      fprintf (file, " %d", newid - 1);
    }

    if (st == 1)
    {
      fscanf (filein, "%s", string);
      fprintf (file, " %s", string);
    }
    fprintf (file, "\n");
  }

  ut_free_1d_char (string);

  return;
}
*/
