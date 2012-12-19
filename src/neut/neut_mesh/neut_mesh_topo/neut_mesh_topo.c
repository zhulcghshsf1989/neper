/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_topo_lcl.h"

void
neut_mesh_elts_comnodes (struct MESH Mesh, int *elts, int eltqty,
		       int** pnodes, int *pnodeqty)
{
  int i;
  int *tmp1 = NULL;
  int *tmp2 = NULL;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  ut_free_1d_int (*pnodes);

  tmp1 = ut_alloc_1d_int (eltnodeqty);
  tmp2 = ut_alloc_1d_int (eltnodeqty);

  ut_array_1d_int_memcpy (tmp1, eltnodeqty, Mesh.EltNodes[elts[0]]);
  (*pnodeqty) = eltnodeqty;

  for (i = 1; i < eltqty; i++)
  {
    ut_array_1d_int_inter (tmp1, *pnodeqty, Mesh.EltNodes[elts[i]],
	                   eltnodeqty, tmp2, pnodeqty);
    ut_array_1d_int_memcpy (tmp1, *pnodeqty, tmp2);
  }

  (*pnodes) = ut_alloc_1d_int (*pnodeqty);
  ut_array_1d_int_memcpy (*pnodes, *pnodeqty, tmp1);

  ut_free_1d_int (tmp1);
  ut_free_1d_int (tmp2);

  return;
}

void
neut_mesh_elt_nodes_ori (struct MESH Mesh, int elt, int* nodes, int* pori)
{
  int pos1, pos2;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, 1);

  pos1 = ut_array_1d_int_eltpos (Mesh.EltNodes[elt], eltnodeqty, nodes[0]);
  pos2 = ut_array_1d_int_eltpos (Mesh.EltNodes[elt], eltnodeqty, nodes[1]);

  if (ut_num_rotpos (0, eltnodeqty - 1, pos1, 1) == pos2)
    (*pori) = 1;
  else if (ut_num_rotpos (0, eltnodeqty - 1, pos1, -1) == pos2)
    (*pori) = -1;
  else
    (*pori) = 0;

  return;
}

void
neut_mesh_nodes_comelts (struct MESH Mesh, int *nodes, int nodeqty,
		       int **pelts, int *peltqty)
{
  int i;
  int *tmp1 = NULL;
  int *tmp2 = NULL;
  int tmp2qty = -1;

  if (Mesh.NodeElts == NULL)
  {
    ut_print_message (2, 0, "neut_mesh_nodes_comelts requires NodeElts\n");
    ut_error_reportbug ();
  }

  if (nodeqty == 1)
  {
    (*peltqty) = Mesh.NodeElts[nodes[0]][0];
    (*pelts) = ut_alloc_1d_int (*peltqty);
    ut_array_1d_int_memcpy (*pelts, *peltqty, Mesh.NodeElts[nodes[0]] + 1);
    return;
  }

  for (i = 0; i < nodeqty; i++)
    if (nodes[i] > Mesh.NodeQty)
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

  (*pelts) = ut_alloc_1d_int (tmp2qty);
  ut_array_1d_int_memcpy (*pelts, tmp2qty, tmp2);
  ut_array_1d_int_sort (*pelts, *peltqty);

  ut_free_1d_int (tmp1);
  ut_free_1d_int (tmp2);

  return;
}

void
neut_mesh_nodes_allelts (struct MESH Mesh, int *nodes, int nodeqty,
		    int** pelts, int *peltqty)
{
  int i, j, node;

  (*peltqty) = 0;
  for (i = 0; i < nodeqty; i++)
  {
    node = nodes[i];
    for (j = 1; j <= Mesh.NodeElts[node][0]; j++)
      if (ut_array_1d_int_eltpos (*pelts, *peltqty, Mesh.NodeElts[node][j]) == -1)
      {
	(*pelts) = ut_realloc_1d_int (*pelts, (*peltqty) + 1);
	(*pelts)[(*peltqty)++] = Mesh.NodeElts[node][j];
      }
  }

  ut_array_1d_int_sort ((*pelts), *peltqty);

  return;
}

int
neut_mesh_nodes_comelt (struct MESH Mesh, int* nodes, int nodeqty, int* pelt)
{
  int* tmp = NULL;
  int tmpqty;

  neut_mesh_nodes_comelts (Mesh, nodes, nodeqty, &tmp, &tmpqty);

  if (tmpqty == 1)
    (*pelt) = tmp[0];
  else
    (*pelt) = -1;

  ut_free_1d_int (tmp);

  return ut_num_min (*pelt, 0);
}

int
neut_mesh_nodes_elsets_comelts (struct MESH Mesh, int *nodes, int nodeqty,
                             int* elsets, int elsetqty, int** pelts, int* peltqty)
{
  int i, j;
  int* tmp = NULL;
  int tmpqty;

  ut_free_1d_int (*pelts);
  (*pelts) = NULL;

  if (Mesh.EltElset == NULL)
  {
    printf ("neut_mesh_nodes_elsets_comelts needs EltElset\n");
    ut_error_reportbug ();
  }

  neut_mesh_nodes_comelts (Mesh, nodes, nodeqty, &tmp, &tmpqty);

  (*peltqty) = 0;
  for (i = 0; i < tmpqty; i++)
    for (j = 0; j < elsetqty; j++)
      if (ut_array_1d_int_eltpos
	    (elsets, elsetqty, Mesh.EltElset[tmp[i]]) != -1)
      {
	(*pelts) = ut_realloc_1d_int (*pelts, (*peltqty) + 1);
	(*pelts)[(*peltqty)++] = tmp[i];
        break;
      }

  ut_free_1d_int (tmp);

  return 0;
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

int
neut_mesh_elt0d_elts1d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
		       int **pelt1d, int *pelt1dqty)
{
  int node = Mesh0D.EltNodes[elt0d][0];

  (*pelt1dqty) = Mesh1D.NodeElts[node][0];
  (*pelt1d) = ut_alloc_1d_int (*pelt1dqty);

  ut_array_1d_int_memcpy (*pelt1d, *pelt1dqty, Mesh1D.NodeElts[node] + 1);

  return *pelt1dqty;
}

