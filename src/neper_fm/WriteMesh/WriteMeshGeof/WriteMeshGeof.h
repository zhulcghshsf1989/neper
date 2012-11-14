/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../../structIn_fm.h"
#include"structAll.h"
#include"AllUtils.h"
#include"ut.h"
#include"neut.h"
#include "WriteMeshGeof0.h"

extern void neut_mesh_fprintf_geof_head (FILE* file);
extern void neut_mesh_fprintf_geof_nodes (FILE*, struct NODES);
extern void neut_mesh_fprintf_geof_elts (FILE*, struct MESH);
extern void neut_mesh_fprintf_geof_nsets (FILE*, struct NSET, struct NSET, struct NSET, char*);
extern void neut_mesh_fprintf_geof_sethead (FILE*);
/* extern void neut_mesh_fprintf_geof_lisets (struct GEO, int **, struct NODES, struct MESH, FILE*); */
extern void neut_mesh_fprintf_geof_elsets (FILE*, struct MESH);
extern void neut_mesh_fprintf_geof_foot (FILE *);
extern void neut_mesh_fprintf_geof_nset (FILE*, char*, int, int*);
extern void neut_mesh_fprintf_geof_part (FILE*, struct PART);
extern void neut_mesh_fprintf_geof_part_nset (FILE*, struct PART);
extern void neut_mesh_fprintf_geof_part_elset (FILE*, struct PART);
