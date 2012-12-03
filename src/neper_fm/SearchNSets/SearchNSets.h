/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

#include"SearchNSets0.h"
#include"SearchNSets_mm/SearchNSets_mm0.h"

extern void SearchNSets_2d (struct GEO, struct MESH, struct NSET*);
extern void SearchNSets_1d (struct GEO, struct MESH, struct NSET, struct NSET*);
extern void SearchNSets_0d (struct GEO, struct MESH, struct NSET, struct NSET*);
extern void SearchNSets_2d_body (struct GEO, struct NSET, struct NSET*);
extern void SearchNSets_1d_body (struct GEO, struct NSET, struct NSET*);
