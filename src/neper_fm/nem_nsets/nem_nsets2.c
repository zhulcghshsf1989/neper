/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_nsets.h"

void
nem_nsets_2d_geo (struct GEO Geo, struct MESH Mesh2D, struct NSET* pNSet2D)
{
  int i;

  (*pNSet2D).qty = Geo.DomFaceQty;
  (*pNSet2D).names = ut_alloc_1d_pchar ((*pNSet2D).qty + 1);

  // setting up face nset labels
  for (i = 1; i <= (*pNSet2D).qty; i++)
  {
    (*pNSet2D).names[i] = ut_alloc_1d_char (strlen (Geo.DomFaceLabel[i]) + 1);
    strcpy ((*pNSet2D).names[i], Geo.DomFaceLabel[i]);
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
nem_nsets_1d_geo (struct GEO Geo, struct MESH Mesh1D, struct NSET NSet2D, struct NSET* pNSet1D)
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
nem_nsets_0d_geo (struct GEO Geo, struct MESH Mesh0D, struct NSET NSet2D, struct NSET* pNSet0D)
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
nem_nsets_2dbody_geo (struct GEO Geo, struct NSET NSet1D, struct NSET* pNSet2D)
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

    if ((*pNSet2D).nodeqty[i] > 0)
      (*pNSet2D).nodes[i] = ut_realloc_1d_int ((*pNSet2D).nodes[i], (*pNSet2D).nodeqty[i]);
  }

  return;
}

void
nem_nsets_1dbody_geo (struct GEO Geo, struct NSET NSet0D, struct NSET* pNSet1D)
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

    if ((*pNSet1D).nodeqty[i] > 0)
      (*pNSet1D).nodes[i] = ut_realloc_1d_int ((*pNSet1D).nodes[i], (*pNSet1D).nodeqty[i]);
  }

  return;
}

void
nem_nsets_1d_geo_hex (struct GEO Geo, struct NSET NSet2D, struct NSET* pNSet1D)
{
  int i, f1, f2;

  (*pNSet1D).qty     = Geo.DomEdgeQty;
  (*pNSet1D).names   = ut_alloc_1d_pchar ((*pNSet1D).qty + 1);
  (*pNSet1D).nodeqty = ut_alloc_1d_int   ((*pNSet1D).qty + 1);
  (*pNSet1D).nodes   = ut_alloc_1d_pint  ((*pNSet1D).qty + 1);

  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    f1 = Geo.DomEdgeFaceNb[i][0];
    f2 = Geo.DomEdgeFaceNb[i][1];

    neut_nsets_inter (NSet2D, f1, f2, &((*pNSet1D).names[i]),
		      &((*pNSet1D).nodes[i]), &((*pNSet1D).nodeqty[i]));
  }

  return;
}

void
nem_nsets_0d_geo_hex (struct GEO Geo, struct NSET NSet2D, struct NSET NSet1D,
                      struct NSET* pNSet0D)
{
  int i, j, e1, e2;
  char** fnames  = NULL;
  int*   domface = NULL;
  int domfaceqty;

  (*pNSet0D).qty     = Geo.DomVerQty;
  (*pNSet0D).names   = ut_alloc_1d_pchar ((*pNSet0D).qty + 1);
  (*pNSet0D).nodeqty = ut_alloc_1d_int   ((*pNSet0D).qty + 1);
  (*pNSet0D).nodes   = ut_alloc_1d_pint  ((*pNSet0D).qty + 1);

  for (i = 1; i <= Geo.DomVerQty; i++)
  {
    e1 = Geo.DomVerEdgeNb[i][0];
    e2 = Geo.DomVerEdgeNb[i][1];

    neut_nsets_inter (NSet1D, e1, e2, NULL,
		      &((*pNSet0D).nodes[i]), &((*pNSet0D).nodeqty[i]));
    neut_geo_domver_domface (Geo, i, &domface, &domfaceqty);

    fnames = ut_alloc_1d_pchar (domfaceqty);
    for (j = 0; j < domfaceqty; j++) 
    {
      fnames[j] = ut_alloc_1d_char (strlen (NSet2D.names[domface[j]]) + 1);
      strcpy (fnames[j], NSet2D.names[domface[j]]);
    }
    (*pNSet0D).names[i] = ut_string_array_paste_cmp (fnames, domfaceqty);

    ut_free_1d_int  (domface);
    domface = NULL;
    ut_free_2d_char (fnames, domfaceqty);
    fnames = NULL;
  }

  return;
}
