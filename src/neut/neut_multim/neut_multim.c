/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_multim.h"

void
neut_multim_set_zero (struct MULTIM* pMultim)
{
  (*pMultim).algoqty = 0;
  (*pMultim).algos = NULL;
  (*pMultim).algohit = NULL;
  
  (*pMultim).mOdis  = NULL;
  (*pMultim).mOsize = NULL;
  (*pMultim).mO     = NULL;

  (*pMultim).Oalgo = NULL;

  (*pMultim).Odis  = NULL;
  (*pMultim).Osize = NULL;
  (*pMultim).O     = NULL;

  return;
}

void
neut_multim_free (struct MULTIM* pMultim, int PolyQty)
{
  
  ut_free_3d_char ((*pMultim).algos, (*pMultim).algoqty, 2);
  ut_free_1d_int ((*pMultim).algohit);
  
  ut_free_2d ((*pMultim).mOdis, PolyQty + 1);
  ut_free_2d ((*pMultim).mOsize, PolyQty + 1);
  ut_free_2d ((*pMultim).mO, PolyQty + 1);

  ut_free_1d_int ((*pMultim).Oalgo);

  ut_free_1d ((*pMultim).Odis);
  ut_free_1d ((*pMultim).Osize);
  ut_free_1d ((*pMultim).O);

  return;
}

void
neut_multim_fromlist (char* list, struct MULTIM* pMultim)
{
  int i, qty;
  char** ind_algo = NULL;
  char** ind_algo2 = NULL;
  ut_string_fnr (list, ':', '/');
  ut_string_separate (list, ',', &ind_algo, &(*pMultim).algoqty);

  (*pMultim).algos = ut_alloc_3d_char ((*pMultim).algoqty, 2, 5);
  (*pMultim).algohit = ut_alloc_1d_int ((*pMultim).algoqty);

  for (i = 0; i < (*pMultim).algoqty; i++)
  {
    ut_string_separate (ind_algo[i], '/', &ind_algo2, &qty);
    sprintf ((*pMultim).algos[i][0], "%s", ind_algo2[0]);
    if (qty == 2)
      sprintf ((*pMultim).algos[i][1], "%s", ind_algo2[1]);
    else
    {
      ut_free_1d_char ((*pMultim).algos[i][1]);
      (*pMultim).algos[i][1] = NULL;
    }
    ut_free_2d_char (ind_algo2, qty);
  }
  ut_free_2d_char (ind_algo, (*pMultim).algoqty);

  return;
}
