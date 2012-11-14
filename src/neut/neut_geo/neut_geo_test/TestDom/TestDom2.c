/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestDom.h"

int
neut_geo_test_dom_def (struct GEO Geo, int verbosity)
{
  int i, j, ver, edge, face;

  if (verbosity)
    ut_print_message (0, 2, "Checking domain definition ...\n");
  
  // DomVer*
  if (Geo.DomVerQty < 4)
  {
    if (verbosity)
      ut_print_message (2, 4, "number of domvers = %d < 4\n", Geo.DomVerQty);
    return 1;
  }

  for (i = 1; i <= Geo.DomVerQty; i++)
  {
    if (Geo.DomVerEdgeQty[i] < 3)
    {
      if (verbosity) 
	ut_print_message (2, 4, "domver %d has  = %d < 4 domedges.\n", i, Geo.DomVerQty);
      return 2;
    }

    for (j = 0; j < Geo.DomVerEdgeQty[i]; j++)
    {
      edge = Geo.DomVerEdgeNb[i][j];
      if (ut_array_1d_int_eltpos (Geo.DomEdgeVerNb[edge], 2, i) == -1)
      {
	if (verbosity) 
	  ut_print_message (2, 4, "domver %d has domedge %d in its list, but domedge not based on domver.\n", i, edge);

	return 3;
      }
    }
  }

  // DomEdge*
  if (Geo.DomEdgeQty < 6)
  {
    if (verbosity) 
      ut_print_message (2, 4, "number of domedges = %d < 6\n", Geo.DomEdgeQty);
    return 4;
  }

  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    for (j = 0; j < 2; j++)
    {
      ver = Geo.DomEdgeVerNb[i][j];
      if (ut_array_1d_int_eltpos (Geo.DomVerEdgeNb[ver], Geo.DomVerEdgeQty[ver], i) == -1)
      {
	if (verbosity)
	  ut_print_message (2, 4, "domedge %d based on domver %d, but domedge not in domver domedge list.\n", i, ver);
	return 5;
      }
    }

    for (j = 0; j < 2; j++)
    {
      face = Geo.DomEdgeFaceNb[i][j];
      if (ut_array_1d_int_eltpos (Geo.DomFaceEdgeNb[face] + 1, Geo.DomFaceVerQty[face], i) == -1)
      {
	if (verbosity)
	  ut_print_message (2, 4, "domedge %d has domface %d in its domface list, but domface not based on domedge.\n", i, face);

	return 6;
      }
    }
  }

  // DomFace*
  if (Geo.DomFaceQty < 4)
  {
    if (verbosity) 
      ut_print_message (2, 4, "number of domface = %d < 6\n", Geo.DomFaceQty);

    return 7;
  }

  for (i = 1; i <= Geo.DomFaceQty; i++)
  {
    if (Geo.DomFaceVerQty[i] < 3)
    {
      if (verbosity)
	ut_print_message (2, 4, "domface %d has %d < 3 vertices\n", i, Geo.DomFaceVerQty[i]);

      return 8;
    }

    for (j = 1; j <= Geo.DomFaceVerQty[i]; j++)
    {
      ver = Geo.DomFaceVerNb[i][j];
      int qty;
      int* tmp = NULL;
      neut_geo_domver_domface (Geo, ver, &tmp, &qty);
      if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
      {
	if (verbosity)
	  ut_print_message (2, 4, "domface %d based on domver %d, but domface not found in domver domfaces.\n", i, ver);

	ut_free_1d_int (tmp);
	return 9;
      }
      ut_free_1d_int (tmp);
    }

    for (j = 1; j <= Geo.DomFaceVerQty[i]; j++)
    {
      edge = Geo.DomFaceEdgeNb[i][j];

      if (ut_array_1d_int_eltpos (Geo.DomEdgeFaceNb[edge], 2, i) == -1)
      {
	if (verbosity)
	  ut_print_message (2, 4, "domface %d based on domedge %d, but domface not found in domedge domface list.\n", i, edge);

	return 10;
      }
    }
  }

  return 0;
}

