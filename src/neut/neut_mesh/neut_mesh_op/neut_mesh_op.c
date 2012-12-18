/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_op_lcl.h"

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

  Mesh.EltType = ut_alloc_1d_char (strlen (EltType) + 1);
  strcpy (Mesh.EltType, EltType);

  Mesh.EltCoo = NULL;
  Mesh.msize = NULL;

  return Mesh;
}

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
    ut_free_2d_int ((*pMesh).NodeElts, (*pMesh).NodeQty + 1);

  ut_free_2d ((*pMesh).EltCoo, (*pMesh).EltQty + 1);

  ut_free_1d_int ((*pMesh).msize);

  neut_mesh_set_zero (pMesh);

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

  (*pMesh).NodeQty = 0;

  (*pMesh).NodeElts = NULL;
  (*pMesh).EltElset = NULL;

  (*pMesh).EltCoo = NULL;
  (*pMesh).msize = NULL;

  return;
}

void
neut_mesh_memcpy (struct MESH Old, struct MESH* pNew)
{
  int i, eltnodeqty;

  neut_mesh_free (pNew);

  (*pNew).Dimension = Old.Dimension;
  (*pNew).EltOrder = Old.EltOrder;
  (*pNew).EltQty = Old.EltQty;
  (*pNew).EltType = ut_alloc_1d_char (strlen (Old.EltType) + 1);
  strcpy ((*pNew).EltType, Old.EltType);
  
  eltnodeqty = neut_elt_nodeqty ((*pNew).EltType, (*pNew).Dimension, (*pNew).EltOrder);

  (*pNew).EltNodes = ut_alloc_2d_int (Old.EltQty + 1, eltnodeqty);

  for (i = 1; i <= Old.EltQty; i++)
    ut_array_1d_int_memcpy ((*pNew).EltNodes[i], eltnodeqty, Old.EltNodes[i]);

  (*pNew).NodeQty = Old.NodeQty;

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

/* nodes & elements are considered to be numbered contiguously from 1 */
void
neut_mesh_init_nodeelts (struct MESH *pMesh, int NodeQty)
{
  int i, j, eltnodeqty, node;

  if (pMesh == NULL || (*pMesh).EltQty == 0)
    return;

  eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
      (*pMesh).EltOrder);

  if (NodeQty <= 0)
    for (i = 1; i <= (*pMesh).EltQty; i++)
      for (j = 0; j < eltnodeqty; j++)
	NodeQty = ut_num_max (NodeQty, (*pMesh).EltNodes[i][j]);

  (*pMesh).NodeElts = ut_alloc_2d_int (NodeQty + 1, 1);
  (*pMesh).NodeQty = NodeQty;

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
  int i, j, k, elt, neltqty, node;
  int* elts = NULL;
  int* nelts = NULL;

  neut_mesh_init_nodeelts (pMesh, 0);

  (*pMesh).ElsetQty  = 0;
  for (i = 1; i <= (*pMesh).EltQty; i++)
    (*pMesh).ElsetQty = ut_num_max ((*pMesh).EltElset[i], (*pMesh).ElsetQty);

  (*pMesh).Elsets = ut_alloc_2d_int ((*pMesh).ElsetQty + 1, 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
    neut_mesh_elset_addelt (pMesh, (*pMesh).EltElset[i], i);

  // In case of 1D elements, ordering elements in the elsets, as well as
  // the nodes in the elements.
  if ((*pMesh).Dimension == 1)
  {
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
    {
      // nothing to do (don't remove this)
      if ((*pMesh).Elsets[i][0] <= 1)
	continue;

      elts = ut_alloc_1d_int ((*pMesh).Elsets[i][0] + 1);
      elts[0] = -1;

      // Looking for an element that is at the extremity of the elset
      for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      {
	elt = (*pMesh).Elsets[i][j];
	neut_mesh_elt_neighelts (*pMesh, elt, &nelts, &neltqty);

	// neltqty > 2 means that the element is at the extremity of the
	// edge since it must have more than 1 neighbour on one end.
	// neltqty == 1 means that the element is at the extremity of
	// the edge and that the edge is not conected to another one
	// (maybe a Mesh1D with only 1 edge?)
	if (neltqty != 2)
	{
	  elts[1] = elt;
	  break;
	}
      }

      // an exception is when the 1D mesh is reconstructed from a 2D mesh
      // with a single elset (e.g. a mesh of a face). In this case, the
      // 1D elset is a loop, which must be handled in a slightly different way.
      int loopelset = 0;
      if (elts[1] == 0)
      {
	loopelset = 1;
	elts[1] = (*pMesh).Elsets[i][1];
      }

      if (loopelset == 0)
      {
	// Ordering the nodes of the first element - the first node must
	// be at the extremity of the elset
	node = (*pMesh).EltNodes[elts[1]][0];
	if ((*pMesh).NodeElts[node][0] == 2) // means not at the extremity
	  neut_mesh_elt_reversenodes (pMesh, elts[1]);
	else
	{
	  // nothing to do, but checking that the other node does have 2
	  // elements
	  node = (*pMesh).EltNodes[elts[1]][1];
	  if ((*pMesh).NodeElts[node][0] != 2)
	    ut_error_reportbug ();
	}
      }

      for (j = 2; j <= (*pMesh).Elsets[i][0]; j++)
      {
	neut_mesh_elt_neighelts (*pMesh, elts[j - 1], &nelts, &neltqty);

	// for an open elset (general case), the neighbour must belong to
	// the same elset and be different from the already registered
	// element (j - 2).
	if (loopelset == 0)
	{
	  for (k = 0; k < neltqty; k++)
	    if (((*pMesh).EltElset[nelts[k]] == i)
		  && (nelts[k] != elts[j - 2]))
	    {
	      elts[j] = nelts[k];
	      break;
	    }
	}
	// for a loop elset, the neighbour must share the second node of
	// the previous element.
	else
	{
	  for (k = 0; k < neltqty; k++)
	  {
	    if (ut_array_1d_int_eltpos ((*pMesh).EltNodes[nelts[k]], 2,
		  (*pMesh).EltNodes[elts[j - 1]][1]) != -1)
	    {
	      elts[j] = nelts[k];
	      break;
	    }
	  }
	}

	if (elts[j] == 0)
	  ut_error_reportbug ();

	// reversing elt nodes if necessary
	if ((*pMesh).EltNodes[elts[j]][0] != (*pMesh).EltNodes[elts[j - 1]][1])
	{
	  neut_mesh_elt_reversenodes (pMesh, elts[j]);
	  if ((*pMesh).EltNodes[elts[j]][0]
	   != (*pMesh).EltNodes[elts[j - 1]][1])
	    ut_error_reportbug ();
	}
      }

      // recording new element sequence
      ut_array_1d_int_memcpy ((*pMesh).Elsets[i] + 1, (*pMesh).Elsets[i][0], elts + 1);

      ut_free_1d_int (elts);
    }
  }

  ut_free_1d_int (nelts);

  return;
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
  int *eltlist = NULL;
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

	/* searching com elements between the two */
	neut_mesh_nodes_comelts (*pMesh3D, nodes, 2, &eltlist, &qty);

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
	    /* searching com elts between the two */
	    neut_mesh_nodes_comelts (*pMesh2D, nodes, 2, &eltlist, &qty);

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
	    /* searching com elts between the two */
	    neut_mesh_nodes_comelts (*pMesh1D, nodes, 2, &eltlist, &qty);

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
neut_mesh_mergeelsets (struct MESH* pEMesh)
{
  int i, tot;

  tot = 0;
  for (i = 1; i <= (*pEMesh).ElsetQty; i++)
    tot += (*pEMesh).Elsets[i][0];

  (*pEMesh).Elsets[1] = ut_realloc_1d_int ((*pEMesh).Elsets[1], tot + 1);

  for (i = 2; i <= (*pEMesh).ElsetQty; i++)
  {
    ut_array_1d_int_memcpy ((*pEMesh).Elsets[1] + (*pEMesh).Elsets[1][0] + 1,
	(*pEMesh).Elsets[i][0], (*pEMesh).Elsets[i] + 1);
    (*pEMesh).Elsets[1][0] += (*pEMesh).Elsets[i][0];
    ut_free_1d_int ((*pEMesh).Elsets[i]);
  }

  if ((*pEMesh).Elsets[1][0] != tot)
    abort ();

  ut_array_1d_int_sort ((*pEMesh).Elsets[1] + 1, (*pEMesh).Elsets[1][0]);

  (*pEMesh).ElsetQty = 1;

  return;
}

/* returns 1 if meshes match, 0 else */
// TODO: this is brute force method; get less computer intensive
int
neut_mesh_cmp (struct NODES N1, struct MESH M1, struct NODES N2, struct MESH M2)
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

// Turn a quad mesh into a tri mesh by  element subdivision
void
neut_mesh_quad_tri (struct NODES Nodes, struct MESH Mesh, struct MESH* pTMesh)
{
  int i, j, k, elt;
  int** nodepos = NULL;
  int eltnodeqty = -1;
  int triqty = -1;
  // `triqty' is the number of tet/tri elements for an hex/quad element.

  if (strcmp (Mesh.EltType, "quad") != 0)
    ut_error_reportbug ();

  // Vertices of the tets describing the cube elt
  if (Mesh.Dimension == 3)
  {
    triqty = 6;
    eltnodeqty = 4;
    nodepos = ut_alloc_2d_int (triqty, eltnodeqty);

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
  }
  else if (Mesh.Dimension == 2)
  {
    triqty = 2;
    eltnodeqty = 3;
    nodepos = ut_alloc_2d_int (triqty, eltnodeqty);

    nodepos[0][0] = 0;
    nodepos[0][1] = 1;
    nodepos[0][2] = 2;

    nodepos[1][0] = 0;
    nodepos[1][1] = 2;
    nodepos[1][2] = 3;
  }
  else
    ut_error_reportbug ();

  // General data
  (*pTMesh).EltType = ut_alloc_1d_char (4);
  strcpy ((*pTMesh).EltType, "tri");
  (*pTMesh).Dimension = Mesh.Dimension;
  (*pTMesh).EltOrder = 1;

  (*pTMesh).EltQty = 0;
  (*pTMesh).EltNodes = ut_alloc_2d_int (Mesh.EltQty * triqty + 1, eltnodeqty);

  // Elt data
  if (Mesh.EltElset != NULL)
    (*pTMesh).EltElset = ut_alloc_1d_int (Mesh.EltQty * triqty + 1);

  for (i = 1; i <= Mesh.EltQty; i++)
    for (j = 0; j < triqty; j++)
    {
      elt = ++((*pTMesh).EltQty);

      for (k = 0; k < eltnodeqty; k++)
	(*pTMesh).EltNodes[elt][k] = Mesh.EltNodes[i][nodepos[j][k]];

      if (Mesh.EltElset != NULL)
	(*pTMesh).EltElset[elt] = Mesh.EltElset[i];
    }

  neut_mesh_init_elsets (pTMesh);

  neut_mesh_init_nodeelts (pTMesh, Nodes.NodeQty);

  ut_free_2d_int (nodepos, triqty);

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

// Ordering elements to all point towards the same side of the face
// (all topological, no buggy normal determination)
void
neut_mesh_orderelsets (struct MESH* pMesh)
{
  int i, j, elt, neltqty;
  int* id = NULL;
  int qty;
  int* elt_ref = NULL;
  int* nelts = NULL;
  int* elts = ut_alloc_1d_int (2);
  int* nodes = NULL;
  int nodeqty;
  int ori1, ori2;
  int* tmp = NULL;

  if ((*pMesh).Dimension != 2)
    ut_error_reportbug ();

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
  {
    id = ut_alloc_1d_int ((*pMesh).Elsets[i][0]);
    elt_ref = ut_alloc_1d_int ((*pMesh).EltQty + 1);

    id[0] = (*pMesh).Elsets[i][1];
    elt = id[0];
    qty = 1;
    neut_mesh_elt_elset_neighelts (*pMesh, elt, i, &tmp, &neltqty);
    ut_array_1d_int_memcpy (id + qty, neltqty, tmp);
    ut_free_1d_int (tmp);
    tmp = NULL;

    qty += neltqty;
    for (j = 0; j < qty; j++)
      elt_ref[id[j]] = elt;

    int pos = 1;
    while (pos < qty)
    {
      elt = id[pos];

      // checking element orientation
      elts[0] = elt_ref[elt];
      elts[1] = elt;
      neut_mesh_elts_comnodes (*pMesh, elts, 2, &nodes, &nodeqty);
      if (nodeqty < 2)
	ut_error_reportbug ();

      neut_mesh_elt_nodes_ori (*pMesh, elts[0], nodes, &ori1);
      neut_mesh_elt_nodes_ori (*pMesh, elts[1], nodes, &ori2);

      if (ori1 == 0 || ori2 == 0)
      {
	neut_debug_mesh (stdout, *pMesh);
	printf ("elts = %d %d\n", elts[0], elts[1]);
	printf ("nodes = %d %d\n", nodes[0], nodes[1]);
	printf ("ori1 = %d ori2 = %d\n", ori1, ori2);
	ut_error_reportbug ();
      }

      if (ori1 == ori2)
	neut_mesh_elt_reversenodes (pMesh, elt);

      // recording element neighbours
      neut_mesh_elt_elset_neighelts (*pMesh, elt, i, &nelts, &neltqty);
      for (j = 0; j < neltqty; j++)
	if (elt_ref[nelts[j]] == 0)
	{
	  elt_ref[nelts[j]] = elt;
	  id[qty++] = nelts[j];
	}

      pos++;
    }

    ut_free_1d_int (elt_ref);
  }

  ut_free_1d_int (nelts);
  ut_free_1d_int (elts);
  ut_free_1d_int (nodes);

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


void
neut_mesh_elt_reversenodes (struct MESH *pMesh, int eltnb)
{
  // implemented for 1D and 2D only.

  // For 2D, the 1st and 2nd order nodes are to be considered separetely
  // for reversing.
  if ((*pMesh).Dimension == 1)
  {
    ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb], 2);
  }
  else if ((*pMesh).Dimension == 2)
  {
    if (! strcmp ((*pMesh).EltType, "tri"))
    {
      if ((*pMesh).EltOrder == 1)
	ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 2);
      else if ((*pMesh).EltOrder == 2)
      {
	ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 2);
	ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 3, 3);
      }
      else
	abort ();
    }
    else if (! strcmp ((*pMesh).EltType, "quad"))
    {
      if ((*pMesh).EltOrder == 1)
	ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 3);
      else if ((*pMesh).EltOrder == 2)
      {
	ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 3);
	ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 4, 4);
      }
      else
	abort ();
    }
    else
      ut_error_reportbug ();
  }
  else
    abort ();

  return;
}
void
neut_mesh_elts_switch_pair (struct MESH *pMesh, int n1, int n2)
{
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  ut_array_2d_int_switchlines ((*pMesh).EltNodes, eltnodeqty, n1, n2);

  return;
}

