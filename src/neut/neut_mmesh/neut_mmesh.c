/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mmesh.h"


extern void
neut_mmesh_elset_fscanf (FILE* file, struct ELSET *pElset, int MaxElsetQty)
{
  int skip, i, test, elset;
  char *String = ut_alloc_1d_char (10000);
  char *trash = ut_alloc_1d_char (1000);
  int *EltQty = ut_alloc_1d_int (MaxElsetQty + 1);
  int tmp;

  (*pElset).Qty = 0;

  /* Counting the number of elsets */
  test = 0;
  tmp = 0;
  skip = -1;
  while (test != -1 && strcmp (String, "***return") != 0)
  {
    if (strcmp (String, "**elset") == 0)
    {
      if (skip == -1)
	skip = tmp - 1;
      (*pElset).Qty++;
      EltQty[(*pElset).Qty - 1] = tmp - 1;
      test = fscanf (file, "%s", trash);
      tmp = 0;
    }

    test = fscanf (file, "%s", String);
    tmp++;
  }
  EltQty[0] = 0;
  EltQty[(*pElset).Qty] = tmp - 1;

  fseek (file, 0, 0);

  /* Reading elsets */
  (*pElset).Elset = ut_alloc_1d_pint ((*pElset).Qty + 1);
  for (i = 1; i <= (*pElset).Qty; i++)
  {
    (*pElset).Names = ut_alloc_2d_char ((*pElset).Qty + 1, 100);
    (*pElset).Id = ut_alloc_1d_int ((*pElset).Qty + 1);
    (*pElset).Elset[i] = ut_alloc_1d_int (EltQty[i] + 1);
    (*pElset).Elset[i][0] = EltQty[i];
  }

  /* Reaching the first elset */
  ut_file_skip (file, skip);

  /* Reading elsets */
  for (elset = 1; elset <= (*pElset).Qty; elset++)
  {
    ut_file_skip (file, 1);
    fscanf (file, "%s", (*pElset).Names[elset]);
    for (i = 1; i <= (*pElset).Elset[elset][0]; i++)
      fscanf (file, "%d", &(*pElset).Elset[elset][i]);
  }

  /* Extracting name Body and id */
  tmp = 0;
  (*pElset).Body = ut_alloc_1d_char (1000);
  for (i = 0; i < (int) strlen ((*pElset).Names[1]); i++)
    if (((*pElset).Names[1][tmp] >= 'a'
	 && (*pElset).Names[1][tmp] <= 'z')
	|| ((*pElset).Names[1][tmp] >= 'A' && (*pElset).Names[1][tmp] <= 'Z'))
    {
      (*pElset).Body[tmp] = (*pElset).Names[1][tmp];
      tmp++;
    }
    else
    {
      (*pElset).Body[++tmp] = '\0';
      break;
    }

  for (elset = 1; elset <= (*pElset).Qty; elset++)
  {
    sscanf ((*pElset).Names[elset] + strlen ((*pElset).Body),
	    "%d", &(*pElset).Id[elset]);
  }

  ut_free_1d_int (EltQty);
  ut_free_1d_char (String);
  ut_free_1d_char (trash);

  return;
}