int
neut_geo_test_dom_tessver (struct GEO Geo, int verbosity)
{
  int i, ver, dver;

  if (verbosity)
    ut_print_message (0, 2, "Checking domain vertices:\n");

  for (dver = 1; dver <= Geo.DomVerQty; dver++)
  {
    if (verbosity)
      ut_print_message (0, 3, "Checking domain vertex %d ...\n", dver);

    ver = Geo.DomTessVerNb[dver];

    if (ver < 1 || ver > Geo.VerQty)
    {
      if (verbosity)
	ut_print_message (2, 4, "has ver %d, but ver < 1 or > verqty = %d.\n",
	    dver, ver, Geo.VerQty);

      return 1;
    }

    if (Geo.VerDom[ver][0] != 0)
    {
      if (verbosity)
	ut_print_message (2, 4, "has ver %d, but ver not marked as domver.\n",
	    dver, ver);

      return 2;
    }

    if (Geo.VerState[ver] == -1)
    {
      if (verbosity)
	ut_print_message (2, 4, "has ver %d, but ver has state = -1.\n",
	    dver, ver);

      return 8;
    }

    if (Geo.VerDom[ver][1] != dver)
    {
      if (verbosity)
	ut_print_message (2, 4, "has ver %d, but ver %d has domver %d.\n",
	    dver, ver, ver, Geo.VerDom[ver][1]);

      return 3;
    }
  }

  if (verbosity)
    ut_print_message (0, 3, "Checking vertex to domain vertex relations ...\n");

  for (i = 1; i <= Geo.VerQty; i++)
  {

    if (Geo.VerState[i] != -1)
      if (Geo.VerDom[i][0] == 0)
      {
	dver = Geo.VerDom[i][1];

	if (dver < 1 || dver > Geo.DomVerQty)
	{
	  if (verbosity)
	    ut_print_message (2, 4, "ver %d has domver %d, but domver does not exist (< 1 or > number of domvers = %d).\n", i, dver, Geo.DomVerQty);
	  return 4;
	}

	if (Geo.DomTessVerNb[dver] != i)
	{
	  if (verbosity)
	    ut_print_message (2, 4, "ver %d has domver %d, but domver does not have ver (%d instead).\n", i, dver, Geo.DomTessVerNb[dver]);
	  return 5;
	}
      }
  }

  return 0;
}

int
neut_geo_test_dom_tessedge (struct GEO Geo, int verbosity)
{
  int i, j, edge, dedge;

  if (verbosity)
    ut_print_message (0, 2, "Checking domain edges:\n");

  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    if (verbosity)
      ut_print_message (0, 3, "Checking domain edge %d ...\n", i);

    if (Geo.DomTessEdgeQty[i] < 1)
    {
      if (verbosity)
	ut_print_message (2, 4, "has %d < 1 tessellation edge.\n", Geo.DomTessEdgeQty[i]);

      return 1;
    }

    for (j = 1; j <= Geo.DomTessEdgeQty[i]; j++)
    {
      edge = Geo.DomTessEdgeNb[i][j];
      
      if (Geo.EdgeState[edge] == -1)
      {
	if (verbosity)
	  ut_print_message (2, 4, "has edge %d, but edge state != -1.\n", edge);

	return 6;
      }

      if (Geo.EdgeDom[edge][0] != 1)
      {
	if (verbosity)
	  ut_print_message (2, 4, "has edge %d, but edge not marked to be on a domedge.\n", edge);

	return 2;
      }

      if (Geo.EdgeDom[edge][1] != i)
      {
	if (verbosity)
	  ut_print_message (2, 4, "has edge %d, but edge does not have domedge (%d instead).\n",
	      edge, Geo.EdgeDom[edge][1]);
	
	return 3;
      }
    }
  }

  if (verbosity)
    ut_print_message (0, 3, "Checking vertex to domain edge relations ...\n");

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
	  if (verbosity)
	    ut_print_message (2, 4, "ver %d is marked to be on domain edge %d, but ver not found in the domedge vers.\n", i, dedge);

	  ut_free_1d_int (tmp);
	  return 4;
	}
	ut_free_1d_int (tmp);
      }

  if (verbosity)
    ut_print_message (0, 3, "Checking edge to domain edge relations ...\n");

  for (i = 1; i <= Geo.EdgeQty; i++)
    if (Geo.EdgeState[i] != -1)
      if (Geo.EdgeDom[i][0] == 1)
      {
	dedge = Geo.EdgeDom[i][1];

	if (ut_array_1d_int_eltpos (Geo.DomTessEdgeNb[dedge] + 1,
				    Geo.DomTessEdgeQty[dedge], i) == -1)
	{
	  if (verbosity)
	    ut_print_message (2, 4, "edge %d has domedge %d, but domedge does not have edge (%d instead)\n", i, dedge, Geo.DomTessVerNb[dedge]);

	  return 5;
	}
      }

  return 0;
}

