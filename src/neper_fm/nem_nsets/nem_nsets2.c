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

void
nem_nsets_1d_hex (struct NSET NSet2D, struct NSET* pNSet1D)
{
  int i;
  char** edgelabel = ut_alloc_2d_char (13, 7);
  int**   edgeface = ut_alloc_2d_int (13, 2);

  strcpy (edgelabel[1] , "x0y0"); edgeface[1][0]  = 1; edgeface[1][1]  = 3;
  strcpy (edgelabel[2] , "x1y0"); edgeface[2][0]  = 2; edgeface[2][1]  = 3;
  strcpy (edgelabel[3] , "x0y1"); edgeface[3][0]  = 1; edgeface[3][1]  = 4;
  strcpy (edgelabel[4] , "x1y1"); edgeface[4][0]  = 2; edgeface[4][1]  = 4;
  strcpy (edgelabel[5] , "y0z0"); edgeface[5][0]  = 3; edgeface[5][1]  = 5;
  strcpy (edgelabel[6] , "y1z0"); edgeface[6][0]  = 4; edgeface[6][1]  = 5;
  strcpy (edgelabel[7] , "y0z1"); edgeface[7][0]  = 3; edgeface[7][1]  = 6;
  strcpy (edgelabel[8] , "y1z1"); edgeface[8][0]  = 4; edgeface[8][1]  = 6;
  strcpy (edgelabel[9] , "x0z0"); edgeface[9][0]  = 1; edgeface[9][1]  = 5;
  strcpy (edgelabel[10], "x1z0"); edgeface[10][0] = 2; edgeface[10][1] = 5;
  strcpy (edgelabel[11], "x0z1"); edgeface[11][0] = 1; edgeface[11][1] = 6;
  strcpy (edgelabel[12], "x1z1"); edgeface[12][0] = 2; edgeface[12][1] = 6;

  (*pNSet1D).qty = 12;
  (*pNSet1D).names = ut_alloc_1d_pchar (13);
  (*pNSet1D).nodeqty = ut_alloc_1d_int (13);
  (*pNSet1D).nodes = ut_alloc_1d_pint (13);

  for (i = 1; i <= 12; i++)
  {
    neut_nsets_inter (NSet2D, edgeface[i][0], edgeface[i][1],
	              &((*pNSet1D).names[i]),
		      &((*pNSet1D).nodes[i]), &((*pNSet1D).nodeqty[i]));
  }

  ut_free_2d_char (edgelabel, 13);
  ut_free_2d_int  (edgeface,  13);

  return;
}

void
nem_nsets_0d_hex (struct NSET NSet1D, struct NSET* pNSet0D)
{
  int i;
  char** verlabel = ut_alloc_2d_char (9, 7);
  int**   veredge = ut_alloc_2d_int  (9, 2);

  strcpy (verlabel[1] , "x0y0z0"); veredge[1][0] = 1; veredge[1][1] = 5;
  strcpy (verlabel[2] , "x1y0z0"); veredge[2][0] = 2; veredge[2][1] = 5;
  strcpy (verlabel[3] , "x0y1z0"); veredge[3][0] = 3; veredge[3][1] = 6;
  strcpy (verlabel[4] , "x1y1z0"); veredge[4][0] = 4; veredge[4][1] = 6;
  strcpy (verlabel[5] , "x0y0z1"); veredge[5][0] = 1; veredge[5][1] = 7;
  strcpy (verlabel[6] , "x1y0z1"); veredge[6][0] = 2; veredge[6][1] = 7;
  strcpy (verlabel[7] , "x0y1z1"); veredge[7][0] = 3; veredge[7][1] = 8;
  strcpy (verlabel[8] , "x1y1z1"); veredge[8][0] = 4; veredge[8][1] = 8;

  (*pNSet0D).qty = 8;
  (*pNSet0D).names = ut_alloc_2d_char (9, 7);
  (*pNSet0D).nodeqty = ut_alloc_1d_int (9);
  (*pNSet0D).nodes = ut_alloc_1d_pint (9);

  for (i = 1; i <= 8; i++)
  {
    neut_nsets_inter (NSet1D, veredge[i][0], veredge[i][1], NULL,
		      &((*pNSet0D).nodes[i]), &((*pNSet0D).nodeqty[i]));
    strcpy ((*pNSet0D).names[i], verlabel[i]);
  }

  ut_free_2d_char (verlabel, 9);
  ut_free_2d_int  (veredge,  9);

  return;
}