/* RAM could be improved by more elegant exchange of lines (not through
 * a big copy eltnodes array */
void
neut_mesh_elts_switch (struct MESH *pMesh, int* elt_nbs)
{
  int i, j;
  int** eltnodes = NULL;
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  eltnodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);

  for (i = 1; i <= elt_nbs[0]; i++)
    ut_array_1d_int_memcpy (eltnodes[i], eltnodeqty, (*pMesh).EltNodes[i]);

  for (i = 1; i <= (*pMesh).EltQty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[elt_nbs[i]],
	                eltnodeqty, eltnodes[i]);

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      (*pMesh).Elsets[i][j] = elt_nbs[(*pMesh).Elsets[i][j]];

  if ((*pMesh).NodeElts != NULL)
  {
    int nodeqty = (*pMesh).NodeQty;
    ut_free_2d_int ((*pMesh).NodeElts, nodeqty + 1);
    neut_mesh_init_nodeelts (pMesh, nodeqty);

    // the code below would be a little faster but does not work
    /*
    for (i = 1; i <= (*pMesh).NodeQty; i++)
      for (j = 1; j <= (*pMesh).NodeElts[i][0]; j++)
	(*pMesh).NodeElts[i][j] = elt_nbs[(*pMesh).NodeElts[i][j]];
    */
  }

  ut_free_2d_int (eltnodes, (*pMesh).EltQty);

  return;
}

