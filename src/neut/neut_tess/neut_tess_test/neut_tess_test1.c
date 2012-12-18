/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_lcl.h"

int
neut_tess_test (struct TESS Tess, int verbosity)
{
  int i;
  int res = 0;

  if (verbosity)
    ut_print_message (0, 1, "Checking vertices:\n");
  
  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerState[i] != -1)
      if (TestVer (Tess, i, verbosity) != 0)
	return -1;

  if (verbosity)
    ut_print_message (0, 1, "Checking edges:\n");

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeState[i] == 0)
      if (TestEdge (Tess, i, verbosity) != 0)
	return -1;

  if (verbosity)
    ut_print_message (0, 1, "Checking faces:\n");

  for (i = 1; i <= Tess.FaceQty; i++)
    if (Tess.FaceState[i] >= 0)
      if (TestFace (Tess, i, verbosity) != 0)
	return -1;

  if (verbosity)
    ut_print_message (0, 1, "Checking polyhedra:\n");

  for (i = 1; i <= Tess.PolyQty; i++)
    if (TestPoly (Tess, i, verbosity) != 0)
      return -1;

  if (verbosity)
    ut_print_message (0, 1, "Checking domain:\n");

  if (neut_tess_test_dom (Tess, verbosity) != 0)
    return -1;

  return res;
}

int
neut_tess_testAroundVer (struct TESS Tess, int ver, int verbosity)
{
  int i, edge;
  int res = 0;
  int faceqty, polyqty;
  int* face = NULL;
  int* poly = NULL;
  
  if (verbosity >= 4)
    printf ("checking TESS AROUND VER %d\n", ver);

  /* Checking of the vertex */
  if (verbosity >= 4)
    printf ("Checking vertex: ");

  if (Tess.VerState[ver] != -1)
  {
    if (verbosity >= 4)
      printf ("%d:", ver);
    res = TestVer (Tess, ver, verbosity);
    if (res != 0)
    {
      if (verbosity >= 4)
	printf ("ver %d: nok\n", ver);
      return res;
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Checking of the edges */
  if (verbosity >= 4)
    printf ("\nChecking edges: ");

  for (i = 0; i <= Tess.VerEdgeQty[ver] - 1; i++)
  {
    edge = Tess.VerEdgeNb[ver][i];

    if (Tess.EdgeState[edge] == 0)
    {
      if (verbosity >= 4)
	printf ("%d:", edge);
      res = TestEdge (Tess, edge, 0);
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

  neut_tess_ver_faces (Tess, ver, &face, &faceqty);

  if (verbosity >= 4)
    printf ("\nChecking faces: ");

  for (i = 0; i < faceqty; i++)
  {
    if (Tess.FaceState[face[i]] >= 0)
    {
      if (verbosity >= 4)
	printf ("%d:", face[i]);

      res = TestFace (Tess, face[i], 0);
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

  /* Checking of the polys */
  neut_tess_ver_polys (Tess, ver, &poly, &polyqty);

  if (verbosity >= 4)
    printf ("\nChecking polys: ");
  for (i = 0; i < polyqty; i++)
  {
    if (poly[i] < 0)
      continue;

    if (verbosity >= 4)
      printf ("%d:", i);
    res = TestPoly (Tess, poly[i], 0);
    if (res != 0)
    {
      if (verbosity >= 4)
	printf ("poly %d: nok\n", poly[i]);
      return res;
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  /* Checking domain */
  if (verbosity >= 4)
    printf ("\nChecking domain: ");
  res = neut_tess_test_dom (Tess, 0);

  ut_free_1d_int (face);
  ut_free_1d_int (poly);

  return res;
}

/* Checking the tessellation around a vertex: all the polys around the ver
 * and all entities of the polys: faces, edges & vertices.
 */
int
neut_tess_testAroundVer2 (struct TESS Tess, int ver, int verbosity)
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
    printf ("checking TESS (2) AROUND VER %d (/%d)\n", ver, Tess.VerQty);

  /* Searching polyhedra */
  neut_tess_ver_polys (Tess, ver, &poly, &polyqty);

  /* searching faces and edges of all the polyhedra */
  faces[0] = 0;
  edges[0] = 0;
  vers[0] = 0;
  for (i = 0; i < polyqty; i++)
  {
    if (poly[i] < 0)
      continue;

    for (j = 1; j <= Tess.PolyFaceQty[poly[i]]; j++)
    {
      face = Tess.PolyFaceNb[poly[i]][j];

      faces[0]++;
      faces[faces[0]] = face;

      for (k = 1; k <= Tess.FaceVerQty[face]; k++)
      {
	edges[0]++;
	edges[edges[0]] = Tess.FaceEdgeNb[face][k];

	vers[0]++;
	vers[vers[0]] = Tess.FaceVerNb[face][k];
      }
    }
  }

  ut_array_1d_int_sort_uniq (faces + 1, faces[0], &(faces[0]));

  ut_array_1d_int_sort_uniq (edges + 1, edges[0], &(edges[0]));

  ut_array_1d_int_sort_uniq (vers  + 1, vers[0] , &(vers[0]));

  /* Checking of the vertex */
  if (verbosity >= 4)
    printf ("Checking vertex: ");

  for (i = 1; i <= vers[0]; i++)
  {
    ver = vers[i];

    if (Tess.VerState[ver] != -1)
    {
      if (verbosity >= 4)
	printf ("%d:", ver);
      res = TestVer (Tess, ver, 0);
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

  /* Checking of the edges */
  if (verbosity >= 4)
    printf ("\nChecking edges: ");

  for (i = 0; i <= edges[0]; i++)
  {
    edge = edges[i];

    if (Tess.EdgeState[edge] == 0)
    {
      if (verbosity >= 4)
	printf ("%d:", edge);
      res = TestEdge (Tess, edge, 0);
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

  /* Checking of the faces */

  if (verbosity >= 4)
    printf ("\nChecking faces: ");

  for (i = 1; i <= faces[0]; i++)
  {
    face = faces[i];

    if (Tess.FaceState[face] >= 0)
    {
      if (verbosity >= 4)
	printf ("%d:", face);
      res = TestFace (Tess, face, 0);
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

  /* Checking of the polys */

  if (verbosity >= 4)
    printf ("\nChecking polys: ");

  for (i = 0; i < polyqty; i++)
  {
    if (poly[i] < 0)
      continue;

    if (verbosity >= 4)
      printf ("%d:", i);
    res = TestPoly (Tess, poly[i], 0);
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
  // Checking domain
  if (verbosity >= 4)
    printf ("\nChecking domain: ");
  res = neut_tess_test_dom (Tess, 0);
  */

  ut_free_1d_int (vers);
  ut_free_1d_int (edges);
  ut_free_1d_int (faces);
  ut_free_1d_int (poly);

  return res;
}
