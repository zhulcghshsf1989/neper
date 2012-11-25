/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "SearchNSets.h"

void
SearchNSets_2d (struct GEO Geo, struct MESH Mesh2D, struct NSET* pNSet2D)
{
  int i;

  (*pNSet2D).qty = Geo.DomFaceQty;
  (*pNSet2D).names = ut_alloc_1d_pchar ((*pNSet2D).qty + 1);

  // setting up face nset labels
  if (! strcmp (Geo.DomType, "cube"))
  {
    for (i = 1; i <= Geo.DomFaceQty; i++)
      (*pNSet2D).names[i] = ut_alloc_1d_char (3);

    sprintf ((*pNSet2D).names[1], "x0");
    sprintf ((*pNSet2D).names[2], "x1");
    sprintf ((*pNSet2D).names[3], "y0");
    sprintf ((*pNSet2D).names[4], "y1");
    sprintf ((*pNSet2D).names[5], "z0");
    sprintf ((*pNSet2D).names[6], "z1");
  }
  else if (! strcmp (Geo.DomType, "cylinder"))
  {
    (*pNSet2D).names[1] = ut_alloc_1d_char (4);
    (*pNSet2D).names[2] = ut_alloc_1d_char (4);

    sprintf ((*pNSet2D).names[1], "z0");
    sprintf ((*pNSet2D).names[2], "z1");

    for (i = 3; i <= Geo.DomFaceQty; i++)
    {
      (*pNSet2D).names[i] = ut_alloc_1d_char (10);
      sprintf ((*pNSet2D).names[i], "f%d", i - 2);
    }
  }
  else
    for (i = 1; i <= Geo.DomFaceQty; i++)
    {
      (*pNSet2D).names[i] = ut_alloc_1d_char (10);
      sprintf ((*pNSet2D).names[i], "f%d", i);
    }

  // Computing nsets
  (*pNSet2D).nodeqty = ut_alloc_1d_int  (Geo.DomFaceQty + 1);
  (*pNSet2D).nodes   = ut_alloc_1d_pint (Geo.DomFaceQty + 1);

  for (i = 1; i <= Geo.DomFaceQty; i++)
    neut_mesh_elsets_nodes (Mesh2D, Geo.DomTessFaceNb[i] + 1, Geo.DomTessFaceQty[i],
			    &((*pNSet2D).nodes[i]), &((*pNSet2D).nodeqty[i]));

  return;
}


void
SearchNSets_1d (struct GEO Geo, struct MESH Mesh1D, struct NSET NSet2D, struct NSET* pNSet1D)
{
  int i;

  // Edges
  (*pNSet1D).qty = Geo.DomEdgeQty;
  (*pNSet1D).names = ut_alloc_1d_pchar (Geo.DomEdgeQty + 1);
  (*pNSet1D).nodeqty = ut_alloc_1d_int (Geo.DomEdgeQty + 1);
  (*pNSet1D).nodes   = ut_alloc_1d_pint (Geo.DomEdgeQty + 1);

  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    int* face = Geo.DomEdgeFaceNb[i];
    int length = strlen (NSet2D.names[face[0]]) + strlen (NSet2D.names[face[1]]) + 1;
    (*pNSet1D).names[i] = ut_alloc_1d_char (length);
    sprintf ((*pNSet1D).names[i], "%s%s", NSet2D.names[face[0]], NSet2D.names[face[1]]);
    
    neut_mesh_elsets_nodes (Mesh1D, Geo.DomTessEdgeNb[i] + 1, Geo.DomTessEdgeQty[i],
			    &((*pNSet1D).nodes[i]), &((*pNSet1D).nodeqty[i]));
  }

  return;
}