void
nem_nsets_2dbody_hex (struct NSET NSet1D, struct NSET* pNSet2D)
{
  int i, j, k, edge;
  int ref, prevqty = (*pNSet2D).qty;
  int** faceedge = ut_alloc_2d_int (7, 4);

  faceedge[1][0] =  1; faceedge[1][1] =  3; faceedge[1][2] =  9; faceedge[1][3] = 11;
  faceedge[2][0] =  2; faceedge[2][1] =  4; faceedge[2][2] = 10; faceedge[2][3] = 12;
  faceedge[3][0] =  1; faceedge[3][1] =  2; faceedge[3][2] =  5; faceedge[3][3] =  7;
  faceedge[4][0] =  3; faceedge[4][1] =  4; faceedge[4][2] =  6; faceedge[4][3] =  8;
  faceedge[5][0] =  5; faceedge[5][1] =  6; faceedge[5][2] =  9; faceedge[5][3] = 10;
  faceedge[6][0] =  7; faceedge[6][1] =  8; faceedge[6][2] = 11; faceedge[6][3] = 12;


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

    for (j = 0; j < 4; j++)
    {
      edge = faceedge[ref][j];

      for (k = 0; k < NSet1D.nodeqty[edge]; k++)
	(*pNSet2D).nodeqty[i] -=
	  ut_array_1d_int_deletencompress ((*pNSet2D).nodes[i], (*pNSet2D).nodeqty[i],
	                                 NSet1D.nodes[edge][k], 1);
    }

    (*pNSet2D).nodes[i] = ut_realloc_1d_int ((*pNSet2D).nodes[i], (*pNSet2D).nodeqty[i]);
  }

  ut_free_2d_int (faceedge, 7);

  return;
}

void
nem_nsets_1dbody_hex (struct NSET NSet0D, struct NSET* pNSet1D)
{
  int i, j, k, ver;
  int ref, prevqty = (*pNSet1D).qty;
  int** edgever = ut_alloc_2d_int (13, 2);

  edgever[1][0]  = 1; edgever[1][1]  = 5;
  edgever[2][0]  = 2; edgever[2][1]  = 6;
  edgever[3][0]  = 3; edgever[3][1]  = 7;
  edgever[4][0]  = 4; edgever[4][1]  = 8;
  edgever[5][0]  = 1; edgever[5][1]  = 2;
  edgever[6][0]  = 3; edgever[6][1]  = 4;
  edgever[7][0]  = 5; edgever[7][1]  = 6;
  edgever[8][0]  = 7; edgever[8][1]  = 8;
  edgever[9][0]  = 1; edgever[9][1]  = 3;
  edgever[10][0] = 2; edgever[10][1] = 4;
  edgever[11][0] = 5; edgever[11][1] = 7;
  edgever[12][0] = 6; edgever[12][1] = 8;

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
      ver = edgever[ref][j];

      for (k = 0; k < NSet0D.nodeqty[ver]; k++)
	(*pNSet1D).nodeqty[i] -=
	  ut_array_1d_int_deletencompress ((*pNSet1D).nodes[i], (*pNSet1D).nodeqty[i],
	                                 NSet0D.nodes[ver][k], 1);
    }

    (*pNSet1D).nodes[i] = ut_realloc_1d_int ((*pNSet1D).nodes[i], (*pNSet1D).nodeqty[i]);
  }

  ut_free_2d_int (edgever, 13);

  return;
}
