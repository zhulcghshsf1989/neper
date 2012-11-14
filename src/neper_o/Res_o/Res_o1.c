/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Res_o.h"

extern void
Res_o (struct IN In, struct ORI Ori)
{
  FILE *file;

  if (strlen (In.format) > 0)
  {
    file = ut_file_open (In.ori, "w");
    WriteOri (In, Ori, In.format, file);
    ut_file_close (file, In.ori, "w");
  }

  return;
}
