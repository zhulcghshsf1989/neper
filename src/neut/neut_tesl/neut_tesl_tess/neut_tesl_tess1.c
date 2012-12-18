/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesl_tess.h"

void
neut_tesl_tess (struct TESL Tess, struct TESS *pTess)
{
  neut_tess_free (pTess);
  
  neut_tesl_tess_general  (Tess, pTess);
  neut_tesl_tess_ver      (Tess, pTess);
  neut_tesl_tess_edge     (Tess, pTess);
  neut_tesl_tess_face     (Tess, pTess);
  neut_tesl_tess_poly     (Tess, pTess);
  
  return;
}
