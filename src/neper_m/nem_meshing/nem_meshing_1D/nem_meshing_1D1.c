/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1D.h"

void
nem_meshing_1D (struct TESS Tess, struct TESSPARA TessPara,
           struct NODES RNodes, struct MESH RMesh1D,
	   int* edge_op,
	   struct NODES* pNodes, struct MESH* pMesh1D)
{
  int i, j, k, elt;
  int ver1 = 0, ver2 = 0;
  struct NODES N;
  struct MESH M;
  int* node_nbs = NULL;
  int* elt_nbs = NULL;
  double cl, pcl;
  double l;
  char* message = ut_alloc_1d_char (8);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (pMesh1D);
  (*pMesh1D).Dimension = 1;
  (*pMesh1D).EltOrder  = 1;
  (*pMesh1D).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh1D).EltType, "tri");

  ut_print_message (0, 2, "1D meshing ... ");

  ut_print_progress (stdout, 0, Tess.EdgeQty, "%3.0f%%", message);
  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    if (edge_op == NULL || edge_op[i] == 0)
    {
      if (TessPara.dboundcl < 0)
      {
	cl  = TessPara.cl;
	pcl  = TessPara.pcl;
      }
      else 
      {
	int var_qty = 2;
	char** vars  = ut_alloc_2d_char (var_qty, 15);
	double* vals = ut_alloc_1d      (var_qty);
	int status = -1;
	double dbound;

	sprintf (vars[0], "body");
	sprintf (vars[1], "true");
	vals[0] = neut_tess_edge_polybodylevelmax (Tess, Tess.PolyBody, i);
	vals[1] = neut_tess_edge_polytruelevelmax (Tess, Tess.PolyTrue, i);

	status = ut_math_eval (TessPara.dbound, var_qty, vars, vals, &dbound);

	if (status == -1)
	  abort ();

	if (dbound == 1)
	{
	  cl  = TessPara.dboundcl;
	  pcl = TessPara.dboundpcl;
	}
	else
	{
	  cl  = TessPara.cl;
	  pcl = TessPara.pcl;
	}

	ut_free_2d_char (vars, var_qty);
	ut_free_1d (vals);
      }

      EdgeMeshing (Tess, i, cl, pcl, *pNodes, &N, &M);
    }
    else
      neut_mesh_elset_mesh (RNodes, RMesh1D, i, &N, &M);
    
    // projecting nodes if needed (remeshing) --------------------------
    
    if (RNodes.NodeQty > 0 && (edge_op == NULL || edge_op[i] == 0))
    {
      int node1, node2;
      double alpha, eps;
      double* rccoo = NULL;
      double* ccoo = NULL;
      double length;

      // recording RNodes node curvilinear coos
      rccoo = ut_alloc_1d (RMesh1D.Elsets[i][0] + 1);

      rccoo[0] = 0;
      l = 0;
      for (j = 1; j <= RMesh1D.Elsets[i][0]; j++)
      {
	elt = RMesh1D.Elsets[i][j];
	node1 = RMesh1D.EltNodes[elt][0];
	node2 = RMesh1D.EltNodes[elt][1];
	l += ut_space_dist (RNodes.NodeCoo[node1], RNodes.NodeCoo[node2]);
	rccoo[j] = l / Tess.EdgeLength[i];
      }

      if (fabs (rccoo[0]) > 1e-6)
      {
	printf ("\nfabs (rccoo[0]) > 1e-6 = %f\n", fabs (rccoo[0]));
	ut_error_reportbug ();
      }

      if (fabs (rccoo[RMesh1D.Elsets[i][0]] - 1) > 1e-6)
      {
	printf ("\nfabs (rccoo[RMesh1D.Elsets[i][0]] - 1) > 1e-6 = %f\n", (rccoo[RMesh1D.Elsets[i][0]] - 1));
	ut_error_reportbug ();
      }

      // recording RNodes node curvilinear coos
      ver1 = Tess.EdgeVerNb[i][0];
      ver2 = Tess.EdgeVerNb[i][1];

      length = ut_space_dist (Tess.VerCoo[ver1], Tess.VerCoo[ver2]);

      ccoo = ut_alloc_1d (N.NodeQty);

      for (j = 0; j < N.NodeQty; j++)
	ccoo[j] = ut_space_dist (N.NodeCoo[1], N.NodeCoo[j + 1]) / length;

      // if (fabs (ccoo[0]) > 1e-6 || fabs (ccoo[N.NodeQty - 1] - 1) > 1e-6)
	// abort ();
      
      // for each N node, calculating its new coordinates in the RMesh

      eps = 1e-9;
      for (j = 0; j < N.NodeQty; j++)
      {
	// looking for elt to project onto from the node coo
	elt = -1;
	alpha = 0;
	for (k = 0; k < RMesh1D.Elsets[i][0] ; k++)
	  if (ccoo[j] > rccoo[k] - eps && ccoo[j] < rccoo[k + 1] + eps)
	  {
	    elt = RMesh1D.Elsets[i][k + 1];
	    alpha = (ccoo[j] - rccoo[k]) / (rccoo[k + 1] - rccoo[k]);
	    break;
	  }
	
	if (elt == -1)
	{
	  ut_print_message (2, 0, "Remeshing 1D failed");
	  ut_print_reportbug ();
	}

	node1 = RMesh1D.EltNodes[elt][0];
	node2 = RMesh1D.EltNodes[elt][1];

	for (k = 0; k < 3; k++)
	  N.NodeCoo[j + 1][k] = (1 - alpha) * RNodes.NodeCoo[node1][k]
			      + alpha * RNodes.NodeCoo[node2][k];
      }

      ut_free_1d (rccoo);
      ut_free_1d (ccoo);
    }

    // sticking nodes to the global nodes ------------------------------

    node_nbs = ut_alloc_1d_int (N.NodeQty + 1);
    // note: we check if the nodes are given in the sense of the edge,
    // or in the opposite sense.  Recording nodes accordingly...

    node_nbs[1] = Tess.EdgeVerNb[i][0];
    node_nbs[N.NodeQty] = Tess.EdgeVerNb[i][1];
    for (j = 2; j < N.NodeQty; j++)
      node_nbs[j] = (*pNodes).NodeQty + j - 1;

    /* adding body nodes to global nodes */
    for (j = 2; j < N.NodeQty; j++)
      neut_nodes_addnode (pNodes, N.NodeCoo[j], N.NodeCl[j]);

    /* renumbering mesh nodes to match global nodes */
    for (j = 1; j <= M.EltQty; j++)
      for (k = 0; k < 2; k++)
	M.EltNodes[j][k] = node_nbs[M.EltNodes[j][k]];

    ut_free_1d_int (node_nbs);
    
    /* renumbering mesh elts */
    elt_nbs = ut_alloc_1d_int (M.EltQty + 1);
    for (j = 1; j <= M.EltQty; j++)
      elt_nbs[j] = (*pMesh1D).EltQty + j;

    for (k = 1; k <= M.Elsets[1][0]; k++)
      M.Elsets[1][k] = elt_nbs[M.Elsets[1][k]];

    ut_free_1d_int (elt_nbs);
    
    for (j = 1; j <= M.EltQty; j++)
      neut_mesh_addelt (pMesh1D, M.EltNodes[j]);
    
    neut_mesh_addelset (pMesh1D, M.Elsets[1] + 1, M.Elsets[1][0]);

    ut_print_progress (stdout, i, Tess.EdgeQty, "%3.0f%%", message);

    neut_mesh_free (&M);
    neut_nodes_free (&N);
  }

  neut_mesh_init_nodeelts (pMesh1D, (*pNodes).NodeQty);
  ut_free_1d_char (message);

  return;
}
