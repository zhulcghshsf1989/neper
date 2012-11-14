/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include <stdio.h>
#include <stdlib.h>
#include "../neut_mesh.h"
#include "structAll.h"
#include "AllUtils.h"
#include "../../../neper_t/Miscellaneous/Miscellaneous0.h"
#include "neut_mesh_renumber0.h"

extern void RandDistrib (struct GERMSET *, double *, int);

extern void neut_nodes_renumber (struct NODES *, int *);
extern void neut_nodes_switch (struct NODES*, int, int);
extern void neut_mesh_switch (struct MESH*, int, int);