int
neut_mesh_elt0d_elsets1d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
			 int** pelset1d, int *pelset1dqty)
{
  int* elt1d = NULL;
  int elt1dqty;

  ut_free_1d_int (*pelset1d);
  (*pelset1d) = NULL;

  neut_mesh_elt0d_elts1d (Mesh0D, elt0d, Mesh1D, &elt1d, &elt1dqty);

  if (Mesh1D.ElsetQty == 1 && elt1dqty == 1)
  {
    (*pelset1d) = ut_alloc_1d_int (2);
    (*pelset1d)[0] = -1;
    (*pelset1d)[1] = Mesh1D.EltElset[elt1d[0]];
    (*pelset1dqty) = 2;
  }
  else
    neut_mesh_elts_elsets (Mesh1D, elt1d, elt1dqty, pelset1d, pelset1dqty);

  ut_free_1d_int (elt1d);

  return *pelset1dqty;
}

int
neut_mesh_elt1d_elts2d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
		       int** pelt2d, int *pelt2dqty)
{
  int nodeqty = neut_elt_nodeqty (Mesh1D.EltType, 1, Mesh1D.EltOrder);

  ut_free_1d_int (*pelt2d);
  (*pelt2d) = NULL;

  if (Mesh2D.Dimension != 2)
    abort ();
  
  neut_mesh_nodes_comelts (Mesh2D, Mesh1D.EltNodes[elt1d],
			 nodeqty, pelt2d, pelt2dqty);

  return *pelt2dqty;
}

int
neut_mesh_elt1d_elsets2d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
			 int** pelset2d, int *pelset2dqty)
{
  int* elt2d = NULL;
  int elt2dqty;

  ut_free_1d_int (*pelset2d);
  (*pelset2d) = NULL;

  neut_mesh_elt1d_elts2d (Mesh1D, elt1d, Mesh2D, &elt2d, &elt2dqty);

  if (Mesh2D.ElsetQty == 1 && elt2dqty == 1)
  {
    (*pelset2d) = ut_alloc_1d_int (2);
    (*pelset2dqty) = 2;
    (*pelset2d)[0] = -1;
    (*pelset2d)[1] = Mesh2D.EltElset[elt2d[0]];
  }
  else
    neut_mesh_elts_elsets (Mesh2D, elt2d, elt2dqty, pelset2d, pelset2dqty);

  ut_free_1d_int (elt2d);

  return *pelset2dqty;
}

int
neut_mesh_elt1d_elts3d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
		       struct MESH Mesh3D, int** pelt3d, int* pelt3dqty)
{
  int i, j, qty, elt2dqty;
  int* elt2d = NULL;
  int* tmp = NULL;

  ut_free_1d_int (*pelt3d);

  neut_mesh_elt1d_elts2d (Mesh1D, elt1d, Mesh2D, &elt2d, &elt2dqty);

  (*pelt3dqty) = 0;
  for (i = 0; i < elt2dqty; i++)
  {
    neut_mesh_elt2d_elts3d (Mesh2D, elt2d[i], Mesh3D, &tmp, &qty);
    for (j = 0; j < qty; j++)
      if (ut_array_1d_int_eltpos (*pelt3d, (*pelt3dqty), tmp[j]) == -1)
      {
	(*pelt3d) = ut_realloc_1d_int (*pelt3d, (*pelt3dqty) + 1);
	(*pelt3d)[(*pelt3dqty)++] = tmp[j];
      }
  }

  ut_free_1d_int (elt2d);
  ut_free_1d_int (tmp);

  return *pelt3dqty;
}

int
neut_mesh_elt0d_elts3d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
		       struct MESH Mesh2D, struct MESH Mesh3D, int **pelt3d, int *pelt3dqty)
{
  int i, j, qty, elt1dqty;
  int* elt1d = NULL;
  int* tmp = NULL;

  neut_mesh_elt0d_elts1d (Mesh0D, elt0d, Mesh1D, &elt1d, &elt1dqty);

  (*pelt3dqty) = 0;
  ut_free_1d_int (*pelt3d);
  (*pelt3d) = NULL;

  for (i = 0; i < elt1dqty; i++)
  {
    neut_mesh_elt1d_elts3d (Mesh1D, elt1d[i], Mesh2D, Mesh3D, &tmp, &qty);
    for (j = 0; j < qty; j++)
      if (ut_array_1d_int_eltpos ((*pelt3d), (*pelt3dqty), tmp[j]) == -1)
      {
	(*pelt3d) = ut_realloc_1d_int (*pelt3d, (*pelt3dqty) + 1);
	(*pelt3d)[(*pelt3dqty)++] = tmp[j];
      }
  }

  ut_free_1d_int (elt1d);
  ut_free_1d_int (tmp);

  return *pelt3dqty;
}


