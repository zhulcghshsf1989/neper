/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing.h"

// this routine tracks the element 2d which are based on two (adjacent)
// 1d elements, whose common node is not a 0d element.  These elements
// are formed for highly deformed mesh of Voronoi tessellations
// (otherwise such 1d elements are colinear and so cannot form a 2d
// element.  Such elements can (do always?) lead to the volume being pinched
// out.
int
nem_remesh_fixmesh2delset (struct GEO Geo, struct MESH Mesh1D,
                           int face, struct MESH* pMesh2D)
{
  int i, j, k, l, m, elt2d, nodeqty, edge, neighedge, elt2db;
  int*  nodes = ut_alloc_1d_int (4);
  int** faceeltnodes = ut_alloc_2d_int ((*pMesh2D).Elsets[face][0] + 1, 3);
  int* tmparray = ut_alloc_1d_int (10);
  int tmp;
  int commonnode;
  int* boundnodes = ut_alloc_1d_int (3); // let 3
  int elt1node3, elt2node3;
  int status = 0;
  int** elt1dpair = NULL;
  int elt1dpair_qty;

  // initializing faceeltnodes
  for (i = 1; i <= (*pMesh2D).Elsets[face][0]; i++)
  {
    elt2d = (*pMesh2D).Elsets[face][i];

    ut_array_1d_int_memcpy (faceeltnodes[i], 3, (*pMesh2D).EltNodes[elt2d]);
    ut_array_1d_int_sort (faceeltnodes[i], 3);
  }

  // initializing elt1dpair: the elt1d pairs to check
  
  elt1dpair = ut_alloc_2d_int (1, 2);
  elt1dpair_qty = 0;

  // 1/ adding the adjacent elts of every edges
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];
    for (j = 1; j < Mesh1D.Elsets[edge][0]; j++)
    {
      elt1dpair = ut_realloc_2d_int_addline (elt1dpair, ++elt1dpair_qty, 2);
      ut_array_1d_int_memcpy (elt1dpair[elt1dpair_qty - 1], 2, Mesh1D.Elsets[edge] + j);
    }
  }

  // 2/ case of 1-elt edges.  Adding pairs based on that elt.
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];
    if (Mesh1D.Elsets[edge][0] == 1)
      for (j = -1; j <= 1; j += 2)
      {
	neighedge = Geo.FaceEdgeNb[face]
	               [ut_num_rotpos (1, Geo.FaceVerQty[face], i, j)];

	elt1dpair = ut_realloc_2d_int_addline (elt1dpair, ++elt1dpair_qty, 2);
	elt1dpair[elt1dpair_qty - 1][0] = Mesh1D.Elsets[edge][1];

	if (ut_array_1d_int_eltpos
	      (Mesh1D.EltNodes[Mesh1D.Elsets[neighedge][1]], 2,
	       Mesh1D.EltNodes[Mesh1D.Elsets[     edge][1]][0]) != -1
	 || ut_array_1d_int_eltpos
	      (Mesh1D.EltNodes[Mesh1D.Elsets[neighedge][1]], 2,
	       Mesh1D.EltNodes[Mesh1D.Elsets[     edge][1]][1]) != -1)
	{
	  elt1dpair[elt1dpair_qty - 1][1] = Mesh1D.Elsets[neighedge][1];
	}
	else if (ut_array_1d_int_eltpos
	      (Mesh1D.EltNodes[Mesh1D.Elsets[neighedge]
	                                  [Mesh1D.Elsets[neighedge][0]]], 2,
	       Mesh1D.EltNodes[Mesh1D.Elsets[     edge][1]][0] != -1)
	 || ut_array_1d_int_eltpos
	      (Mesh1D.EltNodes[Mesh1D.Elsets[neighedge]
	                                  [Mesh1D.Elsets[neighedge][0]]], 2,
	       Mesh1D.EltNodes[Mesh1D.Elsets[     edge][1]][1]) != -1)
	{
	  elt1dpair[elt1dpair_qty - 1][1] =
	       Mesh1D.Elsets[neighedge][Mesh1D.Elsets[neighedge][0]];
	}
	else
	  ut_error_reportbug ();
      }

    for (j = 1; j < Mesh1D.Elsets[edge][0]; j++)
    {
      elt1dpair = ut_realloc_2d_int_addline (elt1dpair, ++elt1dpair_qty, 2);
      ut_array_1d_int_memcpy (elt1dpair[elt1dpair_qty - 1], 2, Mesh1D.Elsets[edge] + j);
    }
  }

  // for every edge of the face, looking at its pairs of adjacent
  // elements.  For each pair, checking if a 2D element is based on
  // them.
  for (i = 0; i < elt1dpair_qty; i++)
  {
    ut_array_1d_int_union (Mesh1D.EltNodes[elt1dpair[i][0]], 2,
			   Mesh1D.EltNodes[elt1dpair[i][1]], 2,
			   nodes, &nodeqty);
    ut_array_1d_int_inter (Mesh1D.EltNodes[elt1dpair[i][0]], 2,
			   Mesh1D.EltNodes[elt1dpair[i][1]], 2,
			   tmparray, &tmp );
    commonnode = tmparray[0];

    if (nodeqty != 3 && tmp != 1)
      ut_error_reportbug ();

    for (k = 1; k <= (*pMesh2D).Elsets[face][0]; k++)
    {
      elt2d = (*pMesh2D).Elsets[face][k];

      if (ut_array_1d_int_equal (faceeltnodes[k], nodes, 3))
      {
	// looking for the other 2D element.
	ut_array_1d_int_memcpy (boundnodes, 3, nodes);
	ut_array_1d_int_deletencompress (boundnodes, 3, commonnode, 1);

	elt1node3 = -1;
	for (m = 0; m < 3; m++)
	  if (ut_array_1d_int_eltpos (boundnodes, 2,
				      (*pMesh2D).EltNodes[elt2d][m]) == -1)
	  {
	    elt1node3 = (*pMesh2D).EltNodes[elt2d][m];
	    break;
	  }

	for (l = 1; l <= (*pMesh2D).Elsets[face][0]; l++)
	{
	  if (l == k)
	    continue;
	  
	  elt2db = (*pMesh2D).Elsets[face][l];

	  if (ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[elt2db],
						 3, boundnodes[0]) != -1
	   && ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[elt2db], 
						 3, boundnodes[1]) != -1)
	  {
	    elt2node3 = -1;
	    for (m = 0; m < 3; m++)
	      if (ut_array_1d_int_eltpos (boundnodes, 2,
		      (*pMesh2D).EltNodes[elt2db][m]) == -1)
	      {
		elt2node3 = (*pMesh2D).EltNodes[elt2db][m];
		break;
	      }

	    /*
	    printf ("elts are %d and %d\n", elt2d, elt2db);
	    printf ("elt1 nodes: ");
	    ut_array_1d_int_fprintf (stdout, (*pMesh2D).EltNodes[elt2d],
				     3, "%d");
	    printf ("elt2 nodes: ");
	    ut_array_1d_int_fprintf (stdout, (*pMesh2D).EltNodes[elt2db],
				     3, "%d");
	    printf ("boundnodes: ");
	    ut_array_1d_int_fprintf (stdout, boundnodes, 2, "%d");

	    printf ("nodes3 = %d and %d\n", elt1node3, elt2node3);
	    */

	    ut_array_1d_int_findnreplace ((*pMesh2D).EltNodes[elt2d], 3,
				 boundnodes[0], elt2node3);
	    ut_array_1d_int_findnreplace ((*pMesh2D).EltNodes[elt2db], 3,
				 boundnodes[1], elt1node3);

	    status++;

	    /*
	    printf ("elt1 nodes: ");
	    ut_array_1d_int_fprintf (stdout, (*pMesh2D).EltNodes[elt2d],
				     3, "%d");
	    printf ("elt2 nodes: ");
	    ut_array_1d_int_fprintf (stdout, (*pMesh2D).EltNodes[elt2db],
				     3, "%d");
				     */
	  }
	}
      }
    }
  }

  ut_free_1d_int (nodes);
  ut_free_2d_int (faceeltnodes, (*pMesh2D).Elsets[face][0] + 1);
  ut_free_1d_int (tmparray);
  ut_free_1d_int (boundnodes);
  ut_free_2d_int (elt1dpair, elt1dpair_qty);

  return status;
}
