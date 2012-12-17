/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_geom_lcl.h"

/* neut_mesh_elt_area computes the area of a 2D element */
int
neut_mesh_elt_area (struct NODES Nodes, struct MESH Mesh, int elt, double* parea)
{
  if (Mesh.Dimension != 2)
    return -1;

  (*parea) = ut_space_triangle_area (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
				     Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
				     Nodes.NodeCoo[Mesh.EltNodes[elt][2]]);

  return 0;
}

/* neut_mesh_elt_volume computes the volume of an element */
int
neut_mesh_elt_volume (struct NODES Nodes, struct MESH Mesh, int elt, double* pvol)
{
  if (Mesh.Dimension != 3)
    return -1;

  if (strcmp (Mesh.EltType, "tri") == 0)
    (*pvol) = ut_space_tet_volume (
	Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][3]]);
  else if (strcmp (Mesh.EltType, "quad") == 0)
    (*pvol) = ut_space_hexa_volume (
	Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][3]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][4]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][5]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][6]],
	Nodes.NodeCoo[Mesh.EltNodes[elt][7]]);
  else
    abort ();

  return 0;
}

int
neut_mesh_volume (struct NODES Nodes, struct MESH Mesh, double* pvol)
{
  int i;
  double eltvol;

  if (Mesh.Dimension != 3)
    return -1;

  (*pvol) = 0;
  for (i = 1; i <= Mesh.EltQty; i++)
  {
    neut_mesh_elt_volume (Nodes, Mesh, i, &eltvol);
    (*pvol) += eltvol;
  }

  return 0;
}

int
neut_mesh_elset_volume (struct NODES Nodes, struct MESH Mesh, int elset, double* pvol)
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
    double* centre)
{
  int i, elt;
  double* coo = ut_alloc_1d (3);
  double vol_tot, vol;

  if (Mesh.Dimension != 3)
    return -1;

  vol_tot = 0;
  vol = 0;
  ut_array_1d_set (centre, 3, 0);
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    neut_mesh_elt_centre (Mesh, Nodes, elt, coo);
    neut_mesh_elt_volume (Nodes, Mesh, elt, &vol);
    
    ut_array_1d_scale (coo, 3, vol);
    ut_array_1d_add (centre, coo, 3, centre);

    vol_tot += vol;
  }
  ut_array_1d_scale (centre, 3, 1./vol_tot);

  ut_free_1d (coo);

  return 0;
}

void
neut_mesh_elt_centre (struct MESH Mesh, struct NODES Nodes,
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

void
neut_mesh_elt_normal (struct MESH Mesh, struct NODES Nodes, int elt,
		     double *n)
{
  ut_space_trianglenormal (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][2]], n);

  return;
}

void
neut_mesh_elt_eq (struct MESH Mesh, struct NODES Nodes, int elt,
		     double *eq)
{
  if (Mesh.Dimension != 2)
    ut_error_reportbug ();

  if (Mesh.EltType == NULL || strcmp (Mesh.EltType, "tri") == 0)
  {
    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][2]], eq);
  }
  else if (strcmp (Mesh.EltType, "quad") == 0)
  {
    double** eqs = ut_alloc_2d (4, 4);
    int i;

    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][2]], eqs[0]);
    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][3]], eqs[1]);
    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][3]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][0]], eqs[2]);
    ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][3]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][1]], eqs[3]);

    ut_array_1d_zero (eq, 4);
    for (i = 0; i < 4; i++)
      ut_array_1d_add (eq, eqs[i], 4, eq);
    ut_array_1d_scale (eq, 4, 1. / ut_vector_norm (eq + 1));

    ut_free_2d (eqs, 4);
  }

  return;
}

int
neut_mesh_elt_length (struct NODES Nodes, struct MESH Mesh, int elt,
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

int
neut_mesh_elset_rr (struct NODES Nodes, struct MESH Mesh, int elset, double* prrmean, double* prrmin, double* prrmax)
{
  int i, elt;
  double rr;

  if (Mesh.Dimension != 3 && strcmp (Mesh.EltType, "tri") != 0)
    ut_error_reportbug ();

  (*prrmin) =  DBL_MAX;
  (*prrmax) = -DBL_MAX;
  (*prrmean) = 0;
  
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++) 
  {
    elt = Mesh.Elsets[elset][i]; 

    rr = neut_elt_rr_3d (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
	                 Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
	                 Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
	                 Nodes.NodeCoo[Mesh.EltNodes[elt][3]]);

    (*prrmin)  =  ut_num_min ((*prrmin), rr);
    (*prrmax)  =  ut_num_max ((*prrmax), rr);
    (*prrmean) += rr;
  }
  (*prrmean) /= Mesh.Elsets[elset][0];

  return 0;
}

int
neut_mesh_elt_rr (struct NODES Nodes, struct MESH Mesh, int elt, double* prr)
{
  (*prr) = neut_elt_rr_3d (
                Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
                Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
                Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
                Nodes.NodeCoo[Mesh.EltNodes[elt][3]]);

  return 0;
}

