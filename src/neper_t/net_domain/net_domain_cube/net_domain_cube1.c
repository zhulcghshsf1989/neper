/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_cube.h"

/* Domain: 
 * 
 */
void
net_domain_cube (double *size, struct POLY *pDomain)
{
  /* initialization of the vertices */
  CubeVer (size, pDomain);
  /* initialization of the faces */
  CubeFaces (size, pDomain);

  return;
}
