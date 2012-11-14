/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structAll.h"
#include "../structIn_fm.h"
#include "AllUtils.h"
#include "neut.h"
#include "ReconMesh0.h"

extern void ReconMesh_2d (struct NODES, struct MESH*, struct MESH*, struct GEO*);
extern void ReconMesh_1d (struct NODES, struct MESH*, struct MESH*, struct GEO*);
extern void ReconMesh_0d (struct NODES, struct MESH*, struct MESH*, struct GEO*);

extern void ReconMesh_finalizegeo (struct GEO*, struct NODES,
    struct MESH, struct MESH, struct MESH, struct MESH);