int
neut_mesh_rr (struct NODES Nodes, struct MESH Mesh, double* prrav, double* prrmin, double* prrmax)
{
  int i;
  double rr, min, max, av;

  min =  DBL_MAX;
  max = -DBL_MAX;
  av = 0;
  
  if (Mesh.Dimension == 2)
    for (i = 1; i <= Mesh.EltQty; i++) 
    {
      rr = neut_elt_rr_2d (Nodes.NodeCoo[Mesh.EltNodes[i][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][2]]);

      min = ut_num_min (min, rr);
      max = ut_num_max (max, rr);
      av += rr;
    }

  else if (Mesh.Dimension == 3)
    for (i = 1; i <= Mesh.EltQty; i++) 
    {
      rr = neut_elt_rr_3d (Nodes.NodeCoo[Mesh.EltNodes[i][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][3]]);

      min = ut_num_min (min, rr);
      max = ut_num_max (max, rr);
      av += rr;
    }

  av /= Mesh.EltQty;

  if (prrmin != NULL)
    (*prrmin) = min;

  if (prrmax != NULL)
    (*prrmax) = max;

  if (prrav != NULL)
    (*prrav) = av;
  
  return 0;
}


int
neut_mesh_Odis (struct NODES Nodes, struct MESH Mesh, char* Odisexpr, double* pOdis)
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

    rr[i] = neut_elt_rr_3d (p1, p2, p3, p4);
  }
    
  int var_qty = 4;
  char** vars = ut_alloc_2d_char (var_qty, 10);
  double* vals = ut_alloc_1d (var_qty);

  strcpy (vars[0], "dis");
  strcpy (vars[1], "dismin");
  strcpy (vars[2], "dismax");
  strcpy (vars[3], "dismean");
  vals[1] = ut_array_1d_min (rr + 1, Mesh.EltQty);
  vals[2] = ut_array_1d_max (rr + 1, Mesh.EltQty);
  vals[3] = ut_array_1d_mean (rr + 1, Mesh.EltQty);

  (*pOdis) = 1;
  for (i = 1; i <= Mesh.EltQty; i++) 
  {
    vals[0] = rr[i];

    if (strcmp (Odisexpr, "dis^(exp((dis^0.1)/(dis^0.1-1)))") == 0)
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
	ut_math_eval (Odisexpr, var_qty, vars, vals, &tmp);
      else
	tmp = 1;
#else
      ut_print_message (2, 2, "This capability requires libmatheval.");
#endif
    }
    (*pOdis) *= tmp;
  }
  
  ut_free_1d (rr);
  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);

  return 0;
}

void
neut_mesh_centre (struct NODES Nodes, struct MESH Mesh, double* centre)
{
  int i;
  double* elsetpos = ut_alloc_1d (3);
  double vol, vol_tot;

  if (Mesh.Dimension != 3)
    ut_error_reportbug ();

  ut_array_1d_zero (centre, 3);

  vol_tot = 0;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    neut_mesh_elset_centre (Nodes, Mesh, i, elsetpos);
    neut_mesh_elset_volume (Nodes, Mesh, i, &vol);

    ut_array_1d_scale (elsetpos, 3, vol);
    vol_tot += vol;

    ut_array_1d_add (centre, elsetpos, 3, centre);
  }
  
  ut_array_1d_scale (centre, 3, 1./vol_tot);

  ut_free_1d (elsetpos);

  return;
}

void
neut_mesh_elt_lengths (struct MESH Mesh, struct NODES Nodes, int elt, double* pavlength, double* pminlength, double* pmaxlength)
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

    if (pavlength != NULL)
      (*pavlength) = ut_array_1d_mean (length, 3);
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
    
    if (pavlength != NULL)
      (*pavlength) = ut_array_1d_mean (length, 4);
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
neut_mesh_elset_Osize (struct NODES Nodes, struct MESH Mesh, int elset,
			    double cl, double* pOsize)
{
  int i, elt;
  double* meanlength = ut_alloc_1d (Mesh.Elsets[elset][0]);

  if (Mesh.EltQty == 0)
  {
    printf ("neut_mesh_Osize: Mesh.EltQty == 0\n");
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
  (*pOsize) = ut_array_1d_mean (meanlength, Mesh.Elsets[elset][0]);

  (*pOsize) = pow (*pOsize, 3);

  ut_free_1d (meanlength);

  return 0;
}

int
neut_mesh_Osize (struct NODES Nodes, struct MESH Mesh, double cl,
    double* pOsize)
{
  int i;
  double* meanlength = ut_alloc_1d (Mesh.EltQty);

  if (Mesh.EltQty == 0)
  {
    printf ("neut_mesh_Osize: Mesh.EltQty == 0\n");
    abort ();
  }

  for (i = 1; i <= Mesh.EltQty; i++)
    neut_mesh_elt_lengths (Mesh, Nodes, i, &(meanlength[i - 1]),
			   NULL, NULL);

  ut_array_1d_scale (meanlength, Mesh.EltQty, 1 / cl);
  
  for (i = 0; i < Mesh.EltQty; i++)
    if (meanlength[i] > 1)
      meanlength[i] = 1 / meanlength[i];
  (*pOsize) = ut_array_1d_mean (meanlength, Mesh.EltQty);

  (*pOsize) = pow (*pOsize, 3);

  ut_free_1d (meanlength);

  return 0;
}

int
neut_mesh_eltlength (struct NODES Nodes, struct MESH Mesh, double* pavlength)
{
  int i;
  double* meanlength = ut_alloc_1d (Mesh.EltQty);

  for (i = 1; i <= Mesh.EltQty; i++)
    neut_mesh_elt_lengths (Mesh, Nodes, i, &(meanlength[i - 1]),
			   NULL, NULL);

  (*pavlength) = ut_array_1d_mean (meanlength, Mesh.EltQty);

  ut_free_1d (meanlength);

  return 0;
}
