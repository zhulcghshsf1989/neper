/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"IntePara.h"

/* IniInternalPara initializes the internal parameters (i.e. those
 * of the IntePara structure) described in the mp.h file.
 */
void
InternalPara (struct INTEPARA *pIntePara)
{
  IniMaxToSort (pIntePara);
  // IniMaxPolyVerQty (pIntePara);
  // IniMaxPolyFaceQty (pIntePara);

  return;
}
