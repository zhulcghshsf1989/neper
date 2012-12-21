/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_3D.h"

int
nem_meshing_3D_poly_cl (struct MESHPARA MeshPara, struct TESS Tess, int poly,
                double* pcl)
{
  if (MeshPara.dboundcl < 0)
    (*pcl)  = MeshPara.cl;
  else 
  {
    int var_qty = 2;
    char** vars  = ut_alloc_2d_char (var_qty, 15);
    double* vals = ut_alloc_1d      (var_qty);
    int status = -1;
    double dbound;

    sprintf (vars[0], "body");
    sprintf (vars[1], "true");
    vals[0] = Tess.PolyBody[poly];
    vals[1] = Tess.PolyTrue[poly];

    status = ut_math_eval (MeshPara.dbound, var_qty, vars, vals, &dbound);

    if (status == -1)
      abort ();

    if (dbound == 1)
	(*pcl)  = MeshPara.dboundcl;
    else
	(*pcl)  = MeshPara.cl;

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

  return 0;
}


int
nem_meshing_3D_poly (struct IN In, double cl, struct MULTIM* pMultim,
		double allowed_t, double* pmax_elapsed_t, struct TESS Tess,
		struct NODES *pNodes, struct MESH Mesh2D, 
		struct MESH *pMesh3D, int id)
{
  int i, j, bak, tmp;
  struct NODES N, N2;
  struct MESH M, M2;
  int *node_nbs = NULL;
  int *elt_nbs = NULL;
  int *skinnodes = NULL;
  int skinnodes_qty;
  double *dist = NULL;
  double elapsed_t;
  int a, status, iter, nb;
  double rnd;
  double *vola = NULL, *volb = NULL;
  int totstatus;
  double acl;
  double rrmean, rrmin;
  char* tmpstring = ut_alloc_1d_char (1000);

  if ((*pMultim).mOdis == NULL)
    (*pMultim).mOdis = ut_alloc_2d (Tess.PolyQty + 1, (*pMultim).algoqty);
  if ((*pMultim).mOsize == NULL)
    (*pMultim).mOsize = ut_alloc_2d (Tess.PolyQty + 1, (*pMultim).algoqty);
  if ((*pMultim).mO == NULL)
    (*pMultim).mO = ut_alloc_2d (Tess.PolyQty + 1, (*pMultim).algoqty);
  if ((*pMultim).Oalgo == NULL)
    (*pMultim).Oalgo = ut_alloc_1d_int (Tess.PolyQty + 1);
  if ((*pMultim).algohit == NULL)
    (*pMultim).algohit = ut_alloc_1d_int ((*pMultim).algoqty);

  if ((*pMultim).Odis == NULL)
    (*pMultim).Odis = ut_alloc_1d (Tess.PolyQty + 1);
  if ((*pMultim).Osize == NULL)
    (*pMultim).Osize = ut_alloc_1d (Tess.PolyQty + 1);
  if ((*pMultim).O == NULL)
    (*pMultim).O = ut_alloc_1d (Tess.PolyQty + 1);

  int var_qty = 2;
  char**  vars = ut_alloc_2d_char (var_qty, 10);
  double* vals = ut_alloc_1d (var_qty);
  sprintf (vars[0], "mOdis");
  sprintf (vars[1], "mOsize");

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_nodes_set_zero (&N2);
  neut_mesh_set_zero (&M2);

  totstatus = -1;

  for (a = 0; a < (*pMultim).algoqty; a++)
  {
    /* meshing */
    rnd = 0;
    status = -1;

    /* mesh (iterate if meshing fails) */
    for (iter = 0; iter < In.mesh3diter; iter++)
    {
      rnd = iter * 1.e-5;

      status = nem_mesh_3d_gmsh (Tess, id, *pNodes, Mesh2D, cl,
	  In.mesh3dclconv, In.gmsh, (*pMultim).algos[a][0],
	  (*pMultim).algos[a][1], rnd, allowed_t, &N, &M, &acl,
	  &elapsed_t);

      if (status == 0) // success
      {
	(*pmax_elapsed_t) = ut_num_max ((*pmax_elapsed_t), elapsed_t);
	allowed_t = ut_num_min (In.mesh3dmaxtime, In.mesh3drmaxtime * (*pmax_elapsed_t));
	break;
      }
      if (status == 2) // meshing succeeded, but skin mesh changed
	break;
    }

    if (status == 0)
    {
      neut_mesh_Odis (N, M, In.mesh3doptidisexpr, &((*pMultim).mOdis[id][a]));
      neut_mesh_rr (N, M, &rrmean, &rrmin, NULL);
      (*pMultim).mOsize[id][a] = acl;
    }
    else 
    {
      (*pMultim).mOdis[id][a] = 0;
      (*pMultim).mOsize[id][a] = 0;
    }

    vals[0] = (*pMultim).mOdis[id][a];
    vals[1] = (*pMultim).mOsize[id][a];

    if (strcmp (In.mesh3doptiexpr, "Odis^0.8*Osize^0.2") == 0)
      (*pMultim).mO[id][a] =
	    pow ((*pMultim).mOdis[id][a] , 0.8)
	  * pow ((*pMultim).mOsize[id][a], 0.2);
    else
      ut_math_eval (In.mesh3doptiexpr, var_qty, vars, vals, &((*pMultim).mO[id][a]));

    if (status == 0)
    {
      if (a == 0 || ((*pMultim).mO[id][a] > (*pMultim).mO[id][(*pMultim).Oalgo[id]]))
      {
	(*pMultim).Oalgo[id] = a;
	neut_nodes_memcpy (N, &N2);
	neut_mesh_memcpy (M, &M2);
      }

      totstatus = 0;
    }

    if (In.mesh3dreport == 1)
    {
      sprintf (tmpstring, "neper-report/poly%d-a%d", id, a);
      FILE* file = ut_file_open (tmpstring, "W");
      double rr, length;

      for (i = 1; i <= M.EltQty; i++)
      {
	neut_mesh_elt_rr (N, M, i, &rr);
	neut_mesh_elt_lengths (M, N, i, &length, NULL, NULL);
	fprintf (file, "%.5g %.5g\n", rr, length);
      }

      ut_file_close (file, tmpstring, "W");
    }
  }

  (*pMultim).Odis[id]  = (*pMultim).mOdis[id][(*pMultim).Oalgo[id]];
  (*pMultim).Osize[id] = (*pMultim).mOsize[id][(*pMultim).Oalgo[id]];
  (*pMultim).O[id]     = (*pMultim).mO[id][(*pMultim).Oalgo[id]];

  if (totstatus != 0)
  {
    printf ("\n");
    ut_print_message (2, 3, "Meshing of poly %d failed\n", id);
    ut_print_message (2, 3, "You should use (more) multimeshing.\n");
    ut_print_message (2, 3, "If you do not find a solution, please report this like a bug.\n");
    abort ();
  }

  neut_nodes_memcpy (N2, &N);
  neut_mesh_memcpy (M2, &M);

  /* calculating the node numbers - some are already recorded ("2D"
   * nodes), the others are new.  We use the node positions. */

  neut_mesh_elsets_nodes (Mesh2D, Tess.PolyFaceNb[id] + 1, Tess.PolyFaceQty[id], &skinnodes, &skinnodes_qty);

  node_nbs = ut_alloc_1d_int (N.NodeQty + 1);

  /* there are skinnodes_qty nodes whose positions in Nodes are
   * recorded in skinnodes.  The nodes are at random places in N.
   * Searching them and their numbers, in turn. */

  dist = ut_alloc_1d (N.NodeQty + 1);

  /* for every skin node (whose pos in Nodes / coo is known, looking
   * for it in N. */
  for (i = 0; i < skinnodes_qty; i++)
  {
    for (j = 1; j <= N.NodeQty; j++)
      dist[j] = ut_space_dist ((*pNodes).NodeCoo[skinnodes[i]], N.NodeCoo[j]);

    tmp = 1 + ut_array_1d_min_index (dist + 1, N.NodeQty);
    
    if (fabs (dist[tmp]) > 1e-11 || node_nbs[tmp] != 0)
    {
      ut_print_message (2, 0, "nem_meshing_3D: skin node not found or bad node_nbs\n");
      abort ();
    }

    node_nbs[tmp] = skinnodes[i];
  }
  ut_free_1d_int (skinnodes);

  nb = (*pNodes).NodeQty;

  /* new nodes: their node_nbs are zero; filling with new values */
  tmp = ut_array_1d_int_eltpos (node_nbs + 1, N.NodeQty, 0);
  while (tmp > 0)
  {
    tmp++;
    node_nbs[tmp] = ++nb;
    tmp = ut_array_1d_int_eltpos (node_nbs + 1, N.NodeQty, 0);
  }

  /* adding body nodes to global nodes */
  bak = (*pNodes).NodeQty;
  for (i = 1; i <= N.NodeQty; i++)
    if (node_nbs[i] > bak)
      neut_nodes_addnode (pNodes, N.NodeCoo[i], -1);

  /* renumbering mesh nodes to match global nodes */
  vola = ut_alloc_1d (M.EltQty + 1);
  for (i = 1; i <= M.EltQty; i++)
  {
    vola[i] = ut_space_tet_volume (
	N.NodeCoo[M.EltNodes[i][0]], N.NodeCoo[M.EltNodes[i][1]],
	N.NodeCoo[M.EltNodes[i][2]], N.NodeCoo[M.EltNodes[i][3]]);
  }

  for (i = 1; i <= M.EltQty; i++)
    for (j = 0; j < 4; j++)
      M.EltNodes[i][j] = node_nbs[M.EltNodes[i][j]];

  volb = ut_alloc_1d (M.EltQty + 1);
  for (i = 1; i <= M.EltQty; i++)
  {
    volb[i] = ut_space_tet_volume (
                (*pNodes).NodeCoo[M.EltNodes[i][0]],
                (*pNodes).NodeCoo[M.EltNodes[i][1]],
                (*pNodes).NodeCoo[M.EltNodes[i][2]],
                (*pNodes).NodeCoo[M.EltNodes[i][3]]);

    if (ut_num_equal (vola[i], volb[i], 1e-6) == 0)
    {
      ut_error_reportbug ();
      printf ("  elt %d of M changes volume!\n", i);
      abort ();
    }
  }

  ut_free_1d_int (node_nbs);

  /* renumbering mesh elts */
  elt_nbs = ut_alloc_1d_int (M.EltQty + 1);
  for (i = 1; i <= M.EltQty; i++)
    elt_nbs[i] = (*pMesh3D).EltQty + i;

  for (j = 1; j <= M.Elsets[1][0]; j++)
    M.Elsets[1][j] = elt_nbs[M.Elsets[1][j]];

  ut_free_1d_int (elt_nbs);

  for (i = 1; i <= M.EltQty; i++)
    neut_mesh_addelt (pMesh3D, M.EltNodes[i]);

  neut_mesh_addelset (pMesh3D, M.Elsets[1] + 1, M.Elsets[1][0]);

  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);

  (*pMultim).algohit[(*pMultim).Oalgo[id]]++;

  ut_free_1d (dist);
  ut_free_1d_char (tmpstring);
  neut_nodes_free (&N);
  neut_nodes_free (&N2);
  neut_mesh_free (&M);
  neut_mesh_free (&M2);
  ut_free_1d (vola);
  ut_free_1d (volb);

  return totstatus;
}
