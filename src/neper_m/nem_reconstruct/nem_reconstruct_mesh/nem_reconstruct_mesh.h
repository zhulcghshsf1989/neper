/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neut_m.h"
#include "nem_reconstruct_mesh0.h"

extern void nem_reconstruct_mesh_2d (struct NODES, struct MESH*, struct MESH*, struct TESS*);
extern void nem_reconstruct_mesh_1d (struct NODES, struct MESH*, struct MESH*, struct TESS*);
extern void nem_reconstruct_mesh_0d (struct NODES, struct MESH*, struct MESH*, struct TESS*);

extern void nem_reconstruct_mesh_finalizetess (struct TESS*, struct NODES,
    struct MESH, struct MESH, struct MESH, struct MESH);
