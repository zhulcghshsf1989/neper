/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "NSite.h"

void
SearchNSite (struct IN *pIn, struct GERMSET GermSet,
	     int *pSiteQty, double ***pSiteCoo, int ***pSiteType)
{
  struct TESS Tess;
  struct GERMSET GermSet2;
  int i;
  int ZoneAWidth, ZoneBWidth;
  double **ZoneAGermsCoo;
  double **ZoneBGermsCoo;
  int ZoneAN, ZoneBN;
  int rngseed;

  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, GermSet.Random);

  ut_print_message (0, 2, "Creating sites ...\n");

  /* skipping the used value */
  for (i = 1; i <= GermSet.nGermRandId[GermSet.nN] + 2; i++)
    if (gsl_rng_uniform (r) > 10) // this is to trash;
      r = NULL;

  rngseed = 4;

  ut_print_message (0, 3, "Calculating zone A ...\n");
  /* Searching zone A and zone B */
  CalcZoneA (pIn, GermSet, &ZoneAWidth, &ZoneAN, &ZoneAGermsCoo, &rngseed, r);

  ut_print_message (0, 3, "Preparing calculation of zone B ...\n");
  CalcNSiteNeigh (GermSet, &GermSet2, ZoneAN, ZoneAGermsCoo, (*pIn).nsitesize);

  ut_print_message (0, 3, "Calculating zone B ...\n");
  CalcZoneB (GermSet, GermSet2, ZoneAWidth, &ZoneBWidth, &ZoneBN,
	     &ZoneBGermsCoo, &rngseed, r);

  ut_print_message (0, 3, "Generating tessellation of zone B ...\n");
  /* Creating the tessellation of the considered medium */
  CreateNSitenNeighTess (GermSet, GermSet2, ZoneAN, ZoneAGermsCoo,
			 ZoneBN, ZoneBGermsCoo, &Tess);

  ut_print_message (0, 2, "Recording sites ...\n");

  struct GEO Geo;

  neut_geo_set_zero (&Geo);

  neut_tess_geo (Tess, &Geo);

  /* Recording the nucleation sites */
  if ((*pIn).nsite == 4)
    RecNSiteGeom ((*pIn), Geo, pSiteQty, pSiteCoo, pSiteType);
  else
  {
    RecNSiteArray ((*pIn), Geo, (*pIn).msize, pSiteQty, pSiteCoo,
		   pSiteType);
  }

  neut_geo_free (&Geo);

  return;
}
