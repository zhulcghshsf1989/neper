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
  struct TESS Tess;

  neut_tess_set_zero (&Tess);

  neut_tess_fscanf_head (&Tess, file);

  n = Tess.N;
  id = Tess.Id;
  strcpy (type, Tess.morpho);

  if (strcmp  (type, "poisson"    ) == 0
   || strcmp  (type, "equiaxed"   ) == 0
   || strcmp  (type, "centroid"   ) == 0
   || strncmp (type, "columnar", 8) == 0
   || strncmp (type, "bamboo"  , 6) == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id, 0, 1) - 1;
  else if (strcmp (type, "cube") == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id, 0, 1) + 1;
  else if (strcmp (type, "dodeca") == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id, 0, 1) + 2;
  else if (strcmp (type, "tocta") == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id, 0, 1) + 3;
  else if (strcmp (type, "custom") == 0)
    (*pOri).Random = neut_rand_nnid2rand (n, id, 0, 1) + 4;
  else
    abort ();
  
  (*pOri).N = n;
  (*pOri).id = id;
  (*pOri).rngid = ut_alloc_1d_int ((*pOri).N + 1);

  if (! Tess.PolyId)
    for (i = 1; i <= (*pOri).N; i++)
      (*pOri).rngid[i] = i;
  else 
    ut_array_1d_int_memcpy ((*pOri).rngid + 1, (*pOri).N, Tess.PolyId + 1);
    
  ut_free_1d_char (type);

  neut_tess_free (&Tess);

  return 0;
}
