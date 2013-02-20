/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#define frand() ((double) rand() / (RAND_MAX+1.0))

#include"net_germ_reg.h"

void
net_germ_reg (struct POLY Dom, struct GERMSET *pGSet)
{
  if (! strcmp ((*pGSet).morpho, "cube"))
    CubeDistrib (Dom, pGSet);
  else if (! strcmp ((*pGSet).morpho, "tocta"))
    TruOctaDistrib (Dom, pGSet);
  else
    ut_error_reportbug ();

  return;
}
    
void
CubeDistrib (struct POLY Dom, struct GERMSET *pGSet)
{
  int x, y, z, nb;
  double* coo = ut_alloc_1d (3);

  /* IniGermCoo initializes the variable: GermCoo */
  (*pGSet).GermCoo = ut_alloc_2d ((*pGSet).N1d * (*pGSet).N1d
				      * (*pGSet).N1d + 1, 3);

  (*pGSet).Size = ut_alloc_1d (7);
  neut_poly_bbox (Dom, (*pGSet).Size);

  /* GermCoo is filled line by line. */
  nb = 0;
  for (x = 1; x <= (*pGSet).N1d; x++)
  {
    coo[0] = (*pGSet).Size[1] + (x - 0.5) / (*pGSet).N1d *
      ((*pGSet).Size[2] - (*pGSet).Size[1]);
    for (y = 1; y <= (*pGSet).N1d; y++)
    {
      coo[1] = (*pGSet).Size[3] + (y - 0.5) / (*pGSet).N1d *
	((*pGSet).Size[4] - (*pGSet).Size[3]);
      for (z = 1; z <= (*pGSet).N1d; z++)
      {
	coo[2] = (*pGSet).Size[5] + (z - 0.5) / (*pGSet).N1d *
	  ((*pGSet).Size[6] - (*pGSet).Size[5]);

	ut_array_1d_memcpy ((*pGSet).GermCoo[++nb], 3, coo);
      }
    }
  }

  ut_free_1d (coo);

  return;
}

void
TruOctaDistrib (struct POLY Dom, struct GERMSET *pGermSet)
{
  int x, y, z, i, j, nb;
  int n = ut_num_d2ri ((*pGermSet).N1d);
  double* coo = ut_alloc_1d (3);
  
  (*pGermSet).Size = ut_alloc_1d (7);
  neut_poly_bbox (Dom, (*pGermSet).Size);

  double* bbox = (*pGermSet).Size;

  /* IniGermCoo initializes the variable: GermCoo */
  (*pGermSet).GermCoo = ut_alloc_1d_pdouble (1);

  /* GermCoo is filled line by line. */
  nb = 0;
  for (x = 1; x < n; x++)
  {
    coo[0] = bbox[1] + (x - 0.5) / (n - 1) * (bbox[2] - bbox[1]);
    for (y = 1; y < n; y++)
    {
      coo[1] = bbox[3] + (y - 0.5) / (n - 1) * (bbox[4] - bbox[3]);
      for (z = 1; z < n; z++)
      {
	coo[2] = bbox[5] + (z - 0.5) / (n - 1) * (bbox[6] - bbox[5]);

	(*pGermSet).GermCoo
	  = ut_realloc_2d_addline ((*pGermSet).GermCoo, ++nb + 1, 3);

	ut_array_1d_memcpy ((*pGermSet).GermCoo[nb], 3, coo);
      }
    }
  }

  for (x = 0; x < n; x++)
  {
    coo[0] = bbox[1] + (double)x / (n - 1) * (bbox[2] - bbox[1]);
    for (y = 0; y < n; y++)
    {
      coo[1] = bbox[3] + (double)y / (n - 1) * (bbox[4] - bbox[3]);
      for (z = 0; z < n; z++)
      {
	coo[2] = bbox[5] + (double)z / (n - 1) * (bbox[6] - bbox[5]);
	
	(*pGermSet).GermCoo
	  = ut_realloc_2d_addline ((*pGermSet).GermCoo, ++nb + 1, 3);

	ut_array_1d_memcpy ((*pGermSet).GermCoo[nb], 3, coo);
      }
    }
  }

  (*pGermSet).N = nb;

  for (i = 1; i <= (*pGermSet).N; i++)
    for (j = 0; j < 3; j++)
      (*pGermSet).GermCoo[i][j]
	= ut_num_bound ((*pGermSet).GermCoo[i][j],
			bbox[2 * j + 1] + 1e-8, bbox[2 * j + 2] - 1e-8);

  ut_free_1d (coo);

  return;
}