/* node_nbs[...] = node qty then new pos */
void
neut_mesh_nodes_switch (struct MESH* pMesh, int* node_nbs)
{
  int i;
  int j, eltnodeqty;

  eltnodeqty =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  /* neut_mesh_switch (pMesh, node_nbs, NULL); */
  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pMesh).EltNodes[i][j] = node_nbs[(*pMesh).EltNodes[i][j]];

  return;
}

void
neut_mesh_switch (struct MESH *pMesh, int *nodes_old_new, int *elts_old_new, int* elsets_old_new)
{
  int i, j, eltnodeqty;

  /* correcting elt nodes */
  if (nodes_old_new != NULL)
  {
    eltnodeqty =
      neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

    for (i = 1; i <= (*pMesh).EltQty; i++)
      for (j = 0; j < eltnodeqty; j++)
	(*pMesh).EltNodes[i][j] = nodes_old_new[(*pMesh).EltNodes[i][j]];
  }

  /* correcting elset elts */
  if (elts_old_new != NULL)
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
      for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
	(*pMesh).Elsets[i][j] = elts_old_new[(*pMesh).Elsets[i][j]];

  /* correcting elt elsets */
  if (elsets_old_new != NULL)
    for (i = 1; i <= (*pMesh).EltQty; i++)
      (*pMesh).EltElset[i] = elsets_old_new[(*pMesh).EltElset[i]];

  return;
}

