/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh.h"

/* neut_mesh_elt_area computes the area of a 2D element */
int
neut_mesh_elt_area (struct NODES Nodes, struct MESH Mesh, int nb, double* parea)
{
  if (Mesh.Dimension != 2)
    return -1;

  (*parea) = ut_space_triangle_area (Nodes.NodeCoo[Mesh.EltNodes[nb][0]],
    Nodes.NodeCoo[Mesh.EltNodes[nb][1]],
    Nodes.NodeCoo[Mesh.EltNodes[nb][2]]);

  return 0;
}

/* neut_mesh_elt_volume computes the volume of an element */
int
neut_mesh_elt_volume (struct NODES Nodes, struct MESH Mesh, int nb, double* pvol)
{
  if (Mesh.Dimension != 3)
    return -1;

  if (strcmp (Mesh.EltType, "tri") == 0)
    (*pvol) = ut_space_tet_volume (
	Nodes.NodeCoo[Mesh.EltNodes[nb][0]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][1]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][2]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][3]]);
  else if (strcmp (Mesh.EltType, "quad") == 0)
    (*pvol) = ut_space_hexa_volume (
	Nodes.NodeCoo[Mesh.EltNodes[nb][0]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][1]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][2]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][3]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][4]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][5]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][6]],
	Nodes.NodeCoo[Mesh.EltNodes[nb][7]]);
  else
    abort ();

  return 0;
}

/* neut_mesh_volume computes the total volume of a 3D mesh + min and max
 * element volumes */
int
neut_mesh_volume (struct NODES Nodes, struct MESH Mesh, double* pvoltot, double* pvolmin, double* pvolmax)
{
  int i;
  double vol, voltot, volmin, volmax;
/*----------------------------------------------------------------------
 * volume of elements */

  if (Mesh.Dimension != 3)
    return -1;

  volmin = DBL_MAX;
  volmax = -DBL_MAX;
  voltot = 0;
  for (i = 1; i <= Mesh.EltQty; i++)
  {
    neut_mesh_elt_volume (Nodes, Mesh, i, &vol);
    voltot += vol;
    volmin = ut_num_min (volmin, vol);
    volmax = ut_num_max (volmax, vol);
  }

  if (pvoltot != NULL)
    (*pvoltot) = voltot;

  if (pvolmin != NULL)
    (*pvolmin) = volmin;
  
  if (pvolmax != NULL)
  (*pvolmax) = volmax;

  return 0;
}

/* neut_mesh_elset_volume computes the volume of a 3D mesh elset */
int
neut_mesh_elset_volume (struct NODES Nodes, struct MESH Mesh, int elset,
    double* pvol)
{
  int i;
  double vol;

  if (Mesh.Dimension != 3)
  {
    (*pvol) = 0;
    return -1;
  }

  (*pvol) = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    neut_mesh_elt_volume (Nodes, Mesh, Mesh.Elsets[elset][i], &vol);
    (*pvol) += vol;
  }

  return 0;
}

/* neut_mesh_elset_centroid computes the centroid of a 3D mesh elset */
int
neut_mesh_elset_centre (struct NODES Nodes, struct MESH Mesh, int elset,
    double* pos)
{
  int i, elt;
  double* coo = ut_alloc_1d (3);
  double vol_tot, vol;

  if (Mesh.Dimension != 3)
    return -1;

  vol_tot = 0;
  vol = 0;
  ut_array_1d_set (pos, 3, 0);
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    neut_mesh_eltcentre (Mesh, Nodes, elt, coo);
    neut_mesh_elt_volume (Nodes, Mesh, elt, &vol);
    
    ut_array_1d_scale (coo, 3, vol);
    ut_array_1d_add (pos, coo, 3, pos);

    vol_tot += vol;
  }
  ut_array_1d_scale (pos, 3, 1./vol_tot);

  ut_free_1d (coo);

  return 0;
}

/* implemented for geof only */
void
neut_mesh_fscanf_geof (FILE * file, struct NODES *pNodes, struct MESH *pMesh)
{
  int i, j, eltnodeqty = 0, dim, order, qty;
  char *string = ut_alloc_1d_char (100);
  char *type = ut_alloc_1d_char (100);
  char c;
  int *nodes_old_new = NULL;
  int *elt_nbs = NULL;
  int *node_nbs = NULL;
  int *elset_nbs = NULL;

  /* read header ---------------------------------------------------- */
  if (fscanf (file, "%s", string) != 1 || strcmp (string, "***geometry") != 0)
  {
    ut_print_message (2, 0, "Start word does not match (%s).\n", string);
    /* abort (); */
  }

  /* read nodes ----------------------------------------------------- */
  if (fscanf (file, "%s", string) != 1 || strcmp (string, "**node") != 0)
  {
    ut_print_message (2, 0, "Start word does not match (%s).\n", string);
    abort ();
  }

  if (fscanf (file, "%d%d", &((*pNodes).NodeQty), &((*pMesh).Dimension)) != 2)
    abort ();

  node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  node_nbs[0] = (*pNodes).NodeQty;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    if (fscanf (file, "%d", &(node_nbs[i])) != 1)
      abort ();
    ut_array_1d_fscanf (file, (*pNodes).NodeCoo[i], 3);
  }

  /* reading elements ----------------------------------------------- */
  
  if (fscanf (file, "%s", string) != 1 || strcmp (string, "**element") != 0)
  {
    ut_print_message (2, 0, "Start word does not match (%s).\n", string);
    abort ();
  }

  if (fscanf (file, "%d", &((*pMesh).EltQty)) != 1)
    abort ();

  elt_nbs = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pMesh).EltNodes = ut_alloc_1d_pint ((*pMesh).EltQty + 1);

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (file, "%d%s", &(elt_nbs[i]), string) != 2)
      abort ();

    neut_elt_name_prop ("geof", string, type, &dim, &order);
    if (i == 1)
    {
      (*pMesh).EltOrder = order;
      eltnodeqty = neut_elt_nodeqty (type, dim, order);
    }
    else if ((*pMesh).EltOrder != order || (*pMesh).Dimension != dim)
    {
      ut_print_messagewnc (2, 72,
			   "Mesh dimension and element order must be the same for all elements.");
      /* abort (); */
    }

    (*pMesh).EltNodes[i] = ut_alloc_1d_int (eltnodeqty);
    if ((*pMesh).EltOrder == 1)
      ut_array_1d_int_fscanf (file, (*pMesh).EltNodes[i], eltnodeqty);
    else
    {
      int status = 0;
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][0]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][1]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][2]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][4]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][5]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][6]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][7]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][9]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][8]));
      status += fscanf (file, "%d", &((*pMesh).EltNodes[i][3]));
      if (status != 10)
	abort ();
    }
  }

  /* reading elsets ------------------------------------------------- */
  (*pMesh).ElsetQty = ut_file_string_number (file, "**elset", "");

  if ((*pMesh).ElsetQty != 0)
  {
    elset_nbs = ut_alloc_1d_int ((*pMesh).ElsetQty + 1);
    (*pMesh).Elsets = ut_alloc_1d_pint ((*pMesh).ElsetQty + 1);

    if (ut_file_string_goto (file, "**elset") == -1)
      abort ();

    for (i = 1; i <= (*pMesh).ElsetQty; i++)
    {
      ut_file_skip (file, 1);
      for (j = 0; j < 5; j++)
	if (fscanf (file, "%c", &c) != 1)
	  abort ();

      if (fscanf (file, "%d", &(elset_nbs[i])) != 1)
	abort ();

      if (i < (*pMesh).ElsetQty)
	qty = ut_file_string_nextpos (file, "**elset");
      else
	qty = ut_file_string_nextpos (file, "***return");

      (*pMesh).Elsets[i] = ut_alloc_1d_int (qty + 1);
      (*pMesh).Elsets[i][0] = qty;

      ut_array_1d_int_fscanf (file, (*pMesh).Elsets[i] + 1, qty);
    }
  }

  ut_free_1d_char (string);
  ut_free_1d_char (type);

  neut_mesh_init_eltelset (pMesh, elset_nbs);

  /* neut_nodes_renumber_continuous (pNodes, node_nbs, &nodes_old_new);
   * */
  neut_mesh_renumber_continuous (pMesh, node_nbs, elt_nbs, elset_nbs);

  ut_free_1d_int (nodes_old_new);

  return;
}

/* Reading an fev file. Assumed to be numbered contiguously */
void
neut_mesh_fscanf_fev (FILE * parms, FILE * mesh, FILE * elsets,
		      struct NODES *pNodes, struct MESH *pMesh)
{
  int i, tmp, eltnodeqty = 0;
  char *string = ut_alloc_1d_char (1000);
  int *elt_nbs = NULL;
  int *node_nbs = NULL;
  int *elset_eltqty = NULL;

  /* read parms ----------------------------------------------------- */
  if (fscanf (parms, "%d%d", &(*pMesh).EltQty, &(*pNodes).NodeQty) != 2)
    abort ();

  (*pMesh).Dimension = 3;

  /* determining the elt order from the number of words on the first
   * line (elt_id + its nodes) */
  ut_file_line_nbwords_pointer (mesh, &eltnodeqty);
  eltnodeqty--;
  fseek (mesh, 0, 0);

  (*pMesh).EltOrder = neut_elt_order ((*pMesh).EltType, 3, eltnodeqty);
  if ((*pMesh).EltOrder == -1)
  {
    ut_print_message (2, 0, "mesh file has a bad format.\n");
    abort ();
  }

  elt_nbs = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);

  /* reading mesh --------------------------------------------------- */
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (mesh, "%d", &(elt_nbs[i])) != 1)
      abort ();

    elt_nbs[i]++;
    if ((*pMesh).EltOrder == 1)
      ut_array_1d_int_fscanf (mesh, (*pMesh).EltNodes[i], eltnodeqty);
    else
    {
      int status = 0;
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][0]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][4]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][1]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][5]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][2]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][6]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][7]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][9]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][8]));
      status += fscanf (mesh, "%d", &((*pMesh).EltNodes[i][3]));
      if (status != 10)
	abort ();
    }

    ut_array_1d_int_addval ((*pMesh).EltNodes[i], eltnodeqty, 1,
			    (*pMesh).EltNodes[i]);
  }

  ut_file_skip (mesh, 3);	/* skipping 1.0 1.0 1.0 */

  /* read nodes ----------------------------------------------------- */
  node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  node_nbs[0] = (*pNodes).NodeQty;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    if (fscanf (mesh, "%d", &(node_nbs[i])) != 10)
      abort ();
    node_nbs[i]++;
    ut_array_1d_fscanf (mesh, (*pNodes).NodeCoo[i], 3);
  }

  /* read elsets ---------------------------------------------------- */
  ut_file_skip (elsets, 1);	// skipping "grain-input"
  if (fscanf (elsets, "%d", &tmp) != 1)
    abort ();

  if (tmp != (*pMesh).EltQty)
  {
    ut_print_message (2, 0, "Elt quantities in parms and opt do not match!\n");
    abort ();
  }
  if (fscanf (elsets, "%d", &((*pMesh).ElsetQty)) != 1)
    abort ();

  /* searching number of element per elset for allocation */
  elset_eltqty = ut_alloc_1d_int ((*pMesh).ElsetQty + 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (elsets, "%d", &tmp) != 1)
      abort ();

    ut_file_skip (elsets, 1);	// skipping phase
    elset_eltqty[tmp]++;
  }

  (*pMesh).Elsets = ut_alloc_1d_pint ((*pMesh).ElsetQty + 1);
  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    (*pMesh).Elsets[i] = ut_alloc_1d_int (elset_eltqty[i] + 1);
  ut_free_1d_int (elset_eltqty);

  fseek (elsets, 0, SEEK_SET);
  ut_file_skip (elsets, 3);

  /* assigning each element to its elset */
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (elsets, "%d", &tmp) != 1)
      abort ();

    ut_file_skip (elsets, 1); // skipping phase
    (*pMesh).Elsets[tmp][++(*pMesh).Elsets[tmp][0]] = i;
  }

  ut_free_1d_char (string);
  ut_free_1d_int (node_nbs);
  ut_free_1d_int (elt_nbs);

  return;
}

void
neut_mesh_fscanf_stellar (FILE * nodes, FILE * elts,
			  struct NODES *pNodes, struct MESH *pMesh)
{
  int i, eltnodeqty, bound;
  int *node_nbs = NULL;
  int *elt_nbs = NULL;

  /* nodes */
  if (fscanf (nodes, "%d", &((*pNodes).NodeQty)) != 1)
    abort ();

  node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  node_nbs[0] = (*pNodes).NodeQty;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  if (fscanf (nodes, "%d%d%d", &bound, &bound, &bound) != 3)
    abort ();

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    if (fscanf (nodes, "%d", &(node_nbs[i])) != 1)
      abort ();

    ut_array_1d_fscanf (nodes, (*pNodes).NodeCoo[i], 3);
    if (fscanf (nodes, "%d", &bound) != 1)
      abort ();
  }

  /* reading elements ----------------------------------------------- */
  (*pMesh).Dimension = 3;

  if (fscanf (elts, "%d%d", &((*pMesh).EltQty), &eltnodeqty) != 2)
    abort ();

  if (eltnodeqty == 4)
    (*pMesh).EltOrder = 1;
  else if (eltnodeqty == 10)
    (*pMesh).EltOrder = 2;
  else
    abort ();

  elt_nbs = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);

  ut_file_skip (elts, 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (elts, "%d", &(elt_nbs[i])) != 1)
      abort ();
    ut_array_1d_int_fscanf (elts, (*pMesh).EltNodes[i], eltnodeqty);
  }

  (*pMesh).Elsets = ut_alloc_1d_pint (2);
  (*pMesh).Elsets[1] = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pMesh).Elsets[1][0] = (*pMesh).EltQty;
  for (i = 1; i <= (*pMesh).EltQty; i++)
    (*pMesh).Elsets[1][i] = i;

  return;
}

void
neut_mesh_prop_fscanf_geof (FILE * file, int *pNodeQty, int *pEltQty)
{
  if (ut_file_string_goto (file, "**node") != 0)
    abort ();

  ut_file_skip (file, 1);
  if (fscanf (file, "%d", pNodeQty) != 1)
    abort ();

  if (ut_file_string_goto (file, "**element") != 1)
    abort ();

  ut_file_skip (file, 1);
  if (fscanf (file, "%d", pEltQty) != 1)
    abort ();

  return;
}

/* avoid using - computer-intensive. use neut_mesh_init_nodeelts? */
void
neut_node_elts (struct MESH Mesh, int node, int *elts, int *peltqty)
{
  int i, j;
  int nodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  (*peltqty) = 0;
  for (i = 1; i <= Mesh.EltQty; i++)
    for (j = 0; j < nodeqty; j++)
      if (Mesh.EltNodes[i][j] == node)
	elts[(*peltqty)++] = i;

  ut_array_1d_int_sort (elts, *peltqty);

  return;
}

struct MESH
neut_mesh_alloc (int Dimension, char* EltType, int EltOrder, int EltQty,
		 int ElsetQty)
{
  struct MESH Mesh;
  int eltnodeqty = neut_elt_nodeqty (EltType, Dimension, EltOrder);

  Mesh.Dimension = Dimension;
  Mesh.EltOrder = EltOrder;

  Mesh.EltQty = EltQty;
  Mesh.EltNodes = ut_alloc_2d_int (EltQty + 1, eltnodeqty);
  Mesh.EltElset = ut_alloc_1d_int (EltQty + 1);

  Mesh.ElsetQty = ElsetQty;
  if (ElsetQty > 0)
    Mesh.Elsets = ut_alloc_2d_int (Mesh.ElsetQty + 1, 2);
  else
    Mesh.Elsets = NULL;

  Mesh.NodeElts = NULL;

  Mesh.EltType = ut_alloc_1d_char (10);
  sprintf (Mesh.EltType, "%s", EltType);

  Mesh.EltCoo = NULL;
  Mesh.msize = NULL;

  return Mesh;
}


void
neut_nodes_commonelts (struct MESH Mesh, int *nodes, int nodeqty,
		       int *elts, int *peltqty)
{
  int i;
  int *tmp1 = NULL;
  int *tmp2 = NULL;
  int tmp2qty = -1;

  if (Mesh.NodeElts == NULL)
  {
    ut_print_message (2, 0, "neut_nodes_commonelts requires NodeElts\n");
    ut_error_reportbug ();
  }

  if (nodeqty == 1)
  {
    ut_array_1d_int_memcpy (elts, Mesh.NodeElts[nodes[0]][0], Mesh.NodeElts[nodes[0]] + 1);
    (*peltqty) = Mesh.NodeElts[nodes[0]][0];
    return;
  }

  for (i = 0; i < nodeqty; i++)
    if (nodes[i] > Mesh.NodeElts[0][0])
    {
      (*peltqty) = 0;
      return;
    }

  tmp1 = ut_alloc_1d_int (Mesh.NodeElts[nodes[0]][0] + 1);
  tmp2 = ut_alloc_1d_int (Mesh.NodeElts[nodes[0]][0] + 1);

  ut_array_1d_int_memcpy (tmp1, Mesh.NodeElts[nodes[0]][0], Mesh.NodeElts[nodes[0]] + 1);

  (*peltqty) = Mesh.NodeElts[nodes[0]][0];

  for (i = 1; i < nodeqty; i++)
  {
    ut_array_1d_int_inter (tmp1, *peltqty, Mesh.NodeElts[nodes[i]] + 1,
	                   Mesh.NodeElts[nodes[i]][0], tmp2, &tmp2qty);

    ut_array_1d_int_memcpy (tmp1, tmp2qty, tmp2);
    (*peltqty) = tmp2qty;
    
     /*
       printf ("node %d elt: ", nodes[i]);
       ut_array_1d_int_fprintf (stdout, tmp, tmpqty, "%d");
       printf ("> elt: ");
       ut_array_1d_int_fprintf (stdout, elts, *peltqty, "%d");
     */
  }

  ut_array_1d_int_memcpy (elts, tmp2qty, tmp2);
  ut_array_1d_int_sort (elts, *peltqty);

  ut_free_1d_int (tmp1);
  ut_free_1d_int (tmp2);

  return;
}

void
neut_nodes_allelts (struct MESH Mesh, int *nodes, int nodeqty,
		    int *elts, int *peltqty)
{
  int i, j, node;

  (*peltqty) = 0;
  for (i = 0; i < nodeqty; i++)
  {
    node = nodes[i];
    for (j = 1; j <= Mesh.NodeElts[node][0]; j++)
      if (ut_array_1d_int_eltpos (elts, *peltqty, Mesh.NodeElts[node][j]) ==
	  -1)
	elts[(*peltqty)++] = Mesh.NodeElts[node][j];
  }

  ut_array_1d_int_sort (elts, *peltqty);

  return;
}

int
neut_nodes_elt (struct MESH Mesh, int *nodes, int nodeqty, int *pelt)
{
  int *tmp = ut_alloc_1d_int (100);
  int tmpqty;

  neut_nodes_commonelts (Mesh, nodes, nodeqty, tmp, &tmpqty);

  if (tmpqty == 1)
    (*pelt) = tmp[0];
  else
    (*pelt) = -1;

  ut_free_1d_int (tmp);

  return ut_num_min (*pelt, 0);
}

int
neut_nodes_elsets_commonelts (struct MESH Mesh, int *nodes, int nodeqty,
                             int* elsets, int elsetqty, int *elts, int* peltqty)
{
  int i, j;
  int *tmp = ut_alloc_1d_int (1000);
  int tmpqty;

  if (Mesh.EltElset == NULL)
  {
    printf ("neut_nodes_elsets_commonelts needs EltElset\n");
    ut_error_reportbug ();
  }

  neut_nodes_commonelts (Mesh, nodes, nodeqty, tmp, &tmpqty);

  (*peltqty) = 0;
  for (i = 0; i < tmpqty; i++)
    for (j = 0; j < elsetqty; j++)
      if (ut_array_1d_int_eltpos
	    (elsets, elsetqty, Mesh.EltElset[tmp[i]]) != -1)
      {
	elts[(*peltqty)++] = tmp[i];
        break;
      }

  ut_free_1d_int (tmp);

  return 0;
}

/*
int
neut_mesh_eltelsetOLD (struct MESH Mesh, int elt, int *pelset)
{
  int i;

  (*pelset) = -1;
  for (i = 1; i <= Mesh.ElsetQty; i++)
    if (ut_array_1d_int_eltpos
	(Mesh.Elsets[i] + 1, Mesh.Elsets[i][0], elt) != -1)
    {
      (*pelset) = i;
      break;
    }

  return ut_num_min (*pelset, 0);
}
*/