void
SearchNSets_0d (struct GEO Geo, struct MESH Mesh0D, struct NSET NSet2D, struct NSET* pNSet0D)
{
  int i, j;

  // Vertices
  (*pNSet0D).qty = Geo.DomVerQty;
  (*pNSet0D).names = ut_alloc_1d_pchar (Geo.DomVerQty + 1);
  (*pNSet0D).nodeqty = ut_alloc_1d_int (Geo.DomVerQty + 1);
  (*pNSet0D).nodes   = ut_alloc_1d_pint (Geo.DomVerQty + 1);

  for (i = 1; i <= Geo.DomVerQty; i++)
  {
    int qty;
    int* face = NULL;
    neut_geo_domver_domface (Geo, i, &face, &qty);
    ut_array_1d_int_sort (face, qty);

    int length = strlen (NSet2D.names[face[0]]) + 1;
    (*pNSet0D).names[i] = ut_alloc_1d_char (length);
    strcpy ((*pNSet0D).names[i], NSet2D.names[face[0]]);

    for (j = 1; j < qty; j++)
    {
      length += strlen (NSet2D.names[face[j]]);
      (*pNSet0D).names[i] = ut_realloc_1d_char ((*pNSet0D).names[i], length);
      (*pNSet0D).names[i] = strcat ((*pNSet0D).names[i], NSet2D.names[face[j]]);
    }

    ut_free_1d_int (face);
    
    neut_mesh_elset_nodes (Mesh0D, Geo.DomTessVerNb[i],
			   &((*pNSet0D).nodes[i]), &((*pNSet0D).nodeqty[i]));
  }

  return;
}

  
void
SearchNSets_2d_body (struct GEO Geo, struct NSET NSet1D, struct NSET* pNSet2D)
{
  int i, j, k, edge;
  int ref, prevqty = (*pNSet2D).qty;

  (*pNSet2D).qty *= 2;
  (*pNSet2D).names = ut_realloc_1d_pchar ((*pNSet2D).names, (*pNSet2D).qty + 1);
  (*pNSet2D).nodeqty = ut_realloc_1d_int ((*pNSet2D).nodeqty, (*pNSet2D).qty + 1);
  (*pNSet2D).nodes = ut_realloc_1d_pint  ((*pNSet2D).nodes , (*pNSet2D).qty + 1);

  for (i = prevqty + 1; i <= (*pNSet2D).qty; i++)
  {
    ref = i - prevqty;

    (*pNSet2D).names[i] = ut_alloc_1d_char (strlen ((*pNSet2D).names[ref]) + 5);
    sprintf ((*pNSet2D).names[i], "%sbody", (*pNSet2D).names[ref]);

    (*pNSet2D).nodeqty[i] = (*pNSet2D).nodeqty[ref];
    (*pNSet2D).nodes[i] = ut_alloc_1d_int ((*pNSet2D).nodeqty[i]);
    ut_array_1d_int_memcpy ((*pNSet2D).nodes[i], (*pNSet2D).nodeqty[i], (*pNSet2D).nodes[ref]);

    for (j = 1; j <= Geo.DomFaceVerQty[ref]; j++)
    {
      edge = Geo.DomFaceEdgeNb[ref][j];

      for (k = 0; k < NSet1D.nodeqty[edge]; k++)
	(*pNSet2D).nodeqty[i] -=
	  ut_array_1d_int_deletencompress ((*pNSet2D).nodes[i], (*pNSet2D).nodeqty[i],
	                                 NSet1D.nodes[edge][k], 1);
    }

    (*pNSet2D).nodes[i] = ut_realloc_1d_int ((*pNSet2D).nodes[i], (*pNSet2D).nodeqty[i]);
  }

  return;
}

void
SearchNSets_1d_body (struct GEO Geo, struct NSET NSet0D, struct NSET* pNSet1D)
{
  int i, j, k, ver;
  int ref, prevqty = (*pNSet1D).qty;

  (*pNSet1D).qty *= 2;
  (*pNSet1D).names = ut_realloc_1d_pchar ((*pNSet1D).names, (*pNSet1D).qty + 1);
  (*pNSet1D).nodeqty = ut_realloc_1d_int ((*pNSet1D).nodeqty, (*pNSet1D).qty + 1);
  (*pNSet1D).nodes = ut_realloc_1d_pint  ((*pNSet1D).nodes , (*pNSet1D).qty + 1);

  for (i = prevqty + 1; i <= (*pNSet1D).qty; i++)
  {
    ref = i - prevqty;

    (*pNSet1D).names[i] = ut_alloc_1d_char (strlen ((*pNSet1D).names[ref]) + 5);
    sprintf ((*pNSet1D).names[i], "%sbody", (*pNSet1D).names[ref]);

    (*pNSet1D).nodeqty[i] = (*pNSet1D).nodeqty[ref];
    (*pNSet1D).nodes[i] = ut_alloc_1d_int ((*pNSet1D).nodeqty[i]);
    ut_array_1d_int_memcpy ((*pNSet1D).nodes[i], (*pNSet1D).nodeqty[i], (*pNSet1D).nodes[ref]);

    for (j = 0; j < 2; j++)
    {
      ver = Geo.DomEdgeVerNb[ref][j];

      for (k = 0; k < NSet0D.nodeqty[ver]; k++)
	(*pNSet1D).nodeqty[i] -=
	  ut_array_1d_int_deletencompress ((*pNSet1D).nodes[i], (*pNSet1D).nodeqty[i],
	                                 NSet0D.nodes[ver][k], 1);
    }

    (*pNSet1D).nodes[i] = ut_realloc_1d_int ((*pNSet1D).nodes[i], (*pNSet1D).nodeqty[i]);
  }

  return;
}
