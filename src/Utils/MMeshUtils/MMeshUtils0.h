/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void BuildMMesh2D (int, int, int, struct NODES *, struct MESH *);
extern void BuildMMesh3D (int *, int, struct NODES *,
			  struct MESH *, int ***);
extern void BuildMMesh3DCentre (int, int, int, int *, double ***);
extern int EltCoo2Id (int, int, int, int, int);
extern int PointElt (double *, int);

extern void WriteMMesh2D (struct NODES, struct MESH, char *);
extern void ReadElsets (struct ELSET *, int, char *);

extern void SearchFoDNodes_cube (struct NODES, int ***);