void
neut_mesh_eltcentre (struct MESH Mesh, struct NODES Nodes,
		     int elt, double *coo)
{
  int i, j, node;
  int nodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, 1);

  /* averaging the coo of the vertex nodes */
  ut_array_1d_zero (coo, 3);
  for (i = 0; i < nodeqty; i++)
  {
    node = Mesh.EltNodes[elt][i];
    for (j = 0; j < 3; j++)
      coo[j] += Nodes.NodeCoo[node][j];
  }

  ut_array_1d_scale (coo, 3, 1. / nodeqty);

  return;
}

int
neut_fod_facefod (int **FoD, int elsetnb, int *pfod)
{
  int i;

  (*pfod) = -1;
  for (i = 1; i <= 6; i++)
    if (ut_array_1d_int_eltpos (FoD[i] + 1, FoD[i][0], elsetnb) != -1)
    {
      (*pfod) = i;
      break;
    }

  return ut_num_min (*pfod, 0);
}

int
neut_mesh_elt0delts1d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
		       int *elt1d, int *pelt1dqty)
{
  int* nodes = Mesh0D.EltNodes[elt0d];

  ut_array_1d_int_memcpy (elt1d, Mesh1D.NodeElts[nodes[0]][0], Mesh1D.NodeElts[nodes[0]] + 1);
    (*pelt1dqty) = Mesh1D.NodeElts[nodes[0]][0];

  return *pelt1dqty;
}

int
neut_mesh_elt0delsets1d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
			 int *elset1d, int *pelset1dqty)
{
  int* elt1d = ut_alloc_1d_int (1000);
  int elt1dqty;

  neut_mesh_elt0delts1d (Mesh0D, elt0d, Mesh1D, elt1d, &elt1dqty);

  if (Mesh1D.ElsetQty == 1 && elt1dqty == 1)
  {
    elset1d[0] = -1;
    elset1d[1] = Mesh1D.EltElset[elt1d[0]];
    (*pelset1dqty) = 2;
  }
  else
    neut_mesh_elts_elsets (Mesh1D, elt1d, elt1dqty, elset1d, pelset1dqty);

  ut_free_1d_int (elt1d);

  return *pelset1dqty;
}

int
neut_mesh_elt1delts2d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
		       int *elt2d, int *pelt2dqty)
{
  int nodeqty = neut_elt_nodeqty (Mesh1D.EltType, 1, Mesh1D.EltOrder);

  if (Mesh2D.Dimension != 2)
    abort ();
  
  neut_nodes_commonelts (Mesh2D, Mesh1D.EltNodes[elt1d],
			 nodeqty, elt2d, pelt2dqty);

  return *pelt2dqty;
}

int
neut_mesh_elt1delsets2d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
			 int *elset2d, int *pelset2dqty)
{
  int* elt2d = ut_alloc_1d_int (1000);
  int elt2dqty;

  neut_mesh_elt1delts2d (Mesh1D, elt1d, Mesh2D, elt2d, &elt2dqty);

  if (Mesh2D.ElsetQty == 1 && elt2dqty == 1)
  {
    elset2d[0] = -1;
    elset2d[1] = Mesh2D.EltElset[elt2d[0]];
    (*pelset2dqty) = 2;
  }
  else
    neut_mesh_elts_elsets (Mesh2D, elt2d, elt2dqty, elset2d, pelset2dqty);

  ut_free_1d_int (elt2d);

  return *pelset2dqty;
}

int
neut_mesh_elt1delts3d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
		       struct MESH Mesh3D, int *elt3d, int *pelt3dqty)
{
  int i, j, qty, elt2dqty;
  int* elt2d = ut_alloc_1d_int (1000);
  int* tmp = ut_alloc_1d_int (1000);

  neut_mesh_elt1delts2d (Mesh1D, elt1d, Mesh2D, elt2d, &elt2dqty);

  (*pelt3dqty) = 0;
  for (i = 0; i < elt2dqty; i++)
  {
    neut_mesh_elt2delts3d (Mesh2D, elt2d[i], Mesh3D, tmp, &qty);
    for (j = 0; j < qty; j++)
      if (ut_array_1d_int_eltpos (elt3d, (*pelt3dqty), tmp[j]) == -1)
	elt3d[(*pelt3dqty)++] = tmp[j];
  }

  ut_free_1d_int (elt2d);
  ut_free_1d_int (tmp);

  return *pelt3dqty;
}

int
neut_mesh_elt0delts3d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
		       struct MESH Mesh2D, struct MESH Mesh3D, int *elt3d, int *pelt3dqty)
{
  int i, j, qty, elt1dqty;
  int* elt1d = ut_alloc_1d_int (1000);
  int* tmp = ut_alloc_1d_int (1000);

  neut_mesh_elt0delts1d (Mesh0D, elt0d, Mesh1D, elt1d, &elt1dqty);

  (*pelt3dqty) = 0;
  for (i = 0; i < elt1dqty; i++)
  {
    neut_mesh_elt1delts3d (Mesh1D, elt1d[i], Mesh2D, Mesh3D, tmp, &qty);
    for (j = 0; j < qty; j++)
      if (ut_array_1d_int_eltpos (elt3d, (*pelt3dqty), tmp[j]) == -1)
	elt3d[(*pelt3dqty)++] = tmp[j];
  }

  ut_free_1d_int (elt1d);
  ut_free_1d_int (tmp);

  return *pelt3dqty;
}


int
neut_mesh_elt2delts3d (struct MESH Mesh2D, int elt2d, struct MESH Mesh3D,
		       int *elt3d, int *pelt3dqty)
{
  int nodeqty = neut_elt_nodeqty (Mesh2D.EltType, 2, Mesh2D.EltOrder);

  neut_nodes_commonelts (Mesh3D, Mesh2D.EltNodes[elt2d],
			 nodeqty, elt3d, pelt3dqty);

  return (*pelt3dqty >= 0 && *pelt3dqty <= 2) ? *pelt3dqty : -1;
}

void
neut_mesh_elt_vermesh (struct MESH Mesh1D, int elt1d, struct MESH *pMesh0D)
{
  int *tab = Mesh1D.EltNodes[elt1d]; // do not free; this is a shortcut

  (*pMesh0D).Dimension = 0;
  (*pMesh0D).EltOrder = Mesh1D.EltOrder;

  (*pMesh0D).EltQty = 2;
  (*pMesh0D).ElsetQty = 0;

  if ((*pMesh0D).EltNodes == NULL)
    (*pMesh0D).EltNodes = ut_alloc_2d_int (3, 1);

  (*pMesh0D).EltNodes[1][0] = tab[0];
  (*pMesh0D).EltNodes[2][0] = tab[1];

  return;
}

void
neut_mesh_elt_edgemesh (struct MESH Mesh2D, int elt2d, struct MESH *pMesh1D)
{
  int *tab = Mesh2D.EltNodes[elt2d]; // do not free; this is a shortcut

  (*pMesh1D).Dimension = 1;
  (*pMesh1D).EltOrder = Mesh2D.EltOrder;

  if (strcmp (Mesh2D.EltType, "tri") == 0)
    (*pMesh1D).EltQty = 3;
  else if (strcmp (Mesh2D.EltType, "quad") == 0)
    (*pMesh1D).EltQty = 4;
  else
    abort ();

  (*pMesh1D).ElsetQty = 0;
  (*pMesh1D).EltType = ut_realloc_1d_char ((*pMesh1D).EltType, 5);
  sprintf ((*pMesh1D).EltType, "%s", Mesh2D.EltType);

  if ((*pMesh1D).EltNodes == NULL)
  {
    if ((*pMesh1D).EltOrder == 1)
      (*pMesh1D).EltNodes = ut_alloc_2d_int ((*pMesh1D).EltQty + 1, 2);
    else if ((*pMesh1D).EltOrder == 2)
      (*pMesh1D).EltNodes = ut_alloc_2d_int ((*pMesh1D).EltQty + 1, 3);
    else
      abort ();
  }

  if (strcmp (Mesh2D.EltType, "tri") == 0)
  {
    (*pMesh1D).EltNodes[1][0] = tab[0];
    (*pMesh1D).EltNodes[1][1] = tab[1];

    (*pMesh1D).EltNodes[2][0] = tab[1];
    (*pMesh1D).EltNodes[2][1] = tab[2];

    (*pMesh1D).EltNodes[3][0] = tab[2];
    (*pMesh1D).EltNodes[3][1] = tab[0];

    if (Mesh2D.EltOrder == 2)
    {
      (*pMesh1D).EltNodes[1][2] = tab[3];
      (*pMesh1D).EltNodes[2][2] = tab[4];
      (*pMesh1D).EltNodes[3][2] = tab[5];
    }
  }
  else if (strcmp (Mesh2D.EltType, "quad") == 0)
  {
    (*pMesh1D).EltNodes[1][0] = tab[0];
    (*pMesh1D).EltNodes[1][1] = tab[1];

    (*pMesh1D).EltNodes[2][0] = tab[1];
    (*pMesh1D).EltNodes[2][1] = tab[2];

    (*pMesh1D).EltNodes[3][0] = tab[2];
    (*pMesh1D).EltNodes[3][1] = tab[3];

    (*pMesh1D).EltNodes[4][0] = tab[3];
    (*pMesh1D).EltNodes[4][1] = tab[0];

    if (Mesh2D.EltOrder == 2)
    {
      (*pMesh1D).EltNodes[1][2] = tab[4];
      (*pMesh1D).EltNodes[2][2] = tab[5];
      (*pMesh1D).EltNodes[3][2] = tab[6];
      (*pMesh1D).EltNodes[4][2] = tab[7];
    }
  }
  else
    abort ();

  return;
}


void
neut_mesh_elt_facetmesh (struct MESH Mesh3D, int elt3d, struct MESH *pMesh2D)
{
  int *tab = Mesh3D.EltNodes[elt3d]; // do not free; this is a shortcut

  (*pMesh2D).Dimension = 2;
  (*pMesh2D).EltOrder = Mesh3D.EltOrder;
  sprintf ((*pMesh2D).EltType, "%s", Mesh3D.EltType);

  (*pMesh2D).ElsetQty = 0;

  if (strcmp ((*pMesh2D).EltType, "tri") == 0)
  {
    (*pMesh2D).EltQty = 4;

    if ((*pMesh2D).EltNodes == NULL)
    {
      if ((*pMesh2D).EltOrder == 1)
	(*pMesh2D).EltNodes = ut_alloc_2d_int ((*pMesh2D).EltQty + 1, 3);
      else if ((*pMesh2D).EltOrder == 2)
	(*pMesh2D).EltNodes = ut_alloc_2d_int ((*pMesh2D).EltQty + 1, 6);
      else
	abort ();
    }

    (*pMesh2D).EltNodes[1][0] = tab[0];
    (*pMesh2D).EltNodes[1][1] = tab[1];
    (*pMesh2D).EltNodes[1][2] = tab[2];

    (*pMesh2D).EltNodes[2][0] = tab[1];
    (*pMesh2D).EltNodes[2][1] = tab[2];
    (*pMesh2D).EltNodes[2][2] = tab[3];

    (*pMesh2D).EltNodes[3][0] = tab[2];
    (*pMesh2D).EltNodes[3][1] = tab[3];
    (*pMesh2D).EltNodes[3][2] = tab[0];

    (*pMesh2D).EltNodes[4][0] = tab[3];
    (*pMesh2D).EltNodes[4][1] = tab[0];
    (*pMesh2D).EltNodes[4][2] = tab[1];

    if (Mesh3D.EltOrder == 2)
    {
      (*pMesh2D).EltNodes[1][3] = tab[4];
      (*pMesh2D).EltNodes[1][4] = tab[5];
      (*pMesh2D).EltNodes[1][5] = tab[6];

      (*pMesh2D).EltNodes[2][3] = tab[5];
      (*pMesh2D).EltNodes[2][4] = tab[8];
      (*pMesh2D).EltNodes[2][5] = tab[9];

      (*pMesh2D).EltNodes[3][3] = tab[8];
      (*pMesh2D).EltNodes[3][4] = tab[7];
      (*pMesh2D).EltNodes[3][5] = tab[6];

      (*pMesh2D).EltNodes[4][3] = tab[7];
      (*pMesh2D).EltNodes[4][4] = tab[4];
      (*pMesh2D).EltNodes[4][5] = tab[9];
    }
  }
  else if (strcmp ((*pMesh2D).EltType, "quad") == 0)
  {
    (*pMesh2D).EltQty = 6;

    if ((*pMesh2D).EltNodes == NULL)
    {
      if ((*pMesh2D).EltOrder == 1)
	(*pMesh2D).EltNodes = ut_alloc_2d_int ((*pMesh2D).EltQty + 1, 4);
      else if ((*pMesh2D).EltOrder == 2)
	(*pMesh2D).EltNodes = ut_alloc_2d_int ((*pMesh2D).EltQty + 1, 8);
      else
	abort ();
    }

    (*pMesh2D).EltNodes[1][0] = tab[0];
    (*pMesh2D).EltNodes[1][1] = tab[1];
    (*pMesh2D).EltNodes[1][2] = tab[2];
    (*pMesh2D).EltNodes[1][3] = tab[3];

    (*pMesh2D).EltNodes[2][0] = tab[4];
    (*pMesh2D).EltNodes[2][1] = tab[5];
    (*pMesh2D).EltNodes[2][2] = tab[6];
    (*pMesh2D).EltNodes[2][3] = tab[7];

    (*pMesh2D).EltNodes[3][0] = tab[0];
    (*pMesh2D).EltNodes[3][1] = tab[1];
    (*pMesh2D).EltNodes[3][2] = tab[5];
    (*pMesh2D).EltNodes[3][3] = tab[4];

    (*pMesh2D).EltNodes[4][0] = tab[3];
    (*pMesh2D).EltNodes[4][1] = tab[2];
    (*pMesh2D).EltNodes[4][2] = tab[6];
    (*pMesh2D).EltNodes[4][3] = tab[7];

    (*pMesh2D).EltNodes[5][0] = tab[1];
    (*pMesh2D).EltNodes[5][1] = tab[2];
    (*pMesh2D).EltNodes[5][2] = tab[6];
    (*pMesh2D).EltNodes[5][3] = tab[5];

    (*pMesh2D).EltNodes[6][0] = tab[0];
    (*pMesh2D).EltNodes[6][1] = tab[3];
    (*pMesh2D).EltNodes[6][2] = tab[7];
    (*pMesh2D).EltNodes[6][3] = tab[4];

    if (Mesh3D.EltOrder == 2)
    {
      (*pMesh2D).EltNodes[1][4] = tab[8];
      (*pMesh2D).EltNodes[1][5] = tab[11];
      (*pMesh2D).EltNodes[1][6] = tab[13];
      (*pMesh2D).EltNodes[1][7] = tab[9];

      (*pMesh2D).EltNodes[2][4] = tab[16];
      (*pMesh2D).EltNodes[2][5] = tab[18];
      (*pMesh2D).EltNodes[2][6] = tab[19];
      (*pMesh2D).EltNodes[2][7] = tab[17];

      (*pMesh2D).EltNodes[3][4] = tab[8];
      (*pMesh2D).EltNodes[3][5] = tab[12];
      (*pMesh2D).EltNodes[3][6] = tab[16];
      (*pMesh2D).EltNodes[3][7] = tab[10];

      (*pMesh2D).EltNodes[4][4] = tab[13];
      (*pMesh2D).EltNodes[4][5] = tab[14];
      (*pMesh2D).EltNodes[4][6] = tab[19];
      (*pMesh2D).EltNodes[4][7] = tab[15];

      (*pMesh2D).EltNodes[5][4] = tab[11];
      (*pMesh2D).EltNodes[5][5] = tab[14];
      (*pMesh2D).EltNodes[5][6] = tab[18];
      (*pMesh2D).EltNodes[5][7] = tab[12];

      (*pMesh2D).EltNodes[6][4] = tab[9];
      (*pMesh2D).EltNodes[6][5] = tab[15];
      (*pMesh2D).EltNodes[6][6] = tab[17];
      (*pMesh2D).EltNodes[6][7] = tab[10];
    }
  }
  else
    abort ();
  
  return;
}

void
neut_mesh_elt_reversenodes (struct MESH *pMesh, int eltnb)
{
  // implemented for 2D only.  The 1st and 2nd order nodes are to be
  // considered separetely for reversing.
  
  if ((*pMesh).Dimension != 2)
    abort ();

  if ((*pMesh).EltOrder == 1)
    ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 2);
  else if ((*pMesh).EltOrder == 2)
  {
    ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 2);
    ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 3, 3);
  }
  else
    abort ();

  return;
}

void
neut_mesh_eltnormal (struct MESH Mesh, struct NODES Nodes, int eltnb,
		     double *n)
{
  ut_space_trianglenormal (Nodes.NodeCoo[Mesh.EltNodes[eltnb][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][2]], n);

  return;
}

void
neut_mesh_elteq (struct MESH Mesh, struct NODES Nodes, int eltnb,
		     double *eq)
{
  if (Mesh.Dimension != 2)
    ut_error_reportbug ();

  if (Mesh.EltType == NULL || strcmp (Mesh.EltType, "tri") == 0)
  {
    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[eltnb][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][2]], eq);
  }
  else if (strcmp (Mesh.EltType, "quad") == 0)
  {
    double** eqs = ut_alloc_2d (4, 4);
    int i;

    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[eltnb][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][2]], eqs[0]);
    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[eltnb][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][3]], eqs[1]);
    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[eltnb][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][3]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][0]], eqs[2]);
    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[eltnb][3]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[eltnb][1]], eqs[3]);

    ut_array_1d_zero (eq, 4);
    for (i = 0; i < 4; i++)
      ut_array_1d_add (eq, eqs[i], 4, eq);
    ut_array_1d_scale (eq, 4, 1. / ut_vector_norm (eq + 1));

    ut_free_2d (eqs, 4);
  }

  return;
}

void
neut_mesh_set_zero (struct MESH *pMesh)
{
  (*pMesh).Dimension = 0;
  (*pMesh).EltOrder = 0;
  (*pMesh).EltType = NULL;

  (*pMesh).EltQty = 0;
  (*pMesh).ElsetQty = 0;

  (*pMesh).EltNodes = NULL;
  (*pMesh).Elsets = NULL;

  (*pMesh).NodeElts = NULL;
  (*pMesh).EltElset = NULL;

  (*pMesh).EltCoo = NULL;
  (*pMesh).msize = NULL;

  return;
}

// In most cases, prefer initializing EltElset.
int
neut_mesh_elt_elset (struct MESH Mesh, int eltnb)
{
  int i;
  int elset = -1;

  for (i = 1; i <= Mesh.ElsetQty; i++)
    if (ut_array_1d_int_eltpos (Mesh.Elsets[i] + 1,
				Mesh.Elsets[i][0], eltnb) != -1)
    {
      elset = i;
      break;
    }

  return elset;
}

void
neut_mesh_elset_nodes (struct MESH Mesh, int id, int **pnodes, int *pqty)
{
  int i, elt, qty, eltnodeqty;
  int *tmp;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  tmp = ut_alloc_1d_int (Mesh.Elsets[id][0] * eltnodeqty);

  qty = 0;
  for (i = 1; i <= Mesh.Elsets[id][0]; i++)
  {
    elt = Mesh.Elsets[id][i];

    ut_array_1d_int_memcpy (tmp + qty, eltnodeqty, Mesh.EltNodes[elt]);
    qty += eltnodeqty;
  }

  ut_array_1d_int_sort_uniq (tmp, qty, &qty);

  (*pqty) = qty;
  (*pnodes) = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (*pnodes, qty, tmp);

  ut_free_1d_int (tmp);

  return;
}

void
neut_mesh_boxelts (struct MESH Mesh, struct NODES Nodes, double **bound,
		   int *elts)
{
  int i;
  double *coo = ut_alloc_1d (3);

  elts[0] = 0;
  for (i = 1; i <= Mesh.EltQty; i++)
  {
    neut_mesh_eltcentre (Mesh, Nodes, i, coo);
    if (coo[0] > bound[0][0] && coo[0] < bound[0][1]
	&& coo[1] > bound[1][0] && coo[1] < bound[1][1]
	&& coo[2] > bound[2][0] && coo[2] < bound[2][1])
      elts[++elts[0]] = i;
  }

  ut_free_1d (coo);

  return;
}

