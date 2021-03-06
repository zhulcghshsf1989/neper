/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Res_mm.h"

/* NSet stuff to be cleaned, but up to now necessary for printing of MMesh */
void
WriteNSets (int nset, char *nsetlist, int **FoDNodes, char *format,
	    FILE * file)
{
  int i;
  char NsetNames[100][100];
  int **FoDBNodes;
  int **EoDNodes;
  int **EoDBNodes;
  int **CoDNodes;
  int nodeqty = 0;

  if (strcmp (format, "fev") == 0)
    fprintf (file, "0\n");

  if (nset <= 0)
    return;

  for (i = 1; i <= 6; i++)
    nodeqty += FoDNodes[i][0];

  if (nset >= 2)
  {
    FoD2EoDNodes (FoDNodes, &EoDNodes);
    EoD2CoDNodes (EoDNodes, &CoDNodes);
    nodeqty += 8;
    for (i = 1; i <= 12; i++)
      nodeqty += EoDNodes[i][0];
  }
  if (nset >= 3)
  {
    FoDBNodes = ut_alloc_1d_pint (7);
    FoD2FoDBNodes (FoDNodes, EoDNodes, &FoDBNodes);
    EoDBNodes = ut_alloc_1d_pint (13);
    EoD2EoDBNodes (EoDNodes, CoDNodes, &EoDBNodes);
    for (i = 1; i <= 6; i++)
      nodeqty += FoDBNodes[i][0];
    for (i = 1; i <= 12; i++)
      nodeqty += EoDBNodes[i][0];
  }

  if (strcmp (format, "fev") == 0)
    fprintf (file, "%d\n", nodeqty);

  /* Printing */
  if (nset >= 1)
  {
    /* printing faces */
    sprintf (NsetNames[1], "x0");
    sprintf (NsetNames[2], "x1");
    sprintf (NsetNames[3], "y0");
    sprintf (NsetNames[4], "y1");
    sprintf (NsetNames[5], "z0");
    sprintf (NsetNames[6], "z1");

    for (i = 1; i <= 6; i++)
      if (nset < 4
	  || (nset == 4
	      && ut_string_inlist (nsetlist, ',', NsetNames[i])) == 1)
	WriteNSet (NsetNames[i], FoDNodes[i], format, file);
  }

  if (nset >= 2)
  {
    /* printing vertices */
    sprintf (NsetNames[1], "x0y0z0");
    sprintf (NsetNames[2], "x1y0z0");
    sprintf (NsetNames[3], "x0y1z0");
    sprintf (NsetNames[4], "x1y1z0");
    sprintf (NsetNames[5], "x0y0z1");
    sprintf (NsetNames[6], "x1y0z1");
    sprintf (NsetNames[7], "x0y1z1");
    sprintf (NsetNames[8], "x1y1z1");

    for (i = 1; i <= 8; i++)
      if (nset < 4
	  || (nset == 4
	      && ut_string_inlist (nsetlist, ',', NsetNames[i])) == 1)
	WriteNSet (NsetNames[i], CoDNodes[i], format, file);

    /* printing edges */
    sprintf (NsetNames[1], "y0z0");
    sprintf (NsetNames[2], "y1z0");
    sprintf (NsetNames[3], "y0z1");
    sprintf (NsetNames[4], "y1z1");
    sprintf (NsetNames[5], "x0z0");
    sprintf (NsetNames[6], "x0z1");
    sprintf (NsetNames[7], "x1z0");
    sprintf (NsetNames[8], "x1z1");
    sprintf (NsetNames[9], "x0y0");
    sprintf (NsetNames[10], "x1y0");
    sprintf (NsetNames[11], "x0y1");
    sprintf (NsetNames[12], "x1y1");

    for (i = 1; i <= 12; i++)
      if (nset < 4
	  || (nset == 4
	      && ut_string_inlist (nsetlist, ',', NsetNames[i])) == 1)
	WriteNSet (NsetNames[i], EoDNodes[i], format, file);
  }

  if (nset >= 3)
  {
    /* printing faces body */
    sprintf (NsetNames[1], "x0body");
    sprintf (NsetNames[2], "x1body");
    sprintf (NsetNames[3], "y0body");
    sprintf (NsetNames[4], "y1body");
    sprintf (NsetNames[5], "z0body");
    sprintf (NsetNames[6], "z1body");

    for (i = 1; i <= 6; i++)
      if (nset < 4
	  || (nset == 4
	      && ut_string_inlist (nsetlist, ',', NsetNames[i])) == 1)
	WriteNSet (NsetNames[i], FoDBNodes[i], format, file);

    /* printing edges body */
    sprintf (NsetNames[1], "y0z0body");
    sprintf (NsetNames[2], "y1z0body");
    sprintf (NsetNames[3], "y0z1body");
    sprintf (NsetNames[4], "y1z1body");
    sprintf (NsetNames[5], "x0z0body");
    sprintf (NsetNames[6], "x0z1body");
    sprintf (NsetNames[7], "x1z0body");
    sprintf (NsetNames[8], "x1z1body");
    sprintf (NsetNames[9], "x0y0body");
    sprintf (NsetNames[10], "x1y0body");
    sprintf (NsetNames[11], "x0y1body");
    sprintf (NsetNames[12], "x1y1body");

    for (i = 1; i <= 12; i++)
      if (nset < 4
	  || (nset == 4
	      && ut_string_inlist (nsetlist, ',', NsetNames[i])) == 1)
	WriteNSet (NsetNames[i], EoDBNodes[i], format, file);
  }

  if (strcmp (format, "fev") == 0)
    fprintf (file, "0\n");

  /* 
     ZZZ
     Free nsets
   */

  return;
}
