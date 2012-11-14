/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_nset.h"

void
neut_nset_expand (struct NSET NSet0D, struct NSET NSet1D, struct NSET NSet2D,
                  char* in, char** pout)
{
  int i, j;
  int qty;
  char** list = NULL;

  ut_free_1d_char (*pout);
  (*pout) = ut_alloc_1d_char (1);
  (*pout)[0] = '\0';

  if (! in || strlen (in) == 0 || ! strcmp (in, "none"))
    return;

  if (ut_string_finds (in, "all") != -1)
  {
    qty = 5;
    list = ut_alloc_2d_char (qty, 11);
    list[0] = strcpy (list[0], "faces");
    list[1] = strcpy (list[1], "facebodies");
    list[2] = strcpy (list[2], "edges");
    list[3] = strcpy (list[3], "edgebodies");
    list[4] = strcpy (list[4], "vertices");
  }
  else
    ut_string_separate (in, ',', &list, &qty);

  for (i = 0; i < qty; i++)
  {
    if (! strcmp (list[i], "faces"))
    {
      for (j = 1; j <= NSet2D.qty; j++)
	if (ut_string_finds (NSet2D.names[j], "body") == -1)
	{
	  (*pout) = ut_realloc_1d_char ((*pout), strlen ((*pout)) + strlen (NSet2D.names[j]) + 2);
	  if (strlen ((*pout)) > 0)
	    (*pout) = strcat ((*pout), ",");
	  (*pout) = strcat ((*pout), NSet2D.names[j]);
	}
    }
    else if (! strcmp (list[i], "facebodies"))
    {
      for (j = 1; j <= NSet2D.qty; j++)
	if (ut_string_finds (NSet2D.names[j], "body") != -1)
	{
	  (*pout) = ut_realloc_1d_char ((*pout), strlen ((*pout)) + strlen (NSet2D.names[j]) + 2);
	  if (strlen ((*pout)) > 0)
	    (*pout) = strcat ((*pout), ",");
	  (*pout) = strcat ((*pout), NSet2D.names[j]);
	}
    }
    else if (! strcmp (list[i], "edges"))
    {
      for (j = 1; j <= NSet1D.qty; j++)
	if (ut_string_finds (NSet1D.names[j], "body") == -1)
	{
	  (*pout) = ut_realloc_1d_char ((*pout), strlen ((*pout)) + strlen (NSet1D.names[j]) + 2);
	  if (strlen ((*pout)) > 0)
	    (*pout) = strcat ((*pout), ",");
	  (*pout) = strcat ((*pout), NSet1D.names[j]);
	}
    }
    else if (! strcmp (list[i], "edgebodies"))
    {
      for (j = 1; j <= NSet1D.qty; j++)
	if (ut_string_finds (NSet1D.names[j], "body") != -1)
	{
	  (*pout) = ut_realloc_1d_char ((*pout), strlen ((*pout)) + strlen (NSet1D.names[j]) + 2);
	  if (strlen ((*pout)) > 0)
	    (*pout) = strcat ((*pout), ",");
	  (*pout) = strcat ((*pout), NSet1D.names[j]);
	}
    }
    else if (! strcmp (list[i], "vertices"))
    {
      for (j = 1; j <= NSet0D.qty; j++)
      {
	(*pout) = ut_realloc_1d_char ((*pout), strlen ((*pout)) + strlen (NSet0D.names[j]) + 2);
	if (strlen ((*pout)) > 0)
	  (*pout) = strcat ((*pout), ",");
	(*pout) = strcat ((*pout), NSet0D.names[j]);
      }
    }
    else
    {
      (*pout) = ut_realloc_1d_char ((*pout), strlen ((*pout)) + strlen (list[i]) + 2);
      if (strlen ((*pout)) > 0)
	(*pout) = strcat ((*pout), ",");
      (*pout) = strcat ((*pout), list[i]);
    }
  }

  ut_free_2d_char (list, qty);

  return;
}

void
neut_nset_set_zero (struct NSET* pNSet)
{
  (*pNSet).qty = 0;
  (*pNSet).names = NULL;
  (*pNSet).nodes = NULL;
  (*pNSet).nodeqty = NULL;

  return;
}

void
neut_nset_free (struct NSET* pNSet)
{
  ut_free_2d_char ((*pNSet).names, (*pNSet).qty + 1);
  ut_free_1d_int  ((*pNSet).nodeqty);
  ut_free_2d_int  ((*pNSet).nodes, (*pNSet).qty + 1);

  return;
}
