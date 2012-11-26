/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Partition.h"
#ifdef HAVE_LIBSCOTCH

void
PartitionNodes (struct IN In, SCOTCH_Mesh * pSCMesh, SCOTCH_Arch * pArch,
		struct NODES *pNodes, struct MESH *pMesh0D,
		struct MESH *pMesh1D, struct MESH *pMesh2D,
		struct MESH *pMesh, struct NSET *pNSet0D,
	       	struct NSET *pNSet1D, struct NSET *pNSet2D,
		struct PART *pPart)
{
  SCOTCH_Num *parttab;
  int eltnodeqty =
    neut_elt_nodeqty ("tri", (*pMesh).Dimension, (*pMesh).EltOrder);
  int i, j, qty;
  SCOTCH_Graph Graph;
  SCOTCH_Strat Strat;
  int *node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  SCOTCH_stratInit (&Strat);
  SCOTCH_graphInit (&Graph);

  int status = SCOTCH_stratGraphMap (&Strat,
			"r{job=t,map=t,poli=S,sep=m{type=h,vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{dif=1,rem=1,pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}|m{type=h,vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{dif=1,rem=1,pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}}");
  if (status != 0)
    status = SCOTCH_stratGraphMap (&Strat,
			  "b{job=t,map=t,poli=S,sep=m{type=h,vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{dif=1,rem=1,pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}|m{type=h,vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{dif=1,rem=1,pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}}");

  parttab = ut_alloc_1d_int ((*pMesh).EltQty * eltnodeqty * 2 + 2);
  if (strcmp (In.partmethod, "none") != 0)
    {
      SCOTCH_meshGraph (pSCMesh, &Graph);

      if (In.partmode == 0)
	SCOTCH_graphPart (&Graph, In.partqty, &Strat, parttab + 1);
      else if (In.partmode == 1)
	SCOTCH_graphMap (&Graph, pArch, &Strat, parttab + 1);
    }
  else if (strcmp (In.partmethod, "none") == 0)
    {
      for (j = 1; j <= (*pNodes).NodeQty; j++)
	parttab[j] = In.partqty * (j - 1) / (*pNodes).NodeQty;
    }
  else
    abort ();

  (*pPart).node_parts = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  (*pPart).node_parts[0] = (*pNodes).NodeQty;
  ut_array_1d_int_memcpy ((*pPart).node_parts + 1, (*pNodes).NodeQty,
			  parttab + 1);
  ut_free_1d_int (parttab);

  /* renumbering nodes and elt nodes */
  if (In.partrenumber)
    {
      qty = 0;
      for (i = 0; i < In.partqty; i++)
	for (j = 1; j <= (*pNodes).NodeQty; j++)
	  if ((*pPart).node_parts[j] == i)
	    node_nbs[j] = ++qty;
      node_nbs[0] = (*pNodes).NodeQty;
      ut_array_1d_int_sort ((*pPart).node_parts + 1, (*pNodes).NodeQty);

      if (qty != (*pNodes).NodeQty)
	ut_error_reportbug ();

      // renumbering nset nodes 
      for (i = 1; i <= (*pNSet0D).qty; i++)
	for (j = 0; j < (*pNSet0D).nodeqty[i]; j++)
	  (*pNSet0D).nodes[i][j] = node_nbs[(*pNSet0D).nodes[i][j]];

      for (i = 1; i <= (*pNSet1D).qty; i++)
	for (j = 0; j < (*pNSet1D).nodeqty[i]; j++)
	  (*pNSet1D).nodes[i][j] = node_nbs[(*pNSet1D).nodes[i][j]];

      for (i = 1; i <= (*pNSet2D).qty; i++)
	for (j = 0; j < (*pNSet2D).nodeqty[i]; j++)
	  (*pNSet2D).nodes[i][j] = node_nbs[(*pNSet2D).nodes[i][j]];

      neut_nodes_renumber_switch (pNodes, node_nbs);
      neut_nodes_renumber_switch_mesh (pMesh, node_nbs);
      neut_nodes_renumber_switch_mesh (pMesh2D, node_nbs);
      neut_nodes_renumber_switch_mesh (pMesh1D, node_nbs);
      neut_nodes_renumber_switch_mesh (pMesh0D, node_nbs);
      ut_array_1d_int_sort ((*pPart).node_parts + 1, (*pNodes).NodeQty);
    }

