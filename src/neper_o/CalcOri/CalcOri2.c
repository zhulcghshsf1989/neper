/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"CalcOri.h"

int
neo_init_ori (FILE* file, struct ORI* pOri)
{
  int i;
  char* type = ut_alloc_1d_char (100);
  int n; // is -n in -T but ndensity in -MM
  int id;
  struct GEO Geo;

  neut_geo_set_zero (&Geo);

  neut_geo_fscanf_head (&Geo, file);

  n = Geo.N;
  id = Geo.Id;
  strcpy (type, Geo.morpho);

  if (strcmp  (type, "poisson"    ) == 0
   || strcmp  (type, "equiaxed"   ) == 0
   || strcmp  (type, "centroid"   ) == 0
   || strncmp (type, "columnar", 8) == 0
   || strncmp (type, "bamboo"  , 6) == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id) - 1;
  else if (strcmp (type, "cube") == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id) + 1;
  else if (strcmp (type, "dodeca") == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id) + 2;
  else if (strcmp (type, "tocta") == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id) + 3;
  else if (strcmp (type, "custom") == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id) + 4;
  else
    abort ();
  
  (*pOri).N = n;
  (*pOri).id = id;
  (*pOri).rngid = ut_alloc_1d_int ((*pOri).N + 1);

  if (! Geo.PolyId)
    for (i = 1; i <= (*pOri).N; i++)
      (*pOri).rngid[i] = i;
  else 
    ut_array_1d_int_memcpy ((*pOri).rngid + 1, (*pOri).N, Geo.PolyId + 1);
    
  ut_free_1d_char (type);

  neut_geo_free (&Geo);

  return 0;
}
