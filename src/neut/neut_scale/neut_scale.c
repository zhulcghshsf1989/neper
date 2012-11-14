/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_scale.h"

void
neut_scale_set_default (struct SCALE* pScale)
{
  (*pScale).colourscheme = ut_alloc_1d_char (100);
  sprintf ((*pScale).colourscheme, "blue,cyan,yellow,red");

  (*pScale).minexpr = ut_alloc_1d_char (100);
  sprintf ((*pScale).minexpr, "min");
  (*pScale).maxexpr = ut_alloc_1d_char (100);
  sprintf ((*pScale).maxexpr, "max");

  (*pScale).min = 0;
  (*pScale).max = 0;
  (*pScale).val = NULL;

  (*pScale).height = 1;

  return;
}

void
neut_scale_free (struct SCALE* pScale)
{
  ut_free_1d_char ((*pScale).colourscheme);

  ut_free_1d_char ((*pScale).minexpr);
  ut_free_1d_char ((*pScale).maxexpr);

  ut_free_1d_char ((*pScale).val);
  
  return;
}
