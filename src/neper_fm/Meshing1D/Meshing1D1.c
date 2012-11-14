/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Meshing1D.h"

void
Meshing1D (struct GEO Geo, struct GEOPARA GeoPara,
           struct NODES* pRNodes, struct MESH* pRMesh1D,
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

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (pMesh1D);
  (*pMesh1D).Dimension = 1;
  (*pMesh1D).EltOrder  = 1;
  (*pMesh1D).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh1D).EltType, "tri");

  ut_print_message (0, 2, "1D meshing ... %3d%%", 0);
  fflush (stdout);

  for (i = 1; i <= Geo.EdgeQty; i++)
  {
    if (edge_op == NULL || edge_op[i] == 0)
    {
      if (GeoPara.dboundcl < 0)
      {
	cl  = GeoPara.cl;
	pcl  = GeoPara.pcl;
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
	vals[0] = neut_geo_edge_polybodylevelmax (Geo, Geo.PolyBody, i);
	vals[1] = neut_geo_edge_polytruelevelmax (Geo, Geo.PolyTrue, i);

	status = ut_math_eval (GeoPara.dbound, var_qty, vars, vals, &dbound);

	if (status == -1)
	  abort ();

	if (dbound == 1)
	{
	  cl  = GeoPara.dboundcl;
	  pcl = GeoPara.dboundpcl;
	}
	else
	{
	  cl  = GeoPara.cl;
	  pcl = GeoPara.pcl;
	}

	ut_free_2d_char (vars, var_qty);
	ut_free_1d (vals);
      }

      EdgeMeshing (Geo, i, cl, pcl, *pNodes, &N, &M);
    }
    else
      neut_mesh_elset_mesh (*pRNodes, *pRMesh1D, i, &N, &M);
    
    // projecting nodes if needed (remeshing) --------------------------
    
    if (pRNodes != NULL && (edge_op == NULL || edge_op[i] == 0))
    {
      int node1, node2;
      double alpha, eps;
      double* rccoo = NULL;
      double* ccoo = NULL;
      double length;

      // recording RNodes node curvilinear coos
      rccoo = ut_alloc_1d ((*pRMesh1D).Elsets[i][0] + 1);

      rccoo[0] = 0;
      l = 0;
      for (j = 1; j <= (*pRMesh1D).Elsets[i][0]; j++)
      {
	elt = (*pRMesh1D).Elsets[i][j];
	node1 = (*pRMesh1D).EltNodes[elt][0];
	node2 = (*pRMesh1D).EltNodes[elt][1];
	l += ut_space_dist ((*pRNodes).NodeCoo[node1], (*pRNodes).NodeCoo[node2]);
	rccoo[j] = l / Geo.EdgeLength[i];
      }

      if (fabs (rccoo[0]) > 1e-6)
      {
	printf ("\nfabs (rccoo[0]) > 1e-6 = %f\n", fabs (rccoo[0]));
	ut_error_reportbug ();
      }

      if (fabs (rccoo[(*pRMesh1D).Elsets[i][0]] - 1) > 1e-6)
      {
	printf ("\nfabs (rccoo[(*pRMesh1D).Elsets[i][0]] - 1) > 1e-6 = %f\n", (rccoo[(*pRMesh1D).Elsets[i][0]] - 1));
	ut_error_reportbug ();
      }

      // recording RNodes node curvilinear coos
      ver1 = Geo.EdgeVerNb[i][0];
      ver2 = Geo.EdgeVerNb[i][1];

      length = ut_space_dist (Geo.VerCoo[ver1], Geo.VerCoo[ver2]);

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
	for (k = 0; k < (*pRMesh1D).Elsets[i][0] ; k++)
	  if (ccoo[j] > rccoo[k] - eps && ccoo[j] < rccoo[k + 1] + eps)
	  {
	    elt = (*pRMesh1D).Elsets[i][k + 1];
	    alpha = (ccoo[j] - rccoo[k]) / (rccoo[k + 1] - rccoo[k]);
	    break;
	  }
	
	if (elt == -1)
	{
	  ut_print_message (2, 0, "Remeshing 1D failed");
	  ut_print_reportbug ();
	}

	node1 = (*pRMesh1D).EltNodes[elt][0];
	node2 = (*pRMesh1D).EltNodes[elt][1];

	for (k = 0; k < 3; k++)
	  N.NodeCoo[j + 1][k] = (1 - alpha) * (*pRNodes).NodeCoo[node1][k]
			      + alpha * (*pRNodes).NodeCoo[node2][k];
      }

      ut_free_1d (rccoo);
      ut_free_1d (ccoo);

      // node1 = (*pRMesh1D).EltNodes[elt][0];
      // ver1 = Geo.EdgeVerNb[i][0];
      // double dist = ut_space_dist ((*pRNodes).NodeCoo[node1], Geo.VerCoo[ver1]);
      // if (dist > 1e-6)
	// printf ("\nedge %d: dist stuff\n", i);
    }

    /*
    if (pRNodes != NULL)
    {
      int coo_qty, pos;
      double alpha, alpha2, eps;
      int flip_x;
      double** coo = ut_alloc_2d ((*pRMesh1D).Elsets[i][0] + 1, 3);
      double** coo_new = NULL;
      double* coo_av = ut_alloc_1d (3);
      double** evect = ut_alloc_2d (3, 3);
      double*  eval  = ut_alloc_1d (3);
      double* ncoo   = ut_alloc_1d (3);

      // recording RNodes node coos
      for (j = 1; j <= (*pRMesh1D).Elsets[i][0]; j++)
      {
	elt = (*pRMesh1D).Elsets[i][j];
	node = (*pRMesh1D).EltNodes[elt][0];
	ut_array_1d_memcpy (coo[j - 1], 3, (*pRNodes).NodeCoo[node]);
      }
      elt = (*pRMesh1D).Elsets[i][(*pRMesh1D).Elsets[i][0]];
      node = (*pRMesh1D).EltNodes[elt][1];
      ut_array_1d_memcpy (coo[(*pRMesh1D).Elsets[i][0]],
			  3, (*pRNodes).NodeCoo[node]);
      coo_qty = (*pRMesh1D).Elsets[i][0] + 1;

      printf ("\ncoo_qty = %d\n", coo_qty);

      printf ("nodes =\n");
      for (j = 0; j < coo_qty; j++)
	printf ("%f %f %f\n", coo[j][0], coo[j][1], coo[j][2]);

      // bringing the coo around the centre
      for (j = 0; j < coo_qty; j++)
	ut_array_1d_add (coo_av, coo[j], 3, coo_av);
      ut_array_1d_scale (coo_av, 3, -1./coo_qty);
      for (j = 0; j < coo_qty; j++)
	ut_array_1d_add (coo[j], coo_av, 3, coo[j]);

      printf ("\ntranslated nodes =\n");
      for (j = 0; j < coo_qty; j++)
	printf ("%f %f %f\n", coo[j][0], coo[j][1], coo[j][2]);

      ut_vector_covar (coo, coo_qty, evect, eval);
      ol_g_inverse (evect, evect);

      // rotating coo in the X-Y-Z csys
      
      for (j = 0; j < coo_qty; j++)
	ol_g_vect_vect (evect, coo[j], coo[j]);

      for (j = 0; j < 3; j++)
	printf ("eval %d = %f about %f %f %f\n", j, eval[j], evect[j][0], evect[j][1], evect[j][2]);

      printf ("\nrotated nodes =\n");
      for (j = 0; j < coo_qty; j++)
	printf ("%f %f %f\n", coo[j][0], coo[j][1], coo[j][2]);

      flip_x = (coo[0][0] < coo[coo_qty - 1][0]) ? 0 : 1;

      if (flip_x)
	for (j = 0; j < coo_qty; j++)
	  ut_array_1d_scale (coo[j], 3, -1);

      printf ("\nflipped nodes =\n");
      for (j = 0; j < coo_qty; j++)
	printf ("%f %f %f\n", coo[j][0], coo[j][1], coo[j][2]);

      ver1 = Geo.EdgeVerNb[i][0];
      ver2 = Geo.EdgeVerNb[i][1];

      // for each N node, calculating its new coordinates in the RMesh
      
      double length = ut_space_dist (Geo.VerCoo[ver1], Geo.VerCoo[ver2]);
      eps = 1e-9 * length;
      coo_new = ut_alloc_2d (N.NodeQty + 1, 3);

      // reversing transformations for backward transformation
      ut_array_1d_scale (coo_av, 3, -1);
      ol_g_inverse (evect, evect);

      for (j = 1; j <= N.NodeQty; j++)
      {
	printf ("N node %d/%d\n", j, N.NodeQty);

	// calculating relative coordinate in the N/M mesh
	alpha = ut_space_dist (N.NodeCoo[j], Geo.VerCoo[ver1]) / length;
	printf ("alpha = %f\n", alpha);
	
	// calculating X-coo in the rotated RMesh
	ncoo[0] = (1 - alpha) * coo[0][0] + alpha * coo[coo_qty - 1][0];
	printf ("ncoo[0] = (1 - %f) * %f + %f * %f = %f\n", alpha, coo[0][0], alpha, coo[coo_qty - 1][0], ncoo[0]);

	// looking for elt to project onto from the node x-coo's for
	// body nodes only (boundary nodes are already and should keep
	// at the right place)

	pos = -1;
	alpha2 = 0;
	for (k = 0; k < coo_qty - 1 ; k++)
	  if (ncoo[0] > coo[k][0] - eps && ncoo[0] < coo[k + 1][0] + eps)
	  {
	    pos = k;
	    alpha2 = (ncoo[0] - coo[k][0]) / (coo[k + 1][0] - coo[k][0]);
	    break;
	  }

	printf ("projecting on elt %d with alpha2 = %f\n", pos, alpha2);

	if (pos == -1)
	{
	  ut_print_message (2, 0, "Remeshing 1D: backward projection failed");
	  ut_print_reportbug ();
	}

	for (k = 0; k < 3; k++)
	  coo_new[j][k] = (1 - alpha2) * coo[pos][k]
			      + alpha2 * coo[pos + 1][k];

	printf ("coo_new\n");
	ut_array_1d_fprintf (stdout, coo_new[j], 3, "%f");

	// flip back if needed
	if (flip_x)
	  ut_array_1d_scale (coo_new[j], 3, -1);

	printf ("flipped coo_new\n");
	ut_array_1d_fprintf (stdout, coo_new[j], 3, "%f");

	// rotate back to the initial orientation
	ol_g_vect_vect (evect, coo_new[j], coo_new[j]);

	printf ("rotated coo_new\n");
	ut_array_1d_fprintf (stdout, coo_new[j], 3, "%f");

	// translating back to the initial position
	ut_array_1d_add (coo_new[j], coo_av, 3, coo_new[j]);

	printf ("translated coo_new\n");
	ut_array_1d_fprintf (stdout, coo_new[j], 3, "%f");

	ut_array_1d_memcpy (N.NodeCoo[j], 3, coo_new[j]);
      }
	
      ut_free_2d (coo, (*pRMesh1D).Elsets[i][0] + 1);
      ut_free_2d (coo_new, N.NodeQty + 1);
      ut_free_1d (coo_av);
      ut_free_1d (ncoo);
      ut_free_2d (evect, 3);
      ut_free_1d (eval);
    }
  */

    // sticking nodes to the global nodes ------------------------------

    node_nbs = ut_alloc_1d_int (N.NodeQty + 1);
    // note: we check if the nodes are given in the sense of the edge,
    // or in the opposite sense.  Recording nodes accordingly...
    /*double dist1, dist2;
    dist1 = ut_space_dist (Geo.VerCoo[Geo.EdgeVerNb[i][0]], N.NodeCoo[1]);
    dist2 = ut_space_dist (Geo.VerCoo[Geo.EdgeVerNb[i][1]], N.NodeCoo[1]);
    */
    node_nbs[1] = Geo.EdgeVerNb[i][0];
    node_nbs[N.NodeQty] = Geo.EdgeVerNb[i][1];
    for (j = 2; j < N.NodeQty; j++)
      node_nbs[j] = (*pNodes).NodeQty + j - 1;
    /*
    if (dist2 < dist1)
    {
      printf (" -> reverse\n");
      printf ("ver1 = %d ver2 = %d\n", ver1, ver2);
      ut_array_1d_int_reverseelts (node_nbs + 2, N.NodeQty - 2);
    }
    */

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

    if (isatty (1))
      printf ("\b\b\b\b%3.0f%%", floor (100*((double) i/Geo.EdgeQty)));
    else
      printf (" %3.0f%%", floor (100*((double) i/Geo.EdgeQty)));
     
    fflush (stdout);

    neut_mesh_free (&M);
    neut_nodes_free (&N);
  }

  printf ("\n");

  neut_mesh_init_nodeelts (pMesh1D, (*pNodes).NodeQty);

  return;
}
