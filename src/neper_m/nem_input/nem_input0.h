/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_input (struct IN*, struct TESSPARA *, int, char **,
    int, char **);
extern void nem_in_set_zero (struct IN*);
extern void nem_in_free (struct IN);
extern void nem_tesspara_set_zero (struct TESSPARA*);
extern void nem_tesspara_free (struct TESSPARA);