  for (i = 0; i < In.partqty; i++)
    (*pPart).nodeqty[i] =
      ut_array_1d_int_nbofthisval ((*pPart).node_parts + 1, (*pNodes).NodeQty,
				   i);

  ut_free_1d_int (node_nbs);

  return;
}

void
PartitionElts (struct IN In, struct MESH *pMesh, struct PART *pPart)
{
  int i, j, qty, partqty;
  int **coms = ut_alloc_2d_int ((*pPart).qty, 2);
  int *elt_nbs = NULL;

  (*pPart).elt_parts = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pPart).elt_parts[0] = (*pMesh).EltQty;
  ut_array_1d_int_set ((*pPart).elt_parts + 1, (*pMesh).EltQty, -1);

  elt_nbs = ut_alloc_1d_int ((*pMesh).EltQty + 1);

  ut_array_1d_int_zero ((*pPart).eltqty, In.partqty);

  if (strcmp (In.partmethod, "none") != 0)
    {
      ut_print_message (0, 3, "Minimizing communications ...\n");
      PartitionElts_Match (In, pMesh, pPart);
      if (In.partbalancing > 0)
	{
	  ut_print_message (0, 3, "Balancing partitions ... ");
	  PartitionElts_Balancing (pMesh, pPart, In.partbalancing);
	}
    }
  else if (strcmp (In.partmethod, "none") == 0)
    {
      ut_print_message (0, 3, "Direct partitioning by numbers ...\n");
      PartitionElts_ByNumber (In, pMesh, pPart);
    }

  qty = 0;
  for (i = 0; i < In.partqty; i++)
    for (j = 1; j <= (*pMesh).EltQty; j++)
      if ((*pPart).elt_parts[j] == i)
	elt_nbs[j] = ++qty;
  elt_nbs[0] = (*pMesh).EltQty;

  if (In.partrenumber)
    {
      neut_mesh_renumber_switch (pMesh, elt_nbs);
      ut_array_1d_int_sort ((*pPart).elt_parts + 1, (*pMesh).EltQty);
    }

  for (i = 1; i <= (*pMesh).EltQty; i++)
    if (neut_mesh_elt_isbound (*pMesh, i, (*pPart).node_parts))
      neut_mesh_elt_coms (*pMesh, i, (*pPart).qty, (*pPart).node_parts,
			  (*pPart).elt_parts, coms, &partqty);

  // reinitializing eltelset (brute force method)
  if ((*pMesh).EltElset != NULL)
    neut_mesh_init_eltelset (pMesh, NULL);

  ut_free_2d_int (coms, (*pPart).qty);
  ut_free_1d_int (elt_nbs);

  return;
}

void
PartitionStats (int partnbr, struct MESH Mesh, struct PART Part)
{
  int i, cutqty;
  int nmin, nmax, emin, emax;

  emin = ut_array_1d_int_min (Part.eltqty, partnbr);
  emax = ut_array_1d_int_max (Part.eltqty, partnbr);
  nmin = ut_array_1d_int_min (Part.nodeqty, partnbr);
  nmax = ut_array_1d_int_max (Part.nodeqty, partnbr);

  cutqty = 0;
  for (i = 1; i <= Mesh.EltQty; i++)
    cutqty += neut_mesh_elt_comqty (Mesh, i, Part.node_parts, Part.elt_parts);

  ut_print_message (0, 3, "Nb of nodes per partition: min = %d, max = %d\n",
		    nmin, nmax);
  ut_print_message (0, 3, "Nb of elts  per partition: min = %d, max = %d\n",
		    emin, emax);
  ut_print_message (0, 3, "Nb of communications: %d\n", cutqty);

  return;
}

#endif
