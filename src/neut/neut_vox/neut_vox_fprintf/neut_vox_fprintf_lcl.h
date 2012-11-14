/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"../neut_vox_fprintf.h"

extern void neut_vox_fprintf_head (FILE*, char*, struct VOX);
extern void neut_vox_fprintf_data (FILE*, char*, struct VOX);
extern void neut_vox_fprintf_foot (FILE*);