#ifdef HAVE_LIBSCOTCH
void
neut_mesh_scotchmesh (struct MESH Mesh, int vnodnbr, SCOTCH_Mesh * pSCMesh)
{
  int i, id, status;
  int velmbas, vnodbas, velmnbr, vertnbr, edgenbr;
  int *verttab, *edgetab;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  velmbas = 1;			/* element base number */
  vnodbas = 1 + Mesh.EltQty;	/* node base number */
  velmnbr = Mesh.EltQty;

  vertnbr = velmnbr + vnodnbr;
  edgenbr = 2 * velmnbr * eltnodeqty;

  verttab = ut_alloc_1d_int (vertnbr + 1);
  edgetab = ut_alloc_1d_int (edgenbr + 1);

  /* recording elements */
  id = 0;
  for (i = 1; i <= velmnbr; i++)
  {
    verttab[i - 1] = id + 1;
    ut_array_1d_int_memcpy (edgetab + id, eltnodeqty, Mesh.EltNodes[i]);
    ut_array_1d_int_addval (edgetab + id, eltnodeqty, vnodbas - 1,
			    edgetab + id);
    id += eltnodeqty;
  }

  /* recording nodes */
  for (i = 1; i <= vnodnbr; i++)
  {
    verttab[i + Mesh.EltQty - 1] = id + 1;
    ut_array_1d_int_memcpy (edgetab + id, Mesh.NodeElts[i][0],
			    Mesh.NodeElts[i] + 1);
    id += Mesh.NodeElts[i][0];
  }
  verttab[vertnbr] = edgenbr + 1;

  SCOTCH_meshBuild (pSCMesh, velmbas, vnodbas, velmnbr, vnodnbr, verttab,
		    NULL, NULL, NULL, NULL, edgenbr, edgetab);
  /*
     file = ut_file_open ("dd", "w");
     SCOTCH_meshSave (pSCMesh, file);
     ut_file_close (file, "dd", "w");
   */

  status = SCOTCH_meshCheck (pSCMesh);
  if (status != 0)
    printf ("error");

  return;
}
#endif

void
neut_mesh_free (struct MESH *pMesh)
{
  if (pMesh == NULL)
    return;

  ut_free_1d_char ((*pMesh).EltType);

  ut_free_2d_int ((*pMesh).EltNodes, (*pMesh).EltQty + 1);
  
  ut_free_2d_int ((*pMesh).Elsets, (*pMesh).ElsetQty + 1);

  ut_free_1d_int ((*pMesh).EltElset);

  if ((*pMesh).NodeElts != NULL)
    ut_free_2d_int ((*pMesh).NodeElts, (*pMesh).NodeElts[0][0] + 1);

  ut_free_2d ((*pMesh).EltCoo, (*pMesh).EltQty + 1);

  ut_free_1d_int ((*pMesh).msize);

  neut_mesh_set_zero (pMesh);

  return;
}

/* nodes & elements are considered to be numbered contiguously from 1 */
void
neut_mesh_init_nodeelts (struct MESH *pMesh, int NodeQty)
{
  int i, j, eltnodeqty, node;

  if (pMesh == NULL || (*pMesh).EltQty == 0)
    return;

  eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
      (*pMesh).EltOrder);

  (*pMesh).NodeElts = ut_alloc_2d_int (NodeQty + 1, 1);
  (*pMesh).NodeElts[0][0] = NodeQty;

  /* first pass: recording, for each node, the quantity of elements */
  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pMesh).NodeElts[(*pMesh).EltNodes[i][j]][0]++;

  /* allocation */
  for (i = 1; i <= NodeQty; i++)
    (*pMesh).NodeElts[i] = ut_realloc_1d_int ((*pMesh).NodeElts[i], (*pMesh).NodeElts[i][0] + 1);

  ut_array_2d_int_zero ((*pMesh).NodeElts + 1, NodeQty, 1);

  /* recording node elements */
  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
    {
      node = (*pMesh).EltNodes[i][j];
      (*pMesh).NodeElts[node][++(*pMesh).NodeElts[node][0]] = i;
    }

  return;
}

void
neut_mesh_init_eltelset (struct MESH *pMesh, int *elset_nbs)
{
  int i, j;

  (*pMesh).EltElset = ut_realloc_1d_int ((*pMesh).EltElset, (*pMesh).EltQty + 1);

  if (elset_nbs != NULL)
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
      for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
	(*pMesh).EltElset[(*pMesh).Elsets[i][j]] = elset_nbs[i];
  else
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
      for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
	(*pMesh).EltElset[(*pMesh).Elsets[i][j]] = i;

  return;
}

void
neut_mesh_init_elsets (struct MESH *pMesh)
{
  int i, elset;
  
  (*pMesh).ElsetQty  = 0;
  for (i = 1; i <= (*pMesh).EltQty; i++)
    (*pMesh).ElsetQty = ut_num_max ((*pMesh).EltElset[i], (*pMesh).ElsetQty);

  // Init Elsets from EltElset
  (*pMesh).Elsets = ut_alloc_2d_int ((*pMesh).ElsetQty + 1, 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    elset = (*pMesh).EltElset[i];
    (*pMesh).Elsets[elset][0]++;
    (*pMesh).Elsets[elset]
      = ut_realloc_1d_int ((*pMesh).Elsets[elset], (*pMesh).Elsets[elset][0] + 1);
    (*pMesh).Elsets[elset][(*pMesh).Elsets[elset][0]] = i;
  }

  return;
}

/* This routine returns the communications of an element. *pcomqty is
 * the number of partitions with which there are communications;
 * for each partition i, coms[i][0] is the number of the partition and
 * coms[i][1] the numbers of communications with it.
 */
int
neut_mesh_elt_coms (struct MESH Mesh, int elt, int partqty, int *parttab,
		    int *parttab2, int **coms, int *pcomqty)
{
  int i, j, pos, eltpart, nodepart, comtotalqty;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  comtotalqty = 0;
  (*pcomqty) = 0;
  ut_array_2d_int_zero (coms, partqty, 2);

  eltpart = parttab2[elt];
  for (i = 0; i < eltnodeqty; i++)
  {
    nodepart = parttab[Mesh.EltNodes[elt][i]];

    if (nodepart != eltpart)
    {
      comtotalqty++;
      pos = -1;
      for (j = 0; j < *pcomqty; j++)
	if (coms[j][0] == nodepart)
	{
	  pos = j;
	  break;
	}

      if (pos != -1)
	coms[pos][1]++;
      else
      {
	coms[(*pcomqty)][0] = nodepart;
	coms[(*pcomqty)][1] = 1;
	(*pcomqty)++;
      }
    }
  }

  ut_array_2d_int_sortbycol_des (coms, *pcomqty, 2, 1);

  return comtotalqty;
}

/* fast routine that returns the number of communications */
int
neut_mesh_elt_comqty (struct MESH Mesh, int elt, int *parttab, int *parttab2)
{
  int i, eltpart, nodepart, comtotalqty;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  comtotalqty = 0;
  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  eltpart = parttab2[elt];
  for (i = 0; i < eltnodeqty; i++)
  {
    nodepart = parttab[Mesh.EltNodes[elt][i]];
    if (eltpart != nodepart)
      comtotalqty++;
  }

  return comtotalqty;
}

int
neut_mesh_elt_partqty (struct MESH Mesh, int elt, int *parttab)
{
  int i, qty, nodepart;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);
  int *part = ut_alloc_1d_int (eltnodeqty);

  qty = 0;

  for (i = 0; i < eltnodeqty; i++)
  {
    nodepart = parttab[Mesh.EltNodes[elt][i]];
    if (ut_array_1d_int_eltpos (part, qty, nodepart) == -1)
      part[qty++] = nodepart;
  }

  ut_free_1d_int (part);

  return qty;
}

int
neut_mesh_elt_isbound (struct MESH Mesh, int elt, int *parttab)
{
  int i, res, part;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  res = 0;
  part = parttab[Mesh.EltNodes[elt][0]];
  for (i = 1; i < eltnodeqty; i++)
  {
    if (parttab[Mesh.EltNodes[elt][i]] != part)
    {
      res = 1;
      break;
    }
  }

  return res;
}

// Generates the 2D mesh corresponding to a 3D mesh.
// It also retuns the FaceQty value and the FacePoly array, which hold
// the number of 2D elsets and their relations to the 3D elsets.  The
// terms `FaceQty' and `FacePoly' are used in reference to the Geo
// structure, and closely match them, except that FacePoly is indexed O
// (vs 1 in the Geo structure)
void
neut_mesh3d_mesh2d (struct NODES Nodes, struct MESH Mesh3D,
		    struct MESH *pMesh2D, int ***pFacePoly,
		    int *pFaceQty, int verbosity)
{
  int i, j, elt3dqty, elset, dir;
  int *elt3d = ut_alloc_1d_int (2);
  struct MESH EltMesh2D;
  int *elset3d = ut_alloc_1d_int (2);
  char* progress = ut_alloc_1d_char (20);
  int facetqty;

  double** fodeq = NULL;
  int fodqty = 0;

  if ((*pMesh2D).EltType == NULL)
    (*pMesh2D).EltType = ut_alloc_1d_char (5);

  sprintf ((*pMesh2D).EltType, "%s", Mesh3D.EltType);

  if (strcmp ((*pMesh2D).EltType, "tri") == 0)
    facetqty = 4;
  else if (strcmp ((*pMesh2D).EltType, "quad") == 0)
    facetqty = 6;
  else
    abort ();

  neut_mesh_free (pMesh2D);
  
  (*pFaceQty) = 0;

  (*pMesh2D).Dimension = 2;
  (*pMesh2D).EltOrder = Mesh3D.EltOrder;
  (*pMesh2D).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh2D).EltType, "%s", Mesh3D.EltType);

  EltMesh2D = neut_mesh_alloc (2, (*pMesh2D).EltType,
			     (*pMesh2D).EltOrder, facetqty, 0);

  // for every 3D elt, for every of its facets, we look for all
  // its parent 3d elts (can be 1 or 2).
  // If it is 1, the facet is a 2D elt of a skin face - we record its
  // 3D elsets (Geo germs). 
  // If it is 2 and the 3D elt elsets are different, the facet is an
  // "internal" 2D elt - we record the 3D elsets (Geo germs).
  // In both cases, we check if this face exists (based on the germs);
  // if not, we add it (new elset).  Then, we add the element to the
  // elset.
  
  if (verbosity)
    ut_print_progress (stdout, 0, Mesh3D.EltQty, "%3.0f%%", progress);

  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    neut_mesh_elt_facetmesh (Mesh3D, i, &EltMesh2D);

    for (j = 1; j <= facetqty; j++)
    {
      neut_mesh_elt2delts3d (EltMesh2D, j, Mesh3D, elt3d, &elt3dqty);

      if (elt3d[0] >= i) // means the face has not been considered yet
      {
	if (elt3dqty == 1)
	{
	  // finding the outer surfaces by gathering all the surface
	  // elements that lie in the same plane in a given surface.
	  // All 2D elements of a given surface have the same normal
	  // (same sign). 

	  neut_mesh_skinelt_fod_b (Nodes, EltMesh2D, j, &fodeq,
	                                    &fodqty, &(elset3d[0]), &dir);
	  
	  if (dir == -1)
	    neut_mesh_elt_reversenodes (&EltMesh2D, j);

	  elset3d[1] = Mesh3D.EltElset[elt3d[0]];
	}
	else
	{
	  elset3d[0] = Mesh3D.EltElset[elt3d[0]];
	  elset3d[1] = Mesh3D.EltElset[elt3d[1]];
	}
	ut_array_1d_int_sort (elset3d, 2);

	if (elset3d[0] != elset3d[1]) // i.e. a boundary element
	{
	  elset = ut_array_2d_int_linepos (*pFacePoly, *pFaceQty, 2, elset3d);
	  elset = (elset == -1) ? -1 : elset + 1;

	  // elset = -1 means the elset has not been recorded yet.
	  if (elset == -1)
	  {
	    (*pFaceQty)++;
	    (*pFacePoly) = ut_realloc_2d_int_addline (*pFacePoly, (*pFaceQty), 2);
	    ut_array_1d_int_memcpy ((*pFacePoly)[(*pFaceQty) - 1], 2, elset3d);
	    elset = (*pFaceQty);

	    neut_mesh_addelset (pMesh2D, NULL, 0);
	  }

	  neut_mesh_addelt (pMesh2D, EltMesh2D.EltNodes[j]);
	  neut_mesh_elset_addelt (pMesh2D, elset, (*pMesh2D).EltQty);
	}
      }
    }
  
    if (verbosity)
      ut_print_progress (stdout, i, Mesh3D.EltQty, "%3.0f%%", progress);
  }

  neut_mesh_free (&EltMesh2D);
  ut_free_2d (fodeq, fodqty);
  ut_free_1d_int (elt3d);
  ut_free_1d_int (elset3d);
  ut_free_1d_char (progress);

  return;
}

// Generates the 1D mesh corresponding to a 2D mesh.  It also retuns the
// EdgeQty value and the EdgeFaceQty and EdgeFaceNb arrays, which hold
// the number of 1D elsets and their relations to the 2D elsets.  These
// terms are used in reference to the Geo structure, and closely match
// them, except that EdgeFaceQty andEdgeFaceNb are indexed O (vs 1 in
// the Geo structure)
void
neut_mesh2d_mesh1d (struct NODES Nodes, struct MESH Mesh2D,
		    struct MESH *pMesh1D, int ***pEdgeFaceNb,
		    int** pEdgeFaceQty, int *pEdgeQty, int verbosity)
{
  int i, j, k, elt2dqty, elset2dqty, elset;
  struct MESH EltMesh1D;
  int *elt2d = ut_alloc_1d_int (1000);
  int *elset2d = ut_alloc_1d_int (1000);
  char* progress = ut_alloc_1d_char (20);

  neut_mesh_set_zero (&EltMesh1D);
  Nodes.NodeQty = Nodes.NodeQty;

  if (Mesh2D.EltElset == NULL)
  {
    ut_print_message (2, 0, "neut_mesh2d_mesh1d needs Mesh2D.EltElset.\n");
    ut_error_reportbug ();
  }

  (*pEdgeQty) = 0;

  neut_mesh_free (pMesh1D);
  (*pMesh1D).Dimension = 1;
  (*pMesh1D).EltOrder = Mesh2D.EltOrder;
  (*pMesh1D).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh1D).EltType, "%s", Mesh2D.EltType);

  // Recording all 1d elts by looping on the edges of all 2d elts.

  if (verbosity)
    ut_print_progress (stdout, 0, Mesh2D.EltQty, "[1/2] %3.0f%%", progress);
  
  for (i = 1; i <= Mesh2D.EltQty; i++)
  {
    neut_mesh_elt_edgemesh (Mesh2D, i, &EltMesh1D);

    for (j = 1; j <= EltMesh1D.EltQty; j++)
    {
      neut_mesh_elt1delts2d   (EltMesh1D, j, Mesh2D, elt2d, &elt2dqty);
      neut_mesh_elt1delsets2d (EltMesh1D, j, Mesh2D, elset2d, &elset2dqty);

      // means not recorded yet
      if (elt2d[0] >= i)
	if (elset2dqty > 1)
	{
	  // elset2dqty > 1 means that the element is shared by several
	  // surfaces.  2 usually denotes an element which is on an edge
	  // of the domain.  However, for an hex mesh, it can also happen
	  // in the bulk and should not be taken as an acceptable
	  // solution.  elt2dqty > 2 means bulk.
	  if (elset2dqty == 2 && elt2dqty > 2)
	      continue;

	  neut_mesh_addelt (pMesh1D, EltMesh1D.EltNodes[j]);
	}
    }

    if (verbosity)
      ut_print_progress_nonl (stdout, i, Mesh2D.EltQty, "[1/2] %3.0f%%", progress);
  }

  neut_mesh_init_nodeelts (pMesh1D, Nodes.NodeQty);
  (*pMesh1D).EltElset = ut_alloc_1d_int ((*pMesh1D).EltQty + 1);

  // The principle is to loop on the 1d elements, and for each:
  // - if it has already been assigned to an edge, skip it.
  // - else, create a new elset and a new edge;
  // - look for the connected elements one by one, and for each:
  //   . if it belongs to the same edge (i.e. no changing faces),
  //     assigned it to the same elset/edge.
  //   . else, do nothing.
  // Set ElsetQty, EltElset, *pEdgeQty, *pEdgeFaceQty and *pEdgeFaceNb

  if (verbosity)
    ut_print_progress (stdout, 0, (*pMesh1D).EltQty, "[2/2] %3.0f%%", progress);

  elset = 0;
  for (i = 1; i <= (*pMesh1D).EltQty; i++)
  {
    if ((*pMesh1D).EltElset[i] == 0)
    {
      // adding elset to *pMesh1D and defining edge (*pEdge...) (topology)

      (*pMesh1D).ElsetQty = ++elset;
      (*pMesh1D).EltElset[i] = elset;
      // printf ("Add elt %d to elset %d\n", i, elset);

      neut_mesh_elt1delsets2d (*pMesh1D, i, Mesh2D, elset2d, &elset2dqty);

      (*pEdgeQty) = elset;
      (*pEdgeFaceQty) = ut_realloc_1d_int (*pEdgeFaceQty, *pEdgeQty);
      (*pEdgeFaceQty)[(*pEdgeQty) - 1] = elset2dqty;
      (*pEdgeFaceNb)
	= ut_realloc_2d_int_addline (*pEdgeFaceNb, *pEdgeQty, elset2dqty);
      ut_array_1d_int_memcpy ((*pEdgeFaceNb)[(*pEdgeQty) - 1], elset2dqty, elset2d);

      // loop on the neighbours

      int testqty, elt1d;
      int* test = ut_alloc_1d_int (1000);

      neut_mesh_elt_neighelts (*pMesh1D, i, test, &testqty);

      for (j = 0; j < testqty; j++)
      {
	// printf ("j = %d testqty = %d test = ", j, testqty);
	// ut_array_1d_int_fprintf (stdout, test, testqty, "%d");

	elt1d = test[j];
	// printf ("testing %d\n", elt1d);

	neut_mesh_elt1delsets2d (*pMesh1D, elt1d, Mesh2D, elset2d, &elset2dqty);

	// the elt has the same topology, therefore same edge -
	// recording it and adding non-registered neighbours to the test list.
	if (! ut_array_1d_int_diff (elset2d, elset2dqty,
	      (*pEdgeFaceNb)[elset - 1], (*pEdgeFaceQty)[elset - 1]))
	{
	  (*pMesh1D).EltElset[elt1d] = elset;
	  // printf ("testqty = %d test = ", testqty);
	  // ut_array_1d_int_fprintf (stdout, test, testqty, "%d");

	  // printf ("Add neigh elt %d to elset %d\n", elt1d, elset);
	  // printf ("(*pMesh1D).EltElset[%d] = %d\n", elt1d,
	      // (*pMesh1D).EltElset[elt1d]);

	  int* tmp = ut_alloc_1d_int (1000);
	  int tmpqty;
	  neut_mesh_elt_neighelts (*pMesh1D, elt1d, tmp, &tmpqty);
	  // If there is 2 neighbours, we are in the body of an edge;
	  // recording elt which is not the previous one.
	  // If there is more, we are done (end of an edge). 
	  // printf ("neut_mesh_elt_neighelts returns tmpqty = %d tmp = ", tmpqty);
	  // ut_array_1d_int_fprintf (stdout, tmp, tmpqty, "%d");
	  if (tmpqty == 2)
	    for (k = 0; k < tmpqty; k++)
	      if ((*pMesh1D).EltElset[tmp[k]] == 0)
	      {
		// printf ("  k = %d Add elt %d to test list\n", k, tmp[k]);

		if (tmp[k] == 0)
		  abort ();
		test[j] = tmp[k];
		j--;
	      }
	  ut_free_1d_int (tmp);
	}
      }

      ut_free_1d_int (test);
    }

    if (verbosity)
      ut_print_progress_nonl (stdout, i, (*pMesh1D).EltQty, "[2/2] %3.0f%%", progress);
  }

  if (verbosity)
    ut_print_progress (stdout, (*pMesh1D).EltQty, (*pMesh1D).EltQty, "%3.0f%%", progress);

  (*pMesh1D).ElsetQty = (*pEdgeQty);
  (*pMesh1D).Elsets = ut_alloc_2d_int ((*pEdgeQty) + 1, 1);

  for (i = 1; i <= (*pMesh1D).EltQty; i++)
    neut_mesh_elset_addelt (pMesh1D, (*pMesh1D).EltElset[i], i);

  neut_mesh_free (&EltMesh1D);
  ut_free_1d_int (elt2d);
  ut_free_1d_int (elset2d);
  ut_free_1d_char (progress);

  return;
}

