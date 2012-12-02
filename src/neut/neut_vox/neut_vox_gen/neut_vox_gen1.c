/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_vox_gen_lcl.h"

void
neut_vox_volume (struct VOX Vox, double* pvol)
{
  (*pvol) = Vox.vsize[0] * Vox.size[0]
          * Vox.vsize[1] * Vox.size[1]
	  * Vox.vsize[2] * Vox.size[2];

  return;
}
