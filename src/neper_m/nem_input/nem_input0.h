/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_input (struct IN*, struct MESHPARA *, int, char **,
    int, char **);
extern void nem_in_set_zero (struct IN*);
extern void nem_in_free (struct IN);
extern void nem_meshpara_set_zero (struct MESHPARA*);
extern void nem_meshpara_free (struct MESHPARA);
