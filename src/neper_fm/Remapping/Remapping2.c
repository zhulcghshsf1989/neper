/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Remapping.h"

void
nefm_remapping_mesh3d_parelt (struct GEO Geo, struct NODES OldNodes,
    struct MESH OldMesh2D, struct MESH OldMesh3D, struct NODES NewNodes,
    struct MESH NewMesh2D, struct MESH NewMesh3D, int* oldelt)
{
  int i, j, elset3d, status;
  double* coo = ut_alloc_1d (3);
  struct MESH Facet;
  char* message = ut_alloc_1d_char (1000);
  int* elts2d = ut_alloc_1d_int (5);
  int message_length;
  double* eq = ut_alloc_1d (4);

  Geo.PolyQty = Geo.PolyQty;
  OldMesh2D.EltQty = OldMesh2D.EltQty;

  neut_mesh_set_zero (&Facet);

  if (NewMesh2D.NodeElts == NULL)
    neut_mesh_init_nodeelts (&NewMesh2D, NewNodes.NodeQty);

  ut_print_message (0, 3, "Searching transport information ... %3d%%", 0);
  fflush (stdout);

  message_length = 0;
  // for each new elt, determining parent elt
  for (i = 1; i <= NewMesh3D.EltQty; i++)
  {
    // parent element = old element in which the new element centre falls.
    elset3d = NewMesh3D.EltElset[i];
    neut_mesh_eltcentre (NewMesh3D, NewNodes, i, coo);
    status = neut_mesh_elset_point_elt (OldMesh3D, OldNodes, elset3d,
	coo, &(oldelt[i]));

    // if it does not fall in any old element, picking the closest.
    if (status != 0)
      status = neut_mesh_elset_point_closestelt (OldMesh3D, OldNodes,
	  elset3d, coo, &(oldelt[i]));

    if (i == 1)
      printf ("\b\b\b\b");
    
    sprintf (message, "%3.0f%%", floor (100 * ((double) i /
	    NewMesh3D.EltQty)));

    for (j = 0; j < message_length; j++)
      fprintf (stdout, "\b");
    fprintf (stdout, "%s", message);
    message_length = strlen (message);
    fflush (stdout);
  }
  fprintf (stdout, "\n");

  ut_free_1d_int (elts2d);
  neut_mesh_free (&Facet);
  ut_free_1d (coo);
  ut_free_1d_char (message);
  ut_free_1d (eq);

  return;
}
      
