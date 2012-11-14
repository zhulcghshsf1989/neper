/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "SpaceUtils.h"

/* threeDDoubleDist returns the euclidian distance between two points.*/
double
twoDDoubleDist (double *a, double *b, int beg)
{
  double res;

  if (beg == 1)
    res =
      sqrt ((b[1] - a[1]) * (b[1] - a[1]) + (b[2] - a[2]) * (b[2] - a[2]));
  else if (beg == 0)
    res =
      sqrt ((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]));
  else
    abort ();

  return res;
}

/* threeDDoubleDist returns the euclidian distance between two points.*/
double
threeDDoubleDist (double *a, double *b, int beg)
{
  double res;

  if (beg == 1)
    res =
      sqrt (pow (b[1] - a[1], 2) + pow (b[2] - a[2], 2) +
	    pow (b[3] - a[3], 2));
  else if (beg == 0)
    res =
      sqrt (pow (b[0] - a[0], 2) + pow (b[1] - a[1], 2) +
	    pow (b[2] - a[2], 2));
  else
    abort ();

  return res;
}


/* Side indicates the half-space whose a specified point belongs to. */
int
Side (double *P, double *C)
{
  if (P[1] * C[1] + P[2] * C[2] + P[3] * C[3] - P[0] < 0)
    return -1;
  else
    return 1;
}

/* Side indicates the half-space whose a specified point belongs to. */
int
Side_tol (double *P, double *C, double eps)
{
  if (P[1] * C[1] + P[2] * C[2] + P[3] * C[3] - P[0] < eps)
    return -1;
  else
    return 1;
}

/* Side2d indicates the half-space whose a specified point belongs to (2d). */
int
Side2d (double *P, double *C)
{
  if (P[1] * C[1] + P[2] * C[2] - P[0] < 0)
    return -1;
  else
    return 1;
}

/* Barycenter calculates the coordinates of the polygon/hedron
 * center, as the barycenter of all its vertices.
 */
void
Barycenter (double **coo, int Vqty, double *res)
{
  int i, j;
  double tmp;

  for (i = 1; i <= 3; i++)
  {
    tmp = 0;
    for (j = 1; j <= Vqty; j++)
      tmp += coo[j][i];

    res[i] = tmp / (double) Vqty;
  }

  return;
}

/* PolygonArea returns the area of a polygon.
 * THE POLYGON MUST BE CONVEX for PolygonArea to return a reliable
 * result. The adopted algorithm consists in the decomposition of
 * the polygon into as many triangles as there are polygon vertices
 * or edges, and to sum their individual contributions. A central
 * point is used to build these triangles.
 * vertex i: coo[i][1,2,3] (i=1,2,3)
 * N.B.: for triangles, the specific subroutine TriangleArea
 * can also be used.
 */
/*
double PolygonArea(struct POLYGON Polygon)
{
    int i;	
    double res=0;
    double** coo;
    
    coo=(double**)twoDDoubleDAlloc(4,3);
    
    for(i=1;i<=Polygon.VerQty-1;i++)
    {
	PolygonTriangle(Polygon,i,i+1,coo);
	res+=TriangleArea(coo);
    }
    PolygonTriangle(Polygon,Polygon.VerQty,1,coo);
    res+=TriangleArea(coo);

    twoDDoubleFree(coo,4);
    
    return res;
}
*/

/* PolyhedronVolume returns the area of a polygon. THE POLYHEDRON
 * MUST BE CONVEX for PolyhedronVolume to return a reliable
 * result. The adopted algorithm consists in the decomposition of
 * the polyhedron into as many pyramids as there are polyhedron faces
 * and to sum their individual contributions. A central
 * point is used to build these pyramids.
 */
/*
double PolyhedronVolume(struct POLY Poly)
{
    double res=0;
    int i,k;
    double*  PCenter;  
    double** TrCoo;
    double** coo;

    TrCoo=  (double**)twoDDoubleDAlloc( 4,4);
    PCenter=(double*) oneDDoubleDAlloc( 4);


    for(i=1;i<=Poly.VerQty;i++)
    {
	printf("i=%d/%d\n",i,Poly.VerQty);
	for(k=0;k<=2;k++)
	{
	    printf("k=%d\n",k);
	    printf("%lf\n",Poly.VerCoo[i][k]);
	    coo[i][k+1]=Poly.VerCoo[i][k];
	}
    }

    printf("hi1\n");
    Barycenter(coo,Poly.VerQty,PCenter);
    printf("hi2\n");

    return res;
}
*/

void
DistribPoints (double *MaxSize, double *MinSize, int Qty, double **Data,
	       gsl_rng * r)
{
  int i, j, id;
  double *coo = oneDDoubleDAlloc (4);

  /* ckecking the zone sizes */

  /* printf("DISTRIB from %lf to %lf: %d germs\n",-MinSize[1],-MaxSize[1],Qty); */

  for (i = 1; i <= 3; i++)
  {
    if (MinSize[2 * i - 1] < MaxSize[2 * i - 1])
    {
      printf ("DistribPoints: Bad zone sizes.\n");
      abort ();
    }
    if (MinSize[2 * i] > MaxSize[2 * i])
    {
      printf ("DistribPoints: Bad zone sizes.\n");
      abort ();
    }
  }

  id = 0;
  while (id != Qty)
  {
    for (i = 1; i <= 3; i++)
      coo[i] =
	MaxSize[2 * i - 1] + gsl_rng_uniform (r) * (MaxSize[2 * i] -
						    MaxSize[2 * i - 1]);

    if (coo[1] < MinSize[1] || coo[1] > MinSize[2]
	|| coo[2] < MinSize[3] || coo[2] > MinSize[4]
	|| coo[3] < MinSize[5] || coo[3] > MinSize[6])
    {
      id++;
      for (j = 1; j <= 3; j++)
	Data[id][j] = coo[j];
    }
  }

  oneDDoubleFree (coo);

  return;
}

void
GetPtNextPos (double **Coo, double *Vel, double dt, int interpolation,
	      double *NextCoo)
{
  int i;

  if (interpolation == 1)
    for (i = 0; i <= 1; i++)
      NextCoo[i] = Coo[0][i] + Vel[i] * dt;
  else if (interpolation == 2)
    for (i = 0; i <= 1; i++)
      NextCoo[i] = Coo[1][i] + 2 * Vel[i] * dt;
  else
    abort ();

  return;
}

int
PointAligned (double *p1, double *p2, double *p3, double eps)
{
  int i;
  double angle;
  double *v1 = oneDDoubleDAlloc (3);
  double *v2 = oneDDoubleDAlloc (3);

  for (i = 0; i < 3; i++)
    v1[i] = p2[i] - p1[i];

  for (i = 0; i < 3; i++)
    v2[i] = p3[i] - p1[i];

  angle = VectorAngle (v1, v2, 0, 0);

  if (angle < eps)
    return 1;
  else
    return 0;
}