// Generates the 0D mesh corresponding to a 1D mesh.  It also retuns the
// VerQty value and the VerEdgeQty and VerEdgeNb arrays, which hold the
// number of 0D elsets and their relations to the 1D elsets.  These
// terms are used in reference to the Geo structure, and closely match
// them, except that VerEdgeQty and VerEdgeNb are indexed O (vs 1 in the
// Geo structure)
void
neut_mesh1d_mesh0d (struct NODES Nodes, struct MESH Mesh1D,
		    struct MESH *pMesh0D, int ***pVerEdgeNb,
		    int** pVerEdgeQty, int *pVerQty, int verbosity)
{
  int i, j, elt1dqty, elset1dqty;
  struct MESH EltMesh0D;
  int *elt1d = ut_alloc_1d_int (1000);
  int *elset1d = ut_alloc_1d_int (1000);
  char* progress = ut_alloc_1d_char (20);

  neut_mesh_set_zero (&EltMesh0D);

  if (Mesh1D.EltElset == NULL)
  {
    ut_print_message (2, 0, "neut_mesh1d_mesh0d needs Mesh1D.EltElset.\n");
    ut_error_reportbug ();
  }

  (*pVerQty) = 0;

  neut_mesh_free (pMesh0D);
  (*pMesh0D).Dimension = 0;
  (*pMesh0D).EltOrder = Mesh1D.EltOrder;
  (*pMesh0D).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh0D).EltType, "%s", Mesh1D.EltType);

  // for every 1D elt, for every of its vertices, we look for all
  // its parent 1D elts (can be any number).

  if (verbosity)
    ut_print_progress (stdout, 0, Mesh1D.EltQty, "[1/2] %3.0f%%", progress);
  
  for (i = 1; i <= Mesh1D.EltQty; i++)
  {
    neut_mesh_elt_vermesh (Mesh1D, i, &EltMesh0D);
    
    for (j = 1; j <= 2; j++)
    {
      neut_mesh_elt0delts1d   (EltMesh0D, j, Mesh1D, elt1d, &elt1dqty);
      // elt1dqty == 1 means that an edge has a dead end.
      if (elt1dqty == 1)
	ut_error_reportbug ();

      neut_mesh_elt0delsets1d (EltMesh0D, j, Mesh1D, elset1d, &elset1dqty);

      // means not recorded yet
      if (elt1d[0] >= i)
      {
	// the following condition works for both conditions (above)
	if (elset1dqty > 1)
	  neut_mesh_addelt (pMesh0D, EltMesh0D.EltNodes[j]);
      }
    }

    if (verbosity)
      ut_print_progress_nonl (stdout, i, Mesh1D.EltQty, "[1/2] %3.0f%%", progress);
  }

  neut_mesh_init_nodeelts (pMesh0D, Nodes.NodeQty);

  (*pMesh0D).EltElset = ut_alloc_1d_int (Mesh1D.EltQty + 1);

  // The principle is to loop on the 0d elements, and for each:
  // - if it has already been assigned to an ver, skip it.
  // - else, create a new elset and a new ver;
  // - look for the connected elements one by one, and for each:
  //   . if it belongs to the same ver (i.e. no changing edges),
  //     assigned it to the same elset/ver.
  //   . else, do nothing.
  // Set ElsetQty, EltElset, *pVerQty, *pVerEdgeQty and *pVerEdgeNb

  (*pVerQty) = (*pMesh0D).EltQty;
  (*pVerEdgeQty) = ut_alloc_1d_int (*pVerQty);

  if (verbosity)
    ut_print_progress (stdout, 0, (*pMesh0D).EltQty, "[2/2] %3.0f%%", progress);

  for (i = 1; i <= (*pMesh0D).EltQty; i++)
  {
    (*pMesh0D).ElsetQty = i;
    (*pMesh0D).EltElset[i] = i;

    neut_mesh_elt0delsets1d (*pMesh0D, i, Mesh1D, elset1d, &elset1dqty);

    (*pVerEdgeNb)
      = ut_realloc_2d_int_addline (*pVerEdgeNb, i, elset1dqty);
    ut_array_1d_int_memcpy ((*pVerEdgeNb)[i - 1], elset1dqty, elset1d);
    (*pVerEdgeQty)[i - 1] = elset1dqty;

    if (verbosity)
      ut_print_progress_nonl (stdout, i, (*pMesh0D).EltQty, "[2/2] %3.0f%%", progress);
  }

  if (verbosity)
    ut_print_progress (stdout, (*pMesh0D).EltQty, (*pMesh0D).EltQty, "%3.0f%%", progress);

  (*pMesh0D).ElsetQty = (*pVerQty);
  (*pMesh0D).Elsets = ut_alloc_2d_int ((*pVerQty) + 1, 1);

  for (i = 1; i <= (*pMesh0D).EltQty; i++)
    neut_mesh_elset_addelt (pMesh0D, (*pMesh0D).EltElset[i], i);

  neut_mesh_free (&EltMesh0D);
  ut_free_1d_int (elt1d);
  ut_free_1d_int (elset1d);
  ut_free_1d_char (progress);

  return;
}

void
neut_mesh_addelt (struct MESH *pMesh, int *EltNodes)
{
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
				     (*pMesh).EltOrder);

  (*pMesh).EltQty++;

  (*pMesh).EltNodes =
    ut_realloc_1d_pint ((*pMesh).EltNodes, (*pMesh).EltQty + 1);
  (*pMesh).EltNodes[(*pMesh).EltQty] = ut_alloc_1d_int (eltnodeqty);
  (*pMesh).EltNodes[0] = NULL; /* mandatory */

  ut_array_1d_int_memcpy ((*pMesh).EltNodes[(*pMesh).EltQty], eltnodeqty,
			  EltNodes);

  /* update NodeElts */
  /* if ((*pMesh).NodeElts != NULL) */

  return;
}

void
neut_mesh_addelset (struct MESH *pMesh, int *ElsetElts, int EltQty)
{
  int i;

  (*pMesh).ElsetQty++;

  (*pMesh).Elsets =
    ut_realloc_1d_pint ((*pMesh).Elsets, (*pMesh).ElsetQty + 1);
  (*pMesh).Elsets[0] = NULL; /* mandatory */

  (*pMesh).Elsets[(*pMesh).ElsetQty] = ut_alloc_1d_int (EltQty + 1);
  (*pMesh).Elsets[(*pMesh).ElsetQty][0] = EltQty;
  if (EltQty == (*pMesh).EltQty || ElsetElts == NULL)
    for (i = 1; i <= EltQty; i++)
      (*pMesh).Elsets[(*pMesh).ElsetQty][i] = i;
  else
    ut_array_1d_int_memcpy ((*pMesh).Elsets[(*pMesh).ElsetQty] + 1, EltQty,
			  ElsetElts);

  return;
}

void
neut_mesh_elset_addelt (struct MESH *pMesh, int elset_nb, int elt)
{
  (*pMesh).Elsets[elset_nb][0]++;

  (*pMesh).Elsets[elset_nb] = ut_realloc_1d_int
    ((*pMesh).Elsets[elset_nb], (*pMesh).Elsets[elset_nb][0] + 1);

  (*pMesh).Elsets[elset_nb][(*pMesh).Elsets[elset_nb][0]] = elt;

  return;
}

/* return the number of the FoD: x0=-1, x1=-2, y0=-3, y1=-4,
 * z0=-5, z1=-6, -7 otherwise. */
int
neut_mesh_skinelt_fod (struct NODES Nodes, struct MESH Mesh, int elt,
                       int* pfod)
{
  int i, eq, status;
  double *n = ut_alloc_1d (3);
  double *nodecoo;

  status = 0;

  if (Nodes.BBox == NULL)
  {
    printf ("_neut_mesh_skinelt_fod needs Nodes.BBox\n");
    printf ("use neut_nodes_init_boundingbox ()\n");
    ut_error_reportbug ();
  }

  neut_mesh_eltnormal (Mesh, Nodes, elt, n);
  nodecoo = Nodes.NodeCoo[Mesh.EltNodes[elt][0]];

  (*pfod) = -7;
  for (i = 0; i < 3; i++)
  {
    eq = 0;
    if (ut_num_equal (n[i], 1, 1e-8))
      eq = 1;
    else if (ut_num_equal (n[i], -1, 1e-8))
      eq = 2;

    if (eq != 0)
    {
      if (ut_num_equal (nodecoo[i], Nodes.BBox[i][0], 1e-8))
      {
	(*pfod) = - (2 * i + 1);

	// NOTE: 21 Apr 2010 -- (eq == 2) changed to (eq == 1)
	if (eq == 1)
	  status = -1;

	break;
      }
      else if (ut_num_equal (nodecoo[i], Nodes.BBox[i][1], 1e-8))
      {
	(*pfod) = - (2 * i + 2);

	if (eq == 2)
	  status = -1;

	// NOTE: 21 Apr 2010 -- below commented
	// if (eq == 1)
	  // neut_mesh_elt_reversenodes (pMesh, elt);

	break;
      }
    }
  }

  ut_free_1d (n);

  return status;
}

int
neut_mesh_skinelt_fod_b (struct NODES Nodes, struct MESH Mesh, int elt,
                         double*** pfodeq, int* pfodqty, int* pfod,
			 int* pdir)
{
  int i, status;
  double *eq = ut_alloc_1d (4);
  int found = 0;

  status = 0;

  neut_mesh_elteq (Mesh, Nodes, elt, eq);

  (*pdir) = 1;
  for (i = 0; i < (*pfodqty) && found == 0; i++)
    if (ut_space_planes_equal ((*pfodeq)[i], eq) == 1)
    {
      (*pfod) = - i - 1;
      found = 1;

      if (ut_vector_scalprod (eq + 1, (*pfodeq)[i] + 1) > 0)
	(*pdir) = 1;
      else
	(*pdir) = -1;
    }

  // adding fod
  if (found == 0)
  {
    (*pfodeq) = ut_realloc_2d_addline ((*pfodeq), ++(*pfodqty), 4);
    ut_array_1d_memcpy ((*pfodeq)[(*pfodqty) - 1], 4, eq);
    (*pfod) = -(*pfodqty);
  }

  ut_free_1d (eq);

  return status;
}

void
neut_skinelsets_fod (int **Elsets, int ElsetQty, int ***pFoD)
{
  int i;
  int *qty = ut_alloc_1d_int (7);

  /* for alloc */
  for (i = 0; i < ElsetQty; i++)
    if (Elsets[i][0] < 0 && Elsets[i][0] > -7)
      qty[-Elsets[i][0]]++;

  (*pFoD) = ut_alloc_1d_pint (7);
  for (i = 1; i <= 6; i++)
    (*pFoD)[i] = ut_alloc_1d_int (qty[i] + 1);

  /* recording data */
  for (i = 0; i < ElsetQty; i++)
    if (Elsets[i][0] < 0 && Elsets[i][0] > -7)
      (*pFoD)[-Elsets[i][0]][++(*pFoD)[-Elsets[i][0]][0]] = i + 1;

  ut_free_1d_int (qty);

  return;
}

int
neut_mesh_eltlength (struct NODES Nodes, struct MESH Mesh, int elt,
		     double *plength)
{
  int node1, node2;

  if (Mesh.Dimension != 1)
    return -1;

  node1 = Mesh.EltNodes[elt][0];
  node2 = Mesh.EltNodes[elt][1];

  (*plength) = ut_space_dist (Nodes.NodeCoo[node1], Nodes.NodeCoo[node2]);

  return 0;
}


void
neut_mesh_order1to2 (struct NODES *pNodes, struct MESH* pMesh1D,
                     struct MESH *pMesh2D, struct MESH *pMesh3D)
{
  int i, j, k, pos1, pos2, pos, eltnb, nodenb, qty;
  int **fir3 = ut_alloc_2d_int (10, 2);
  int **sec3 = ut_alloc_2d_int (4, 4);
  int **fir2 = ut_alloc_2d_int (6, 2);
  int **sec2 = ut_alloc_2d_int (3, 3);
  double *coo = ut_alloc_1d (3);
  int *eltlist = ut_alloc_1d_int (1000);
  int *inter = NULL;
  int qty_bak = (*pNodes).NodeQty;
  int node2min = INT_MAX, node2max = INT_MIN;
  int node1min = INT_MAX, node1max = INT_MIN;
  int* nodes = ut_alloc_1d_int (2);

  sec3[0][1] = sec3[1][0] = 4;
  sec3[1][2] = sec3[2][1] = 5;
  sec3[2][0] = sec3[0][2] = 6;
  sec3[3][0] = sec3[0][3] = 7;
  sec3[2][3] = sec3[3][2] = 8;
  sec3[3][1] = sec3[1][3] = 9;

  fir3[4][0] = 0;
  fir3[4][1] = 1;
  fir3[5][0] = 1;
  fir3[5][1] = 2;
  fir3[6][0] = 2;
  fir3[6][1] = 0;
  fir3[7][0] = 3;
  fir3[7][1] = 0;
  fir3[8][0] = 2;
  fir3[8][1] = 3;
  fir3[9][0] = 3;
  fir3[9][1] = 1;

  sec2[0][1] = sec2[1][0] = 3;
  sec2[1][2] = sec2[2][1] = 4;
  sec2[2][0] = sec2[0][2] = 5;

  fir2[3][0] = 0;
  fir2[3][1] = 1;
  fir2[4][0] = 1;
  fir2[4][1] = 2;
  fir2[5][0] = 2;
  fir2[5][1] = 0;

/*----------------------------------------------------------------------
 * numbering: 
 * nodes 0 1 2 3 at the vertices
 * 4: 0 - 1
 * 5: 1 - 2
 * 6: 2 - 0
 * 7: 3 - 0
 * 8: 2 - 3
 * 9: 3 - 1
 *
 * algorithm:
 * loop over the elements; creating the second order nodes, and, for
 * each, copying it to the other elements that share the element edge.
 * */

  /* switching to order 2 and initializing temporary variables */
  (*pMesh3D).EltOrder = 2;
  
  if (pMesh2D != NULL)
  {
    (*pMesh2D).EltOrder = 2;
    node2min = INT_MAX;
    node2max = INT_MIN;

    for (i = 1; i <= (*pMesh2D).EltQty; i++)
      for (j = 0; j < 3; j++)
      {
	node2min = ut_num_min (node2min, (*pMesh2D).EltNodes[i][j]);
	node2max = ut_num_max (node2max, (*pMesh2D).EltNodes[i][j]);
      }
  }
  
  if (pMesh1D != NULL)
  {
    (*pMesh1D).EltOrder = 2;
    node1min = INT_MAX;
    node1max = INT_MIN;

    for (i = 1; i <= (*pMesh1D).EltQty; i++)
      for (j = 0; j < 2; j++)
      {
	node1min = ut_num_min (node1min, (*pMesh1D).EltNodes[i][j]);
	node1max = ut_num_max (node1max, (*pMesh1D).EltNodes[i][j]);
      }
  }

  /* (re)allocating EltNodes  */
  for (i = 1; i <= (*pMesh3D).EltQty; i++)
  {
    (*pMesh3D).EltNodes[i] = ut_realloc_1d_int ((*pMesh3D).EltNodes[i], 10);
    for (j = 4; j < 10; j++)
      (*pMesh3D).EltNodes[i][j] = 0;
  }

  if (pMesh2D != NULL)
    for (i = 1; i <= (*pMesh2D).EltQty; i++)
    {
      (*pMesh2D).EltNodes[i] = ut_realloc_1d_int ((*pMesh2D).EltNodes[i], 6);
      for (j = 3; j < 6; j++)
	(*pMesh2D).EltNodes[i][j] = 0;
    }

  if (pMesh1D != NULL)
    for (i = 1; i <= (*pMesh1D).EltQty; i++)
    {
      (*pMesh1D).EltNodes[i] = ut_realloc_1d_int ((*pMesh1D).EltNodes[i], 3);
      (*pMesh1D).EltNodes[i][2] = 0;
    }

  /* for every element */
  for (i = 1; i <= (*pMesh3D).EltQty; i++)
  {
    /* for every second order node */
    for (j = 4; j < 10; j++)
      /* if not recorded yet */
      if ((*pMesh3D).EltNodes[i][j] == 0)
      {
	/* recording corresponding first order nodes */
	nodes[0] = (*pMesh3D).EltNodes[i][fir3[j][0]];
	nodes[1] = (*pMesh3D).EltNodes[i][fir3[j][1]];

	/* searching common elements between the two */
	neut_nodes_commonelts (*pMesh3D, nodes, 2, eltlist, &qty);

	/* if the first element is >= i, that means that the node at
	 * this position has not been recorded through another elements
	 * before; recording it in 3D mesh, so as lower dimension meshes */
	if (eltlist[0] < i)
	  ut_error_reportbug ();

	for (k = 0; k < 3; k++)
	  coo[k] = .5 * ((*pNodes).NodeCoo[nodes[0]][k]
		       + (*pNodes).NodeCoo[nodes[1]][k]);

	neut_nodes_addnode (pNodes, coo, -1);

	nodenb = (*pNodes).NodeQty;

	/* adding node to 3D mesh */
	(*pMesh3D).EltNodes[i][j] = nodenb;

	for (k = 1; k < qty; k++)
	{
	  eltnb = eltlist[k];
	  pos1=ut_array_1d_int_eltpos ((*pMesh3D).EltNodes[eltnb], 4, nodes[0]);
	  pos2=ut_array_1d_int_eltpos ((*pMesh3D).EltNodes[eltnb], 4, nodes[1]);
	  pos = sec3[pos1][pos2];
	  (*pMesh3D).EltNodes[eltnb][pos] = nodenb;
	}

	/* adding node to 2D mesh if needed */
	if (pMesh2D != NULL)
	  if (nodes[0] >= node2min && nodes[0] <= node2max &&
	      nodes[1] >= node2min && nodes[1] <= node2max   )
	  {
	    /* searching common elts between the two */
	    neut_nodes_commonelts (*pMesh2D, nodes, 2, eltlist, &qty);

	    /* note: qty may be zero */
	    for (k = 0; k < qty; k++)
	    {
	      eltnb = eltlist[k];
	      if (ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[eltnb], 6,
					     nodenb) == -1)
	      {
		pos1 =
		  ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[eltnb], 3, nodes[0]);
		pos2 =
		  ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[eltnb], 3, nodes[1]);
		pos = sec2[pos1][pos2];
		(*pMesh2D).EltNodes[eltnb][pos] = nodenb;
	      }
	    }
	  }
	/* adding node to 1D mesh if needed */
	if (pMesh1D != NULL)
	  if (nodes[0] >= node1min && nodes[0] <= node1max &&
	      nodes[1] >= node1min && nodes[1] <= node1max   )
	  {
	    /* searching common elts between the two */
	    neut_nodes_commonelts (*pMesh1D, nodes, 2, eltlist, &qty);

	    for (k = 0; k < qty; k++)
	    {
	      eltnb = eltlist[k];
	      if (ut_array_1d_int_eltpos ((*pMesh1D).EltNodes[eltnb], 3,
					     nodenb) == -1)
	      (*pMesh1D).EltNodes[eltnb][2] = nodenb;
	    }
	  }
      }
  }

  for (i = 1; i <= (*pMesh3D).EltQty; i++)
  {
    for (j = 0; j < 10; j++)
      if ((*pMesh3D).EltNodes[i][j] <= 0)
      {
	printf ("(*pMesh3D).EltNodes[i][j] = %d <= 0\n",
		(*pMesh3D).EltNodes[i][j]);
	abort ();
      }
  }

  if (pMesh2D != NULL)
  for (i = 1; i <= (*pMesh2D).EltQty; i++)
  {
    for (j = 0; j < 6; j++)
    {
      if ((*pMesh2D).EltNodes[i][j] <= 0)
      {
	printf ("(*pMesh2D).EltNodes[i][j] = %d <= 0\n",
		(*pMesh2D).EltNodes[i][j]);
	printf ("this can be due to a 3D mesh that does not match its 2D mesh boundary\n");
	ut_error_reportbug ();

	abort ();
      }
    }
  }

  if (pMesh1D != NULL)
  for (i = 1; i <= (*pMesh1D).EltQty; i++)
  {
    for (j = 0; j < 3; j++)
    {
      if ((*pMesh1D).EltNodes[i][j] <= 0)
      {
	printf ("(*pMesh1D).EltNodes[i][j] = %d <= 0\n",
		(*pMesh1D).EltNodes[i][j]);
	ut_error_reportbug ();
	abort ();
      }
    }
  }

  /* reinitializing 3D NodeElts */
  if ((*pMesh3D).NodeElts != NULL)
    ut_free_2d_int ((*pMesh3D).NodeElts, qty_bak + 1);
  neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);

  /* reinitializing 2D NodeElts */
  if (pMesh2D != NULL)
  {
    if ((*pMesh2D).NodeElts != NULL)
      ut_free_2d_int ((*pMesh2D).NodeElts, node2max + 1);
    neut_mesh_init_nodeelts (pMesh2D, (*pNodes).NodeQty);
  }

  /* reinitializing 1D NodeElts */
  if (pMesh1D != NULL)
  {
    if ((*pMesh1D).NodeElts != NULL)
      ut_free_2d_int ((*pMesh1D).NodeElts, node1max + 1);
    neut_mesh_init_nodeelts (pMesh1D, (*pNodes).NodeQty);
  }

  ut_free_2d_int (fir3, 10);
  ut_free_2d_int (sec3, 4);
  ut_free_2d_int (fir2, 6);
  ut_free_2d_int (sec2, 3);
  ut_free_1d (coo);
  ut_free_1d_int (inter);
  ut_free_1d_int (eltlist);
  ut_free_1d_int (nodes);

  return;
}

