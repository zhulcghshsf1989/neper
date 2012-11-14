/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_op_lcl.h"

void
neut_geo_init_domain_memcpy (struct GEO* pGeo, struct POLY Domain, char* type)
{
  int i;
  struct GEO DomGeo;

  neut_geo_set_zero (&DomGeo);
  neut_poly_geo (Domain, &DomGeo);

  // General
  (*pGeo).DomType = ut_alloc_1d_char (strlen (type) + 1);
  strcpy ((*pGeo).DomType, type);

  // Faces

  (*pGeo).DomFaceQty = DomGeo.FaceQty;
  (*pGeo).DomFaceEq = ut_alloc_2d ((*pGeo).DomFaceQty + 1, 4);
  (*pGeo).DomFaceVerQty = ut_alloc_1d_int  ((*pGeo).DomFaceQty + 1);
  (*pGeo).DomFaceVerNb  = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
  (*pGeo).DomFaceEdgeNb = ut_alloc_1d_pint ((*pGeo).DomFaceQty + 1);
    
  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
    ut_array_1d_memcpy ((*pGeo).DomFaceEq[i], 4, DomGeo.FaceEq[i]);

  ut_array_1d_int_memcpy ((*pGeo).DomFaceVerQty, (*pGeo).DomFaceQty + 1, DomGeo.FaceVerQty);

  for (i = 1; i <= (*pGeo).DomFaceQty; i++)
  {
    (*pGeo).DomFaceVerNb[i]  = ut_alloc_1d_int ((*pGeo).DomFaceVerQty[i]  + 1);
    (*pGeo).DomFaceEdgeNb[i] = ut_alloc_1d_int ((*pGeo).DomFaceVerQty[i] + 1);
    
    ut_array_1d_int_memcpy ((*pGeo).DomFaceVerNb[i] + 1, (*pGeo).DomFaceVerQty[i],
	                        DomGeo.FaceVerNb[i] + 1);
    
    ut_array_1d_int_memcpy ((*pGeo).DomFaceEdgeNb[i] + 1, (*pGeo).DomFaceVerQty[i],
	                        DomGeo.FaceEdgeNb[i] + 1);
  }

  // Edges
  (*pGeo).DomEdgeQty = DomGeo.EdgeQty;
  (*pGeo).DomEdgeVerNb = ut_alloc_2d_int (DomGeo.EdgeQty + 1, 2);
  (*pGeo).DomEdgeFaceNb = ut_alloc_2d_int (DomGeo.EdgeQty + 1, 2);

  for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
    ut_array_1d_int_memcpy ((*pGeo).DomEdgeVerNb[i], 2, DomGeo.EdgeVerNb[i]);

  for (i = 1; i <= (*pGeo).DomEdgeQty; i++)
  {
    ut_array_1d_int_memcpy ((*pGeo).DomEdgeFaceNb[i], 2, DomGeo.EdgeFaceNb[i]);
    if (DomGeo.EdgeFaceQty[i] != 2)
      ut_error_reportbug ();
  }

  // Vertices
  (*pGeo).DomVerQty = DomGeo.VerQty;
  (*pGeo).DomVerEdgeQty = ut_alloc_1d_int (DomGeo.VerQty + 1);
  (*pGeo).DomVerEdgeNb  = ut_alloc_1d_pint (DomGeo.VerQty + 1);

  ut_array_1d_int_memcpy ((*pGeo).DomVerEdgeQty + 1, (*pGeo).DomVerQty, DomGeo.VerEdgeQty + 1);
  for (i = 1; i <= (*pGeo).DomVerQty; i++)
  {
    (*pGeo).DomVerEdgeNb[i] = ut_alloc_1d_int ((*pGeo).DomVerEdgeQty[i]);
    ut_array_1d_int_memcpy ((*pGeo).DomVerEdgeNb[i], (*pGeo).DomVerEdgeQty[i],
	                    DomGeo.VerEdgeNb[i]);
  }

  neut_geo_free (&DomGeo);

  return;
}
