/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconmesh.h"

void
nem_reconmesh_2d (struct NODES Nodes, struct MESH *pMesh2D,
              struct MESH *pMesh3D, struct GEO* pGeo)
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

    (*pGeo).FaceQty = FaceQty;
    (*pGeo).FacePoly = ut_alloc_2d_int ((*pGeo).FaceQty + 1, 2); 
    for (i = 1; i <= (*pGeo).FaceQty; i++)
      ut_array_1d_int_memcpy ((*pGeo).FacePoly[i], 2, FacePoly[i - 1]);
  }
  else
  {
    (*pGeo).FaceQty = (*pMesh2D).ElsetQty;
    neut_mesh_facepoly (Nodes, *pMesh2D, *pMesh3D, &((*pGeo).FacePoly));
  }

  neut_geo_init_polystuff_fromfacepoly (pGeo);

  if ((*pMesh2D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh2D, Nodes.NodeQty);

  // face equations: the elt normals are (weighted) averaged, so as the
  // constant for the equation. 

  int elt, j;
  double area, norm;
  double* eq  = ut_alloc_1d (4);
  double* eqe = ut_alloc_1d (4);

  if ((*pGeo).FaceState == NULL)
    (*pGeo).FaceState = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
    
  ut_array_1d_int_set ((*pGeo).FaceState + 1, (*pGeo).FaceQty, 1);

  if ((*pGeo).FaceEq == NULL)
    (*pGeo).FaceEq = ut_alloc_2d ((*pGeo).FaceQty + 1, 4);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
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

    ut_array_1d_memcpy ((*pGeo).FaceEq[i], 4, eq);
  }

  ut_free_1d (eq);
  ut_free_1d (eqe);

  ut_free_2d_int (FacePoly, (*pGeo).FaceQty);

  return;
}

void
nem_reconmesh_1d (struct NODES Nodes, struct MESH *pMesh1D,
              struct MESH *pMesh2D, struct GEO* pGeo)
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

    (*pGeo).EdgeQty = EdgeQty;
    (*pGeo).EdgeFaceQty = ut_alloc_1d_int ((*pGeo).EdgeQty + 1); 
    ut_array_1d_int_memcpy ((*pGeo).EdgeFaceQty + 1, (*pGeo).EdgeQty,
	EdgeFaceQty);
    (*pGeo).EdgeFaceNb = ut_alloc_1d_pint ((*pGeo).EdgeQty + 1); 
    (*pGeo).EdgeFaceNb[0] = ut_alloc_1d_int (1);
    for (i = 1; i <= (*pGeo).EdgeQty; i++)
    {
      (*pGeo).EdgeFaceNb[i] = ut_alloc_1d_int ((*pGeo).EdgeFaceQty[i]);
      ut_array_1d_int_memcpy ((*pGeo).EdgeFaceNb[i], (*pGeo).EdgeFaceQty[i], EdgeFaceNb[i - 1]);
    }
  }
  else
  {
    (*pGeo).EdgeQty = (*pMesh1D).ElsetQty;
    neut_mesh_edgeface (*pMesh1D, *pMesh2D, &((*pGeo).EdgeFaceNb), &((*pGeo).EdgeFaceQty));
  }

  (*pGeo).EdgeState = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);
  (*pGeo).EdgeDel   = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);

  if ((*pMesh1D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh1D, Nodes.NodeQty);
  
  neut_geo_init_facestuff_fromedge (pGeo);

  ut_free_1d_int (EdgeFaceQty);
  ut_free_2d_int (EdgeFaceNb,(*pGeo).EdgeQty);

  return;
}

void
nem_reconmesh_0d (struct NODES Nodes, struct MESH *pMesh0D,
              struct MESH *pMesh1D, struct GEO* pGeo)
{
  int i, VerQty;
  int** VerEdgeNb = NULL;
  int*  VerEdgeQty = NULL;
  struct GEO Geob;
  struct GEO* pGeob = &Geob;

  neut_geo_set_zero (pGeob);

  if (pGeo != NULL)
    pGeob = pGeo;

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

    (*pGeob).VerQty = VerQty;
    (*pGeob).VerEdgeQty = ut_alloc_1d_int ((*pGeob).VerQty + 1);
    ut_array_1d_int_memcpy ((*pGeob).VerEdgeQty + 1, (*pGeob).VerQty,
	VerEdgeQty);
    (*pGeob).VerEdgeNb = ut_alloc_1d_pint ((*pGeob).VerQty + 1);
    (*pGeob).VerEdgeNb[0] = ut_alloc_1d_int (1);
    for (i = 1; i <= (*pGeob).VerQty; i++)
    {
      (*pGeob).VerEdgeNb[i] = ut_alloc_1d_int ((*pGeob).VerEdgeQty[i]);
      ut_array_1d_int_memcpy ((*pGeob).VerEdgeNb[i], (*pGeob).VerEdgeQty[i], VerEdgeNb[i - 1]);
    }
  }
  else
  {
    (*pGeob).VerQty = (*pMesh0D).ElsetQty;
    neut_mesh_veredge (*pMesh0D, *pMesh1D, &((*pGeob).VerEdgeNb), &((*pGeob).VerEdgeQty));
  }

  // Filling out VerCoo
  (*pGeob).VerCoo = ut_alloc_2d ((*pGeob).VerQty + 1, 3);
  for (i = 1; i <= (*pGeob).VerQty; i++)
    ut_array_1d_memcpy ((*pGeob).VerCoo[i], 3, Nodes.NodeCoo[(*pMesh0D).EltNodes[i][0]]);

  if (pGeo != NULL)
  {
    // Filling out VerState
    (*pGeob).VerState = ut_alloc_1d_int ((*pGeob).VerQty + 1);

    neut_geo_init_edgestuff_fromver (pGeob);

    // aligning edge definition with mesh elset
    int elt, node1;
    double dist1, dist2;
    for (i = 1; i <= (*pGeob).EdgeQty; i++)
    {
      elt = (*pMesh1D).Elsets[i][1];
      node1 = (*pMesh1D).EltNodes[elt][0];
      dist1 = ut_space_dist (Nodes.NodeCoo[node1],
			    (*pGeob).VerCoo[(*pGeob).EdgeVerNb[i][0]]);
      dist2 = ut_space_dist (Nodes.NodeCoo[node1],
			    (*pGeob).VerCoo[(*pGeob).EdgeVerNb[i][1]]);

      if (dist1 > dist2)
	ut_array_1d_int_reverseelts ((*pGeob).EdgeVerNb[i], 2);
    }

    neut_geo_init_facestuff_fromver (pGeob);
  }

  if (pGeo == NULL)
    neut_geo_free (pGeob);
  
  if (pGeo != NULL)
    ut_free_2d_int (VerEdgeNb, (*pGeo).VerQty);

  ut_free_1d_int (VerEdgeQty);

  return;
}

