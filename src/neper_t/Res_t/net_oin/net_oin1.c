/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_oin.h"

int
net_oin_fprintf (FILE* file, struct GERMSET GermSet, int* id)
{
  int i;

  fprintf (file, "%s\n", GermSet.morpho);
  if (! strcmp  (GermSet.morpho, "poisson")    
   || ! strcmp  (GermSet.morpho, "equiaxed")   
   || ! strncmp (GermSet.morpho, "columnar", 8)
   || ! strncmp (GermSet.morpho, "bamboo"  , 6)
   || GermSet.morpho[0] == '@')
    fprintf (file, "%d %d %d\n", GermSet.N, GermSet.Id, GermSet.N);
  else if (! strcmp (GermSet.morpho, "cube")
        || ! strcmp (GermSet.morpho, "dodeca")
        || ! strcmp (GermSet.morpho, "tocta"))
    fprintf (file, "%d %d %d\n", GermSet.N1d, 1, GermSet.N);
  else
    abort ();

  if (id == NULL)
    fprintf (file, "continuous\n");
  else
  {
    fprintf (file, "list\n");
    for (i = 1; i <= GermSet.N; i++)
      fprintf (file, "%d\n", id[i]);
  }

  return 0;
}
