/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_geo_test_lcl.h"

int
neut_geo_test (struct GEO Geo)
{
  int i;
  int res = 0;
  int verbosity = 0;
  
  /* Testing of the vertices */
  if (verbosity >= 4)
    printf ("Testing vertices: ");
  for (i = 1; i <= Geo.VerQty; i++)
    if (Geo.VerState[i] != -1)
    {
      if (verbosity >= 4)
	printf ("%d:", i);
      res = TestVer (Geo, i, verbosity);
      if (res != 0)
      {
	if (verbosity >= 4)
	  printf ("ver %d: nok\n", i);
	return res;
      }
    }
  if (verbosity >= 4)
    printf ("\n");

  /* Testing of the edges */
  if (verbosity >= 4)
    printf ("\nTesting edges: ");
  for (i = 1; i <= Geo.EdgeQty; i++)
    if (Geo.EdgeState[i] == 0)
    {
      if (verbosity >= 4)
	printf ("%d:", i);
      res = TestEdge (Geo, i);
      if (res != 0)
      {
	if (verbosity >= 4)
	  printf ("edge %d: nok\n", i);
	return res;
      }
    }
  if (verbosity >= 4)
    printf ("\n");

  /* Testing of the faces */
  if (verbosity >= 4)
    printf ("\nTesting faces: ");
  for (i = 1; i <= Geo.FaceQty; i++)
    if (Geo.FaceState[i] >= 0)
    {
      if (verbosity >= 4)
	printf ("%d:", i);
      res = TestFace (Geo, i);
      if (res != 0)
      {
	if (verbosity >= 4)
	  printf ("face %d: nok (TestFace returned %d)\n", i, res);
	return res;
      }
    }
  if (verbosity >= 4)
    printf ("\n");

  /* Testing of the polys */
  if (verbosity >= 4)
    printf ("\nTesting polys: ");
  for (i = 1; i <= Geo.PolyQty; i++)
  {
    if (verbosity >= 4)
      printf ("%d:", i);
    res = TestPoly (Geo, i);
    if (res != 0)
    {
      if (verbosity >= 4)
	printf ("poly %d: nok\n", i);
      return res;
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  /* Testing domain */
  if (verbosity >= 4)
    printf ("\nTesting domain: ");
  res = neut_geo_test_dom (Geo);

  return res;
}

int
neut_geo_testAroundVer (struct GEO Geo, int ver, int verbosity)
{
  int i, edge;
  int res = 0;
  int faceqty, polyqty;
  int* face = NULL;
  int* poly = NULL;
  
  if (verbosity >= 4)
    printf ("testing GEO AROUND VER %d\n", ver);

  /* Testing of the vertex */
  if (verbosity >= 4)
    printf ("Testing vertex: ");

  if (Geo.VerState[ver] != -1)
  {
    if (verbosity >= 4)
      printf ("%d:", ver);
    res = TestVer (Geo, ver, verbosity);
    if (res != 0)
    {
      if (verbosity >= 4)
	printf ("ver %d: nok\n", ver);
      return res;
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Testing of the edges */
  if (verbosity >= 4)
    printf ("\nTesting edges: ");

  for (i = 0; i <= Geo.VerEdgeQty[ver] - 1; i++)
  {
    edge = Geo.VerEdgeNb[ver][i];

    if (Geo.EdgeState[edge] == 0)
    {
      if (verbosity >= 4)
	printf ("%d:", edge);
      res = TestEdge (Geo, edge);
      if (res != 0)
      {
	if (verbosity >= 4)
	  printf ("edge %d: nok\n", edge);
	return res;
      }
    }
    else
    {
      res = 1;
      abort ();
      return res;
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  neut_geo_ver_faces (Geo, ver, &face, &faceqty);

  if (verbosity >= 4)
    printf ("\nTesting faces: ");

  for (i = 0; i < faceqty; i++)
  {
    if (Geo.FaceState[face[i]] >= 0)
    {
      if (verbosity >= 4)
	printf ("%d:", face[i]);
      res = TestFace (Geo, face[i]);
      if (res != 0)
      {
	if (verbosity >= 4)
	  printf ("face[i] %d: nok\n", face[i]);
	return res;
      }
    }
    else
    {
      abort ();
      return 1;
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Testing of the polys */
  neut_geo_ver_polys (Geo, ver, &poly, &polyqty);

  if (verbosity >= 4)
    printf ("\nTesting polys: ");
  for (i = 0; i < polyqty; i++)
  {
    if (poly[i] < 0)
      continue;

    if (verbosity >= 4)
      printf ("%d:", i);
    res = TestPoly (Geo, poly[i]);
    if (res != 0)
    {
      if (verbosity >= 4)
	printf ("poly %d: nok\n", poly[i]);
      return res;
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  /* Testing domain */
  if (verbosity >= 4)
    printf ("\nTesting domain: ");
  res = neut_geo_test_dom (Geo);

  ut_free_1d_int (face);
  ut_free_1d_int (poly);

  return res;
}

/* Testing the geometry around a vertex: all the polys around the ver
 * and all entities of the polys: faces, edges & vertices.
 */
int
neut_geo_testAroundVer2 (struct GEO Geo, int ver, int verbosity)
{
  int i, j, k;
  int edge, face;
  int res = 0;
  int *vers = ut_alloc_1d_int (10000);
  int *edges = ut_alloc_1d_int (10000);
  int *faces = ut_alloc_1d_int (10000);
  int polyqty;
  int *poly = NULL;

  if (verbosity >= 4)
    printf ("testing GEO (2) AROUND VER %d (/%d)\n", ver, Geo.VerQty);

  /* Searching polyhedra */
  neut_geo_ver_polys (Geo, ver, &poly, &polyqty);

  /* searching faces and edges of all the polyhedra */
  faces[0] = 0;
  edges[0] = 0;
  vers[0] = 0;
  for (i = 0; i < polyqty; i++)
  {
    if (poly[i] < 0)
      continue;

    for (j = 1; j <= Geo.PolyFaceQty[poly[i]]; j++)
    {
      face = Geo.PolyFaceNb[poly[i]][j];

      faces[0]++;
      faces[faces[0]] = face;

      for (k = 1; k <= Geo.FaceVerQty[face]; k++)
      {
	edges[0]++;
	edges[edges[0]] = Geo.FaceEdgeNb[face][k];

	vers[0]++;
	vers[vers[0]] = Geo.FaceVerNb[face][k];
      }
    }
  }

  ut_array_1d_int_sort_uniq (faces + 1, faces[0], &(faces[0]));

  ut_array_1d_int_sort_uniq (edges + 1, edges[0], &(edges[0]));

  ut_array_1d_int_sort_uniq (vers  + 1, vers[0] , &(vers[0]));

  /* Testing of the vertex */
  if (verbosity >= 4)
    printf ("Testing vertex: ");

  for (i = 1; i <= vers[0]; i++)
  {
    ver = vers[i];

    if (Geo.VerState[ver] != -1)
    {
      if (verbosity >= 4)
	printf ("%d:", ver);
      res = TestVer (Geo, ver, 0);
      if (res != 0)
      {
	if (verbosity >= 4)
	  printf ("ver %d: nok\n", ver);
	ut_free_1d_int (vers);
	ut_free_1d_int (edges);
	ut_free_1d_int (faces);
	ut_free_1d_int (poly);
	return res;
      }
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Testing of the edges */
  if (verbosity >= 4)
    printf ("\nTesting edges: ");

  for (i = 0; i <= edges[0]; i++)
  {
    edge = edges[i];

    if (Geo.EdgeState[edge] == 0)
    {
      if (verbosity >= 4)
	printf ("%d:", edge);
      res = TestEdge (Geo, edge);
      if (res != 0)
      {
	if (verbosity >= 4)
	  printf ("edge %d: nok\n", edge);
	ut_free_1d_int (vers);
	ut_free_1d_int (edges);
	ut_free_1d_int (faces);
	ut_free_1d_int (poly);
	return res;
      }
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  /* Testing of the faces */

  if (verbosity >= 4)
    printf ("\nTesting faces: ");

  for (i = 1; i <= faces[0]; i++)
  {
    face = faces[i];

    if (Geo.FaceState[face] >= 0)
    {
      if (verbosity >= 4)
	printf ("%d:", face);
      res = TestFace (Geo, face);
      if (res != 0)
      {
	if (verbosity >= 4)
	  printf ("face %d: nok\n", face);
	ut_free_1d_int (vers);
	ut_free_1d_int (edges);
	ut_free_1d_int (faces);
	ut_free_1d_int (poly);
	return res;
      }
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Testing of the polys */

  if (verbosity >= 4)
    printf ("\nTesting polys: ");

  for (i = 0; i < polyqty; i++)
  {
    if (poly[i] < 0)
      continue;

    if (verbosity >= 4)
      printf ("%d:", i);
    res = TestPoly (Geo, poly[i]);
    if (res != 0)
    {
      if (verbosity >= 4)
	printf ("poly %d: nok\n", poly[i]);
	ut_free_1d_int (vers);
	ut_free_1d_int (edges);
	ut_free_1d_int (faces);
	ut_free_1d_int (poly);
      return res;
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  /*
  // Testing domain
  if (verbosity >= 4)
    printf ("\nTesting domain: ");
  res = neut_geo_test_dom (Geo);
  */

  ut_free_1d_int (vers);
  ut_free_1d_int (edges);
  ut_free_1d_int (faces);
  ut_free_1d_int (poly);

  return res;
}
