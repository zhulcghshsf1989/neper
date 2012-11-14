/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DeleteEdge.h"

int
DeleteEdge (struct GEO *pGeo, int edge, int *pver, double *pmaxff)
{
  int delver, test;
  int verbosity = 0;

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

  if ((*pGeo).VerDom[ver1][0] == 1 && (*pGeo).VerDom[ver2][0] == 1
      && ((*pGeo).VerDom[ver1][1] != (*pGeo).VerDom[ver2][1]))
    ut_error_reportbug ();

  UpdateEdgeState (pGeo, edge);
  (*pGeo).EdgeDel[edge] = 1;

  SearchDelNNewVer (pGeo, edge, &delver, pver);

  /* UpdateFace updates the properties of the parent faces
   * of the edge and more generally of delver. */
  UpdateFaceVerNEdge (pGeo, edge, delver, *pver, verbosity);

  UpdateVer (pGeo, edge, delver, *pver, verbosity);

  UpdateEdgeVerNb (pGeo, delver, *pver, verbosity);

  int status = UpdateVerCoo (pGeo, *pver, verbosity);
  if (status != 0)
  {
    (*pmaxff) = 888.8;
    test = -1;
    return test;
  }

  UpdateEdgeLength (pGeo, *pver);
  
  nefm_edgedel_edgedom_fromverdom (pGeo, *pver);

  nefm_geo_reg_ver_facedom (pGeo, *pver);
  
  if (verbosity >= 2)
    ut_print_message (0, 3, "Testing geometry ...\n");

  {
    test = neut_geo_testAroundVer2 (*pGeo, *pver, 0);

    if (test != 0)
      (*pmaxff) = 888.8;
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
