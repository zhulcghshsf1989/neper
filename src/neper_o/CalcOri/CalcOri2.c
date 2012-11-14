/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"CalcOri.h"

int
neo_oin_fscanf (FILE* file, struct ORI* pOri)
{
  int i;
  char* type = ut_alloc_1d_char (100);
  int n; // is -n in -T but ndensity in -MM
  int id;

  fscanf (file, "%s", type);

  fscanf (file, "%d%d", &n, &id);

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
  
  fscanf (file, "%d", &(*pOri).N);

  fscanf (file, "%s", type);

  (*pOri).id = id;
  (*pOri).rngid = ut_alloc_1d_int ((*pOri).N + 1);
  if (strcmp (type, "continuous") == 0)
    for (i = 1; i <= (*pOri).N; i++)
      (*pOri).rngid[i] = i;
  else if (strcmp (type, "list") == 0)
    for (i = 1; i <= (*pOri).N; i++)
      fscanf (file, "%d", &(*pOri).rngid[i]);
    
  ut_free_1d_char (type);

  return 0;
}