void
neut_mesh_elsets_nodes (struct MESH Mesh, int *elsets,
			int elsetqty, int **pnodes, int *pnodeqty)
{
  int i, elset, nodeqty;
  int* nodes = NULL;
  
  (*pnodes) = ut_alloc_1d_int (1);

  (*pnodeqty) = 0;
  for (i = 0; i < elsetqty; i++)
  {
    elset = elsets[i];
    neut_mesh_elset_nodes (Mesh, elset, &nodes, &nodeqty);
    (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty) + nodeqty);
    ut_array_1d_int_memcpy ((*pnodes) + (*pnodeqty), nodeqty, nodes);
    ut_free_1d_int (nodes);

    (*pnodeqty) += nodeqty;
  }

  ut_array_1d_int_sort_uniq ((*pnodes), (*pnodeqty), &(*pnodeqty));
  (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty));

  return;
}

/* neut_mesh_3d_rr calculates the mean, min and max radius ratios of the
 * elements of a 3D mesh */
int
neut_mesh_elset_3d_rr (struct NODES Nodes, struct MESH Mesh, int elset, double* prrmean, double* prrmin, double* prrmax)
{
  int i, elt;
  double rr;
  double* p1 = NULL;
  double* p2 = NULL;
  double* p3 = NULL;
  double* p4 = NULL;

  (*prrmin) =  1e32;
  (*prrmax) = -1e32;
  (*prrmean) = 0;
  
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++) 
  {
    elt = Mesh.Elsets[elset][i]; 
    p1 = Nodes.NodeCoo[Mesh.EltNodes[elt][0]];
    p2 = Nodes.NodeCoo[Mesh.EltNodes[elt][1]];
    p3 = Nodes.NodeCoo[Mesh.EltNodes[elt][2]];
    p4 = Nodes.NodeCoo[Mesh.EltNodes[elt][3]];

    rr = neut_elt_3d_rr (p1, p2, p3, p4);

    (*prrmin) = ut_num_min ((*prrmin), rr);
    (*prrmax) = ut_num_max ((*prrmax), rr);
    (*prrmean) += rr;
  }
  (*prrmean) /= Mesh.Elsets[elset][0];

  return 0;
}

/* neut_mesh_3d_rr calculates the mean, min and max radius ratios of the
 * elements of a 3D mesh */
int
neut_mesh_3d_elt_rr (struct NODES Nodes, struct MESH Mesh, int id, double* prr)
{
  (*prr) = neut_elt_3d_rr (
                Nodes.NodeCoo[Mesh.EltNodes[id][0]],
                Nodes.NodeCoo[Mesh.EltNodes[id][1]],
                Nodes.NodeCoo[Mesh.EltNodes[id][2]],
                Nodes.NodeCoo[Mesh.EltNodes[id][3]]);

  return 0;
}

/* neut_mesh_3d_rr calculates the mean, min and max radius ratios of the
 * elements of a 3D mesh */
int
neut_mesh_3d_rr (struct NODES Nodes, struct MESH Mesh, double* prrmean, double* prrmin, double* prrmax)
{
  int i;
  double rr;
  double* p1 = NULL;
  double* p2 = NULL;
  double* p3 = NULL;
  double* p4 = NULL;
  double min, max, mean;

  min =  1e32;
  max = -1e32;
  mean = 0;
  
  for (i = 1; i <= Mesh.EltQty; i++) 
  {
    p1 = Nodes.NodeCoo[Mesh.EltNodes[i][0]];
    p2 = Nodes.NodeCoo[Mesh.EltNodes[i][1]];
    p3 = Nodes.NodeCoo[Mesh.EltNodes[i][2]];
    p4 = Nodes.NodeCoo[Mesh.EltNodes[i][3]];

    rr = neut_elt_3d_rr (p1, p2, p3, p4);

    min = ut_num_min (min, rr);
    max = ut_num_max (max, rr);
    mean += rr;
  }
  mean /= Mesh.EltQty;

  if (prrmin != NULL)
    (*prrmin) = min;

  if (prrmax != NULL)
    (*prrmax) = max;

  if (prrmean != NULL)
    (*prrmean) = mean;
  
  return 0;
}

int
neut_mesh_3d_rr2 (struct NODES Nodes, struct MESH Mesh, char* expr, double* prr)
{
  int i;
  double* p1 = NULL;
  double* p2 = NULL;
  double* p3 = NULL;
  double* p4 = NULL;
  double* rr = ut_alloc_1d (Mesh.EltQty + 1);
  double tmp = 1;

  for (i = 1; i <= Mesh.EltQty; i++) 
  {
    p1 = Nodes.NodeCoo[Mesh.EltNodes[i][0]];
    p2 = Nodes.NodeCoo[Mesh.EltNodes[i][1]];
    p3 = Nodes.NodeCoo[Mesh.EltNodes[i][2]];
    p4 = Nodes.NodeCoo[Mesh.EltNodes[i][3]];

    rr[i] = neut_elt_3d_rr (p1, p2, p3, p4);
  }
    
  int var_qty = 4;
  char** vars = ut_alloc_2d_char (var_qty, 10);
  double* vals = ut_alloc_1d (var_qty);

  sprintf (vars[0], "dis");
  sprintf (vars[1], "dismin");
  sprintf (vars[2], "dismax");
  sprintf (vars[3], "dismean");
  vals[1] = ut_array_1d_min (rr + 1, Mesh.EltQty);
  vals[2] = ut_array_1d_max (rr + 1, Mesh.EltQty);
  vals[3] = ut_array_1d_mean (rr + 1, Mesh.EltQty);

  (*prr) = 1;
  for (i = 1; i <= Mesh.EltQty; i++) 
  {
    vals[0] = rr[i];

    if (strcmp (expr, "dis^(exp((dis^0.1)/(dis^0.1-1)))") == 0)
    {
      if (rr[i] < 1)
	tmp = pow (rr[i], exp (pow (rr[i], 0.1) / (pow (rr[i], 0.1) - 1)));
      else
	tmp = 1;
    }
    else
    {
#ifdef HAVE_LIBMATHEVAL
      if (rr[i] < 1)
	ut_math_eval (expr, var_qty, vars, vals, &tmp);
      else
	tmp = 1;
#else
      ut_print_message (2, 2, "This capability requires libmatheval.");
#endif
    }
    (*prr) *= tmp;
  }
  
  ut_free_1d (rr);
  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);

  return 0;
}

int
neut_mesh_3d_rr2_array (double* rr, int qty, char* expr, double* prr)
{
  int i;
  int var_qty = 4;
  double tmp = 1;
  char** vars = ut_alloc_2d_char (var_qty, 10);
  double* vals = ut_alloc_1d (var_qty);

  sprintf (vars[0], "dis");
  sprintf (vars[1], "dismin");
  sprintf (vars[2], "dismax");
  sprintf (vars[3], "dismean");
  vals[1] = ut_array_1d_min (rr + 1, qty);
  vals[2] = ut_array_1d_max (rr + 1, qty);
  vals[3] = ut_array_1d_mean (rr + 1, qty);

  (*prr) = 1;
  for (i = 1; i <= qty; i++) 
  {
    vals[0] = rr[i];

    if (strcmp (expr, "dis^(exp((dis^0.1)/(dis^0.1-1)))") == 0)
      tmp = pow (rr[i], exp (pow (rr[i], 0.1) / (pow (rr[i], 0.1) - 1)));
    else
    {
#ifdef HAVE_LIBMATHEVAL
      ut_math_eval (expr, var_qty, vars, vals, &tmp);
#else
      ut_print_message (2, 2, "This capability requires libmatheval.");
#endif
    }
    (*prr) *= tmp;
  }
  
  ut_free_2d_char (vars, 1);
  ut_free_1d (vals);

  return 0;
}

/* neut_mesh_2d_rr calculates the mean, min and max radius ratios of the
 * elements of a 2D mesh */
int
neut_mesh_2d_rr (struct NODES Nodes, struct MESH Mesh, double* prrmean, double* prrmin, double* prrmax)
{
  int i;
  double rr;
  double* p1 = NULL;
  double* p2 = NULL;
  double* p3 = NULL;

  (*prrmin) =  1e32;
  (*prrmax) = -1e32;
  (*prrmean) = 0;
  
  for (i = 1; i <= Mesh.EltQty; i++) 
  {
    p1 = Nodes.NodeCoo[Mesh.EltNodes[i][0]];
    p2 = Nodes.NodeCoo[Mesh.EltNodes[i][1]];
    p3 = Nodes.NodeCoo[Mesh.EltNodes[i][2]];

    rr = neut_elt_2d_rr (p1, p2, p3);

    (*prrmin) = ut_num_min ((*prrmin), rr);
    (*prrmax) = ut_num_max ((*prrmax), rr);
    (*prrmean) += rr;
  }
  (*prrmean) /= Mesh.EltQty;

  return 0;
}

void
neut_mesh_mesh (struct MESH Old, struct MESH* pNew)
{
  int i, eltnodeqty;

  neut_mesh_free (pNew);

  (*pNew).Dimension = Old.Dimension;
  (*pNew).EltOrder = Old.EltOrder;
  (*pNew).EltQty = Old.EltQty;
  (*pNew).EltType = ut_alloc_1d_char (5);
  sprintf ((*pNew).EltType, "%s", Old.EltType);
  
  eltnodeqty = neut_elt_nodeqty ((*pNew).EltType, (*pNew).Dimension, (*pNew).EltOrder);

  (*pNew).EltNodes = ut_alloc_2d_int (Old.EltQty + 1, eltnodeqty);

  for (i = 1; i <= Old.EltQty; i++)
    ut_array_1d_int_memcpy ((*pNew).EltNodes[i], eltnodeqty, Old.EltNodes[i]);

  (*pNew).ElsetQty = Old.ElsetQty;

  if (Old.Elsets != NULL)
  {
    (*pNew).Elsets = ut_alloc_1d_pint (Old.ElsetQty + 1);
    for (i = 1; i <= Old.ElsetQty; i++)
    {
      (*pNew).Elsets[i] = ut_alloc_1d_int (Old.Elsets[i][0] + 1);
      ut_array_1d_int_memcpy ((*pNew).Elsets[i], Old.Elsets[i][0] + 1,
	  Old.Elsets[i]);
    }
  }
  
  if (Old.EltElset != NULL)
  {
    (*pNew).EltElset = ut_alloc_1d_int (Old.EltQty + 1);
    ut_array_1d_int_memcpy ((*pNew).EltElset + 1, Old.EltQty, Old.EltElset + 1);
  }

  return;
}

void
neut_mesh_poly_skinmesh (struct GEO Geo, int poly, struct MESH Mesh2D,
    struct MESH* pSkin)
{
  int f, i, j, face, elt;
  int* nodes = ut_alloc_1d_int (3);
  int** seq = ut_alloc_2d_int (2, 3);
  int ori;
  int* elts = NULL;

  seq[0][0] = 0;
  seq[0][1] = 1;
  seq[0][2] = 2;
  seq[1][0] = 0;
  seq[1][1] = 2;
  seq[1][2] = 1;

  (*pSkin).Dimension = Mesh2D.Dimension;
  (*pSkin).EltOrder = Mesh2D.EltOrder;
  (*pSkin).EltType = ut_alloc_1d_char (5);
  sprintf ((*pSkin).EltType, "%s", Mesh2D.EltType);

  for (f = 1; f <= Geo.PolyFaceQty[poly]; f++)
  {
    face = Geo.PolyFaceNb[poly][f];
    ori = (Geo.PolyFaceOri[poly][f] == 1) ? 0 : 1;

    for (i = 1; i <= Mesh2D.Elsets[face][0]; i++)
    {
      elt = Mesh2D.Elsets[face][i];
      
      for (j = 0; j < 3; j++)
	nodes[j] = Mesh2D.EltNodes[elt][seq[ori][j]];

      neut_mesh_addelt (pSkin, nodes);
    }
  }

  elts = ut_alloc_1d_int ((*pSkin).EltQty);
  for (i = 0; i < (*pSkin).EltQty; i++)
    elts[i] = i + 1;
  
  neut_mesh_addelset (pSkin, elts, (*pSkin).EltQty);
  
  neut_mesh_init_eltelset (pSkin, NULL);

  /*----------------------------------------------------------------------
   * foot */

  ut_free_1d_int (nodes);
  ut_free_1d_int (elts);
  ut_free_2d_int (seq, 2);

  return;
}

void
neut_mesh_face_skinmesh (struct GEO Geo, int face, struct MESH Mesh1D,
    struct MESH* pSkin)
{
  int e, i, j, edge, elt;
  int* nodes = ut_alloc_1d_int (3);
  int** seq = ut_alloc_2d_int (2, 2);
  int ori;
  int* elts = NULL;

  seq[0][0] = 0;
  seq[0][1] = 1;
  seq[1][0] = 1;
  seq[1][1] = 0;

  (*pSkin).Dimension = Mesh1D.Dimension;
  (*pSkin).EltOrder = Mesh1D.EltOrder;
  (*pSkin).EltType = ut_alloc_1d_char (5);
  sprintf ((*pSkin).EltType, "%s",  Mesh1D.EltType);

  for (e = 1; e <= Geo.FaceVerQty[face]; e++)
  {
    edge = Geo.FaceEdgeNb[face][e];
    ori = (Geo.FaceEdgeOri[face][e] == 1) ? 0 : 1;

    for (i = 1; i <= Mesh1D.Elsets[edge][0]; i++)
    {
      elt = Mesh1D.Elsets[edge][i];
      
      for (j = 0; j < 2; j++)
	nodes[j] = Mesh1D.EltNodes[elt][seq[ori][j]];

      neut_mesh_addelt (pSkin, nodes);
    }
  }

  elts = ut_alloc_1d_int ((*pSkin).EltQty);
  for (i = 0; i < (*pSkin).EltQty; i++)
    elts[i] = i + 1;
  
  neut_mesh_addelset (pSkin, elts, (*pSkin).EltQty);
  
  neut_mesh_init_eltelset (pSkin, NULL);

  /*----------------------------------------------------------------------
   * foot */

  ut_free_1d_int (nodes);
  ut_free_1d_int (elts);
  ut_free_2d_int (seq, 2);

  return;
}

void
neut_mesh_mergeelsets (struct MESH* pM)
{
  int i, tot;

  tot = 0;
  for (i = 1; i <= (*pM).ElsetQty; i++)
    tot += (*pM).Elsets[i][0];

  (*pM).Elsets[1] = ut_realloc_1d_int ((*pM).Elsets[1], tot + 1);

  for (i = 2; i <= (*pM).ElsetQty; i++)
  {
    ut_array_1d_int_memcpy ((*pM).Elsets[1] + (*pM).Elsets[1][0] + 1,
	(*pM).Elsets[i][0], (*pM).Elsets[i] + 1);
    (*pM).Elsets[1][0] += (*pM).Elsets[i][0];
    ut_free_1d_int ((*pM).Elsets[i]);
  }

  if ((*pM).Elsets[1][0] != tot)
    abort ();

  ut_array_1d_int_sort ((*pM).Elsets[1] + 1, (*pM).Elsets[1][0]);

  (*pM).ElsetQty = 1;

  return;
}

/* returns 1 if meshes match, 0 else */
// TODO: this is brute force method; get less computer intensive
int
neut_mesh_mesh_match (struct NODES N1, struct MESH M1, struct NODES N2, struct MESH M2)
{
  int i, j, k, l;
  int res;
  int eltnodeqty = neut_elt_nodeqty (M1.EltType, M1.Dimension, M1.EltOrder);
  double* dist = ut_alloc_1d (eltnodeqty);
  double min;

  res = 0;

  if (M1.EltQty != M2.EltQty)
    res = 0;
  else
    // loop over M1 elts; for each, loop over M2 to search a matching elt
    for (i = 1; i <= M1.EltQty; i++)
    {
      res = 0;
      for (j = 1; j <= M2.EltQty; j++)
      {
	res = 1;
	for (k = 0; k < eltnodeqty; k++) // loop over M1 elt nodes
	{
	  for (l = 0; l < eltnodeqty; l++) // loop over M2 elt nodes
	    dist[l] = ut_space_dist (N1.NodeCoo[M1.EltNodes[i][k]],
		N2.NodeCoo[M2.EltNodes[j][l]]);
	
	  min = ut_array_1d_min (dist, eltnodeqty);
	  if (min > 1e-9)
	  {
	    res = 0;
	    break;
	  }
	}

	if (res == 1)
	  break;
      }

      if (res == 0)
	break;
    }
	
  ut_free_1d (dist);

  return res;
}

int
neut_mesh_elset_averagecl (struct MESH Mesh, struct NODES Nodes, int face, double* pacl)
{
  int i, elt, node;

  (*pacl) = 0;
  for (i = 1; i <= Mesh.Elsets[face][0]; i++)
  {
    elt = Mesh.Elsets[face][i];
    node = Mesh.EltNodes[elt][0];

    (*pacl) += Nodes.NodeCl[node];
  }
  (*pacl) /= Mesh.Elsets[face][0];

  return 0;
}

int
neut_mesh_face_overrefinement (struct GEO Geo, int face, struct NODES
    Nodes, struct MESH Mesh1D, struct NODES N, struct MESH M)
{
  int i, j;
  int res;
  int nb, qty, edge;
  int *nodes = ut_alloc_1d_int (3);
  double per, minper;		/* minimum perimeter for a 2D element */
  double *eltlength;
  double *length;
  int elt;

  qty = 0;
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];
    qty += Mesh1D.Elsets[edge][0];
  }

  length = ut_alloc_1d (qty + 2);

  nb = 0;
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];

    eltlength = ut_alloc_1d (Mesh1D.Elsets[edge][0] + 1);

    for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
    {
      elt = Mesh1D.Elsets[edge][j];
      neut_mesh_eltlength (Nodes, Mesh1D, elt, &(eltlength[j]));
    }

    if (Geo.FaceEdgeOri[face][i] == 1)
      for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
	length[++nb] = eltlength[j];
    else
      for (j = Mesh1D.Elsets[edge][0]; j >= 1; j--)
	length[++nb] = eltlength[j];

    ut_free_1d (eltlength);
  }

  /* Calculating the minimum allowed perimeter */
  minper = DBL_MAX;
  for (i = 1; i < qty; i++)
    minper = ut_num_min (minper, length[i] + length[i + 1]);
  minper = ut_num_min (minper, length[qty] + length[1]);

  res = 1;

  for (i = 1; i <= M.EltQty; i++)
  {
    for (j = 0; j < 3; j++)
      nodes[j] = M.EltNodes[i][j];

    per = ut_space_dist (N.NodeCoo[nodes[0]], N.NodeCoo[nodes[1]])
      + ut_space_dist (N.NodeCoo[nodes[1]], N.NodeCoo[nodes[2]])
      + ut_space_dist (N.NodeCoo[nodes[2]], N.NodeCoo[nodes[0]]);

    if (per < minper * 1.1)
    {
      res = -1;
      break;
    }
  }

  ut_free_1d (length);
  ut_free_1d_int (nodes);

  return res;
}

int
neut_mesh_point_elt (struct MESH Mesh, struct NODES Nodes, 
    double* coo, int* pelt)
{
  int i, status;

  status = 1;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    status = neut_mesh_elset_point_elt (Mesh, Nodes, i, coo, pelt);

    if (status == 0)
      break;
  }

  return status;
}

int
neut_mesh_elset_point_elt (struct MESH Mesh, struct NODES Nodes, int elset,
    double* coo, int* pelt)
{
  int i, inelt;

  inelt = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    (*pelt) = Mesh.Elsets[elset][i];

    neut_mesh_point_elt_in (Mesh, Nodes, coo, (*pelt), &inelt);

    if (inelt == 1)
      break;
  }

  return (inelt == 1) ? 0 : 1;
}

int
neut_mesh_elset_point_closestelt (struct MESH Mesh, struct NODES Nodes,
    int elset, double* coo, int* pelt)
{
  int i, elt, id;
  double* dist = ut_alloc_1d (Mesh.Elsets[elset][0]);
  double* eltcoo = ut_alloc_1d (3);

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    neut_mesh_eltcentre (Mesh, Nodes, elt, eltcoo);
    dist[i - 1] = ut_space_dist (coo, eltcoo);
  }
  id = 1 + ut_array_1d_min_index (dist, Mesh.Elsets[elset][0]);
  
  (*pelt) = Mesh.Elsets[elset][id];

  ut_free_1d (dist);
  ut_free_1d (eltcoo);

  return 0;
}


void
neut_mesh_point_elt_in (struct MESH Mesh, struct NODES Nodes,
    double* coo, int elt, int* pinelt)
{

  (*pinelt) =
    ut_space_tet_point_in (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
                           Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
                           Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
                           Nodes.NodeCoo[Mesh.EltNodes[elt][3]], coo);

  return;
}

