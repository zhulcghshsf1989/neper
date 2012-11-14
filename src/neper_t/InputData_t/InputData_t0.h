/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void InputData_t (struct IN *, struct GERMSET *,
			 int, char **, int, char **);

extern void net_in_set_zero (struct IN*);
extern void net_in_free (struct IN);

extern int SetOptions_t_inputdata (struct GERMSET*, int, char**, int*, char*, int*);
