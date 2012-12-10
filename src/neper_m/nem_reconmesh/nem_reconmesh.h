/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structIn_m.h"
#include "neut.h"
#include "nem_reconmesh0.h"

extern void nem_reconmesh_2d (struct NODES, struct MESH*, struct MESH*, struct GEO*);
extern void nem_reconmesh_1d (struct NODES, struct MESH*, struct MESH*, struct GEO*);
extern void nem_reconmesh_0d (struct NODES, struct MESH*, struct MESH*, struct GEO*);

extern void nem_reconmesh_finalizegeo (struct GEO*, struct NODES,
    struct MESH, struct MESH, struct MESH, struct MESH);