void
neut_mesh_elt_boundmesh (struct NODES Nodes, struct MESH Mesh, int elt, struct MESH* pBMesh)
{
  int i;
  int* othernode = NULL;
  int *tab = Mesh.EltNodes[elt]; // do not free; this is a shortcut
  double* eq = ut_alloc_1d (4);

  (*pBMesh).Dimension = Mesh.Dimension - 1;
  (*pBMesh).EltOrder = Mesh.EltOrder;
  (*pBMesh).EltType
    = ut_realloc_1d_char ((*pBMesh).EltType, strlen (Mesh.EltType) + 1);
  strcpy ((*pBMesh).EltType, Mesh.EltType);

  if (Mesh.Dimension == 1)
  {
    (*pBMesh).EltQty = 2;
    (*pBMesh).ElsetQty = 0;

    if ((*pBMesh).EltNodes == NULL)
      (*pBMesh).EltNodes = ut_alloc_2d_int (3, 1);

    (*pBMesh).EltNodes[1][0] = tab[0];
    (*pBMesh).EltNodes[2][0] = tab[1];
  }

  else if (Mesh.Dimension == 2)
  {
    if (strcmp (Mesh.EltType, "tri") == 0)
      (*pBMesh).EltQty = 3;
    else if (strcmp (Mesh.EltType, "quad") == 0)
      (*pBMesh).EltQty = 4;
    else
      abort ();

    (*pBMesh).ElsetQty = 0;

    if ((*pBMesh).EltNodes == NULL)
    {
      if ((*pBMesh).EltOrder == 1)
	(*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 2);
      else if ((*pBMesh).EltOrder == 2)
	(*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 3);
      else
	abort ();
    }

    if (strcmp (Mesh.EltType, "tri") == 0)
    {
      (*pBMesh).EltNodes[1][0] = tab[0];
      (*pBMesh).EltNodes[1][1] = tab[1];

      (*pBMesh).EltNodes[2][0] = tab[1];
      (*pBMesh).EltNodes[2][1] = tab[2];

      (*pBMesh).EltNodes[3][0] = tab[2];
      (*pBMesh).EltNodes[3][1] = tab[0];

      if (Mesh.EltOrder == 2)
      {
	(*pBMesh).EltNodes[1][2] = tab[3];
	(*pBMesh).EltNodes[2][2] = tab[4];
	(*pBMesh).EltNodes[3][2] = tab[5];
      }
    }
    else if (strcmp (Mesh.EltType, "quad") == 0)
    {
      (*pBMesh).EltNodes[1][0] = tab[0];
      (*pBMesh).EltNodes[1][1] = tab[1];

      (*pBMesh).EltNodes[2][0] = tab[1];
      (*pBMesh).EltNodes[2][1] = tab[2];

      (*pBMesh).EltNodes[3][0] = tab[2];
      (*pBMesh).EltNodes[3][1] = tab[3];

      (*pBMesh).EltNodes[4][0] = tab[3];
      (*pBMesh).EltNodes[4][1] = tab[0];

      if (Mesh.EltOrder == 2)
      {
	(*pBMesh).EltNodes[1][2] = tab[4];
	(*pBMesh).EltNodes[2][2] = tab[5];
	(*pBMesh).EltNodes[3][2] = tab[6];
	(*pBMesh).EltNodes[4][2] = tab[7];
      }
    }
    else
      abort ();
  }

  else if (Mesh.Dimension == 3)
  {
    (*pBMesh).ElsetQty = 0;

    if (strcmp ((*pBMesh).EltType, "tri") == 0)
    {
      (*pBMesh).EltQty = 4;
      othernode = ut_alloc_1d_int ((*pBMesh).EltQty + 1);

      if ((*pBMesh).EltNodes == NULL)
      {
	if ((*pBMesh).EltOrder == 1)
	  (*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 3);
	else if ((*pBMesh).EltOrder == 2)
	  (*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 6);
	else
	  abort ();
      }

      (*pBMesh).EltNodes[1][0] = tab[0];
      (*pBMesh).EltNodes[1][1] = tab[1];
      (*pBMesh).EltNodes[1][2] = tab[2];
      othernode[1] = tab[3];

      (*pBMesh).EltNodes[2][0] = tab[1];
      (*pBMesh).EltNodes[2][1] = tab[2];
      (*pBMesh).EltNodes[2][2] = tab[3];
      othernode[2] = tab[0];

      (*pBMesh).EltNodes[3][0] = tab[2];
      (*pBMesh).EltNodes[3][1] = tab[3];
      (*pBMesh).EltNodes[3][2] = tab[0];
      othernode[3] = tab[1];

      (*pBMesh).EltNodes[4][0] = tab[3];
      (*pBMesh).EltNodes[4][1] = tab[0];
      (*pBMesh).EltNodes[4][2] = tab[1];
      othernode[4] = tab[2];

      if (Mesh.EltOrder == 2)
      {
	(*pBMesh).EltNodes[1][3] = tab[4];
	(*pBMesh).EltNodes[1][4] = tab[5];
	(*pBMesh).EltNodes[1][5] = tab[6];

	(*pBMesh).EltNodes[2][3] = tab[5];
	(*pBMesh).EltNodes[2][4] = tab[8];
	(*pBMesh).EltNodes[2][5] = tab[9];

	(*pBMesh).EltNodes[3][3] = tab[8];
	(*pBMesh).EltNodes[3][4] = tab[7];
	(*pBMesh).EltNodes[3][5] = tab[6];

	(*pBMesh).EltNodes[4][3] = tab[7];
	(*pBMesh).EltNodes[4][4] = tab[4];
	(*pBMesh).EltNodes[4][5] = tab[9];
      }
    }
    else if (strcmp ((*pBMesh).EltType, "quad") == 0)
    {
      (*pBMesh).EltQty = 6;
      othernode = ut_alloc_1d_int ((*pBMesh).EltQty + 1);

      if ((*pBMesh).EltNodes == NULL)
      {
	if ((*pBMesh).EltOrder == 1)
	  (*pBMesh).EltNodes = ut_alloc_2d_int (Mesh.EltQty + 1, 4);
	else if ((*pBMesh).EltOrder == 2)
	  (*pBMesh).EltNodes = ut_alloc_2d_int (Mesh.EltQty + 1, 8);
	else
	  abort ();
      }

      (*pBMesh).EltNodes[1][0] = tab[0];
      (*pBMesh).EltNodes[1][1] = tab[1];
      (*pBMesh).EltNodes[1][2] = tab[2];
      (*pBMesh).EltNodes[1][3] = tab[3];
      othernode[1] = tab[4];

      (*pBMesh).EltNodes[2][0] = tab[4];
      (*pBMesh).EltNodes[2][1] = tab[5];
      (*pBMesh).EltNodes[2][2] = tab[6];
      (*pBMesh).EltNodes[2][3] = tab[7];
      othernode[2] = tab[0];

      (*pBMesh).EltNodes[3][0] = tab[0];
      (*pBMesh).EltNodes[3][1] = tab[1];
      (*pBMesh).EltNodes[3][2] = tab[5];
      (*pBMesh).EltNodes[3][3] = tab[4];
      othernode[3] = tab[2];

      (*pBMesh).EltNodes[4][0] = tab[3];
      (*pBMesh).EltNodes[4][1] = tab[2];
      (*pBMesh).EltNodes[4][2] = tab[6];
      (*pBMesh).EltNodes[4][3] = tab[7];
      othernode[4] = tab[1];

      (*pBMesh).EltNodes[5][0] = tab[1];
      (*pBMesh).EltNodes[5][1] = tab[2];
      (*pBMesh).EltNodes[5][2] = tab[6];
      (*pBMesh).EltNodes[5][3] = tab[5];
      othernode[5] = tab[3];

      (*pBMesh).EltNodes[6][0] = tab[0];
      (*pBMesh).EltNodes[6][1] = tab[3];
      (*pBMesh).EltNodes[6][2] = tab[7];
      (*pBMesh).EltNodes[6][3] = tab[4];
      othernode[6] = tab[1];

      if (Mesh.EltOrder == 2)
      {
	(*pBMesh).EltNodes[1][4] = tab[8];
	(*pBMesh).EltNodes[1][5] = tab[11];
	(*pBMesh).EltNodes[1][6] = tab[13];
	(*pBMesh).EltNodes[1][7] = tab[9];

	(*pBMesh).EltNodes[2][4] = tab[16];
	(*pBMesh).EltNodes[2][5] = tab[18];
	(*pBMesh).EltNodes[2][6] = tab[19];
	(*pBMesh).EltNodes[2][7] = tab[17];

	(*pBMesh).EltNodes[3][4] = tab[8];
	(*pBMesh).EltNodes[3][5] = tab[12];
	(*pBMesh).EltNodes[3][6] = tab[16];
	(*pBMesh).EltNodes[3][7] = tab[10];

	(*pBMesh).EltNodes[4][4] = tab[13];
	(*pBMesh).EltNodes[4][5] = tab[14];
	(*pBMesh).EltNodes[4][6] = tab[19];
	(*pBMesh).EltNodes[4][7] = tab[15];

	(*pBMesh).EltNodes[5][4] = tab[11];
	(*pBMesh).EltNodes[5][5] = tab[14];
	(*pBMesh).EltNodes[5][6] = tab[18];
	(*pBMesh).EltNodes[5][7] = tab[12];

	(*pBMesh).EltNodes[6][4] = tab[9];
	(*pBMesh).EltNodes[6][5] = tab[15];
	(*pBMesh).EltNodes[6][6] = tab[17];
	(*pBMesh).EltNodes[6][7] = tab[10];
      }
    }

    else
      abort ();
  }

  // if 2D mesh and Nodes present, making 2D elements point outwards from the 3D element
  if ((*pBMesh).Dimension == 2 && Nodes.NodeQty > 0)
    for (i = 1; i <= (*pBMesh).EltQty; i++)
    {
      neut_mesh_elt_eq (*pBMesh, Nodes, i, eq);
      if (ut_space_planeside (eq, Nodes.NodeCoo[othernode[i]] - 1) == 1)
	neut_mesh_elt_reversenodes (pBMesh, i);
    }

  ut_free_1d (eq);
  ut_free_1d_int (othernode);

  return;
}

