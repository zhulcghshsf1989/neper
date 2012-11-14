/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"CubeDomain.h"

void
CubeVer (double *size, struct POLY *pDomain)
{
  IniVerQty (pDomain);
  IniVerFace (pDomain);
  IniVerCoo (size, pDomain);

  return;
}

void
CubeFaces (double *size, struct POLY *pDomain)
{
  IniFaceQty (pDomain);
  IniFaceEq (size, pDomain);
  IniFacePoly (pDomain);
  IniFaceVerQty (pDomain);
  IniFaceVerNb (pDomain);

  return;
}
