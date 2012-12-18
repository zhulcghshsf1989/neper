/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_nsets.h"

void
nem_nsets_1d_names (struct TESS Tess, int id, struct NSET NSet2D,
                    struct NSET* pNSet1D)
{
  int* face = Tess.DomEdgeFaceNb[id];
  int length = strlen (NSet2D.names[face[0]]) + strlen (NSet2D.names[face[1]]) + 1;
  (*pNSet1D).names[id] = ut_alloc_1d_char (length);
  if (strcmp (NSet2D.names[face[0]], NSet2D.names[face[1]]) < 0)
    sprintf ((*pNSet1D).names[id], "%s%s", NSet2D.names[face[0]], NSet2D.names[face[1]]);
  else
    sprintf ((*pNSet1D).names[id], "%s%s", NSet2D.names[face[1]], NSet2D.names[face[0]]);

  return;
}

void
nem_nsets_0d_names (struct TESS Tess, int id, struct NSET NSet2D,
                    struct NSET* pNSet0D)
{
  int i;
  char** fnames  = NULL;
  int* domface = NULL;
  int domfaceqty;

  neut_tess_domver_domface (Tess, id, &domface, &domfaceqty);

  fnames = ut_alloc_1d_pchar (domfaceqty);
  for (i = 0; i < domfaceqty; i++) 
  {
    fnames[i] = ut_alloc_1d_char (strlen (NSet2D.names[domface[i]]) + 1);
    strcpy (fnames[i], NSet2D.names[domface[i]]);
  }
  (*pNSet0D).names[id] = ut_string_array_paste_cmp (fnames, domfaceqty);

  ut_free_2d_char (fnames, domfaceqty);
  ut_free_1d_int (domface);

  return;
}
