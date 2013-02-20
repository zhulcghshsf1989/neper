/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_tess.h"

void
nem_tess_face_1delts (struct TESS Tess, struct MESH Mesh1D, int
			     face, int** pelts, int* peltqty)
{
  int i, j, EdgeQty, edge, ori;

  (*peltqty) = 0;

  EdgeQty = Tess.FaceVerQty[face];
  for (i = 1; i <= EdgeQty; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];
    (*peltqty) += Mesh1D.Elsets[edge][0];
  }

  (*pelts) = ut_alloc_1d_int (*peltqty);
  (*peltqty) = 0;

  for (i = 1; i <= EdgeQty; i++)
  {
    ori = Tess.FaceEdgeOri[face][i];
    edge = Tess.FaceEdgeNb[face][i];

    if (ori == 1)
      for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
	(*pelts)[(*peltqty)++] = Mesh1D.Elsets[edge][j];
    else
      for (j = Mesh1D.Elsets[edge][0]; j >= 1; j--)
	(*pelts)[(*peltqty)++] = -Mesh1D.Elsets[edge][j];
  }

  return;
}

void
nem_tess_updatefrommesh_geom (struct TESS* pTess, struct NODES
    RNodes, struct MESH RMesh0D, struct MESH RMesh1D, struct MESH
    RMesh2D, struct MESH RMesh3D)
{
  int i, j, elt, node;
  double length, norm;
  double* eq = ut_alloc_1d (4);
  double* eqe = ut_alloc_1d (4);

  // reinitializing tess properties from nodes / mesh
  // ver coo
  if ((*pTess).VerCoo == NULL)
    (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);

  if (RMesh0D.ElsetQty > 0)
    for (i = 1; i <= (*pTess).VerQty; i++)
    {
      node = RMesh0D.EltNodes[i][0];
      ut_array_1d_memcpy ((*pTess).VerCoo[i], 3, RNodes.NodeCoo[node]);
    }

  // edge length (sum over the elements to account for curvature)
  if ((*pTess).EdgeState == NULL)
    (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  ut_array_1d_int_set ((*pTess).EdgeState + 1, (*pTess).EdgeQty, 1);

  if ((*pTess).EdgeLength == NULL)
    (*pTess).EdgeLength = ut_alloc_1d ((*pTess).VerQty + 1);

  ut_array_1d_set ((*pTess).EdgeLength + 1, (*pTess).EdgeQty, 0);

  if (RMesh1D.ElsetQty > 0)
    for (i = 1; i <= (*pTess).EdgeQty; i++)
      for (j = 1; j <= RMesh1D.Elsets[i][0]; j++)
      {
	neut_mesh_elt_length (RNodes, RMesh1D, RMesh1D.Elsets[i][j], &length);
	(*pTess).EdgeLength[i] += length;
      }

  // face equations
  // the elt normals are averaged, so as the constant for the equation
  // the constant does not really matter because it does not change the
  // projection (which is done along the normal).
  if ((*pTess).FaceState == NULL)
    (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  ut_array_1d_int_set ((*pTess).FaceState + 1, (*pTess).FaceQty, 1);

  if ((*pTess).FaceEq == NULL)
    (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);

  if (RMesh2D.ElsetQty > 0)
    for (i = 1; i <= (*pTess).FaceQty; i++)
    {
      ut_array_1d_zero (eq, 4);
      for (j = 1; j <= RMesh2D.Elsets[i][0]; j++)
      {
	elt = RMesh2D.Elsets[i][j];
	neut_mesh_elt_eq (RMesh2D, RNodes, elt, eqe);
	ut_array_1d_add (eqe, eq, 4, eq);
      }

      norm = ut_vector_norm (eq + 1);
      ut_array_1d_scale (eq, 4, 1./norm);

      ut_array_1d_memcpy ((*pTess).FaceEq[i], 4, eq);
    }

  // poly centre: centre of mass of the elsets
  if ((*pTess).CenterCoo == NULL)
    (*pTess).CenterCoo = ut_alloc_2d ((*pTess).PolyQty + 1, 3);

  if (RMesh3D.ElsetQty > 0)
    for (i = 1; i <= (*pTess).PolyQty; i++)
      neut_mesh_elset_centre (RNodes, RMesh3D, i, (*pTess).CenterCoo[i]);

  ut_free_1d (eqe);
  ut_free_1d (eq);

  return;
}

void
nem_tess_updatefrommesh_polyfaceori (struct TESS* pTess, struct NODES RNodes,
				     struct MESH RMesh2D, struct MESH RMesh3D)
{
  int i, j;

  if (RMesh2D.EltQty > 0 && RMesh3D.EltQty > 0)
  {
    // poly face ori
    if ((*pTess).PolyFaceOri == NULL)
    {
      (*pTess).PolyFaceOri = ut_alloc_1d_pint ((*pTess).PolyQty + 1);
      (*pTess).PolyFaceOri[0] = ut_alloc_1d_int (1);
    }

    int k;
    int face, elt2d, elt3dqty, elt3d;
    int* elts3d = NULL;
    double* eq = ut_alloc_1d (4);
    double* eltcentre = ut_alloc_1d (3);
    for (i = 1; i <= (*pTess).PolyQty; i++)
    {
      // printf ("\npoly = %d ----------\n", i);
      (*pTess).PolyFaceOri[i] = ut_alloc_1d_int ((*pTess).PolyFaceQty[i] + 1);
      for (j = 1; j <= (*pTess).PolyFaceQty[i]; j++)
      {
	face = (*pTess).PolyFaceNb[i][j];
	elt2d = RMesh2D.Elsets[face][1];
	neut_mesh_elt2d_elts3d (RMesh2D, elt2d, RMesh3D, &elts3d, &elt3dqty);
	elt3d = -1;
	for (k = 0; k < elt3dqty; k++)
	{
	  if (RMesh3D.EltElset[elts3d[k]] == i)
	  {
	    elt3d = elts3d[k];
	    break;
	  }
	}
	if (elt3d == -1)
	  ut_error_reportbug ();

	neut_mesh_elt_eq (RMesh2D, RNodes, elt2d, eq);
	neut_mesh_elt_centre (RMesh3D, RNodes, elt3d, eltcentre);

	/*
	printf ("\nface = %d FaceEq = ", face);
	ut_array_1d_fprintf (stdout, (*pTess).FaceEq[face], 4, "%f");
	printf ("elt2d = %d    eq = ", elt2d);
	ut_array_1d_fprintf (stdout, eq, 4, "%f");
	printf ("eltcentre = ");
	ut_array_1d_fprintf (stdout, eltcentre, 3, "%f");
	printf ("planeside = %d\n", ut_space_planeside (eq, eltcentre - 1));
	*/
	(*pTess).PolyFaceOri[i][j] = -ut_space_planeside (eq, eltcentre - 1);
	if (ut_vector_scalprod ((*pTess).FaceEq[face] + 1, eq + 1) < 0)
	  (*pTess).PolyFaceOri[i][j] *= -1;
	// printf (" --> faceori = %d\n", (*pTess).PolyFaceOri[i][j]);
      }
    }
    ut_free_1d (eq);
    ut_free_1d (eltcentre);
    ut_free_1d_int (elts3d);
  }

  // if (strcmp (RMesh3D.EltType, "tri") == 0 && (*pTess).PolyTrue == NULL)

  // ut_free_1d (eqe);
  // ut_free_1d (eq);

  // If the domain is not defined, initializing it.
  if ((*pTess).PolyQty && (*pTess).DomFaceQty == 0)
    neut_tess_init_domain (pTess);

  if ((*pTess).PolyTrue == NULL)
    neut_tess_init_polytrue (pTess);

  if ((*pTess).PolyBody == NULL)
    neut_tess_init_polybody (pTess);

  return;
}

void
nem_tess_edgebodies_nodes (struct TESS Tess, struct MESH Mesh0D,
			   struct MESH Mesh1D,
			   int** pnodes, int* pnodeqty)
{
  int i, j, elt, qty;
  int* tmp = NULL;

  // determining the nodes of the body of the edges
  (*pnodeqty) = 0;
  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    neut_mesh_elset_nodes (Mesh1D, i, &tmp, &qty);

    for (j = 0; j < 2; j++)
    {
      elt = Mesh0D.Elsets[Tess.EdgeVerNb[i][j]][1];

      if (ut_array_1d_int_deletencompress (tmp, qty,
	  Mesh0D.EltNodes[elt][0], 1) != 1)
	ut_error_reportbug ();

      qty--;
    }

    (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty) + qty);
    ut_array_1d_int_memcpy ((*pnodes) + (*pnodeqty), qty, tmp);
    (*pnodeqty) += qty;

    ut_free_1d_int (tmp);
    tmp = NULL;
  }

  return;
}

void
nem_tess_facebodies_nodes (struct TESS Tess, struct MESH Mesh1D,
			   struct MESH Mesh2D,
			   int** pnodes, int* pnodeqty)
{
  int i, j, k, qty;
  int* tmp = NULL;
  int* edgenodes = NULL;
  int edgenodeqty;

  // determining the nodes of the body of the faces
  (*pnodeqty) = 0;
  for (i = 1; i <= Tess.FaceQty; i++)
  {
    neut_mesh_elset_nodes (Mesh2D, i, &tmp, &qty);

    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
    {
      neut_mesh_elset_nodes (Mesh1D, Tess.FaceEdgeNb[i][j], &edgenodes, &edgenodeqty);

      for (k = 0; k < edgenodeqty; k++)
	qty -= ut_array_1d_int_deletencompress (tmp, qty, edgenodes[k], 1);

      ut_free_1d_int (edgenodes);
      edgenodes = NULL;
    }

    (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty) + qty);
    ut_array_1d_int_memcpy ((*pnodes) + (*pnodeqty), qty, tmp);
    (*pnodeqty) += qty;

    ut_free_1d_int (tmp);
    tmp = NULL;
  }

  return;
}
