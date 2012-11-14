/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_scale.h"

/*
void
nevs_scale_init (struct DATA** pData, int DataQty)
{
  int i;

  for (i = 0; i < DataQty; i++)
  {
    if ((*pData)[i].qty[1] == 1)
    {
      if (strcmp ((*pData)[i].minexpr, "min") == 0)
	(*pData)[i].min = ut_array_2d_col_min ((*pData)[i].val + 1, 0, (*pData)[i].qty[0]);
      else
	sscanf ((*pData)[i].minexpr, "%lf", &((*pData)[i].min));

      if (strcmp ((*pData)[i].maxexpr, "max") == 0)
	(*pData)[i].max = ut_array_2d_col_max ((*pData)[i].val + 1, 0, (*pData)[i].qty[0]);
      else
	sscanf ((*pData)[i].maxexpr, "%lf", &((*pData)[i].max));
    }
  }

  return;
}
*/

/*
void
nevs_scale (char** argv, int* pi, struct SCALE* pScale)
{
  if (DataQty != 1)
  {
    printf ("nevs_scale: DataQty = %d != 1\n", DataQty);
    ut_error_reportbug ();
  }

  // load node coordinates ---------------------------------------------
  if (strcmp (argv[(*pi)], "-scalecolourscheme") == 0
   || strcmp (argv[(*pi)], "-scalecolorscheme" ) == 0)
    sscanf (argv[++(*pi)], "%s", (*pData)[0].colourscheme);
  else if (strcmp (argv[(*pi)], "-scalemin") == 0)
    sscanf (argv[++(*pi)], "%s", (*pData)[0].minexpr);
  else if (strcmp (argv[(*pi)], "-scalemax") == 0)
    sscanf (argv[++(*pi)], "%s", (*pData)[0].maxexpr);
  else if (strcmp (argv[(*pi)], "-scaleheight") == 0)
    sscanf (argv[++(*pi)], "%lf", &((*pData)[0].height));
  else if (strcmp (argv[(*pi)], "-scaleticks") == 0)
  {
    (*pi)++;
    (*pData)[0].ticks = ut_alloc_1d_char (strlen (argv[(*pi)]) + 1);
    sscanf (argv[(*pi)], "%s", (*pData)[0].ticks);
  }
  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}
*/