/*
// no parent element found in the old mesh.  This usually occurs
// when the new mesh is (at least locally) coarsened close to
// concave grain boundaries, which causes the element (its centre)
// to be "out" of the old grain.
//
// Solution 1: looking for the grain boundary to which the element
// belongs, projecting the tet element centre onto it (on the old
// mesh) to determine the triangle element into which it falls,
// then searching the corresponding 3D element to which the data
// has to be picked.

// printf ("solution 1\n");
neut_mesh_elt3d_elts2d (NewNodes, NewMesh3D, i, NewMesh2D,
			elts2d + 1, &(elts2d[0]));

printf ("elts2d: ");
ut_array_1d_int_fprintf (stdout, elts2d, elts2d[0] + 1, "%d");

neut_mesh_elts_elsets (NewMesh2D, elts2d + 1, elts2d[0],
		       elsets2d + 1, &(elsets2d[0]));

printf ("elsets2d: ");
ut_array_1d_int_fprintf (stdout, elsets2d, elsets2d[0] + 1, "%d");
// printf ("\n");
// printf ("coo = %f %f %f\n", coo[0], coo[1], coo[2]);

// printf ("elsets2d = ");
// ut_array_1d_int_fprintf (stdout, elsets2d + 1, elsets2d[0], "%d");

oelt = -1;
if (elsets2d[0] > 0)
  for (j = 1; j <= elsets2d[0]; j++)
  {
    elset2d = elsets2d[j];
    // printf ("projecting to face %d along %f %f %f: ", elset2d, Geo.FaceEq[elset2d][1], Geo.FaceEq[elset2d][2], Geo.FaceEq[elset2d][3]);
    oelt  = neut_point_proj_alongontomesh (coo, Geo.FaceEq[elset2d] + 1,
	OldNodes, OldMesh2D, elset2d);
    if (oelt > 0)
      break;
  }


// Solution 2: if the element does not belong to any grain
// boundary, looking for the correct grain boundary by:
// - projecting its center to all grain boundaries (for some
// grains, it will not fall in the grain boundary);
// - selecting the closest grain boundary;
// - searching the triangle in which it falls;
// - searching the corresponding 3D element.
//
// Note: solution 2 leading to no result is possible, but very
// unlikely.

if (oelt == -1)
{
  // printf ("entering special case\n");

  dist = ut_alloc_1d (Geo.PolyFaceQty[elset3d]);

  // projecting to all faces & recording distances
  for (j = 1; j <= Geo.PolyFaceQty[elset3d]; j++)
  {
    // printf ("j = %d ", j);
    elset2d = Geo.PolyFaceNb[elset3d][j];
    // printf ("elset2d = %d\n  ", elset2d);

    ut_array_1d_memcpy (proj_coo, 3, coo);
    oelt  = neut_point_proj_alongontomesh (proj_coo,
	Geo.FaceEq[elset2d] + 1, NewNodes, NewMesh2D, elset2d);
    dist[j - 1] = (oelt == -1) ? DBL_MAX
				 : ut_space_dist (coo, proj_coo);

    // printf ("coo = %f %f %f proj_coo = %f %f %f\n", coo[0], coo[1], coo[2], proj_coo[0], proj_coo[1], proj_coo[2]);

  }
  // printf ("dist = \n");
  // ut_array_1d_fprintf (stdout, dist, Geo.PolyFaceQty[elset3d], "%g");

  // picking the closest
  elset2d = 1 + ut_array_1d_min_index (dist, Geo.PolyFaceQty[elset3d]);
  elset2d = Geo.PolyFaceNb[elset3d][elset2d];
  
  // printf ("elset2d = %d\n", elset2d);

  if (dist[elset2d - 1] > 0.99 * DBL_MAX) // unlikely error
  {
    ut_print_message (2, 0, "Parent element not found (error 1).\n");
    ut_print_message (2, 0, "Element centroid does not project on any grain boundary.\n");
    printf ("elt = %d, elset3d = %d\n", i, elset3d);
    ut_error_reportbug ();
    abort ();
  }

  oelt  = neut_point_proj_alongontomesh (coo, Geo.FaceEq[elset2d] + 1,
      OldNodes, OldMesh2D, elset2d);
  // printf ("-> oelt = %d\n", oelt);

  if (oelt < 0) // unlikely error
  {
    ut_print_message (2, 0, "Parent element not found (error 2).\n");
    printf ("elt = %d, elset3d = %d\n", i, elset3d);
    ut_error_reportbug ();
    abort ();
  }

  ut_free_1d (dist);
}

neut_mesh_elt2delts3d (OldMesh2D, oelt, OldMesh3D, oldelts3d, &qty);

if (qty == 1)
  oldelt[i] = oldelts3d[0];
else if (qty == 2)
{
  elset3d = NewMesh3D.EltElset[i];
  if (OldMesh3D.EltElset[oldelts3d[0]] == elset3d)
    oldelt[i] = oldelts3d[0];
  else if (OldMesh3D.EltElset[oldelts3d[1]] == elset3d)
    oldelt[i] = oldelts3d[1];
  else
  {
    ut_print_message (2, 0, "parent and child elsets do not match!");
    ut_error_reportbug ();
  }
}
else
{
  printf ("qty = %d != 1 && != 2\n", qty);
  ut_error_reportbug ();
}

// printf ("  --> parent element = %d\n", oldelt[i]);
//
*/