int
neut_geo_test_dom_tessface (struct GEO Geo, int verbosity)
{
  int i, j, face, dface;

  if (verbosity)
    ut_print_message (0, 2, "Checking domain faces:\n");

  for (i = 1; i <= Geo.DomFaceQty; i++)
  {
    if (verbosity)
      ut_print_message (0, 3, "Checking domain face %d ...\n", i);

    if (Geo.DomTessFaceQty[i] < 1)
    {
      if (verbosity)
	ut_print_message (2, 4, "has %d < 1 tessellation face.\n", Geo.DomTessFaceQty[i]);

      return 1;
    }

    for (j = 1; j <= Geo.DomTessFaceQty[i]; j++)
    {
      face = Geo.DomTessFaceNb[i][j];
      
      if (Geo.FaceState[face] == -1)
      {
	if (verbosity)
	  ut_print_message (2, 4, "has face %d, but face has state != -1.\n",
	      Geo.FaceState[face]);

	return 8;
      }

      if (Geo.FaceDom[face][0] != 2)
      {
	if (verbosity)
	  ut_print_message (2, 4, "has face %d, but face not marked to be on a tessface.\n",
	      face);

	return 2;
      }

      if (Geo.FaceDom[face][1] != i)
      {
	if (verbosity)
	  ut_print_message (2, 4, "has face %d, but face does not have domface (%d instead).\n",
	      Geo.FaceDom[face][1]);

	return 3;
      }
    }
  }

  if (verbosity)
    ut_print_message (0, 3, "Checking vertex to domain face relations ...\n");

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

	  if (verbosity)
	    ut_print_message (2, 4, "ver %d marked to belong to domface %d, but ver not found in domface vers.\n", i, dface);

	  return 4;
	}
	ut_free_1d_int (tmp);
      }

  if (verbosity)
    ut_print_message (0, 3, "Checking edge to domain face relations ...\n");

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

	  if (verbosity)
	    ut_print_message (2, 4, "edge %d marked to belong to domface %d, but edge not found in domface edges.\n", i, dface);

	  return 5;
	}
	ut_free_1d_int (tmp);
      }

  if (verbosity)
    ut_print_message (0, 3, "Checking face to domain face relations ...\n");

  for (i = 1; i <= Geo.FaceQty; i++)
    if (Geo.FaceState[i] != -1)
      if (Geo.FaceDom[i][0] == 2)
      {
	dface = Geo.FaceDom[i][1];
	if (ut_array_1d_int_eltpos (Geo.DomTessFaceNb[dface] + 1,
				    Geo.DomTessFaceQty[dface], i) == -1)
	{
	  if (verbosity)
	    ut_print_message (2, 4, "face %d marked to belong to domface %d, but face not found in domface faces.\n", i, dface);

	  return 4;
	}
      }

  return 0;
}
