/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void WriteNSets (int, char *, int **, char *, FILE *);

extern void FoD2EoDNodes (int **, int ***);
extern void EoD2CoDNodes (int **, int ***);
extern void FoD2FoDBNodes (int **, int **, int ***);
extern void EoD2EoDBNodes (int **, int **, int ***);
/* extern int* Search2FacesCommonNodes (struct MESH, int, int); */
extern int Search3FacesCommonNode (int **, int, int, int);
extern int *SearchElsetNodes (struct MESH, int);

extern void WriteNSet (char *, int *, char *, FILE *);

extern void WriteBCFaceBody (int, int, struct NODES, int **, int, int,
			     FILE *);
extern void WriteBCEdgeBody (int, int, struct NODES, int **, int, FILE *);

extern void WriteBC (int, struct NODES, int **, char *);


extern void MeshElsetNodes (struct MESH, int, int **, int *);
