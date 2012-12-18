/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_tess_lcl.h"

void
neut_tess_tess (struct TESS TessA, struct TESS *pTessB)
{
  neut_tess_free (pTessB);

  neut_tess_tessGen  (TessA, pTessB);
  neut_tess_tessVer  (TessA, pTessB);
  neut_tess_tessEdge (TessA, pTessB);
  neut_tess_tessFace (TessA, pTessB);
  neut_tess_tessPoly (TessA, pTessB);
  neut_tess_tessDomain (TessA, pTessB);

  return;
}
