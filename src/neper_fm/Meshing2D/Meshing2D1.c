/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Meshing2D.h"

void
Meshing2D (struct IN In, struct GEOPARA GeoPara,
	   struct GEO Geo, int* face_sim, double*** face_sim_A, double**
	   face_sim_B, double** face_proj, int* face_op, struct NODES*
	   pRNodes, struct MESH* pRMesh2D, struct MESH Mesh0D, struct
	   MESH Mesh1D, struct NODES *pNodes, struct MESH *pMesh2D)
{
  int i, j, k, id, nb, bak;
  struct NODES N, Nint;
  struct MESH M, Mint;
  struct NODES N2;
  struct MESH M2;
  int *node_nbs = NULL;
  int *elt_nbs = NULL;
  int *skinnodes = NULL;
  int skinnodes_qty;
  double *dist = NULL;
  char** algo = NULL;
  int algoqty;
  double allowed_t, elapsed_t, max_elapsed_t;
  double rnd, rrmin, rrmax, rrmean, qmin, qmean, qmax, totqmean, totqmin;
  double totqmin0, qmin0; 
  double totqmax;
  int a, status, qty;
  char** list = NULL;
  int* algohit = NULL;
  double* qual_dis = NULL;
  double* qual_size = NULL;
  double* qual = NULL;
  char*** algos = NULL;
  int actual_algo;
  int iter, totstatus;
  char* message = ut_alloc_1d_char (1000);
  char* message2 = ut_alloc_1d_char (1000);
  int message_length;
  struct MESH Garbage;
  double acl;

  Mesh0D.EltQty = Mesh0D.EltQty;

  neut_mesh_set_zero (&Garbage);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_nodes_set_zero (&Nint);
  neut_mesh_set_zero (&Mint);
  neut_nodes_set_zero (&N2);
  neut_mesh_set_zero (&M2);
  (*pMesh2D).Dimension = 2;
  (*pMesh2D).EltOrder = 1;
  (*pMesh2D).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh2D).EltType, "tri");

  allowed_t = In.mesh2dmaxtime;
  max_elapsed_t = 0;

  char* mesh2dalgo = ut_alloc_1d_char (100);
  // retrieving list of algorithms to use
  if (strcmp (In.mesh2dalgo, "default") == 0)
    sprintf (mesh2dalgo, "fron,mead");
  else if (strcmp (In.mesh2dalgo, "qualmax") == 0)
    sprintf (mesh2dalgo, "mead,dela,fron");
  else
    sprintf (mesh2dalgo, "%s", In.mesh2dalgo);

  ut_string_separate (mesh2dalgo, ',', &algo, &algoqty);
  ut_free_1d_char (mesh2dalgo);

  algos = ut_alloc_3d_char (algoqty, 2, 5);
  qual_dis = ut_alloc_1d (algoqty);
  qual_size = ut_alloc_1d (algoqty);
  qual = ut_alloc_1d (algoqty);
  algohit = ut_alloc_1d_int (algoqty);

  for (a = 0; a < algoqty; a++)
  {
    ut_string_separate (algo[a], '/', &list, &qty);
    sprintf (algos[a][0], "%s", list[0]);
    if (qty == 2)
      sprintf (algos[a][1], "%s", list[1]);
    else
    {
      ut_free_1d_char (algos[a][1]);
      algos[a][1] = NULL;
    }
    ut_free_2d_char (list, qty);
  }

  totqmin0 = 1;
  totqmin = 1;
  totqmean = 0;
  totqmax = 0;
  actual_algo = -1;

  message_length = 0;

  ut_print_message (0, 2, "2D meshing ... ", 0);
  fflush (stdout);

  // Meshing faces in turn ---------------------------------------------
  int* meshface = ut_alloc_1d_int (Geo.FaceQty + 1);
  ut_array_1d_int_set (meshface + 1, Geo.FaceQty, 1);
  meshface[0] = Geo.FaceQty;

  if (In.meshface != NULL)
    neut_geo_expr_facetab (Geo, In.meshface, meshface);
  
  int fixed_face = 0;
  for (i = 1; i <= Geo.FaceQty; i++)
  {
    // printf ("facesim = %d\n", face_sim[i]);
    if (meshface[i] == 0)
    {
      neut_mesh_addelset (pMesh2D, NULL, 0);
      continue;
    }

    qmin = 0;
    qmin0 = 0;
    qmax = 0;
    qmean = 0;
    totstatus = -1;

    if ((face_sim == NULL || face_sim[i] == 0) && (face_op == NULL || face_op[i] == 0))
    {
      // multimeshing loop
      for (a = 0; a < algoqty; a++)
      {
	/* meshing */
	rnd = 0;
	status = -1;

	/* mesh (iterate if meshing fails) */
	for (iter = 0; iter < In.mesh2diter; iter++)
	{
	  rnd = (iter + 1) * 1.e-5; // mandatory

	  status = nefm_mesh_2d_gmsh (Geo, i, face_proj[i], *pNodes,
	      Mesh1D, GeoPara.cl, In.gmsh, algos[a][0], rnd, allowed_t,
	      &N, &M, &acl, &elapsed_t);

	  if (status == 0) // success
	  {
	    max_elapsed_t = ut_num_max (max_elapsed_t, elapsed_t);
	    allowed_t = ut_num_min (In.mesh2dmaxtime, In.mesh2drmaxtime * max_elapsed_t);
	    break;
	  }
	}

	if (status == 0)
	  neut_mesh_2d_rr (N, M, &rrmean, &rrmin, &rrmax);
	else 
	  rrmean = rrmin = rrmax = 0;

	qual_dis[a] = rrmin;
	qual_size[a] = pow (acl, 3.);
	
	// sizefac = 1;

	qual[a] = pow (qual_dis[a] , 1 - qual_dis[a])
		* pow (qual_size[a], qual_dis[a]);

	if (qual[a] > qmin)
	{
	  actual_algo = a;
	  qmin0 = qual_dis[a];
	  qmin = qual[a];
	  qmax = rrmax;
	  qmean = rrmean;
	  neut_nodes_nodes (N, &N2);
	  neut_mesh_mesh (M, &M2);
	}

	if (status == 0)
	  totstatus = 0;
      }

      // checking for multimeshing failure
      if (totstatus != 0)
      {
	ut_print_message (2, 2, "Meshing of face %d failed\n", i);
	ut_print_message (2, 2, "You should use (more) multimeshing.\n");
	ut_print_message (2, 2, "If you do not find a solution, please report this like a bug.\n");
	abort ();
      }
      
      neut_nodes_free (&N);
      neut_mesh_free (&M);
      neut_nodes_nodes (N2, &N);
      neut_mesh_mesh (M2, &M);
      neut_nodes_free (&N2);
      neut_mesh_free (&M2);

      algohit[actual_algo]++;
      totqmin0  =  ut_num_min (totqmin0, qmin0);
      totqmin  =  ut_num_min (totqmin, qmin);
      totqmax  =  ut_num_min (totqmax, qmax);
      totqmean = (totqmean * (i - 1) + qmean) / i;

      if (Geo.FaceState[i] > 0 && pRNodes == NULL)
      {
	neut_geo_face_interpolmesh (Geo, i, &Nint, &Mint);
	neut_nodes_proj_alongontomesh (&N, face_proj[i], Nint, Mint, 1);
	neut_nodes_free (&Nint);
	neut_mesh_free (&Mint);
      }

      if (pRNodes != NULL)
	neut_nodes_proj_alongontomesh (&N, face_proj[i], *pRNodes, *pRMesh2D, i);
    }
    else if (face_op != NULL && face_op[i] != 0)
      neut_mesh_elset_mesh (*pRNodes, *pRMesh2D, i, &N, &M);
    else if (face_sim != NULL && face_sim[i] != 0)
    {
      printf ("copying\n");
      face_sim_A[1][0][0] = face_sim_A[1][0][0];

      neut_mesh_elset_mesh (*pNodes, *pMesh2D, 
	                    ut_num_fabs_int (face_sim[i]), &N, &M);
      if (face_sim[i] < 0)
	for (j = 1; j <= M.EltQty; j++)
	{
	  printf ("M.EltNodes =\n");
	  ut_array_1d_int_fprintf (stdout, M.EltNodes[j], 3, "%d");
	  neut_mesh_elt_reversenodes (&M, j);
	  ut_array_1d_int_fprintf (stdout, M.EltNodes[j], 3, "%d");
	}

      for (j = 1; j <= N.NodeQty; j++)
	ut_array_1d_add (N.NodeCoo[j], face_sim_B[i], 3, N.NodeCoo[j]);
    }

    /* calculating the node numbers - some are already recorded ("1D"
     * nodes), others are new.  The nodes already recorded are searched
     * out in N without a priori assumption, by using the coordinates
     */

    neut_mesh_elsets_nodes (Mesh1D, Geo.FaceEdgeNb[i] + 1,
			    Geo.FaceVerQty[i], &skinnodes, &skinnodes_qty);

    node_nbs = ut_alloc_1d_int (N.NodeQty + 1);

    dist = ut_alloc_1d (N.NodeQty + 1);

    /* for every skin node (whose pos in Nodes / coo is known, looking
     * for it in N. */
    for (j = 0; j < skinnodes_qty; j++)
    {
      for (k = 1; k <= N.NodeQty; k++)
	dist[k] =
	  ut_space_dist ((*pNodes).NodeCoo[skinnodes[j]], N.NodeCoo[k]);

      id = 1 + ut_array_1d_min_index (dist + 1, N.NodeQty);

      if (fabs (dist[id]) > 1e-7 || node_nbs[id] != 0)
      {
	ut_print_message (2, 0, "Meshing2D: skin node not found or bad node_nbs\n");
        printf ("dist = %g > 1e-9 for parent node:", dist[id]);
	ut_array_1d_fprintf (stdout, (*pNodes).NodeCoo[skinnodes[j]], 3, "%f");
        printf ("projection along:");
	ut_array_1d_fprintf (stdout, face_proj[i], 3, "%f");

	printf ("parent 1D nodes (%d):\n", skinnodes_qty);
	for (j = 0; j < skinnodes_qty; j++)
	  ut_array_1d_fprintf (stdout, (*pNodes).NodeCoo[skinnodes[j]], 3, "%f");
	printf ("returned nodes (%d):\n", N.NodeQty);
	ut_array_2d_fprintf (stdout, N.NodeCoo + 1, N.NodeQty, 3, "%f");

	ut_error_reportbug ();
	abort ();
      }

      node_nbs[id] = skinnodes[j];
    }
    ut_free_1d_int (skinnodes);
    ut_free_1d (dist);

    nb = (*pNodes).NodeQty;

    /* new nodes: their node_nbs are zero; filling with new values */
    id = ut_array_1d_int_eltpos (node_nbs + 1, N.NodeQty, 0);
    while (id > 0)
    {
      id++;
      node_nbs[id] = ++nb;
      id = ut_array_1d_int_eltpos (node_nbs + 1, N.NodeQty, 0);
    }

    // adding body nodes to global nodes
    bak = (*pNodes).NodeQty;
    for (j = 1; j <= N.NodeQty; j++)
      if (node_nbs[j] > bak)
      {
	for (k = 1; k <= (*pNodes).NodeQty; k++)
	  if (ut_space_dist (N.NodeCoo[j], (*pNodes).NodeCoo[k]) < 1e-8)
	  {
	    printf ("face %d\n", i);
	    printf ("attempting to overwrite a node!\n");
	    printf ("node at pos %d / %d of N, node_nbs = %d overwrite Nodes node %d\n", j, N.NodeQty, node_nbs[j], k);
	    printf ("coo in N:     "); ut_array_1d_fprintf (stdout, N.NodeCoo[j], 3, "%f");
	    printf ("coo in Nodes: "); ut_array_1d_fprintf (stdout, (*pNodes).NodeCoo[k], 3, "%f");
	    abort ();
	  }

	if (node_nbs[j] != (*pNodes).NodeQty + 1)
	{
	  printf ("not good\n");
	  abort ();
	}

	neut_nodes_addnode (pNodes, N.NodeCoo[j], -1);
      }

    // renumbering mesh nodes to match global nodes
    for (j = 1; j <= M.EltQty; j++)
      for (k = 0; k < 3; k++)
	M.EltNodes[j][k] = node_nbs[M.EltNodes[j][k]];

    ut_free_1d_int (node_nbs);

    // renumbering mesh elts
    elt_nbs = ut_alloc_1d_int (M.EltQty + 1);
    for (j = 1; j <= M.EltQty; j++)
      elt_nbs[j] = (*pMesh2D).EltQty + j;

    for (k = 1; k <= M.Elsets[1][0]; k++)
      M.Elsets[1][k] = elt_nbs[M.Elsets[1][k]];

    ut_free_1d_int (elt_nbs);

    for (j = 1; j <= M.EltQty; j++)
      neut_mesh_addelt (pMesh2D, M.EltNodes[j]);

    neut_mesh_addelset (pMesh2D, M.Elsets[1] + 1, M.Elsets[1][0]);

    neut_nodes_free (&N);
    neut_mesh_free (&M);

    // we are done ...
    sprintf (message, "%3.0f%% (%.2g|%.2g/", floor (100 *
	    ((double) i / Geo.FaceQty)), totqmin, totqmean);

    for (a = 0; a < algoqty; a++)
    {
      sprintf (message2, "%s%2d%%%c", message, ut_num_d2ri (100 * (double)algohit[a])/i, (a < algoqty - 1)? '|':')');
      sprintf (message, "%s", message2);
    }

    if (isatty (1))
    {
      for (k = 0; k < message_length; k++)
	fprintf (stdout, "\b");
      fprintf (stdout, "%s      \b\b\b\b\b\b", message);
    }
    else
      fprintf (stdout, " %s", message);

    fflush (stdout);
    
    message_length = strlen (message);
  }
  fprintf (stdout, "\n");

  ut_free_1d_int (meshface);

  if (fixed_face != 0)
    ut_print_message (0, 3, "%d %s been fixed ... ", fixed_face, (fixed_face == 1) ? "face has" : "faces have");
  
  // neut_debug_nodes (stdout, *pNodes);
  // neut_debug_mesh (stdout, *pMesh2D);
  neut_mesh_init_nodeelts (pMesh2D, (*pNodes).NodeQty);

  ut_free_2d_char (algo, algoqty);
  ut_free_1d (qual_size);
  ut_free_1d (qual_dis);
  ut_free_1d (qual);
  ut_free_1d_int (algohit);
  ut_free_3d_char (algos, algoqty, 2);
  ut_free_1d_char (message);
  ut_free_1d_char (message2);

  return;
}
