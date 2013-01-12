/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconstruct_mesh.h"

void
nem_reconstruct_mesh_2d (struct NODES Nodes, struct MESH *pMesh2D,
              struct MESH *pMesh3D, struct TESS* pTess)
{
  int i, FaceQty;
  int** FacePoly = NULL;

  // check for the right dimensions
  if ((*pMesh3D).Dimension != 3)
    ut_error_reportbug ();

  if (pMesh2D != NULL && (*pMesh2D).EltQty != 0)
    if ((*pMesh2D).Dimension != 2)
      ut_error_reportbug ();

  // Reconstructing 2D mesh
  if (pMesh2D == NULL || (*pMesh2D).EltQty == 0)
  {
    ut_print_message (0, 2, "Reconstructing 2D mesh ... ");
    if (pMesh3D == NULL || (*pMesh3D).EltQty == 0)
    {
      ut_print_message (2, 0, "Mesh3D is void.\n");
      abort ();
    }
    neut_mesh_init_nodeelts (pMesh3D, Nodes.NodeQty);
    neut_mesh_init_eltelset (pMesh3D, NULL);
    neut_mesh3d_mesh2d (Nodes, *pMesh3D, pMesh2D, &FacePoly, &FaceQty, 1);

    (*pTess).FaceQty = FaceQty;
    (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
    for (i = 1; i <= (*pTess).FaceQty; i++)
      ut_array_1d_int_memcpy ((*pTess).FacePoly[i], 2, FacePoly[i - 1]);
  }
  else
  {
    (*pTess).FaceQty = (*pMesh2D).ElsetQty;
    neut_mesh_facepoly (Nodes, *pMesh2D, *pMesh3D, &((*pTess).FacePoly));
  }

  neut_tess_init_polystuff_fromfacepoly (pTess);

  if ((*pMesh2D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh2D, Nodes.NodeQty);

  // face equations: the elt normals are (weighted) averaged, so as the
  // constant for the equation.

  int elt, j;
  double area, norm;
  double* eq  = ut_alloc_1d (4);
  double* eqe = ut_alloc_1d (4);

  if ((*pTess).FaceState == NULL)
    (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  ut_array_1d_int_set ((*pTess).FaceState + 1, (*pTess).FaceQty, 1);

  if ((*pTess).FaceEq == NULL)
    (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    ut_array_1d_zero (eq, 4);
    for (j = 1; j <= (*pMesh2D).Elsets[i][0]; j++)
    {
      elt = (*pMesh2D).Elsets[i][j];
      neut_mesh_elt_eq ((*pMesh2D), Nodes, elt, eqe);
      neut_mesh_elt_area (Nodes, (*pMesh2D), elt, &area);
      ut_array_1d_scale (eqe, 4, area);
      ut_array_1d_add (eqe, eq, 4, eq);
    }

    norm = ut_vector_norm (eq + 1);
    ut_array_1d_scale (eq, 4, 1./norm);

    ut_array_1d_memcpy ((*pTess).FaceEq[i], 4, eq);
  }

  ut_free_1d (eq);
  ut_free_1d (eqe);

  ut_free_2d_int (FacePoly, (*pTess).FaceQty);

  return;
}

void
nem_reconstruct_mesh_1d (struct NODES Nodes, struct MESH *pMesh1D,
              struct MESH *pMesh2D, struct TESS* pTess)
{
  int i, EdgeQty;
  int*  EdgeFaceQty = NULL;
  int** EdgeFaceNb = NULL;

  // check for the right dimensions
  if ((*pMesh2D).Dimension != 2)
    ut_error_reportbug ();

  if (pMesh1D != NULL && (*pMesh1D).EltQty != 0)
    if ((*pMesh1D).Dimension != 1)
      ut_error_reportbug ();

  // Reconstructing 1D mesh
  if (pMesh1D == NULL || (*pMesh1D).EltQty == 0)
  {
    ut_print_message (0, 2, "Reconstructing 1D mesh ... ");
    fflush (stdout);

    if (pMesh2D == NULL || (*pMesh2D).EltQty == 0)
    {
      ut_print_message (2, 0, "Mesh2D is void.\n");
      abort ();
    }
    neut_mesh_init_nodeelts (pMesh2D, Nodes.NodeQty);
    neut_mesh_init_eltelset (pMesh2D, NULL);
    neut_mesh2d_mesh1d (*pMesh2D, pMesh1D, &EdgeFaceNb,
	&EdgeFaceQty, &EdgeQty, 1);

    (*pTess).EdgeQty = EdgeQty;
    (*pTess).EdgeFaceQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
    ut_array_1d_int_memcpy ((*pTess).EdgeFaceQty + 1, (*pTess).EdgeQty,
	EdgeFaceQty);
    (*pTess).EdgeFaceNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
    (*pTess).EdgeFaceNb[0] = ut_alloc_1d_int (1);
    for (i = 1; i <= (*pTess).EdgeQty; i++)
    {
      (*pTess).EdgeFaceNb[i] = ut_alloc_1d_int ((*pTess).EdgeFaceQty[i]);
      ut_array_1d_int_memcpy ((*pTess).EdgeFaceNb[i], (*pTess).EdgeFaceQty[i], EdgeFaceNb[i - 1]);
    }
  }
  else
  {
    (*pTess).EdgeQty = (*pMesh1D).ElsetQty;
    neut_mesh_edgeface (*pMesh1D, *pMesh2D, &((*pTess).EdgeFaceNb), &((*pTess).EdgeFaceQty));
  }

  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDel   = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  if ((*pMesh1D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh1D, Nodes.NodeQty);

  neut_tess_init_facestuff_fromedge (pTess);

  ut_free_1d_int (EdgeFaceQty);
  ut_free_2d_int (EdgeFaceNb,(*pTess).EdgeQty);

  return;
}

void
nem_reconstruct_mesh_0d (struct NODES Nodes, struct MESH *pMesh0D,
              struct MESH *pMesh1D, struct TESS* pTess)
{
  int i, VerQty;
  int** VerEdgeNb = NULL;
  int*  VerEdgeQty = NULL;
  struct TESS Tessb;
  struct TESS* pTessb = &Tessb;

  neut_tess_set_zero (pTessb);

  if (pTess != NULL)
    pTessb = pTess;

  // check for the right dimensions
  if ((*pMesh1D).Dimension != 1)
    ut_error_reportbug ();

  if (pMesh0D != NULL && (*pMesh0D).EltQty != 0)
    if ((*pMesh0D).Dimension != 0)
      ut_error_reportbug ();

  // Reconstructing 0D mesh
  if (pMesh0D == NULL || (*pMesh0D).EltQty == 0)
  {
    ut_print_message (0, 2, "Reconstructing 0D mesh ... ");
    fflush (stdout);

    if (pMesh1D == NULL || (*pMesh1D).EltQty == 0)
    {
      ut_print_message (2, 0, "Mesh1D is void.\n");
      abort ();
    }
    neut_mesh_init_nodeelts (pMesh1D, Nodes.NodeQty);
    neut_mesh_init_eltelset (pMesh1D, NULL);
    neut_mesh1d_mesh0d (*pMesh1D, pMesh0D, &VerEdgeNb, &VerEdgeQty, &VerQty, 1);

    (*pTessb).VerQty = VerQty;
    (*pTessb).VerEdgeQty = ut_alloc_1d_int ((*pTessb).VerQty + 1);
    ut_array_1d_int_memcpy ((*pTessb).VerEdgeQty + 1, (*pTessb).VerQty,
	VerEdgeQty);
    (*pTessb).VerEdgeNb = ut_alloc_1d_pint ((*pTessb).VerQty + 1);
    (*pTessb).VerEdgeNb[0] = ut_alloc_1d_int (1);
    for (i = 1; i <= (*pTessb).VerQty; i++)
    {
      (*pTessb).VerEdgeNb[i] = ut_alloc_1d_int ((*pTessb).VerEdgeQty[i]);
      ut_array_1d_int_memcpy ((*pTessb).VerEdgeNb[i], (*pTessb).VerEdgeQty[i], VerEdgeNb[i - 1]);
    }
  }
  else
  {
    (*pTessb).VerQty = (*pMesh0D).ElsetQty;
    neut_mesh_veredge (*pMesh0D, *pMesh1D, &((*pTessb).VerEdgeNb), &((*pTessb).VerEdgeQty));
  }

  // Filling out VerCoo
  (*pTessb).VerCoo = ut_alloc_2d ((*pTessb).VerQty + 1, 3);
  for (i = 1; i <= (*pTessb).VerQty; i++)
    ut_array_1d_memcpy ((*pTessb).VerCoo[i], 3, Nodes.NodeCoo[(*pMesh0D).EltNodes[i][0]]);

  if (pTess != NULL)
  {
    // Filling out VerState
    (*pTessb).VerState = ut_alloc_1d_int ((*pTessb).VerQty + 1);

    neut_tess_init_edgestuff_fromver (pTessb);

    // aligning edge definition with mesh elset
    int elt, node1;
    double dist1, dist2;
    for (i = 1; i <= (*pTessb).EdgeQty; i++)
    {
      elt = (*pMesh1D).Elsets[i][1];
      node1 = (*pMesh1D).EltNodes[elt][0];
      dist1 = ut_space_dist (Nodes.NodeCoo[node1],
			    (*pTessb).VerCoo[(*pTessb).EdgeVerNb[i][0]]);
      dist2 = ut_space_dist (Nodes.NodeCoo[node1],
			    (*pTessb).VerCoo[(*pTessb).EdgeVerNb[i][1]]);

      if (dist1 > dist2)
	ut_array_1d_int_reverseelts ((*pTessb).EdgeVerNb[i], 2);
    }

    neut_tess_init_facestuff_fromver (pTessb);
  }

  if (pTess == NULL)
    neut_tess_free (pTessb);

  if (pTess != NULL)
    ut_free_2d_int (VerEdgeNb, (*pTess).VerQty);

  ut_free_1d_int (VerEdgeQty);

  return;
}

void
nem_reconstruct_mesh_finalizetess (struct TESS* pTess, struct NODES RNodes,
    struct MESH RMesh0D, struct MESH RMesh1D, struct MESH RMesh2D,
    struct MESH RMesh3D)
{
  int i, j, elt, node;
  double length;
  // , norm, area;
  // double* eq = ut_alloc_1d (4);
  // double* eqe = ut_alloc_1d (4);

  ut_array_1d_int_set ((*pTess).FaceState + 1, (*pTess).FaceQty, 1);
  ut_array_1d_int_set ((*pTess).VerState + 1, (*pTess).VerQty, 1);


  // (re)initializing tess properties from nodes / mesh ver coo
  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    node = RMesh0D.EltNodes[i][0];
    ut_array_1d_memcpy ((*pTess).VerCoo[i], 3, RNodes.NodeCoo[node]);
  }

  // edge length (sum over the elements to account for curvature)
  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    length = 0;
    for (j = 1; j <= RMesh1D.Elsets[i][0]; j++)
    {
      elt = RMesh1D.Elsets[i][j];
      length += ut_space_dist (RNodes.NodeCoo[RMesh1D.EltNodes[elt][0]],
                               RNodes.NodeCoo[RMesh1D.EltNodes[elt][1]]);
    }
    (*pTess).EdgeLength[i] = length;
  }

  // face equations: the elt normals are (weighted) averaged, so as the
  // constant for the equation.

  /*
  if ((*pTess).FaceState == NULL)
    (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  if ((*pTess).FaceEq == NULL)
    (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    ut_array_1d_zero (eq, 4);
    for (j = 1; j <= RMesh2D.Elsets[i][0]; j++)
    {
      elt = RMesh2D.Elsets[i][j];
      neut_mesh_elt_eq (RMesh2D, RNodes, elt, eqe);
      neut_mesh_elt_area (RNodes, RMesh2D, elt, &area);
      ut_array_1d_scale (eqe, 4, area);
      ut_array_1d_add (eqe, eq, 4, eq);
    }

    norm = ut_vector_norm (eq + 1);
    ut_array_1d_scale (eq, 4, 1./norm);

    ut_array_1d_memcpy ((*pTess).FaceEq[i], 4, eq);

    // checking the face normal against the orientations of the edges
    // (see TestFace)
    if (TestFace (*pTess, i) != 0)
    {
      printf ("face normal against the orientations of the edges\n");
      printf ("did not work\n");
      abort ();
      ut_array_1d_scale ((*pTess).FaceEq[i], 4, -1);
    }
  }
  */

  // poly centre: centre of mass of the elsets
  if ((*pTess).CenterCoo == NULL)
    (*pTess).CenterCoo = ut_alloc_2d ((*pTess).PolyQty + 1, 3);
  for (i = 1; i <= (*pTess).PolyQty; i++)
    neut_mesh_elset_centre (RNodes, RMesh3D, i, (*pTess).CenterCoo[i]);

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

  // if (strcmp (RMesh3D.EltType, "tri") == 0 && (*pTess).PolyTrue == NULL)

  // ut_free_1d (eqe);
  // ut_free_1d (eq);

  // If the domain is not defined, initializing it.
  if ((*pTess).DomFaceQty == 0)
    neut_tess_init_domain (pTess);

  if ((*pTess).PolyTrue == NULL)
    neut_tess_init_polytrue (pTess);

  if ((*pTess).PolyBody == NULL)
    neut_tess_init_polybody (pTess);

  return;
}

void
nem_reconstruct_mesh_tess_updating (struct TESS* pTess, struct NODES
    RNodes, struct MESH RMesh0D, struct MESH RMesh1D, struct MESH
    RMesh2D, struct MESH RMesh3D)
{
  int i, j, elt, node;
  double length, norm;
  double* eq = ut_alloc_1d (4);
  double* eqe = ut_alloc_1d (4);

  ut_print_message (0, 2, "Retrieving tessellation properties ...\n");

  // reinitializing tess properties from nodes / mesh
  // ver coo
  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    node = RMesh0D.EltNodes[i][0];
    ut_array_1d_memcpy ((*pTess).VerCoo[i], 3, RNodes.NodeCoo[node]);
  }
  
  // edge length (sum over the elements to account for curvature)
  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    length = 0;
    for (j = 1; j <= RMesh1D.Elsets[i][0]; j++)
    {
      elt = RMesh1D.Elsets[i][j];
      length += ut_space_dist (RNodes.NodeCoo[RMesh1D.EltNodes[elt][0]],
                               RNodes.NodeCoo[RMesh1D.EltNodes[elt][1]]);
    }
    (*pTess).EdgeLength[i] = length;
  }
  
  // face equations
  // the elt normals are averaged, so as the constant for the equation
  // the constant does not really matter because it does not change the
  // projection (which is done along the normal).

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceState[i] = 1;

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
  for (i = 1; i <= (*pTess).PolyQty; i++)
    neut_mesh_elset_centre (RNodes, RMesh3D, i, (*pTess).CenterCoo[i]);

  ut_free_1d (eqe);
  ut_free_1d (eq);
  
  return;
}
