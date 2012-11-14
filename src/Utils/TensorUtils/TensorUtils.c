/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "TensorUtils.h"

void
Leq2Std (double *Leq, double *Std)
{
  Std[1] = -Leq[1] / sqrt (2.) - Leq[2] / sqrt (6.);
  Std[2] = Leq[1] / sqrt (2.) - Leq[2] / sqrt (6.);
  Std[3] = Leq[3] / sqrt (2.);
  Std[4] = Leq[4] / sqrt (2.);
  Std[5] = Leq[5] / sqrt (2.);

  return;
}

void
Std2Leq (double *Std, double *Leq)
{
  Leq[1] = (Std[2] - Std[1]) / sqrt (2.);
  Leq[2] = -sqrt (1.5) * (Std[2] + Std[1]);
  Leq[3] = sqrt (2.) * Std[3];
  Leq[4] = sqrt (2.) * Std[4];
  Leq[5] = sqrt (2.) * Std[5];

  return;
}