void
neut_mesh_renumber_continuous (struct MESH *pMesh, int* node_nbs,
                               int* elt_nbs, int* elset_nbs)
{
  int i, elt_nb_max, elset_nb_max;
  int* nodes_old_new = NULL;
  int* elts_old_new = NULL;
  int* elsets_old_new = NULL;
  
  int node_nb_max = ut_array_1d_int_max (node_nbs + 1, node_nbs[0]);

  if (node_nbs != NULL)
  {
    nodes_old_new = ut_alloc_1d_int (node_nb_max + 1);
    for (i = 1; i <= node_nbs[0]; i++)
      nodes_old_new[node_nbs[i]] = i;
  }

  if (elt_nbs != NULL)
  {
    elt_nb_max = ut_array_1d_int_max (elt_nbs + 1, (*pMesh).EltQty);

    elts_old_new = ut_alloc_1d_int (elt_nb_max + 1);
    for (i = 1; i <= (*pMesh).EltQty; i++)
      elts_old_new[elt_nbs[i]] = i;
    
    elset_nb_max = ut_array_1d_int_max (elset_nbs + 1, (*pMesh).ElsetQty);

    elsets_old_new = ut_alloc_1d_int (elset_nb_max + 1);
    for (i = 1; i <= elset_nbs[0]; i++)
      elsets_old_new[elset_nbs[i]] = i;

    neut_mesh_switch (pMesh, nodes_old_new, elts_old_new, elsets_old_new);
  }

  ut_free_1d_int (nodes_old_new);
  ut_free_1d_int (elts_old_new);
  ut_free_1d_int (elsets_old_new);
  
  return;
}
