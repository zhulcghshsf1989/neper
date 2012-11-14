/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestDom.h"

int
neut_geo_test_dom_def (struct GEO Geo)
{
  int i, j, ver, edge, face;
  
  // DomVer*
  if (Geo.DomVerQty < 4)
    return 1;

  for (i = 1; i <= Geo.DomVerQty; i++)
  {
    if (Geo.DomVerEdgeQty[i] < 3)
      return 2;

    for (j = 0; j < Geo.DomVerEdgeQty[i]; j++)
    {
      edge = Geo.DomVerEdgeNb[i][j];
      if (ut_array_1d_int_eltpos (Geo.DomEdgeVerNb[edge], 2, i) == -1)
	return 3;
    }
  }

  // DomEdge*
  if (Geo.DomEdgeQty < 6)
    return 4;

  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    for (j = 0; j < 2; j++)
    {
      ver = Geo.DomEdgeVerNb[i][j];
      if (ut_array_1d_int_eltpos (Geo.DomVerEdgeNb[ver], Geo.DomVerEdgeQty[ver], i) == -1)
	return 5;
    }

    for (j = 0; j < 2; j++)
    {
      face = Geo.DomEdgeFaceNb[i][j];
      if (ut_array_1d_int_eltpos (Geo.DomFaceEdgeNb[face] + 1, Geo.DomFaceVerQty[face], i) == -1)
	return 6;
    }
  }

  // DomFace*
  if (Geo.DomFaceQty < 4)
    return 7;

  for (i = 1; i <= Geo.DomFaceQty; i++)
  {
    if (Geo.DomFaceVerQty[i] < 3)
      return 8;

    for (j = 1; j <= Geo.DomFaceVerQty[i]; j++)
    {
      ver = Geo.DomFaceVerNb[i][j];
      int qty;
      int* tmp = NULL;
      neut_geo_domver_domface (Geo, ver, &tmp, &qty);
      if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
      {
	ut_free_1d_int (tmp);
	return 9;
      }
      ut_free_1d_int (tmp);
    }

    for (j = 1; j <= Geo.DomFaceVerQty[i]; j++)
    {
      edge = Geo.DomFaceEdgeNb[i][j];
      if (ut_array_1d_int_eltpos (Geo.DomEdgeFaceNb[edge], 2, i) == -1)
	return 10;
    }
  }

  return 0;
}

int
neut_geo_test_dom_tessver (struct GEO Geo)
{
  int i, ver, dver;

  for (dver = 1; dver <= Geo.DomVerQty; dver++)
  {
    ver = Geo.DomTessVerNb[dver];

    if (ver < 1 || ver > Geo.VerQty)
      return 1;

    if (Geo.VerDom[ver][0] != 0)
      return 2;

    if (Geo.VerState[ver] == -1)
      return 8;

    if (Geo.VerDom[ver][1] != dver)
    {
      printf ("dom ver %d has ver %d but ver %d has dom ver %d\n", dver, ver, ver, Geo.VerDom[ver][1]);
      return 3;
    }
  }

  for (i = 1; i <= Geo.VerQty; i++)
    if (Geo.VerState[i] != -1)
      if (Geo.VerDom[i][0] == 0)
      {
	dver = Geo.VerDom[i][1];

	if (dver < 1 || dver > Geo.DomVerQty)
	  return 4;

	if (Geo.DomTessVerNb[dver] != i)
	  return 5;
      }

  return 0;
}

int
neut_geo_test_dom_tessedge (struct GEO Geo)
{
  int i, j, edge, dedge;

  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    if (Geo.DomTessEdgeQty[i] < 1)
    {
      printf ("Geo.DomTessEdgeQty[%d] = %d\n", i, Geo.DomTessEdgeQty[i]);
      return 1;
    }

    for (j = 1; j <= Geo.DomTessEdgeQty[i]; j++)
    {
      edge = Geo.DomTessEdgeNb[i][j];
      
      if (Geo.EdgeState[edge] == -1)
	return 6;

      if (Geo.EdgeDom[edge][0] != 1)
      {
	printf ("Geo.EdgeDom[%d][0] == %d != 1\n", edge, Geo.EdgeDom[edge][0]);
	printf ("(Geo.EdgeDom[%d][1] == %d)\n", edge, Geo.EdgeDom[edge][1]);
	return 2;
      }

      if (Geo.EdgeDom[edge][1] != i)
      {
	printf ("Geo.EdgeDom[%d][1] = %d != %d\n", edge, Geo.EdgeDom[edge][1], i);
	return 3;
      }
    }
  }

  for (i = 1; i <= Geo.VerQty; i++)
    if (Geo.VerState[i] != -1)
      if (Geo.VerDom[i][0] == 1)
      {
	dedge = Geo.VerDom[i][1];
	int* tmp = NULL;
	int qty;
	neut_geo_domedge_ver (Geo, dedge, &tmp, &qty);
	if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
	{
	  printf ("ver %d is set to be on domain edge %d\n", i, dedge);
	  printf ("but the ver is not found in the domedge vers: ");
	  ut_array_1d_int_fprintf (stdout, tmp, qty, "%d");
	  ut_free_1d_int (tmp);
	  return 4;
	}
	ut_free_1d_int (tmp);
      }

  for (i = 1; i <= Geo.EdgeQty; i++)
    if (Geo.EdgeState[i] != -1)
      if (Geo.EdgeDom[i][0] == 1)
      {
	dedge = Geo.EdgeDom[i][1];
	if (ut_array_1d_int_eltpos (Geo.DomTessEdgeNb[dedge] + 1,
				    Geo.DomTessEdgeQty[dedge], i) == -1)
	  return 5;
      }

  return 0;
}

int
neut_geo_test_dom_tessface (struct GEO Geo)
{
  int i, j, face, dface;

  for (i = 1; i <= Geo.DomFaceQty; i++)
  {
    if (Geo.DomTessFaceQty[i] < 1)
      return 1;

    for (j = 1; j <= Geo.DomTessFaceQty[i]; j++)
    {
      face = Geo.DomTessFaceNb[i][j];
      
      if (Geo.FaceState[face] == -1)
	return 8;

      if (Geo.FaceDom[face][0] != 2)
	return 2;

      if (Geo.FaceDom[face][1] != i)
	return 3;
    }
  }

  for (i = 1; i <= Geo.VerQty; i++)
    if (Geo.VerState[i] != -1)
      if (Geo.VerDom[i][0] == 2)
      {
	dface = Geo.VerDom[i][1];
	int* tmp = NULL;
	int qty;
	neut_geo_domface_ver (Geo, dface, &tmp, &qty);
	if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
	{
	  ut_free_1d_int (tmp);
	  return 4;
	}
	ut_free_1d_int (tmp);
      }

  for (i = 1; i <= Geo.EdgeQty; i++)
    if (Geo.EdgeState[i] != -1)
      if (Geo.EdgeDom[i][0] == 2)
      {
	dface = Geo.EdgeDom[i][1];
	int* tmp = NULL;
	int qty;
	neut_geo_domface_edges (Geo, dface, &tmp, &qty);
	if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
	{
	  ut_free_1d_int (tmp);
	  return 5;
	}
	ut_free_1d_int (tmp);
      }

  for (i = 1; i <= Geo.FaceQty; i++)
    if (Geo.FaceState[i] != -1)
      if (Geo.FaceDom[i][0] == 2)
      {
	dface = Geo.FaceDom[i][1];
	if (ut_array_1d_int_eltpos (Geo.DomTessFaceNb[dface] + 1,
				    Geo.DomTessFaceQty[dface], i) == -1)
	  return 4;
      }

  return 0;
}
