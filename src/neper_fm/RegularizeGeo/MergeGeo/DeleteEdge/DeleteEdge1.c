/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DeleteEdge.h"

// Delete an edge.  Returns 0 for success, -1 if the Geo test fails
// and -2 if the edge deletion aborted.
int
DeleteEdge (struct GEO *pGeo, int edge, int *pver, double *pmaxff)
{
  int i, delver, test;
  int verbosity = 0;

  // setting default values
  (*pver) = -1;
  (*pmaxff) = 888.8;

  if (verbosity >= 2)
    printf ("\ndeleting edge %5d  (length = %f)------------------------------\n",
	    edge, (*pGeo).EdgeLength[edge]);

  int ver1 = (*pGeo).EdgeVerNb[edge][0];
  int ver2 = (*pGeo).EdgeVerNb[edge][1];

  if (verbosity >= 2)
  {
    ut_print_lineheader (-1);
    printf ("has ver %d and %d of bounds %d %d and %d %d\n", ver1, ver2,
	(*pGeo).VerDom[ver1][0], (*pGeo).VerDom[ver1][1],
	(*pGeo).VerDom[ver2][0], (*pGeo).VerDom[ver2][1]);
  }

  // test for edge not on a domain edge, but shrinking into a domain
  // vertex - this case is not treated properly.
  if ((*pGeo).VerDom[ver1][0] == 1 && (*pGeo).VerDom[ver2][0] == 1
      && ((*pGeo).VerDom[ver1][1] != (*pGeo).VerDom[ver2][1]))
    return -2;

  // test for edge equal to a domain edge.
  if ((*pGeo).EdgeDom[edge][0] == 1
   && (*pGeo).DomTessEdgeQty[(*pGeo).EdgeDom[edge][1]] == 1)
    return -2;

  // test for edge belonging to a tet polyhedron, which
  // would mean deleting the polyhedron - this is not allowed.
  // test on the number of vertices, NOT on the number of faces.
  // (a regularized poly can have 4 faces, but > 4 vertices).
  int* poly = NULL;
  int polyqty;
  int* ver = NULL;
  int verqty;
  neut_geo_edge_polys (*pGeo, edge, &poly, &polyqty);
  for (i = 0; i < polyqty; i++)
  {
    neut_geo_poly_vers (*pGeo, poly[i], &ver, &verqty);
    if (verqty == 4)
    {
      ut_free_1d_int (poly);
      return -2;
    }
  }
  ut_free_1d_int (poly);
  ut_free_1d_int (ver);

  // test for edge leading to the a "bow tie"-shaped face after being
  // deleted
  if (neut_geo_edge_fake (*pGeo, edge) == 1)
  {
    int face1, face2;
    face1 = (*pGeo).EdgeFaceNb[edge][0];
    face2 = (*pGeo).EdgeFaceNb[edge][1];
    if ((*pGeo).FaceVerQty[face1] > 3 && (*pGeo).FaceVerQty[face2] > 3)
    {
      return -2;
    }
  }

  UpdateEdgeState (pGeo, edge);
  (*pGeo).EdgeDel[edge] = 1;

  SearchDelNNewVer (pGeo, edge, &delver, pver);

  /* UpdateFace updates the properties of the parent faces
   * of the edge and more generally of delver. */
  if (UpdateFaceVerNEdge (pGeo, edge, delver, *pver, verbosity) != 0)
    return -2;

  if (UpdateVer (pGeo, edge, delver, *pver, verbosity) != 0)
    return -2;

  UpdateEdgeVerNb (pGeo, delver, *pver, verbosity);

  if (UpdateVerCoo (pGeo, *pver, verbosity) != 0)
    return -2;

  UpdateEdgeLength (pGeo, *pver);
  
  nefm_edgedel_edgedom_fromverdom (pGeo, *pver);

  nefm_geo_reg_ver_facedom (pGeo, *pver);
  
  if (verbosity >= 2)
    ut_print_message (0, 3, "Testing geometry ...\n");

  {
    test = neut_geo_testAroundVer2 (*pGeo, *pver, 0);

    if (test != 0)
    {
      test = -1;
      (*pmaxff) = 888.8;
    }
    else
    {
      if (verbosity >= 2)
	ut_print_message (0, 3, "Updating face flatness faults.\n");
      (*pmaxff) = UpdateFaceFF (pGeo, *pver);
    }
  }

  if (verbosity >= 2)
  {
    ut_print_lineheader (0);
    printf ("maxff=%.2f\n", (*pmaxff));
  }
  
  return test;
}
