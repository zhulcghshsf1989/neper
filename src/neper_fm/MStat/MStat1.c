/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"MStat.h"

void
MStat (struct NODES Nodes,
       struct MESH Mesh0D, struct MESH Mesh1D,
       struct MESH Mesh2D, struct MESH Mesh3D, 
       struct IN In,
       struct GEOPARA GeoPara, struct GEO* pGeo)
{
  PrintEStat (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, In, GeoPara, pGeo);

  return;
}