void
neut_mesh_elt3d_elts2d (struct NODES Nodes, struct MESH Mesh3D, int elt3d, struct MESH Mesh2D, int* elts2d, int* pelt2dqty)
{
  int i, nb;
  struct MESH Facet;
  double* elts2darea = ut_alloc_1d (4);
  int* pos = ut_alloc_1d_int (4);

  neut_mesh_set_zero (&Facet);

  neut_mesh_elt_facetmesh (Mesh3D, elt3d, &Facet);

  (*pelt2dqty) = 0;

  for (i = 0; i < 4; i++)
  {
    neut_nodes_elt (Mesh2D, Facet.EltNodes[i + 1], 3, &nb);
    if (nb > 0)
    {
      elts2d[(*pelt2dqty)] = nb;
      neut_mesh_elt_area (Nodes, Mesh2D, nb, &(elts2darea[(*pelt2dqty)]));

      (*pelt2dqty)++;
    }
  }

  ut_array_1d_sort_des_index (elts2darea, (*pelt2dqty), pos);
  ut_array_1d_permutation_int (elts2d, (*pelt2dqty), pos);

  neut_mesh_free (&Facet);
  ut_free_1d (elts2darea);
  ut_free_1d_int (pos);

  return;
}

void
neut_mesh_elts_elsets (struct MESH Mesh, int* elts, int eltqty, int* elsets, int* pelsetqty)
{
  int i, elset;

  (*pelsetqty) = 0;
  for (i = 0; i < eltqty; i++)
    if (elts[i] >= 1 && elts[i] <= Mesh.EltQty)
    {
      elset = Mesh.EltElset[elts[i]];
      if (ut_array_1d_int_eltpos (elsets, *pelsetqty, elset) == -1)
	elsets[(*pelsetqty)++] = elset;
    }

  ut_array_1d_int_sort (elsets, *pelsetqty);

  return;
}


void
neut_mesh_elts_to_elsets (struct MESH Mesh, int* elts, int eltqty, int* elsets, int* pelsetqty, int** pleftelts, int* plefteltqty)
{
  int i, j, elset;
  int nonfullelsetqty = 0;
  int* nonfullelsets = NULL;

  // looking for all elsets
  neut_mesh_elts_elsets (Mesh, elts, eltqty, elsets, pelsetqty);

  nonfullelsets = ut_alloc_1d_int (*pelsetqty);

  // deleting non-full elsets
  for (i = 0; i < *pelsetqty; i++)
  {
    elset = elsets[i];

    for (j = 1; j <= Mesh.Elsets[elset][0]; j++)
      if (ut_array_1d_int_eltpos (elts, eltqty, Mesh.Elsets[elset][j]) == -1)
      {
	ut_array_1d_int_deletencompress (elsets, *pelsetqty, elset, 1);
	(*pelsetqty)--;
	nonfullelsets[nonfullelsetqty++] = elset;
	break;
      }
  }

  (*plefteltqty) = 0;
  if (nonfullelsetqty > 0)
  {
    (*pleftelts) = ut_alloc_1d_int (1);

    for (i = 0; i < nonfullelsetqty; i++)
    {
      elset = elsets[i];

      for (j = 1; j <= Mesh.Elsets[elset][0]; j++)
	if (ut_array_1d_int_eltpos (elts, eltqty, Mesh.Elsets[elset][j]) != -1)
	{
	  (*pleftelts) = ut_realloc_1d_int ((*pleftelts), ++(*plefteltqty));
	  (*pleftelts)[(*plefteltqty) - 1] = Mesh.Elsets[elset][j];
	}
    }
  }

  return;
}


void
neut_mesh_centre (struct NODES Nodes, struct MESH Mesh, double* pos)
{
  int i;
  double* elsetpos = ut_alloc_1d (3);
  double vol, vol_tot;

  ut_array_1d_zero (pos, 3);

  vol_tot = 0;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    neut_mesh_elset_centre (Nodes, Mesh, i, elsetpos);
    neut_mesh_elset_volume (Nodes, Mesh, i, &vol);

    ut_array_1d_scale (elsetpos, 3, vol);
    vol_tot += vol;

    ut_array_1d_add (pos, elsetpos, 3, pos);
  }
  
  ut_array_1d_scale (pos, 3, 1./vol_tot);

  ut_free_1d (elsetpos);

  return;
}


void
neut_mesh_facepoly (struct NODES Nodes, struct MESH Mesh2D, struct MESH Mesh3D, int*** pfacepoly)
{
  int i, elt2d, elt3dqty, elset3dqty;
  int* elts3d = ut_alloc_1d_int (2);

  Nodes.NodeQty = Nodes.NodeQty;

  (*pfacepoly) = ut_alloc_2d_int (Mesh2D.ElsetQty + 1, 2);

  int fodqty = 0;
  double** fodeq = NULL;
  int dir;

  // for every face, taking one element (the 1st) and looking for its 2
  // 3D elements. Their elsets are the face polys.
  // in case of only one elt, recording one elset as -7.
  for (i = 1; i <= Mesh2D.ElsetQty; i++)
  {
    elt2d = Mesh2D.Elsets[i][1];

    neut_mesh_elt2delts3d (Mesh2D, elt2d, Mesh3D, elts3d, &elt3dqty);
    if (elt3dqty != 2 && elt3dqty != 1)
      ut_error_reportbug ();
    neut_mesh_elts_elsets (Mesh3D, elts3d, elt3dqty, (*pfacepoly)[i],
	&elset3dqty);

    if (elset3dqty != 2 && elset3dqty != 1)
      ut_error_reportbug ();

    if (elset3dqty == 1)
    {
      (*pfacepoly)[i][1] = (*pfacepoly)[i][0];
      // neut_mesh_skinelt_fod (Nodes, Mesh2D, elt2d, &((*pfacepoly)[i][0]));
      neut_mesh_skinelt_fod_b (Nodes, Mesh2D, elt2d = Mesh2D.Elsets[i][1], &fodeq,
					&fodqty, &((*pfacepoly)[i][0]), &dir);
    }
      
    if ((*pfacepoly)[i][0] < 0 && (*pfacepoly)[i][1] > 0)
      ut_array_1d_int_reverseelts ((*pfacepoly)[i], 2);
  }

  ut_free_1d_int (elts3d);
  ut_free_2d (fodeq, fodqty);

  return;
}
  
void
neut_mesh_edgeface (struct MESH Mesh1D, struct MESH Mesh2D, int*** pedgefacenb, int** pedgefaceqty)
{
  int i, elt1d, elt2dqty, elset2dqty;
  int* elts2d = ut_alloc_1d_int (1000);
  int* elsets2d = ut_alloc_1d_int (1000);

  (*pedgefacenb) = ut_alloc_1d_pint (Mesh1D.ElsetQty + 1);
  (*pedgefaceqty) = ut_alloc_1d_int (Mesh1D.ElsetQty + 12);

  // for every edge, taking one element (the 1st) and looking for its
  // 2D elements. Their elsets are the edge faces.
  for (i = 1; i <= Mesh1D.ElsetQty; i++)
  {
    elt1d = Mesh1D.Elsets[i][1];

    neut_mesh_elt1delts2d (Mesh1D, elt1d, Mesh2D, elts2d, &elt2dqty);
    if (elt2dqty == 0)
      ut_error_reportbug ();
    neut_mesh_elts_elsets (Mesh2D, elts2d, elt2dqty, elsets2d,
	&elset2dqty);
    (*pedgefacenb)[i] = ut_alloc_1d_int (elset2dqty);
    ut_array_1d_int_memcpy ((*pedgefacenb)[i], elset2dqty, elsets2d);
    (*pedgefaceqty)[i] = elset2dqty;

    if (elset2dqty == 0)
      ut_error_reportbug ();
  }

  ut_free_1d_int (elts2d);
  ut_free_1d_int (elsets2d);

  return;
}
  
void
neut_mesh_veredge (struct MESH Mesh0D, struct MESH Mesh1D, int*** pveredgenb, int** pveredgeqty)
{
  int i, elt0d, elt2dqty, elset1dqty;
  int* elts1d = ut_alloc_1d_int (1000);
  int* elsets1d = ut_alloc_1d_int (1000);

  (*pveredgenb) = ut_alloc_1d_pint (Mesh0D.ElsetQty + 1);
  (*pveredgeqty) = ut_alloc_1d_int (Mesh0D.ElsetQty + 12);

  // for every ver, looking for its 1D elements. Their elsets are the
  // ver edges.
  for (i = 1; i <= Mesh0D.ElsetQty; i++)
  {
    elt0d = Mesh0D.Elsets[i][1];

    neut_mesh_elt0delts1d (Mesh0D, elt0d, Mesh1D, elts1d, &elt2dqty);
    if (elt2dqty == 0)
      ut_error_reportbug ();
    neut_mesh_elts_elsets (Mesh1D, elts1d, elt2dqty, elsets1d,
	&elset1dqty);
    (*pveredgenb)[i] = ut_alloc_1d_int (elset1dqty);
    ut_array_1d_int_memcpy ((*pveredgenb)[i], elset1dqty, elsets1d);
    (*pveredgeqty)[i] = elset1dqty;

    if (elset1dqty == 0)
      ut_error_reportbug ();
  }

  ut_free_1d_int (elts1d);
  ut_free_1d_int (elsets1d);

  return;
}

void
neut_mesh_elt_lengths (struct MESH Mesh, struct NODES Nodes, int elt, double* pmeanlength, double* pminlength, double* pmaxlength)
{
  int node1, node2, node3, node4;
  double* length = ut_alloc_1d (4);

  if (Mesh.Dimension == 2)
  {
    node1 = Mesh.EltNodes[elt][0];
    node2 = Mesh.EltNodes[elt][1];
    node3 = Mesh.EltNodes[elt][2];

    length[0] = ut_space_dist (Nodes.NodeCoo[node1], Nodes.NodeCoo[node2]);
    length[1] = ut_space_dist (Nodes.NodeCoo[node2], Nodes.NodeCoo[node3]);
    length[2] = ut_space_dist (Nodes.NodeCoo[node3], Nodes.NodeCoo[node1]);

    if (pmeanlength != NULL)
      (*pmeanlength) = ut_array_1d_mean (length, 3);
    if (pminlength != NULL)
      (*pminlength) = ut_array_1d_min (length, 3);
    if (pmaxlength != NULL)
      (*pmaxlength) = ut_array_1d_max (length, 3);
  }
  else if (Mesh.Dimension == 3)
  {
    node1 = Mesh.EltNodes[elt][0];
    node2 = Mesh.EltNodes[elt][1];
    node3 = Mesh.EltNodes[elt][2];
    node4 = Mesh.EltNodes[elt][3];

    length[0] = ut_space_dist (Nodes.NodeCoo[node1], Nodes.NodeCoo[node2]);
    length[1] = ut_space_dist (Nodes.NodeCoo[node2], Nodes.NodeCoo[node3]);
    length[2] = ut_space_dist (Nodes.NodeCoo[node3], Nodes.NodeCoo[node4]);
    length[3] = ut_space_dist (Nodes.NodeCoo[node4], Nodes.NodeCoo[node1]);
    
    if (pmeanlength != NULL)
      (*pmeanlength) = ut_array_1d_mean (length, 4);
    if (pminlength != NULL)
      (*pminlength) = ut_array_1d_min (length, 4);
    if (pmaxlength != NULL)
      (*pmaxlength) = ut_array_1d_max (length, 4);
  }
  else
    ut_error_reportbug ();

  ut_free_1d (length);

  return;
}

int
neut_mesh_elt_neighelts (struct MESH Mesh, int elt, int* nelts, int* pneltqty)
{
  int i, j, eltqty;
  int* elts = ut_alloc_1d_int (1000);
  int* nodes = ut_alloc_1d_int (3);
  int boundqty;
  int boundnodeqty;
  int** seq = NULL;
  
  if (Mesh.Dimension == 1)
  {
    boundqty = 2;
    boundnodeqty = 1;
  }
  else if (Mesh.Dimension == 2)
  {
    boundqty = 3;
    boundnodeqty = 2;
  }
  else if (Mesh.Dimension == 3)
  {
    boundqty = 4;
    boundnodeqty = 3;
  }
  else
    abort ();

  // seq: node ids of every elt boundary (facets for a 3D elt, node for a 1D elt)

  seq = ut_alloc_2d_int (boundqty, boundnodeqty);

  if (Mesh.Dimension == 1)
  {
    seq[0][0] = 0;
    seq[1][0] = 1;
  }
  else if (Mesh.Dimension == 2)
  {
    seq[0][0] = 0; seq[0][1] = 1;
    seq[1][0] = 1; seq[1][1] = 2;
    seq[2][0] = 2; seq[2][1] = 0;
  }
  else if (Mesh.Dimension == 3)
  {
    seq[0][0] = 0; seq[0][1] = 1; seq[0][2] = 2;
    seq[1][0] = 1; seq[1][1] = 2; seq[1][2] = 3;
    seq[2][0] = 2; seq[2][1] = 3; seq[2][2] = 0;
    seq[3][0] = 0; seq[3][1] = 1; seq[3][2] = 3;
  }

  // for each elt boundary, looking for the neighbouring elt
  (*pneltqty) = 0;
  for (i = 0; i < boundqty; i++)
  {
    for (j = 0; j < boundnodeqty; j++)
      nodes[j] = Mesh.EltNodes[elt][seq[i][j]];

    neut_nodes_commonelts (Mesh, nodes, boundnodeqty, elts, &eltqty);

    if ((eltqty == 0)
     || (eltqty == 1 && elts[0] != elt)
     || (eltqty  > 2 && Mesh.Dimension == 3))
      ut_error_reportbug ();

    if (eltqty == 1)
    {
      nelts[0] = 0;
      (*pneltqty) = 0;
    }
    else
      for (j = 0; j < eltqty; j++)
	if (elts[j] != elt)
	  nelts[(*pneltqty)++] = elts[j];
  }

  ut_free_1d_int (nodes);
  ut_free_2d_int (seq, boundqty);
  ut_free_1d_int (elts);
  
  return 0;
}

void
neut_mesh_elts_skinelts (struct MESH Mesh, int* elts, int eltqty,
                         int** pskinelts, int* pskineltqty)
{
  int i, j, neigh;
  int* eltsinv = NULL;
  int eltsmax;
  int elt3dfaceqty;
  int** eltneighs = NULL;
  int* eltneighqty = NULL;

  if (strcmp (Mesh.EltType, "tri") == 0)
    elt3dfaceqty = 4;
  else if (strcmp (Mesh.EltType, "quad") == 0)
    elt3dfaceqty = 6;
  else
    abort ();

  eltneighs = ut_alloc_2d_int (eltqty, elt3dfaceqty);
  eltneighqty = ut_alloc_1d_int (eltqty);
  
  (*pskinelts) = ut_alloc_1d_int (eltqty);

  ut_array_1d_int_inv (elts, eltqty, &eltsinv, &eltsmax);

  // for each elt, recording neighbouring elts
  for (i = 0; i < eltqty; i++)
    neut_mesh_elt_neighelts (Mesh, elts[i], eltneighs[i], &(eltneighqty[i]));

  // for each elt, checking if all its neighbours are in the elts array.
  // If not, the elt is a skin elt, and recorded as such.
  // note: if eltneighqty != 4, the elt is systematically a skin elt.

  (*pskineltqty) = 0;
  for (i = 0; i < eltqty; i++)
  {
    if (eltneighqty[i] != elt3dfaceqty)
      (*pskinelts)[(*pskineltqty)++] = elts[i];
    else
    {
      for (j = 0; j < elt3dfaceqty; j++)
      {
	neigh = eltneighs[i][j];
	if (neigh > eltsmax)
	{
	  (*pskinelts)[(*pskineltqty)++] = elts[i];
	  break;
	}
	else
	  if (eltsinv[neigh] == -1)
	  {
	    (*pskinelts)[(*pskineltqty)++] = elts[i];
	    break;
	  }
      }
    }
  }

  (*pskinelts) = ut_realloc_1d_int ((*pskinelts), *pskineltqty);

  ut_free_2d_int (eltneighs, eltqty);
  ut_free_1d_int (eltneighqty);
  ut_free_1d_int (eltsinv);

  return;
}



void
neut_mesh_face_boundnodes (struct GEO Geo, int face, struct MESH Mesh1D, int** pnodes, int* pnodeqty)
{
  int i, j, edge, elt;
  // note: only the 1st-order nodes are recorded
  
  (*pnodeqty) = 0;
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];
    (*pnodeqty) += Mesh1D.Elsets[edge][0];
  }

  (*pnodes) = ut_alloc_1d_int (*pnodeqty);

  (*pnodeqty) = 0;
  // for every edge
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];

    if (Geo.FaceEdgeOri[face][i] == 1)
      for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
      {
	elt = Mesh1D.Elsets[edge][j];
	(*pnodes)[(*pnodeqty)++] = Mesh1D.EltNodes[elt][0];
      }
    else
      for (j = Mesh1D.Elsets[edge][0]; j >= 1; j--)
      {
	elt = Mesh1D.Elsets[edge][j];
	(*pnodes)[(*pnodeqty)++] = Mesh1D.EltNodes[elt][1];
      }
  }

  return;
}

void
neut_mesh_elset_mesh (struct NODES Nodes, struct MESH Mesh,
                      int elset, struct NODES* pN,   struct MESH* pM)
{
  int i, j, node, elt, eltnodeqty;
  double l;
  int* node_nbs = NULL;
  int* node_nbs_inv = NULL;

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);
  
  (*pM).Dimension = Mesh.Dimension;
  (*pM).EltOrder = 1;
  eltnodeqty = neut_elt_nodeqty ((*pM).EltType, (*pM).Dimension, (*pM).EltOrder);

  node_nbs = ut_alloc_1d_int (eltnodeqty * Mesh.Elsets[elset][0]);

  l = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];

    neut_mesh_addelt (pM, Mesh.EltNodes[elt]);

    for (j = 0; j < eltnodeqty; j++)
    {
      node = Mesh.EltNodes[elt][j];
      if (ut_array_1d_int_eltpos (node_nbs + 1, node_nbs[0], node) == -1)
      {
	node_nbs[++node_nbs[0]] = Mesh.EltNodes[elt][j];

	neut_mesh_eltlength (Nodes, Mesh, elt, &l);
	neut_nodes_addnode (pN, Nodes.NodeCoo[node], l);
      }
    }
  }

  (*pM).ElsetQty = 1;
  (*pM).Elsets = ut_alloc_2d_int (2, (*pM).EltQty + 1);
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
    (*pM).Elsets[1][++(*pM).Elsets[1][0]] = i;

  neut_mesh_init_eltelset (pM, NULL);

  int qty;
  ut_array_1d_int_inv (node_nbs + 1, node_nbs[0], &node_nbs_inv, &qty);
  for (i = 1; i <= (*pM).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pM).EltNodes[i][j] = node_nbs_inv[(*pM).EltNodes[i][j]] + 1;

  ut_free_1d_int (node_nbs);
  ut_free_1d_int (node_nbs_inv);

  return;
}


int
neut_mesh_elset_meancl (struct NODES Nodes, struct MESH Mesh, int elset, double* pmeancl)
{
  int i, elt;
  double* meanlength = ut_alloc_1d (Mesh.Elsets[elset][0]);

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i]; 
    neut_mesh_elt_lengths (Mesh, Nodes, elt, &(meanlength[i - 1]),
			   NULL, NULL);
  }


  (*pmeancl) = ut_array_1d_mean (meanlength, Mesh.Elsets[elset][0]);

  ut_free_1d (meanlength);

  return 0;
}


int
neut_mesh_meancl (struct NODES Nodes, struct MESH Mesh, double* pmeancl)
{
  int i;
  double* meanlength = ut_alloc_1d (Mesh.EltQty);

  for (i = 1; i <= Mesh.EltQty; i++)
    neut_mesh_elt_lengths (Mesh, Nodes, i, &(meanlength[i - 1]),
			   NULL, NULL);

  (*pmeancl) = ut_array_1d_mean (meanlength, Mesh.EltQty);

  ut_free_1d (meanlength);

  return 0;
}

int
neut_mesh_elset_dispfromcl (struct NODES Nodes, struct MESH Mesh, int elset,
			    double cl, double* pacl)
{
  int i, elt;
  double* meanlength = ut_alloc_1d (Mesh.Elsets[elset][0]);

  if (Mesh.EltQty == 0)
  {
    printf ("neut_mesh_dispfromcl: Mesh.EltQty == 0\n");
    ut_error_reportbug ();
  }
  
  if (elset > Mesh.ElsetQty)
  {
    printf ("elset > Mesh.ElsetQty\n");
    ut_error_reportbug ();
  }

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    neut_mesh_elt_lengths (Mesh, Nodes, elt, &(meanlength[i - 1]),
			   NULL, NULL);
  }

  ut_array_1d_scale (meanlength, Mesh.Elsets[elset][0], 1 / cl);
  
  for (i = 0; i < Mesh.Elsets[elset][0]; i++)
    if (meanlength[i] > 1)
      meanlength[i] = 1 / meanlength[i];
  (*pacl) = ut_array_1d_mean (meanlength, Mesh.Elsets[elset][0]);

  ut_free_1d (meanlength);

  return 0;
}

