/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"../neut_vox_fscanf.h"

extern void neut_vox_fscanf_version (FILE*, char*);
extern void neut_vox_fscanf_head (struct VOX*, FILE*);
extern void neut_vox_fscanf_foot (FILE *);
extern void neut_vox_fscanf_data (struct VOX* pVox, FILE * file);
