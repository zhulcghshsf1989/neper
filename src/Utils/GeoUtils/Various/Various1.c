/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Various.h"

int
VerEdgeOnFace (struct GEO Geo, int ver, int face, int *edges)
{
  int i;
  int tmpedge;
  int qty = 0;

  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    tmpedge = Geo.FaceEdgeNb[face][i];
    if (oneDIntEltPos (Geo.EdgeVerNb[tmpedge], 0, 1, ver, 0) != -1)
    {
      edges[qty] = tmpedge;
      qty++;
    }
  }

  if (qty != 2)
    abort ();

  return qty;
}

void
NormalAtVer (struct GEO Geo, int face, int ver, double *normal, int beg)
{
  int i;
  double *v1 = oneDDoubleDAlloc (4);
  double *v2 = oneDDoubleDAlloc (4);
  double norm;
  int pos1, pos2, pos3;
  int verbef, veraft;

  /* we search the position of ver to determine those of the neighbouring */
  /* ones. */
  pos2 = oneDIntEltPos (Geo.FaceVerNb[face], 1, Geo.FaceVerQty[face], ver, 0);

  /* test */
  if (pos2 == -1)
  {
    LineHeader (-1);
    printf ("NormalAtVer: ver %d not found in face %d\n", ver, face);
    abort ();
  }

  /* verbef is the nb of the vertex situated before ver in face. */
  pos1 =
    oneDIntEltPos (Geo.FaceVerNb[face], 1, Geo.FaceVerQty[face], ver, -1);
  verbef = Geo.FaceVerNb[face][pos1];

  /* veraft is the nb of the vertex situated after ver in face. */
  pos3 = oneDIntEltPos (Geo.FaceVerNb[face], 1, Geo.FaceVerQty[face], ver, 1);
  veraft = Geo.FaceVerNb[face][pos3];

  /* calculation of the two vectors, the one from verbef to ver */
  /* and the one from ver to veraft */
  for (i = 0; i <= 2; i++)
  {
    v1[beg + i] = Geo.VerCoo[verbef][i] - Geo.VerCoo[ver][i];
    v2[beg + i] = Geo.VerCoo[ver][i] - Geo.VerCoo[veraft][i];
  }

  /* calculation of the vector normal to v1 and v2 */
  VectorVectProd (v1, v2, normal, beg);

  /* and it is normed */
  norm = VectorNorm (normal, 1);
  for (i = 1; i <= 3; i++)
    normal[i] /= norm;

  oneDDoubleFree (v1);
  oneDDoubleFree (v2);

  return;
}


void
AngleAtVer (struct GEO Geo, int face, int ver, double *pangle)
{
  int i;
  int verbef, veraft;
  double *v1 = oneDDoubleDAlloc (3);
  double *v2 = oneDDoubleDAlloc (3);
  int pos1, pos2, pos3;

  /* we search the position of ver to determine those of the neighbouring */
  /* ones. */
  pos2 = oneDIntEltPos (Geo.FaceVerNb[face], 1, Geo.FaceVerQty[face], ver, 0);

  /* test */
  if (pos2 == -1)
  {
    LineHeader (2);
    printf ("AngleAtVer: ver %d not found in face %d\n", ver, face);
    abort ();
  }

  /* verbef is the nb of the vertex situated before ver in face. */
  pos1 =
    oneDIntEltPos (Geo.FaceVerNb[face], 1, Geo.FaceVerQty[face], ver, -1);
  verbef = Geo.FaceVerNb[face][pos1];

  /* veraft is the nb of the vertex situated after ver in face. */
  pos3 = oneDIntEltPos (Geo.FaceVerNb[face], 1, Geo.FaceVerQty[face], ver, 1);
  veraft = Geo.FaceVerNb[face][pos3];

  /* calculation of the two vectors, the one from verbef to ver */
  /* and the one from ver to veraft */
  for (i = 0; i <= 2; i++)
  {
    v1[i] = Geo.VerCoo[verbef][i] - Geo.VerCoo[ver][i];
    v2[i] = Geo.VerCoo[veraft][i] - Geo.VerCoo[ver][i];
  }

  /* calculation of the angle between v1 and v2 */
  (*pangle) = VectorAngle (v1, v2, 0, 1);

  oneDDoubleFree (v1);
  oneDDoubleFree (v2);

  return;
}
