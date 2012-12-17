/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transport.h"

void
nem_transport_mesh3d_parelt (struct GEO Geo, struct NODES OldNodes,
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
    neut_mesh_elt_centre (NewMesh3D, NewNodes, i, coo);
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