int
neut_mesh_dispfromcl (struct NODES Nodes, struct MESH Mesh, double cl, double* pacl)
{
  int i;
  double* meanlength = ut_alloc_1d (Mesh.EltQty);

  if (Mesh.EltQty == 0)
  {
    printf ("neut_mesh_dispfromcl: Mesh.EltQty == 0\n");
    abort ();
  }

  for (i = 1; i <= Mesh.EltQty; i++)
    neut_mesh_elt_lengths (Mesh, Nodes, i, &(meanlength[i - 1]),
			   NULL, NULL);

  ut_array_1d_scale (meanlength, Mesh.EltQty, 1 / cl);
  
  for (i = 0; i < Mesh.EltQty; i++)
    if (meanlength[i] > 1)
      meanlength[i] = 1 / meanlength[i];
  (*pacl) = ut_array_1d_mean (meanlength, Mesh.EltQty);

  ut_free_1d (meanlength);

  return 0;
}

/*
void
neut_mesh_face_boundnodes (struct GEO Geo, int face, struct MESH Mesh1D, int** pnodes, int* pnodeqty)
{
  int i, j, edge, elt;
  
  (*pnodeqty) = 0;
  (*pnodes) = ut_alloc_1d_int (1);

  // for every edge
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];


    (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty) + 2 *
			       Mesh1D.Elsets[edge][0]);

    for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
    {
      elt = Mesh1D.Elsets[edge][j];
      ut_array_1d_int_memcpy ((*pnodes) + (*pnodeqty), 2, Mesh1D.EltNodes[elt]);
      (*pnodeqty) += 2;
    }
  }

  ut_array_1d_int_sort_uniq ((*pnodes), (*pnodeqty), &((*pnodeqty)));
  (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty));

  return;
}
*/


/* rsel2sel sets sel from rsel */
void
rsel2sel (double rsel, double vol, int PolyQty, double* psel)
{
  (*psel) = 0.5 * rsel * 0.5 / pow (PolyQty / vol, 0.3333333333333333333333333333);
  
  return;
}

/* rcl2cl sets cl from rcl */
void
rcl2cl (double rcl, double vol, int PolyQty, double* pcl)
{
  (*pcl) = rcl * 0.5 / pow (PolyQty / vol, 0.3333333333333333333333333333);
  
  return;
}

void
neut_mesh_fprintf_gmsh (FILE* file, char* dim, struct NODES Nodes,
    struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
    struct MESH Mesh3D, 
    struct PART Part)
{
  neut_meshheader_fprintf_gmsh (file);

  neut_nodes_fprintf_gmsh (file, Nodes);

  neut_elts_fprintf_gmsh (file, Mesh0D, Mesh1D, Mesh2D, Mesh3D, Part, dim);

/*----------------------------------------------------------------------
 * node sets */

//  if (nset != NULL)
//  {
//    fprintf (file, "\n/* node sets\n");
//    neut_mesh_fprintf_gmsh_nsets (file, nset, NSets, NSetNames);
//    fprintf (file, "\n*/\n");
//  }
  
  return;
}

void
neut_meshheader_fprintf_gmsh (FILE* file)
{
  fprintf (file, "$MeshFormat\n");
  fprintf (file, "2 0 8\n");
  fprintf (file, "$Comments\n");
  fprintf (file, "contiguous\n");
  fprintf (file, "$EndComments\n");
  fprintf (file, "$EndMeshFormat\n");

  return;
}


void
neut_elts_fprintf_gmsh (FILE* file, struct MESH Mesh0D,
                        struct MESH Mesh1D, struct MESH Mesh2D,
			struct MESH Mesh3D, struct PART Part,
			char* dim)
{
  int i, id, elt_type0D, elt_type1D, elt_type2D, elt_type3D;
  int eltnodeqty1D, eltnodeqty2D, eltnodeqty3D;

  eltnodeqty1D = neut_elt_nodeqty (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder);
  eltnodeqty2D = neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);
  eltnodeqty3D = neut_elt_nodeqty (Mesh3D.EltType, Mesh3D.Dimension, Mesh3D.EltOrder);

  if (Mesh0D.EltQty != 0 && neut_gmsh_elt_nb (Mesh0D.EltType, Mesh0D.Dimension,
	Mesh0D.EltOrder, &elt_type0D) != 0)
    ut_error_reportbug ();

  if (Mesh1D.EltQty != 0 && neut_gmsh_elt_nb (Mesh1D.EltType, Mesh1D.Dimension,
	Mesh1D.EltOrder, &elt_type1D) != 0)
    ut_error_reportbug ();

  if (Mesh2D.EltQty != 0 && neut_gmsh_elt_nb (Mesh2D.EltType, Mesh2D.Dimension,
	Mesh2D.EltOrder, &elt_type2D) != 0)
    ut_error_reportbug ();

  if (Mesh3D.EltQty != 0 && neut_gmsh_elt_nb (Mesh3D.EltType, Mesh3D.Dimension,
	Mesh3D.EltOrder, &elt_type3D) != 0)
    ut_error_reportbug ();


  fprintf (file, "$Elements\n");
  int eltqty = 0;
  if (ut_string_inlist (dim, ',', "0"))
    eltqty += Mesh0D.EltQty;
  if (ut_string_inlist (dim, ',', "1"))
    eltqty += Mesh1D.EltQty;
  if (ut_string_inlist (dim, ',', "2"))
    eltqty += Mesh2D.EltQty;
  if (ut_string_inlist (dim, ',', "3"))
    eltqty += Mesh3D.EltQty;

  fprintf (file, "%d\n", eltqty);

  id = 1;

  // 0D mesh
  if (ut_string_inlist (dim, ',', "0"))
  {
    if (Mesh0D.EltElset == NULL)
      neut_mesh_init_eltelset (&Mesh0D, NULL);
    for (i = 1; i <= Mesh0D.EltQty; i++)
    {
      fprintf (file, "%d %d 3 0 %d 0 ", id++, elt_type0D, Mesh0D.EltElset[i]);
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
      fprintf (file, "%d %d 3 0 %d 0 ", id++, elt_type1D, Mesh1D.EltElset[i]);
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
      fprintf (file, "%d %d 3 0 %d 0 ", id++, elt_type2D, Mesh2D.EltElset[i]);
      ut_array_1d_int_fprintf (file, Mesh2D.EltNodes[i], eltnodeqty2D, "%d");
    }
  }

  // 3D mesh
  if (ut_string_inlist (dim, ',', "3"))
  {
    if (Part.qty > 0)
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
	fprintf (file, "%d %d 3 0 %d %d ", id++, elt_type3D, Mesh3D.EltElset[i], Part.elt_parts[i] + 1);
	ut_array_1d_int_fprintf (file, Mesh3D.EltNodes[i], eltnodeqty3D, "%d");
      }
    else
      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
	fprintf (file, "%d %d 3 0 %d 0 ", id++, elt_type3D, Mesh3D.EltElset[i]);
	ut_array_1d_int_fprintf (file, Mesh3D.EltNodes[i], eltnodeqty3D, "%d");
      }
  }

  fprintf (file, "$EndElements\n");
  
  return;
}

/*
void
neut_mesh_fprintf_gmsh_nsets (FILE* file, char* nset, int** NSets, char** NSetNames)
{
  int i, j;
  char** name;
  int qty;

  ut_string_separate (nset, ',', &name, &qty);

  for (i = 0; i < qty; i++)
    for (j = 1; j <= 44; j++)
      if (strcmp (name[i], NSetNames[j]) == 0)
      {
	neut_mesh_fprintf_gmsh_nset (file, NSetNames[j], NSets[j]);
	break;
      }
  
  ut_free_2d_char (name, qty);

  return;
}
*/

void
neut_mesh_fprintf_gmsh_nset (FILE* file, char* name, int* nodeset)
{
  int i, col;

  fprintf (file, "\n**nset %s\n", name);

  col = 0;
  for (i = 1; i <= nodeset[0]; i++)
    ut_print_wnc_int (file, nodeset[i], &col, 72);

  fprintf (file, "\n");

  return;
}


void
neut_mesh_scale (struct MESH *pMesh, double scalex, double scaley, double scalez)
{
  int i;

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    (*pMesh).EltCoo[i][0] *= scalex;
    (*pMesh).EltCoo[i][1] *= scaley;
    (*pMesh).EltCoo[i][2] *= scalez;
  }

  return;
}

void
neut_mesh_shift (struct MESH *pMesh, double shiftx, double
    shifty, double shiftz)
{
  int i;

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    (*pMesh).EltCoo[i][0] += shiftx;
    (*pMesh).EltCoo[i][1] += shifty;
    (*pMesh).EltCoo[i][2] += shiftz;
  }

  return;
}

void
neut_mesh3d_slice (struct NODES Nodes, struct MESH Mesh, double* eq,
		   struct NODES* pSNodes, struct MESH* pSMesh,
	           int** pelt_newold, int*** pnode_newold, double** pnode_fact)
{
  int i;

  if (Mesh.Dimension != 3)
    ut_error_reportbug ();

  (*pSMesh).Dimension = 2;
  (*pSMesh).EltOrder = 1;
  (*pSMesh).EltType = ut_alloc_1d_char (10);
  sprintf ((*pSMesh).EltType, "tri");

  (*pelt_newold)  = ut_alloc_1d_int (1);
  (*pnode_newold) = ut_alloc_2d_int (1, 2);
  (*pnode_fact)   = ut_alloc_1d     (1);
  
  if (! strcmp (Mesh.EltType, "tri"))
    for (i = 1; i <= Mesh.EltQty; i++)
      neut_mesh3d_elt_slice_tet (Nodes, Mesh, i, eq, pSNodes, pSMesh, pelt_newold,
				 pnode_newold, pnode_fact);
  else if (! strcmp (Mesh.EltType, "quad"))
    for (i = 1; i <= Mesh.EltQty; i++)
      neut_mesh3d_elt_slice_quad (Nodes, Mesh, i, eq, pSNodes, pSMesh, pelt_newold,
				  pnode_newold, pnode_fact);
  else
    ut_error_reportbug ();

  neut_mesh_addelset (pSMesh, NULL, (*pSMesh).EltQty);

  return;
}


void
neut_meshdata_mesh2slice (struct NODES Nodes, struct MESH Mesh, struct MESHDATA MeshData,
		          struct NODES SNodes, struct MESH SMesh,
	                  int* elt_newold, int** node_newold, double* node_fact,
			  struct MESHDATA* pSMeshData)
{
  int i, j, k;
  int qty;
  int idmax;

  (*pSMeshData).elset0dqty = 0;
  (*pSMeshData).elset1dqty = 0;
  (*pSMeshData).elset2dqty = 0;
  (*pSMeshData).elset3dqty = 0;

  (*pSMeshData).elt0dqty = 0;
  (*pSMeshData).elt1dqty = 0;
  (*pSMeshData).elt2dqty = SMesh.EltQty;
  (*pSMeshData).elt3dqty = 0;
  
  (*pSMeshData).nodeqty  = SNodes.NodeQty;

  neut_meshdata_idmax (&idmax);

  Mesh  = Mesh;
  SMesh = SMesh;
  Nodes = Nodes;

  int qty0;
  int** id_newold = ut_alloc_2d_int (2, 2);
  neut_meshdata_entity_id ("node2elt", &(id_newold[0][0]));
  id_newold[0][1] = id_newold[0][0];
  neut_meshdata_entity_id ("elt2d", &(id_newold[1][0]));
  neut_meshdata_entity_id ("elt3d", &(id_newold[1][1]));

  for (i = 0; i <= 1; i++)
  {
    if (MeshData.coldatatype[id_newold[i][1]] == NULL)
      continue;

    (*pSMeshData).coldatatype[id_newold[i][0]]
      = ut_alloc_1d_char (strlen (MeshData.coldatatype[id_newold[i][1]]) + 1);
    strcpy ((*pSMeshData).coldatatype[id_newold[i][0]], MeshData.coldatatype[id_newold[i][1]]);
	    
    neut_meshdata_id_qty   ((*pSMeshData), id_newold[i][0], &qty0);
    neut_meshdata_type_qty (MeshData.coldatatype[id_newold[i][1]], &qty);

    (*pSMeshData).coldata[id_newold[i][0]] = ut_alloc_2d (qty0 + 1, qty);

    for (j = 1; j <= qty0; j++)
    {
      char* ent = ut_alloc_1d_char (100);
      neut_meshdata_id_entity (id_newold[i][0], ent);

      if (! strncmp (ent, "elt", 3))
      {
	for (k = 0; k < qty; k++)
	  (*pSMeshData).coldata[id_newold[i][0]][j][k] =
	    MeshData.coldata[id_newold[i][1]][elt_newold[j]][k];
      }
      else if (! strncmp (ent, "node", 4))
	for (k = 0; k < qty; k++)
	  (*pSMeshData).coldata[id_newold[i][0]][j][k] =
		(1 - node_fact[j]) * MeshData.coldata[id_newold[i][1]][node_newold[j][0]][k]
		 +   node_fact[j]    * MeshData.coldata[id_newold[i][1]][node_newold[j][1]][k];
    }
  
    if (MeshData.scalemin[id_newold[i][1]] != NULL)
    {
      (*pSMeshData).scalemin[id_newold[i][0]]
	= ut_alloc_1d_char (strlen (MeshData.scalemin[id_newold[i][1]]) + 1);
      strcpy ((*pSMeshData).scalemin[id_newold[i][0]],
		   MeshData.scalemin[id_newold[i][1]]);
    }

    if (MeshData.scalemax[id_newold[i][1]] != NULL)
    {
      (*pSMeshData).scalemax[id_newold[i][0]]
	= ut_alloc_1d_char (strlen (MeshData.scalemax[id_newold[i][1]]) + 1);
      strcpy ((*pSMeshData).scalemax[id_newold[i][0]],
	           MeshData.scalemax[id_newold[i][1]]);
    }

    if (MeshData.scaleticks[id_newold[i][1]] != NULL)
    {
      (*pSMeshData).scaleticks[id_newold[i][0]]
	= ut_alloc_1d_char (strlen (MeshData.scaleticks[id_newold[i][1]]) + 1);
      strcpy ((*pSMeshData).scaleticks[id_newold[i][0]],
	           MeshData.scaleticks[id_newold[i][1]]);
    }

    if (MeshData.colscheme[id_newold[i][1]] != NULL)
    {
      (*pSMeshData).colscheme[id_newold[i][0]]
	= ut_alloc_1d_char (strlen (MeshData.colscheme[id_newold[i][1]]) + 1);
      strcpy ((*pSMeshData).colscheme[id_newold[i][0]],
	           MeshData.colscheme[id_newold[i][1]]);
    }
  }

  return;
}


void
neut_mesh_set_elttype (struct MESH* pMesh, char* elttype)
{
  if ((*pMesh).EltType == NULL)
    (*pMesh).EltType = ut_alloc_1d_char (10);

  sprintf ((*pMesh).EltType, "%s", elttype);

  return;
}

int
neut_mesh_elt_point_pos (struct MESH Mesh, struct NODES Nodes,
                         int elt, double* coo, double* pos)
{
  int i, status;
  int** seq = ut_alloc_2d_int (4, 2);
  double d, d2;
  
  seq[0][0] = 0;
  seq[0][1] = 1;
  
  seq[1][0] = 1;
  seq[1][1] = 2;
  
  seq[2][0] = 2;
  seq[2][1] = 3;
  
  seq[3][0] = 1;
  seq[3][1] = 3;

  ut_array_1d_zero (pos, 4);

  status = -1;
  for (i = 0; i < 4; i++)
  {
    int node1 = Mesh.EltNodes[elt][seq[i][0]];
    int node2 = Mesh.EltNodes[elt][seq[i][1]];

    if (ut_space_pointaligned (Nodes.NodeCoo[node1], coo,
			       Nodes.NodeCoo[node2], 1e-3))
    {
      d  = ut_space_dist (Nodes.NodeCoo[node1],
	                  Nodes.NodeCoo[node2]);
      d2 = ut_space_dist (Nodes.NodeCoo[node1], coo);

      pos[seq[i][0]] = 1 - d2 / d;
      pos[seq[i][1]] = d2 / d;
      
      status = 0;
      break;
    }

    if (status == 0)
      break;
  }

  if (status == -1)
  {
    ut_print_message (1, 1,
	"neut_mesh_elt_point_pos: taking the closer node.\n");
    ut_print_message (1, 1, "debug me!\n");

    d = 1e32;
    int id = 0;
    for (i = 0; i < 4; i++)
    {
      double dist =
	ut_space_dist (Nodes.NodeCoo[Mesh.EltNodes[elt][i]], coo);
      if (dist < d)
      {
	dist = d;
	id = i;
      }
    }

    pos[id] = 1;
    status = 0;
  }

  ut_free_2d_int (seq, 4);

  return status;
}

int
neut_mesh3d_elt_slice_tet (struct NODES Nodes, struct MESH Mesh, int elt, double* eq,
			   struct NODES* pSNodes, struct MESH* pSMesh,
			   int** pelt_newold, int*** pnode_newold, double** pnode_fact)
{
  int i, j, node;
  int* side = ut_alloc_1d_int (4);
  int siden, intertype, nodemaster, id;
  int* pos = ut_alloc_1d_int (2);
  int* pos2 = ut_alloc_1d_int (2);
  double* coo = ut_alloc_1d (3);
  int* eltnodes = ut_alloc_1d_int (3);
  double dist, distmaster;

  // intertype = 0 -> no intersection
  //
  // intertype = 1 -> 1|3 vertices (two sides of the plane): the
  // intersection between the elt and the plane is a triangle.
  //
  // intertype = 2 -> 2|2 vertices (two sides of the plane): the 
  // intersection between the elt and the plane is a quadrangle;
  // spliting it into two triangles.

  for (i = 0; i < 4; i++)
  {
    node = Mesh.EltNodes[elt][i];
    side[i] = ut_space_planeside (eq, Nodes.NodeCoo[node] - 1);
  }
  siden = ut_array_1d_int_nbofthisval (side, 4, -1);
  intertype = ut_num_min (siden, 4 - siden);

  if (intertype == 1)
  {
    // finding master node
    if (siden == 1)
      pos[0] = ut_array_1d_int_eltpos (side, 4, -1);
    else if (siden == 3)
      pos[0] = ut_array_1d_int_eltpos (side, 4,  1);
    else
      ut_error_reportbug ();

    nodemaster = Mesh.EltNodes[elt][pos[0]];
    ut_space_point_plane_dist (Nodes.NodeCoo[nodemaster], eq, 
			       &distmaster);

    // finding intersection between the tet and the plane (triangle)
    id = 0;
    for (i = 0; i < 4; i++)
      if (i != pos[0])
      {
	node = Mesh.EltNodes[elt][i];
	ut_space_segment_plane_intersect (Nodes.NodeCoo[nodemaster],
	    Nodes.NodeCoo[node], eq, coo);
	neut_nodes_addnode (pSNodes, coo, 0);
	
	ut_space_point_plane_dist (Nodes.NodeCoo[node], eq, &dist);
	  
	eltnodes[id++] = (*pSNodes).NodeQty;

	(*pnode_newold)  = ut_realloc_2d_int_addline ((*pnode_newold), (*pSNodes).NodeQty + 1, 2);
	(*pnode_fact)    = ut_realloc_1d             ((*pnode_fact)  , (*pSNodes).NodeQty + 1);
    
	(*pnode_newold)[(*pSNodes).NodeQty][0] = nodemaster;
	(*pnode_newold)[(*pSNodes).NodeQty][1] = node;
	(*pnode_fact)  [(*pSNodes).NodeQty]    = distmaster / (distmaster + dist);
      }

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;
  }

  else if (intertype == 2)
  {
    int id = 0;
    int id2 = 0;
    for (i = 0; i < 4; i++)
      if (side[i] == -1)
	pos[id++] = i;
      else
	pos2[id2++] = i;

    // Finding intersection between the tet and the plane
    // (quadrangle).  Splitting up the quadrangle into two triangles
    // for plotting. 
    
    for (j = 0; j < 2; j++)
    {
      nodemaster = Mesh.EltNodes[elt][pos[j]];
      ut_space_point_plane_dist (Nodes.NodeCoo[nodemaster], eq, 
				 &distmaster);

      for (i = 0; i < 2; i++)
      {
	node = Mesh.EltNodes[elt][pos2[i]];
	ut_space_point_plane_dist (Nodes.NodeCoo[node], eq,
				   &dist);

	ut_space_segment_plane_intersect (Nodes.NodeCoo[nodemaster],
	    Nodes.NodeCoo[node], eq, coo);
	neut_nodes_addnode (pSNodes, coo, 0);

	(*pnode_newold)  = ut_realloc_2d_int_addline ((*pnode_newold), (*pSNodes).NodeQty + 1, 2);
	(*pnode_fact)    = ut_realloc_1d             ((*pnode_fact)  , (*pSNodes).NodeQty + 1);
    
	(*pnode_newold)[(*pSNodes).NodeQty][0] = nodemaster;
	(*pnode_newold)[(*pSNodes).NodeQty][1] = node;
	(*pnode_fact)  [(*pSNodes).NodeQty]    = distmaster / (distmaster + dist);
      }
    }
    
    eltnodes[0] = (*pSNodes).NodeQty - 3;
    eltnodes[1] = (*pSNodes).NodeQty - 2;
    eltnodes[2] = (*pSNodes).NodeQty - 1;

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;

    eltnodes[0] = (*pSNodes).NodeQty - 1;
    eltnodes[1] = (*pSNodes).NodeQty - 2;
    eltnodes[2] = (*pSNodes).NodeQty;

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;
  }
  
  ut_free_1d_int (side);
  ut_free_1d     (coo);
  ut_free_1d_int (pos);
  ut_free_1d_int (pos2);
  ut_free_1d_int (eltnodes);

  return intertype;
}

