/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconstruct_mesh.h"

void
nem_reconstruct_mesh_2d (struct NODES Nodes, struct MESH *pMesh2D,
              struct MESH *pMesh3D, struct TESS* pTess)
{
  int i, FaceQty;

  // Tests on the input and output
  if (pMesh2D != NULL && (*pMesh2D).EltQty != 0)
    if ((*pMesh2D).Dimension != 2)
      ut_error_reportbug ();

  if (pMesh3D != NULL && (*pMesh3D).EltQty > 0)
    if ((*pMesh3D).Dimension != 3)
      ut_error_reportbug ();

  // If 3D mesh present, reconstructing the 2D mesh (if not void) and
  // setting FacePoly & Poly*
  if (pMesh3D != NULL && (*pMesh3D).EltQty > 0)
  {
    int** FacePoly = NULL;

    // Reconstructing 2D mesh
    if (pMesh2D == NULL || (*pMesh2D).EltQty == 0)
    {
      ut_print_message (0, 2, "Reconstructing 2D mesh ... ");

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

    ut_free_2d_int (FacePoly, (*pTess).FaceQty);
  }
  else
  {
    (*pTess).FaceQty = (*pMesh2D).ElsetQty;
  }

  if ((*pMesh2D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh2D, Nodes.NodeQty);

  // Ordering elsets so that all elts of an elset point towards the same
  // side of the elset.
  neut_mesh_init_eltelset (pMesh2D, NULL);
  neut_mesh_orderelsets (pMesh2D);

  // Setting up Tess face information

  if ((*pTess).FaceState == NULL)
    (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);

  ut_array_1d_int_set ((*pTess).FaceState + 1, (*pTess).FaceQty, 1);

  if ((*pTess).FaceEq == NULL)
    (*pTess).FaceEq = ut_alloc_2d ((*pTess).FaceQty + 1, 4);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    neut_mesh_elset_eq (*pMesh2D, Nodes, i, (*pTess).FaceEq[i]);

  return;
}

void
nem_reconstruct_mesh_1d (struct NODES Nodes, struct MESH *pMesh1D,
              struct MESH *pMesh2D, struct TESS* pTess)
{
  int i, EdgeQty;

  // Tests on the input and output
  if (pMesh1D != NULL && (*pMesh1D).EltQty != 0)
    if ((*pMesh1D).Dimension != 1)
      ut_error_reportbug ();

  if (pMesh2D != NULL && (*pMesh2D).EltQty > 0)
    if ((*pMesh2D).Dimension != 2)
      ut_error_reportbug ();

  // If 2D mesh present, reconstructing the 1D mesh (if not void) and
  // setting FacePoly & Poly*
  if (pMesh2D != NULL && (*pMesh2D).EltQty > 0)
  {
    int*  EdgeFaceQty = NULL;
    int** EdgeFaceNb = NULL;

    // Reconstructing 1D mesh
    if (pMesh1D == NULL || (*pMesh1D).EltQty == 0)
    {
      ut_print_message (0, 2, "Reconstructing 1D mesh ... ");

      neut_mesh_init_nodeelts (pMesh2D, Nodes.NodeQty);
      neut_mesh_init_eltelset (pMesh2D, NULL);
      neut_mesh2d_mesh1d (*pMesh2D, pMesh1D, &EdgeFaceNb, &EdgeFaceQty, &EdgeQty, 1);

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

    neut_tess_init_facestuff_fromedge (pTess);

    ut_free_1d_int (EdgeFaceQty);
    ut_free_2d_int (EdgeFaceNb,(*pTess).EdgeQty);
  }
  else
  {
    (*pTess).EdgeQty = (*pMesh1D).ElsetQty;
  }

  (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  (*pTess).EdgeDel   = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  if ((*pMesh1D).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh1D, Nodes.NodeQty);

  return;
}

void
nem_reconstruct_mesh_0d (struct NODES Nodes, struct MESH *pMesh0D,
              struct MESH *pMesh1D, struct TESS* pTess)
{
  int i, VerQty;

  // Tests on the input and output
  if (pMesh0D != NULL && (*pMesh0D).EltQty != 0)
    if ((*pMesh0D).Dimension != 0)
      ut_error_reportbug ();

  if (pMesh1D != NULL && (*pMesh1D).EltQty > 0)
    if ((*pMesh1D).Dimension != 1)
      ut_error_reportbug ();

  // If 1D mesh present, reconstructing the 0D mesh (if not void) and
  // setting FacePoly & Poly*
  if (pMesh1D != NULL && (*pMesh1D).EltQty > 0)
  {
    int** VerEdgeNb = NULL;
    int*  VerEdgeQty = NULL;

    // Reconstructing 0D mesh
    if (pMesh0D == NULL || (*pMesh0D).EltQty == 0)
    {
      ut_print_message (0, 2, "Reconstructing 0D mesh ... ");

      neut_mesh_init_nodeelts (pMesh1D, Nodes.NodeQty);
      neut_mesh_init_eltelset (pMesh1D, NULL);
      neut_mesh1d_mesh0d (*pMesh1D, pMesh0D, &VerEdgeNb, &VerEdgeQty, &VerQty, 1);

      if (pTess != NULL)
      {
	(*pTess).VerQty = VerQty;

	(*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
	ut_array_1d_int_memcpy ((*pTess).VerEdgeQty + 1, (*pTess).VerQty,
	    VerEdgeQty);
	(*pTess).VerEdgeNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);
	(*pTess).VerEdgeNb[0] = ut_alloc_1d_int (1);
	for (i = 1; i <= (*pTess).VerQty; i++)
	{
	  (*pTess).VerEdgeNb[i] = ut_alloc_1d_int ((*pTess).VerEdgeQty[i]);
	  ut_array_1d_int_memcpy ((*pTess).VerEdgeNb[i], (*pTess).VerEdgeQty[i], VerEdgeNb[i - 1]);
	}
      }
    }
    else
    {
      if (pTess != NULL)
      {
	(*pTess).VerQty = (*pMesh0D).ElsetQty;
	neut_mesh_veredge (*pMesh0D, *pMesh1D, &((*pTess).VerEdgeNb), &((*pTess).VerEdgeQty));
      }
    }

    if (pTess != NULL)
    {
      // Filling out VerCoo
      (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
      for (i = 1; i <= (*pTess).VerQty; i++)
	ut_array_1d_memcpy ((*pTess).VerCoo[i], 3, Nodes.NodeCoo[(*pMesh0D).EltNodes[i][0]]);

      neut_tess_init_edgestuff_fromver (pTess);

      // aligning edge definition with mesh elset
      int elt, node1;
      double dist1, dist2;
      for (i = 1; i <= (*pTess).EdgeQty; i++)
      {
	elt = (*pMesh1D).Elsets[i][1];
	node1 = (*pMesh1D).EltNodes[elt][0];
	dist1 = ut_space_dist (Nodes.NodeCoo[node1],
			      (*pTess).VerCoo[(*pTess).EdgeVerNb[i][0]]);
	dist2 = ut_space_dist (Nodes.NodeCoo[node1],
			      (*pTess).VerCoo[(*pTess).EdgeVerNb[i][1]]);

	if (dist1 > dist2)
	  ut_array_1d_int_reverseelts ((*pTess).EdgeVerNb[i], 2);
      }

      neut_tess_init_facestuff_fromver (pTess);
    }

    ut_free_2d_int (VerEdgeNb, (*pTess).VerQty);
    ut_free_1d_int (VerEdgeQty);
  }
  else
  {
    if (pTess != NULL)
    (*pTess).VerQty = (*pMesh0D).ElsetQty;
  }

  // Filling out VerState
  if (pTess != NULL)
    (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);

  return;
}
