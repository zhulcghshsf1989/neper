/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_DEBUG0_H
#define NEUT_DEBUG0_H

#include"../neut_structs.h"

extern void neut_debug_nodes (FILE*, struct NODES);
extern void neut_debug_mesh (FILE*, struct MESH);
extern void neut_debug_nset (FILE*, struct NSET);
extern void neut_debug_tess (FILE*, struct TESS);
extern void neut_debug_germset (FILE*, struct GERMSET);
extern void neut_debug_tessdata (FILE*, struct TESSDATA);
extern void neut_debug_poly (FILE*, struct POLY);
extern void neut_debug_polymod (FILE*, struct POLYMOD);

extern void neut_debug_nodes_name (char*, struct NODES);
extern void neut_debug_mesh_name (char*, struct MESH);
extern void neut_debug_nset_name (char*, struct NSET);
extern void neut_debug_tess_name (char*, struct TESS);
extern void neut_debug_germset_name (char*, struct GERMSET);
extern void neut_debug_tessdata_name (char*, struct TESSDATA);
extern void neut_debug_poly_name (char*, struct POLY);
extern void neut_debug_polymod_name (char*, struct POLYMOD);

#endif /* NEUT_DEBUG0_H */