void
nem_reconmesh_finalizegeo (struct GEO* pGeo, struct NODES RNodes,
    struct MESH RMesh0D, struct MESH RMesh1D, struct MESH RMesh2D,
    struct MESH RMesh3D)
{
  int i, j, elt, node;
  double length;
  // , norm, area;
  // double* eq = ut_alloc_1d (4);
  // double* eqe = ut_alloc_1d (4);

  // (re)initializing geo properties from nodes / mesh ver coo
  for (i = 1; i <= (*pGeo).VerQty; i++)
  {
    node = RMesh0D.EltNodes[i][0];
    ut_array_1d_memcpy ((*pGeo).VerCoo[i], 3, RNodes.NodeCoo[node]);
  }
  
  // edge length (sum over the elements to account for curvature)
  for (i = 1; i <= (*pGeo).EdgeQty; i++)
  {
    length = 0;
    for (j = 1; j <= RMesh1D.Elsets[i][0]; j++)
    {
      elt = RMesh1D.Elsets[i][j];
      length += ut_space_dist (RNodes.NodeCoo[RMesh1D.EltNodes[elt][0]],
                               RNodes.NodeCoo[RMesh1D.EltNodes[elt][1]]);
    }
    (*pGeo).EdgeLength[i] = length;
  }
  
  // face equations: the elt normals are (weighted) averaged, so as the
  // constant for the equation. 

  /*
  if ((*pGeo).FaceState == NULL)
    (*pGeo).FaceState = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  if ((*pGeo).FaceEq == NULL)
    (*pGeo).FaceEq = ut_alloc_2d ((*pGeo).FaceQty + 1, 4);

  for (i = 1; i <= (*pGeo).FaceQty; i++)
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

    ut_array_1d_memcpy ((*pGeo).FaceEq[i], 4, eq);

    // checking the face normal against the orientations of the edges
    // (see TestFace)
    if (TestFace (*pGeo, i) != 0)
    {
      printf ("face normal against the orientations of the edges\n");
      printf ("did not work\n");
      abort ();
      ut_array_1d_scale ((*pGeo).FaceEq[i], 4, -1);
    }
  }
  */

  // poly centre: centre of mass of the elsets
  if ((*pGeo).CenterCoo == NULL)
    (*pGeo).CenterCoo = ut_alloc_2d ((*pGeo).PolyQty + 1, 3);
  for (i = 1; i <= (*pGeo).PolyQty; i++)
    neut_mesh_elset_centre (RNodes, RMesh3D, i, (*pGeo).CenterCoo[i]);

  // poly face ori
  if ((*pGeo).PolyFaceOri == NULL)
  {
    (*pGeo).PolyFaceOri = ut_alloc_1d_pint ((*pGeo).PolyQty + 1);
    (*pGeo).PolyFaceOri[0] = ut_alloc_1d_int (1);
  }

  int k;
  int face, elt2d, elt3dqty, elt3d;
  int* elts3d = NULL;
  double* eq = ut_alloc_1d (4);
  double* eltcentre = ut_alloc_1d (3);
  for (i = 1; i <= (*pGeo).PolyQty; i++)
  {
    // printf ("\npoly = %d ----------\n", i);
    (*pGeo).PolyFaceOri[i] = ut_alloc_1d_int ((*pGeo).PolyFaceQty[i] + 1);
    for (j = 1; j <= (*pGeo).PolyFaceQty[i]; j++)
    {
      face = (*pGeo).PolyFaceNb[i][j];
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
      ut_array_1d_fprintf (stdout, (*pGeo).FaceEq[face], 4, "%f");
      printf ("elt2d = %d    eq = ", elt2d);
      ut_array_1d_fprintf (stdout, eq, 4, "%f");
      printf ("eltcentre = ");
      ut_array_1d_fprintf (stdout, eltcentre, 3, "%f");
      printf ("planeside = %d\n", ut_space_planeside (eq, eltcentre - 1));
      */
      (*pGeo).PolyFaceOri[i][j] = -ut_space_planeside (eq, eltcentre - 1);
      if (ut_vector_scalprod ((*pGeo).FaceEq[face] + 1, eq + 1) < 0)
	(*pGeo).PolyFaceOri[i][j] *= -1;
      // printf (" --> faceori = %d\n", (*pGeo).PolyFaceOri[i][j]);
    }
  }
  ut_free_1d (eq);
  ut_free_1d (eltcentre);
  ut_free_1d_int (elts3d);

  // if (strcmp (RMesh3D.EltType, "tri") == 0 && (*pGeo).PolyTrue == NULL)

  // ut_free_1d (eqe);
  // ut_free_1d (eq);
  
  return;
}