int
neut_mesh3d_elt_slice_quad (struct NODES Nodes, struct MESH Mesh, int elt, double* eq,
			    struct NODES* pSNodes, struct MESH* pSMesh,
			    int** pelt_newold, int*** pnode_newold, double** pnode_fact)
{
  int i, j, sl_elt, sl_node, eltqty, nodeqty;
  struct NODES ENodes;
  struct  MESH EMesh;
  struct  MESH TEMesh;

  neut_nodes_set_zero (&ENodes);
  neut_mesh_set_zero (&EMesh);
  neut_mesh_set_zero (&TEMesh);

  neut_mesh_elt_mesh (Nodes, Mesh, elt, &ENodes, &EMesh);
  
  neut_mesh_quad_tri (ENodes, EMesh, &TEMesh);
  
  for (i = 1; i <= 6; i++)
  {
    nodeqty = (*pSNodes).NodeQty;
    eltqty  = (*pSMesh).EltQty;
    neut_mesh3d_elt_slice_tet (ENodes, TEMesh, i, eq, pSNodes, pSMesh,
			       pelt_newold, pnode_newold, pnode_fact);

    // Renumbering added elts
    for (sl_elt = eltqty + 1; sl_elt <= (*pSMesh).EltQty; sl_elt++)
      (*pelt_newold)[sl_elt] = elt;

    // Renumbering added nodes
    for (sl_node = nodeqty + 1; sl_node <= (*pSNodes).NodeQty; sl_node++)
      for (j = 0; j < 2; j++)
	(*pnode_newold)[sl_node][j] = Mesh.EltNodes[elt][(*pnode_newold)[sl_node][j] - 1];
  }
  
  neut_nodes_free (&ENodes);
  neut_mesh_free (&EMesh);
  neut_mesh_free (&TEMesh);

  return 0;
}


void
neut_mesh_elt_bbox_v (struct NODES Nodes, struct MESH Mesh, int elt, double* bbox)
{
  double** tmp = ut_alloc_2d (3, 2);

  neut_mesh_elt_bbox (Nodes, Mesh, elt, tmp);

  bbox[0] = tmp[0][0];
  bbox[1] = tmp[0][1];
  bbox[2] = tmp[1][0];
  bbox[3] = tmp[1][1];
  bbox[4] = tmp[2][0];
  bbox[5] = tmp[2][1];

  ut_free_2d (tmp, 3);

}

void
neut_mesh_elt_bbox (struct NODES Nodes, struct MESH Mesh, int elt, double** bbox)
{
  int i, j, node;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  for (i = 0; i < 3; i++)
  {
    bbox[i][0] =  DBL_MAX;
    bbox[i][1] = -DBL_MAX;

    for (j = 0; j < eltnodeqty; j++)
    {
      node = Mesh.EltNodes[elt][j];
      bbox[i][0] = ut_num_min (bbox[i][0], Nodes.NodeCoo[node][i]);
      bbox[i][1] = ut_num_max (bbox[i][1], Nodes.NodeCoo[node][i]);
    }
  }

  return;
}

void
neut_mesh_elt_poly (struct NODES Nodes, struct MESH Mesh, int elt, struct POLY* pPoly)
{
  int i, node;

  printf ("neut_mesh_elt_poly: CHECK me! (I've never been used.)\n");

  if (Mesh.EltOrder != 3)
    ut_error_reportbug ();

  if (! strcmp (Mesh.EltType, "tri"))
  {
    (*pPoly).VerQty = 4;
    (*pPoly).FaceQty = 4;

    (*pPoly).VerCoo = ut_alloc_2d ((*pPoly).FaceQty + 1, 3);
    for (i = 1; i <= (*pPoly).VerQty; i++)
    {
      node = Mesh.EltNodes[elt][i - 1];
      ut_array_1d_memcpy ((*pPoly).VerCoo[i], 4, Nodes.NodeCoo[node]);
    }

    (*pPoly).FacePoly = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
    for (i = 1; i <= (*pPoly).FaceQty; i++)
      (*pPoly).FacePoly[i] = -i;

    (*pPoly).FaceEq = ut_alloc_2d ((*pPoly).FaceQty + 1, 4);
    (*pPoly).FaceVerQty = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
    (*pPoly).FaceVerNb  = ut_alloc_2d_int ((*pPoly).FaceQty + 1, 4);

    ut_array_1d_int_set ((*pPoly).FaceVerQty + 1, (*pPoly).VerQty, 3);
    (*pPoly).FaceVerNb[1][0] = 1;
    (*pPoly).FaceVerNb[1][1] = 2;
    (*pPoly).FaceVerNb[1][2] = 3;
    (*pPoly).FaceVerNb[2][0] = 1;
    (*pPoly).FaceVerNb[2][1] = 3;
    (*pPoly).FaceVerNb[2][2] = 4;
    (*pPoly).FaceVerNb[3][0] = 1;
    (*pPoly).FaceVerNb[3][1] = 4;
    (*pPoly).FaceVerNb[3][2] = 2;
    (*pPoly).FaceVerNb[4][0] = 2;
    (*pPoly).FaceVerNb[4][1] = 3;
    (*pPoly).FaceVerNb[4][2] = 4;

    for (i = 1; i <= 4; i++)
    {
      // not sure the normals are outgoing - check the order of the
      // nodes
      ut_space_points_plane ((*pPoly).VerCoo[(*pPoly).FaceVerNb[i][0]],
                             (*pPoly).VerCoo[(*pPoly).FaceVerNb[i][1]],
                             (*pPoly).VerCoo[(*pPoly).FaceVerNb[i][2]],
			     ((*pPoly).FaceEq[i]));
    }
  }
  else if (! strcmp (Mesh.EltType, "quad"))
  {
    (*pPoly).VerQty = 8;
    (*pPoly).FaceQty = 6;

    (*pPoly).VerCoo = ut_alloc_2d ((*pPoly).VerQty + 1, 3);
    for (i = 1; i <= (*pPoly).VerQty; i++)
    {
      node = Mesh.EltNodes[elt][i - 1];
      ut_array_1d_memcpy ((*pPoly).VerCoo[i], 4, Nodes.NodeCoo[node]);
    }

    (*pPoly).FacePoly = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
    for (i = 1; i <=(*pPoly).FaceQty; i++)
      (*pPoly).FacePoly[i] = -i;

    (*pPoly).FaceEq = ut_alloc_2d ((*pPoly).FaceQty + 1, 4);
    (*pPoly).FaceVerQty = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
    (*pPoly).FaceVerNb  = ut_alloc_2d_int ((*pPoly).FaceQty + 1, 4);

    ut_array_1d_int_set ((*pPoly).FaceVerQty + 1, (*pPoly).VerQty, 4);
    (*pPoly).FaceVerNb[1][0] = 0;
    (*pPoly).FaceVerNb[1][1] = 3;
    (*pPoly).FaceVerNb[1][2] = 2;
    (*pPoly).FaceVerNb[1][3] = 1;
    (*pPoly).FaceVerNb[2][0] = 4;
    (*pPoly).FaceVerNb[2][1] = 5;
    (*pPoly).FaceVerNb[2][2] = 6;
    (*pPoly).FaceVerNb[2][3] = 7;
    (*pPoly).FaceVerNb[3][0] = 1;
    (*pPoly).FaceVerNb[3][1] = 2;
    (*pPoly).FaceVerNb[3][2] = 6;
    (*pPoly).FaceVerNb[3][3] = 5;
    (*pPoly).FaceVerNb[4][0] = 0;
    (*pPoly).FaceVerNb[4][1] = 4;
    (*pPoly).FaceVerNb[4][2] = 7;
    (*pPoly).FaceVerNb[4][3] = 3;
    (*pPoly).FaceVerNb[5][0] = 0;
    (*pPoly).FaceVerNb[5][1] = 1;
    (*pPoly).FaceVerNb[5][2] = 5;
    (*pPoly).FaceVerNb[5][3] = 4;
    (*pPoly).FaceVerNb[6][0] = 3;
    (*pPoly).FaceVerNb[6][1] = 7;
    (*pPoly).FaceVerNb[6][2] = 6;
    (*pPoly).FaceVerNb[6][3] = 2;

    for (i = 1; i <= 4; i++)
      ut_space_points_plane ((*pPoly).VerCoo[(*pPoly).FaceVerNb[i][0]],
                             (*pPoly).VerCoo[(*pPoly).FaceVerNb[i][1]],
                             (*pPoly).VerCoo[(*pPoly).FaceVerNb[i][2]],
			     ((*pPoly).FaceEq[i]));
  }
  else
    abort ();

  return;
}

int
neut_mesh_elt_mesh (struct NODES Nodes, struct MESH Mesh, int elt,
                    struct NODES* pENodes, struct MESH* pEMesh)
{
  int i, node;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  // Setting *pENodes
  neut_nodes_set_zero (pENodes);

  (*pENodes).NodeQty = eltnodeqty;

  (*pENodes).NodeCoo = ut_alloc_2d ((*pENodes).NodeQty + 1, 3);

  for (i = 1; i <= eltnodeqty; i++)
  {
    node = Mesh.EltNodes[elt][i - 1];
    ut_array_1d_memcpy ((*pENodes).NodeCoo[i], 3, Nodes.NodeCoo[node]);
  }

  // Setting *pEMesh
  neut_mesh_set_zero (pEMesh);

  (*pEMesh).EltType = ut_alloc_1d_char (strlen (Mesh.EltType) + 1);
  strcpy ((*pEMesh).EltType, Mesh.EltType);

  (*pEMesh).Dimension = Mesh.Dimension;
  (*pEMesh).EltOrder = Mesh.EltOrder;

  (*pEMesh).EltQty = 1;
  (*pEMesh).EltNodes = ut_alloc_2d_int (2, eltnodeqty);

  for (i = 0; i < eltnodeqty; i++)
    (*pEMesh).EltNodes[1][i] = i + 1;

  return 0;
}

// Turn a quad mesh into a tri mesh by  element subdivision
void
neut_mesh_quad_tri (struct NODES Nodes, struct MESH Mesh, struct MESH* pTMesh)
{
  int i, j, k, elt;
  
  if (strcmp (Mesh.EltType, "quad") != 0)
    ut_error_reportbug ();

  if (Mesh.Dimension != 3)
    ut_error_reportbug ();

  // Vertices of the tets describing the cube elt
  int** nodepos = ut_alloc_2d_int (6, 4);

  nodepos[0][0] = 0;
  nodepos[0][1] = 1;
  nodepos[0][2] = 2;
  nodepos[0][3] = 6;

  nodepos[1][0] = 0;
  nodepos[1][1] = 3;
  nodepos[1][2] = 2;
  nodepos[1][3] = 6;

  nodepos[2][0] = 0;
  nodepos[2][1] = 6;
  nodepos[2][2] = 3;
  nodepos[2][3] = 7;

  nodepos[3][0] = 0;
  nodepos[3][1] = 6;
  nodepos[3][2] = 4;
  nodepos[3][3] = 7;

  nodepos[4][0] = 0;
  nodepos[4][1] = 4;
  nodepos[4][2] = 5;
  nodepos[4][3] = 6;

  nodepos[5][0] = 0;
  nodepos[5][1] = 5;
  nodepos[5][2] = 1;
  nodepos[5][3] = 6;

  // General data
  (*pTMesh).EltType = ut_alloc_1d_char (4);
  strcpy ((*pTMesh).EltType, "tri");
  (*pTMesh).Dimension = 3;
  (*pTMesh).EltOrder = 1;
  
  (*pTMesh).EltQty = 0;
  (*pTMesh).EltNodes = ut_alloc_2d_int (Mesh.EltQty * 6 + 1, 3);

  // Elt data
  if (Mesh.EltElset != NULL)
    (*pTMesh).EltElset = ut_alloc_1d_int (Mesh.EltQty * 6 + 1);

  for (i = 1; i <= Mesh.EltQty; i++)
    for (j = 0; j < 6; j++)
    {
      elt = ++((*pTMesh).EltQty);

      for (k = 0; k < 4; k++)
	(*pTMesh).EltNodes[elt][k] = Mesh.EltNodes[i][nodepos[j][k]];

      if (Mesh.EltElset != NULL)
	(*pTMesh).EltElset[elt] = Mesh.EltElset[i];
    }

  // Elsets
  (*pTMesh).ElsetQty = Mesh.ElsetQty;
  (*pTMesh).Elsets = ut_alloc_2d_int ((*pTMesh).ElsetQty + 1, 1);
  for (i = 1; i <= (*pTMesh).ElsetQty; i++)
  {
    (*pTMesh).Elsets[i][0] = 6 * Mesh.Elsets[i][0];

    for (j = 1; j <= Mesh.Elsets[i][0]; j++)
      for (k = 0; k < 6; k++)
	(*pTMesh).Elsets[i][++((*pTMesh).Elsets[i][0])] = Mesh.Elsets[i][j];
  }

  // NodeElts
  neut_mesh_init_nodeelts (pTMesh, Nodes.NodeQty);

  ut_free_2d_int (nodepos, 6);

  return;
}

int
neut_mesh_rmelts (struct MESH* pMesh, struct NODES Nodes, int* rmelt, int rmeltqty)
{
  int i, j, eltmin, eltmax, eltqty;
  int* new_old = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  int* old_new = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  ut_array_1d_int_sort (rmelt, rmeltqty);
  eltmin = rmelt[0];
  eltmax = rmelt[rmeltqty - 1];

  eltqty = 0;
  for (i = 1; i < eltmin; i++)
    new_old[++eltqty] = i;

  if ((eltmax - eltmin + 1) != rmeltqty)
    for (i = eltmin; i <= eltmax; i++)
      if (ut_array_1d_int_eltpos (rmelt, rmeltqty, i) == -1)
	new_old[++eltqty] = i;

  for (i = eltmax + 1; i <= (*pMesh).EltQty; i++)
    new_old[++eltqty] = i;

  for (i = 1; i <= (*pMesh).EltQty; i++)
    old_new[new_old[i]] = i;
  
  // Updating EltNodes
  for (i = 1; i <= eltqty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[i], eltnodeqty, (*pMesh).EltNodes[new_old[i]]);
  (*pMesh).EltNodes = ut_realloc_2d_int_delline ((*pMesh).EltNodes, (*pMesh).EltQty + 1, eltqty + 1);

  // Updating EltCoo
  if ((*pMesh).EltCoo != NULL)
    for (i = 1; i <= eltqty; i++)
      ut_array_1d_memcpy ((*pMesh).EltCoo[i], 3, (*pMesh).EltCoo[new_old[i]]);
  (*pMesh).EltCoo = ut_realloc_2d_delline ((*pMesh).EltCoo, (*pMesh).EltQty + 1, eltqty + 1);

  // Updating EltElset
  if ((*pMesh).EltElset != NULL)
    for (i = 1; i <= eltqty; i++)
      (*pMesh).EltElset[i] = (*pMesh).EltElset[new_old[i]];
  (*pMesh).EltElset = ut_realloc_1d_int ((*pMesh).EltElset, eltqty + 1);

  // Updating NodeElts
  if ((*pMesh).NodeElts != NULL)
  {
    ut_free_2d_int ((*pMesh).NodeElts, Nodes.NodeQty + 1);
    neut_mesh_init_nodeelts (pMesh, Nodes.NodeQty);
  }

  // Updating Elsets
  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      (*pMesh).Elsets[i][j] = old_new[(*pMesh).Elsets[i][j]];

  (*pMesh).EltQty = eltqty;

  ut_free_1d_int (new_old);
  ut_free_1d_int (old_new);

  return 0;
}

int
neut_mesh_rmelt (struct MESH* pMesh, int elt)
{
  int i, elset, status, node;
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);
  
  elset = (*pMesh).EltElset[elt];

  // Updating EltNodes
  for (i = elt; i < (*pMesh).EltQty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[i], eltnodeqty, (*pMesh).EltNodes[i + 1]);

  // Updating Elsets
  status = ut_array_1d_int_deletencompress
             ((*pMesh).Elsets[elset] + 1, (*pMesh).Elsets[elset][0], elt, 1);
  if (status != 1)
    ut_error_reportbug ();
  else
    (*pMesh).Elsets[elset][0]--;

  // Updating EltCoo
  if ((*pMesh).EltCoo != NULL)
    for (i = elt; i < (*pMesh).EltQty; i++)
      ut_array_1d_memcpy ((*pMesh).EltCoo[i], 3, (*pMesh).EltCoo[i + 1]);
  (*pMesh).EltCoo = ut_realloc_2d_delline ((*pMesh).EltCoo,
                                           (*pMesh).EltQty + 1, (*pMesh).EltQty);

  // Updating EltElset
  if ((*pMesh).EltElset != NULL)
    for (i = elt; i < (*pMesh).EltQty; i++)
      (*pMesh).EltElset[i] = (*pMesh).EltElset[i + 1];
  (*pMesh).EltElset = ut_realloc_1d_int ((*pMesh).EltElset,
                                         (*pMesh).EltQty);

  // Updating NodeElts
  if ((*pMesh).NodeElts != NULL)
    for (i = 0; i < eltnodeqty; i++)
    {
      node = (*pMesh).EltNodes[elt][i];
      status = ut_array_1d_int_deletencompress
	         ((*pMesh).NodeElts[node] + 1, (*pMesh).NodeElts[node][0], elt, 1);
      if (status != 1)
	ut_error_reportbug ();
      (*pMesh).NodeElts[node][0]--;
    }

  (*pMesh).EltQty--;

  return 0;
}

int
neut_mesh_rmelset (struct MESH* pMesh, struct NODES Nodes, int elset)
{
  int i, j, eltqty;
  int* new_old = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  int* old_new = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  ut_array_1d_int_sort ((*pMesh).Elsets[elset] + 1, (*pMesh).Elsets[elset][0]);

  eltqty = 0;
  for (i = 1; i <= (*pMesh).EltQty; i++)
    if ((*pMesh).EltElset[i] != elset)
      new_old[++eltqty] = i;

  for (i = 1; i <= (*pMesh).EltQty; i++)
    old_new[new_old[i]] = i;
  
  // Updating EltNodes
  for (i = 1; i <= eltqty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[i], eltnodeqty, (*pMesh).EltNodes[new_old[i]]);
  (*pMesh).EltNodes = ut_realloc_2d_int_delline ((*pMesh).EltNodes, (*pMesh).EltQty + 1, eltqty + 1);

  // Updating EltCoo
  if ((*pMesh).EltCoo != NULL)
    for (i = 1; i <= eltqty; i++)
      ut_array_1d_memcpy ((*pMesh).EltCoo[i], 3, (*pMesh).EltCoo[new_old[i]]);
  (*pMesh).EltCoo = ut_realloc_2d_delline ((*pMesh).EltCoo, (*pMesh).EltQty + 1, eltqty + 1);

  // Updating EltElset
  if ((*pMesh).EltElset != NULL)
    for (i = 1; i <= eltqty; i++)
      (*pMesh).EltElset[i] = (*pMesh).EltElset[new_old[i]];
  (*pMesh).EltElset = ut_realloc_1d_int ((*pMesh).EltElset, eltqty + 1);

  // Updating Elsets
  (*pMesh).Elsets[elset] = ut_realloc_1d_int ((*pMesh).Elsets[elset], 1);
  (*pMesh).Elsets[elset][0] = 0;

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      (*pMesh).Elsets[i][j] = old_new[(*pMesh).Elsets[i][j]];

  (*pMesh).EltQty = eltqty;

  // Updating NodeElts
  if ((*pMesh).NodeElts != NULL)
  {
    ut_free_2d_int ((*pMesh).NodeElts, Nodes.NodeQty + 1);
    neut_mesh_init_nodeelts (pMesh, Nodes.NodeQty);
  }

  ut_free_1d_int (new_old);
  ut_free_1d_int (old_new);

  return 0;
}