// Generates the 2D mesh corresponding to a 3D mesh.
// It also retuns the FaceQty value and the FacePoly array, which hold
// the number of 2D elsets and their relations to the 3D elsets.  The
// terms `FaceQty' and `FacePoly' are used in reference to the Tess
// structure, and closely match them, except that FacePoly is indexed O
// (vs 1 in the Tess structure)
void
neut_mesh3d_mesh2d (struct NODES Nodes, struct MESH Mesh3D,
		    struct MESH *pMesh2D, int ***pFacePoly,
		    int *pFaceQty, int verbosity)
{
  int i, j, elt3dqty, elset, dir;
  int *elt3d = NULL;
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
    neut_mesh_elt_boundmesh (Nodes, Mesh3D, i, &EltMesh2D);

    for (j = 1; j <= facetqty; j++)
    {
      neut_mesh_elt2d_elts3d (EltMesh2D, j, Mesh3D, &elt3d, &elt3dqty);

      if (elt3d[0] >= i) // means the face has not been considered yet
      {
	if (elt3dqty == 1)
	{
	  // finding the outer surfaces by gathering all the surface
	  // elements that lie in the same plane in a given surface.
	  // All 2D elements of a given surface have the same normal
	  // (same sign).

	  neut_mesh_skinelt_fod (Nodes, EltMesh2D, j, &fodeq,
	                                    &fodqty, &(elset3d[1]), &dir);

	  if (dir == -1)
	    neut_mesh_elt_reversenodes (&EltMesh2D, j);

	  elset3d[0] = Mesh3D.EltElset[elt3d[0]];
	  // RQ 10/12/2012: switched elset3d[0] and [1] above for
	  // consistency with FacePoly ordering in the TESS structure.
	  // If one of the poly is < 0 (meaning we are at the domain
	  // boundary) we put it in [1].
	}
	else
	{
	  elset3d[0] = Mesh3D.EltElset[elt3d[0]];
	  elset3d[1] = Mesh3D.EltElset[elt3d[1]];
	  ut_array_1d_int_sort (elset3d, 2);
	}

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
// terms are used in reference to the Tess structure, and closely match
// them, except that EdgeFaceQty andEdgeFaceNb are indexed O (vs 1 in
// the Tess structure)
void
neut_mesh2d_mesh1d (struct MESH Mesh2D,
		    struct MESH *pMesh1D, int ***pEdgeFaceNb,
		    int** pEdgeFaceQty, int *pEdgeQty, int verbosity)
{
  int i, j, k, elt2dqty, elset2dqty, elset;
  struct MESH EltMesh1D;
  int *elt2d = NULL;
  int *elset2d = NULL;
  char* progress = ut_alloc_1d_char (20);
  struct NODES fake;

  neut_nodes_set_zero (&fake);

  neut_mesh_set_zero (&EltMesh1D);

  if (Mesh2D.EltElset == NULL)
  {
    ut_print_message (2, 0, "neut_mesh2d_mesh1d needs Mesh2D.EltElset.\n");
    ut_error_reportbug ();
  }

  (*pEdgeQty) = 0;

  neut_mesh_free (pMesh1D);
  (*pMesh1D).Dimension = 1;
  (*pMesh1D).EltOrder = Mesh2D.EltOrder;
  (*pMesh1D).EltType = ut_alloc_1d_char (strlen (Mesh2D.EltType) + 1);
  strcpy ((*pMesh1D).EltType, Mesh2D.EltType);

  // Recording all 1d elts by looping on the edges of all 2d elts.

  if (verbosity)
    ut_print_progress (stdout, 0, Mesh2D.EltQty, "[1/2] %3.0f%%", progress);
  
  for (i = 1; i <= Mesh2D.EltQty; i++)
  {
    neut_mesh_elt_boundmesh (fake, Mesh2D, i, &EltMesh1D);

    for (j = 1; j <= EltMesh1D.EltQty; j++)
    {
      neut_mesh_elt1d_elts2d   (EltMesh1D, j, Mesh2D, &elt2d, &elt2dqty);
      neut_mesh_elt1d_elsets2d (EltMesh1D, j, Mesh2D, &elset2d, &elset2dqty);

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

  neut_mesh_init_nodeelts (pMesh1D, 0);
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

      neut_mesh_elt1d_elsets2d (*pMesh1D, i, Mesh2D, &elset2d, &elset2dqty);

      (*pEdgeQty) = elset;
      (*pEdgeFaceQty) = ut_realloc_1d_int (*pEdgeFaceQty, *pEdgeQty);
      (*pEdgeFaceQty)[(*pEdgeQty) - 1] = elset2dqty;
      (*pEdgeFaceNb)
	= ut_realloc_2d_int_addline (*pEdgeFaceNb, *pEdgeQty, elset2dqty);
      ut_array_1d_int_memcpy ((*pEdgeFaceNb)[(*pEdgeQty) - 1], elset2dqty, elset2d);

      // loop on the neighbours

      int testqty, elt1d;
      int* test = NULL;

      neut_mesh_elt_neighelts (*pMesh1D, i, &test, &testqty);

      for (j = 0; j < testqty; j++)
      {
	elt1d = test[j];

	neut_mesh_elt1d_elsets2d (*pMesh1D, elt1d, Mesh2D, &elset2d, &elset2dqty);

	// the elt has the same topology, therefore same edge -
	// recording it and adding non-registered neighbours to the test list.
	if (! ut_array_1d_int_diff (elset2d, elset2dqty,
	      (*pEdgeFaceNb)[elset - 1], (*pEdgeFaceQty)[elset - 1]))
	{
	  (*pMesh1D).EltElset[elt1d] = elset;

	  int* tmp = NULL;
	  int tmpqty;
	  neut_mesh_elt_neighelts (*pMesh1D, elt1d, &tmp, &tmpqty);
	  // If there is 2 neighbours, we are in the body of an edge;
	  // recording elt which is not the previous one.
	  // If there is more, we are done (end of an edge). 
	  // printf ("neut_mesh_elt_neighelts returns tmpqty = %d tmp = ", tmpqty);
	  // ut_array_1d_int_fprintf (stdout, tmp, tmpqty, "%d");
	  if (tmpqty == 2)
	    for (k = 0; k < tmpqty; k++)
	      if ((*pMesh1D).EltElset[tmp[k]] == 0)
	      {
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

  neut_mesh_init_elsets (pMesh1D);

  neut_mesh_free (&EltMesh1D);
  ut_free_1d_int (elt2d);
  ut_free_1d_int (elset2d);
  ut_free_1d_char (progress);
  neut_nodes_free (&fake);

  return;
}

// Generates the 0D mesh corresponding to a 1D mesh.  It also retuns the
// VerQty value and the VerEdgeQty and VerEdgeNb arrays, which hold the
// number of 0D elsets and their relations to the 1D elsets.  These
// terms are used in reference to the Tess structure, and closely match
// them, except that VerEdgeQty and VerEdgeNb are indexed O (vs 1 in the
// Tess structure)
void
neut_mesh1d_mesh0d (struct MESH Mesh1D,
		    struct MESH *pMesh0D, int ***pVerEdgeNb,
		    int** pVerEdgeQty, int *pVerQty, int verbosity)
{
  int i, j, elt1dqty, elset1dqty;
  struct MESH EltMesh0D;
  int *elt1d = NULL;
  int *elset1d = NULL;
  char* progress = ut_alloc_1d_char (20);
  struct NODES fake;

  neut_nodes_set_zero (&fake);

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
  (*pMesh0D).EltType = ut_alloc_1d_char (strlen (Mesh1D.EltType) + 1);
  strcpy ((*pMesh0D).EltType, Mesh1D.EltType);

  // for every 1D elt, for every of its vertices, we look for all
  // its parent 1D elts (can be any number).

  if (verbosity)
    ut_print_progress (stdout, 0, Mesh1D.EltQty, "[1/2] %3.0f%%", progress);
  
  for (i = 1; i <= Mesh1D.EltQty; i++)
  {
    neut_mesh_elt_boundmesh (fake, Mesh1D, i, &EltMesh0D);
    
    for (j = 1; j <= 2; j++)
    {
      neut_mesh_elt0d_elts1d   (EltMesh0D, j, Mesh1D, &elt1d, &elt1dqty);
      // elt1dqty == 1 means that an edge has a dead end.
      if (elt1dqty == 1)
	ut_error_reportbug ();

      neut_mesh_elt0d_elsets1d (EltMesh0D, j, Mesh1D, &elset1d, &elset1dqty);

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

  neut_mesh_init_nodeelts (pMesh0D, 0);

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

    neut_mesh_elt0d_elsets1d (*pMesh0D, i, Mesh1D, &elset1d, &elset1dqty);

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
  neut_nodes_free (&fake);

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


int
neut_mesh_elt2d_elts3d (struct MESH Mesh2D, int elt2d, struct MESH Mesh3D,
		       int **pelt3d, int *pelt3dqty)
{
  int nodeqty = neut_elt_nodeqty (Mesh2D.EltType, 2, Mesh2D.EltOrder);

  neut_mesh_nodes_comelts (Mesh3D, Mesh2D.EltNodes[elt2d],
			 nodeqty, pelt3d, pelt3dqty);

  return (*pelt3dqty >= 0 && *pelt3dqty <= 2) ? *pelt3dqty : -1;
}

void
neut_mesh_elt3d_elts2d (struct MESH Mesh3D, int elt3d, struct MESH Mesh2D, int** pelts2d, int* pelt2dqty)
{
  int i, nb;
  struct MESH Facet;
  int* pos = ut_alloc_1d_int (4);
  struct NODES fake;

  neut_nodes_set_zero (&fake);

  ut_free_1d_int (*pelts2d);

  neut_mesh_set_zero (&Facet);

  neut_mesh_elt_boundmesh (fake, Mesh3D, elt3d, &Facet);

  (*pelt2dqty) = 0;

  for (i = 0; i < 4; i++)
  {
    neut_mesh_nodes_comelt (Mesh2D, Facet.EltNodes[i + 1], 3, &nb);
    if (nb > 0)
    {
      (*pelts2d) = ut_realloc_1d_int (*pelts2d, (*pelt2dqty) + 1);
      (*pelts2d)[(*pelt2dqty)++] = nb;
    }
  }

  neut_mesh_free (&Facet);
  ut_free_1d_int (pos);
  neut_nodes_free (&fake);

  return;
}

void
neut_mesh_elts_elsets (struct MESH Mesh, int* elts, int eltqty, int** pelsets, int* pelsetqty)
{
  int i, elset;

  ut_free_1d_int (*pelsets);
  (*pelsets) = NULL;

  (*pelsetqty) = 0;
  for (i = 0; i < eltqty; i++)
    if (elts[i] >= 1 && elts[i] <= Mesh.EltQty)
    {
      elset = Mesh.EltElset[elts[i]];
      if (ut_array_1d_int_eltpos (*pelsets, *pelsetqty, elset) == -1)
      {
	(*pelsets) = ut_realloc_1d_int (*pelsets, (*pelsetqty) + 1);
	(*pelsets)[(*pelsetqty)++] = elset;
      }
    }

  ut_array_1d_int_sort (*pelsets, *pelsetqty);

  return;
}

void
neut_mesh_facepoly (struct NODES Nodes, struct MESH Mesh2D, struct MESH Mesh3D, int*** pfacepoly)
{
  int i, elt2d, elt3dqty, elset3dqty;
  int* elts3d = NULL;

  (*pfacepoly) = ut_alloc_1d_pint (Mesh2D.ElsetQty + 1);

  int fodqty = 0;
  double** fodeq = NULL;
  int dir;

  // for every face, taking one element (the 1st) and looking for its 2
  // 3D elements. Their elsets are the face polys.
  // in case of only one elt, recording one elset as -7.
  for (i = 1; i <= Mesh2D.ElsetQty; i++)
  {
    elt2d = Mesh2D.Elsets[i][1];

    neut_mesh_elt2d_elts3d (Mesh2D, elt2d, Mesh3D, &elts3d, &elt3dqty);
    if (elt3dqty != 2 && elt3dqty != 1)
      ut_error_reportbug ();
    neut_mesh_elts_elsets (Mesh3D, elts3d, elt3dqty, &((*pfacepoly)[i]),
	&elset3dqty);

    if (elset3dqty != 2 && elset3dqty != 1)
      ut_error_reportbug ();

    if (elset3dqty == 1)
    {
      (*pfacepoly)[i][1] = (*pfacepoly)[i][0];
      neut_mesh_skinelt_fod (Nodes, Mesh2D, elt2d, &fodeq,
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
  int* elsets2d = NULL;
  int* elts2d = NULL;

  (*pedgefacenb) = ut_alloc_1d_pint (Mesh1D.ElsetQty + 1);
  (*pedgefaceqty) = ut_alloc_1d_int (Mesh1D.ElsetQty + 12);

  // for every edge, taking one element (the 1st) and looking for its
  // 2D elements. Their elsets are the edge faces.
  for (i = 1; i <= Mesh1D.ElsetQty; i++)
  {
    elt1d = Mesh1D.Elsets[i][1];

    neut_mesh_elt1d_elts2d (Mesh1D, elt1d, Mesh2D, &elts2d, &elt2dqty);
    if (elt2dqty == 0)
      ut_error_reportbug ();
    neut_mesh_elts_elsets (Mesh2D, elts2d, elt2dqty, &elsets2d,
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
  int* elts1d = NULL;
  int* elsets1d = NULL;

  (*pveredgenb) = ut_alloc_1d_pint (Mesh0D.ElsetQty + 1);
  (*pveredgeqty) = ut_alloc_1d_int (Mesh0D.ElsetQty + 12);

  // for every ver, looking for its 1D elements. Their elsets are the
  // ver edges.
  for (i = 1; i <= Mesh0D.ElsetQty; i++)
  {
    elt0d = Mesh0D.Elsets[i][1];

    neut_mesh_elt0d_elts1d (Mesh0D, elt0d, Mesh1D, &elts1d, &elt2dqty);
    if (elt2dqty == 0)
      ut_error_reportbug ();
    neut_mesh_elts_elsets (Mesh1D, elts1d, elt2dqty, &elsets1d,
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
neut_mesh_elt_neighelts (struct MESH Mesh, int elt, int** pnelts, int* pneltqty)
{
  int i, j, eltqty;
  int* elts = NULL;
  int* nodes = ut_alloc_1d_int (3);
  int boundqty;
  int boundnodeqty;
  int** seq = NULL;

  ut_free_1d_int (*pnelts);
  (*pnelts) = NULL;
  
  boundqty = -1;
  boundnodeqty = -1;
  if (Mesh.Dimension == 1)
  {
    boundqty = 2;
    boundnodeqty = 1;
  }
  else if (Mesh.Dimension == 2)
  {
    if (! strcmp (Mesh.EltType, "tri"))
      boundqty = 3;
    else if (! strcmp (Mesh.EltType, "quad"))
      boundqty = 4;
    else
      ut_error_reportbug ();

    boundnodeqty = 2;
  }
  else if (Mesh.Dimension == 3)
  {
    if (! strcmp (Mesh.EltType, "tri"))
    {
      boundqty = 4;
      boundnodeqty = 3;
    }
    else if (! strcmp (Mesh.EltType, "quad"))
    {
      boundqty = 6;
      boundnodeqty = 4;
    }
    else
      ut_error_reportbug ();
  }
  else
    ut_error_reportbug ();

  // seq: node ids of every elt boundary (facets for a 3D elt, node for a 1D elt)

  seq = ut_alloc_2d_int (boundqty, boundnodeqty);

  if (Mesh.Dimension == 1)
  {
    seq[0][0] = 0;
    seq[1][0] = 1;
  }
  else if (Mesh.Dimension == 2)
  {
    if (! strcmp (Mesh.EltType, "tri"))
    {
      seq[0][0] = 0; seq[0][1] = 1;
      seq[1][0] = 1; seq[1][1] = 2;
      seq[2][0] = 2; seq[2][1] = 0;
    }
    else if (! strcmp (Mesh.EltType, "quad"))
    {
      seq[0][0] = 0; seq[0][1] = 1;
      seq[1][0] = 1; seq[1][1] = 2;
      seq[2][0] = 2; seq[2][1] = 3;
      seq[3][0] = 3; seq[3][1] = 0;
    }
  }
  else if (Mesh.Dimension == 3)
  {
    if (! strcmp (Mesh.EltType, "tri"))
    {
      seq[0][0] = 0; seq[0][1] = 1; seq[0][2] = 2;
      seq[1][0] = 1; seq[1][1] = 2; seq[1][2] = 3;
      seq[2][0] = 2; seq[2][1] = 3; seq[2][2] = 0;
      seq[3][0] = 0; seq[3][1] = 1; seq[3][2] = 3;
    }
    else if (! strcmp (Mesh.EltType, "quad"))
    {
      seq[0][0] = 0; seq[0][1] = 1; seq[0][2] = 2; seq[0][2] = 3;
      seq[1][0] = 4; seq[1][1] = 5; seq[1][2] = 6; seq[1][2] = 7;
      seq[2][0] = 1; seq[2][1] = 2; seq[2][2] = 6; seq[2][2] = 5;
      seq[3][0] = 0; seq[3][1] = 3; seq[3][2] = 7; seq[3][2] = 4;
      seq[4][0] = 2; seq[4][1] = 3; seq[4][2] = 7; seq[4][2] = 6;
      seq[5][0] = 1; seq[5][1] = 0; seq[5][2] = 4; seq[5][2] = 5;
    }
  }

  // for each elt boundary, looking for the neighbouring elt
  (*pneltqty) = 0;
  for (i = 0; i < boundqty; i++)
  {
    for (j = 0; j < boundnodeqty; j++)
      nodes[j] = Mesh.EltNodes[elt][seq[i][j]];

    neut_mesh_nodes_comelts (Mesh, nodes, boundnodeqty, &elts, &eltqty);

    if ((eltqty == 0)
     || (eltqty == 1 && elts[0] != elt)
     || (eltqty  > 2 && Mesh.Dimension == 3))
      ut_error_reportbug ();

    if (eltqty == 1)
      (*pneltqty) = 0;
    else
      for (j = 0; j < eltqty; j++)
	if (elts[j] != elt)
	{
	  (*pnelts) = ut_realloc_1d_int (*pnelts, (*pneltqty) + 1);
	  (*pnelts)[(*pneltqty)++] = elts[j];
	}
  }

  ut_free_1d_int (nodes);
  ut_free_2d_int (seq, boundqty);
  ut_free_1d_int (elts);
  
  return 0;
}

int
neut_mesh_elt_elset_neighelts (struct MESH Mesh, int elt, int elset, int** pnelts, int* pneltqty)
{
  int i;

  if (Mesh.EltElset == NULL)
  {
    printf ("neut_mesh_elt_elset_neighelts needs EltElset\n");
    ut_error_reportbug ();
  }

  neut_mesh_elt_neighelts (Mesh, elt, pnelts, pneltqty);

  for (i = 0; i < (*pneltqty); i++)
    if (Mesh.EltElset[(*pnelts)[i]] != elset)
      (*pnelts)[i] = 0;

  (*pneltqty) -= ut_array_1d_int_deletencompress (*pnelts, (*pneltqty), 0, INT_MAX);

  return 0;
}

void
neut_mesh_elts_boundelts (struct MESH Mesh, int* elts, int eltqty,
                         int** pboundelts, int* pboundeltqty)
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

  eltneighs = ut_alloc_1d_pint (eltqty);
  eltneighqty = ut_alloc_1d_int (eltqty);
  
  (*pboundelts) = ut_alloc_1d_int (eltqty);

  ut_array_1d_int_inv (elts, eltqty, &eltsinv, &eltsmax);

  // for each elt, recording neighbouring elts
  for (i = 0; i < eltqty; i++)
    neut_mesh_elt_neighelts (Mesh, elts[i], &(eltneighs[i]), &(eltneighqty[i]));

  // for each elt, checking if all its neighbours are in the elts array.
  // If not, the elt is a bound elt, and recorded as such.
  // note: if eltneighqty != 4, the elt is systematically a bound elt.

  (*pboundeltqty) = 0;
  for (i = 0; i < eltqty; i++)
  {
    if (eltneighqty[i] != elt3dfaceqty)
      (*pboundelts)[(*pboundeltqty)++] = elts[i];
    else
    {
      for (j = 0; j < elt3dfaceqty; j++)
      {
	neigh = eltneighs[i][j];
	if (neigh > eltsmax)
	{
	  (*pboundelts)[(*pboundeltqty)++] = elts[i];
	  break;
	}
	else
	  if (eltsinv[neigh] == -1)
	  {
	    (*pboundelts)[(*pboundeltqty)++] = elts[i];
	    break;
	  }
      }
    }
  }

  (*pboundelts) = ut_realloc_1d_int ((*pboundelts), *pboundeltqty);

  ut_free_2d_int (eltneighs, eltqty);
  ut_free_1d_int (eltneighqty);
  ut_free_1d_int (eltsinv);

  return;
}

int
neut_mesh_node_dim (struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D, struct MESH Mesh3D, int node)
{
  int j, dim = -1;

  int eltnodeqty0d, eltnodeqty1d, eltnodeqty2d;

  eltnodeqty0d = neut_elt_nodeqty (Mesh0D.EltType, Mesh0D.Dimension, Mesh0D.EltOrder);
  eltnodeqty1d = neut_elt_nodeqty (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder);
  eltnodeqty2d = neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);

  for (j = 1; dim == -1 && j <= Mesh0D.EltQty; j++)
    if (ut_array_1d_int_eltpos (Mesh0D.EltNodes[j], eltnodeqty0d, node) != -1)
      dim = 0;
      
  for (j = 1; dim == -1 && j <= Mesh1D.EltQty; j++)
    if (ut_array_1d_int_eltpos (Mesh1D.EltNodes[j], eltnodeqty1d, node) != -1)
      dim = 1;

  for (j = 1; dim == -1 && j <= Mesh2D.EltQty; j++)
    if (ut_array_1d_int_eltpos (Mesh2D.EltNodes[j], eltnodeqty2d, node) != -1)
      dim = 2;

  if (dim == -1)
  {
    Mesh3D = Mesh3D;
    dim = 3;
  }

  return dim;
}

int
neut_mesh_skinelt_fod (struct NODES Nodes, struct MESH Mesh, int elt,
		       double*** pfodeq, int* pfodqty, int* pfod,
		       int* pdir)
{
  int i, status;
  double *eq = ut_alloc_1d (4);
  int found = 0;

  status = 0;

  neut_mesh_elt_eq (Mesh, Nodes, elt, eq);

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
