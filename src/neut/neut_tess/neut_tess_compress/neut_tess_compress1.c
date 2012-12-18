/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_compress_lcl.h"

/* This function deletes the dummy entities (ver, edge, face) in a Tess */
void
neut_tess_compress (struct TESS* pTess)
{
  neut_tess_compress_faces (pTess);
  neut_tess_compress_edges (pTess);
  neut_tess_compress_vers (pTess);

  return;
}
