/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Tess.h"

/* Recording of PolyQty */
int
TPolyQty (struct GERMSET GermSet)
{
  return GermSet.N;
}

/* Recording of FaceQty */
int
TFaceQty (struct GERMSET GermSet, struct POLY *Poly)
{
  int i, j, nb = 0;

  for (i = 1; i <= GermSet.N; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      if (Poly[i].FacePoly[j] > i || Poly[i].FacePoly[j] < 0)
	nb++;

  return nb;
}

/* Recording of VerQty */
int
TVerQty (struct GERMSET GermSet, struct POLY *Poly)
{
  int i, j, nb = 0;

  for (i = 1; i <= GermSet.N; i++)
    for (j = 1; j <= Poly[i].VerQty; j++)
      if (Poly[i].FacePoly[Poly[i].VerFace[j][0]] > i
	  || Poly[i].FacePoly[Poly[i].VerFace[j][0]] < 0)
	if (Poly[i].FacePoly[Poly[i].VerFace[j][1]] > i
	    || Poly[i].FacePoly[Poly[i].VerFace[j][1]] < 0)
	  if (Poly[i].FacePoly[Poly[i].VerFace[j][2]] > i
	      || Poly[i].FacePoly[Poly[i].VerFace[j][2]] < 0)
	    nb++;

  return nb;
}

/* Recording of EdgeQty */
int
TEdgeQty (struct GERMSET GermSet, struct POLY *Poly)
{
  int i, j, nb = 0;

  for (i = 1; i <= GermSet.N; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      nb += Poly[i].FaceVerQty[j];

  return nb;
}

/* Recording of TFNb.
 */
void
IniTFNb (struct TESS Tess, struct POLY *Poly, int **TFNb)
{
  int i, j, PrevF;		/* mute variables */
  int nb = 0;			/* tmp variable containing the face nb (in TFace).   */
  int PrevP;			/* temporary variable containing the number of the   */
  /* previous polyhedron in which the face has already */
  /* been recorded.                                    */

  for (i = 1; i <= Tess.PolyQty; i++)
  {
    TFNb[i] = ut_alloc_1d_int (Poly[i].FaceQty + 1);
    for (j = 1; j <= Poly[i].FaceQty; j++)
      /* If the vertex has not been created yet, we do it.
       */
      if (i < Poly[i].FacePoly[j] || Poly[i].FacePoly[j] < 0)
      {
	nb++;
	TFNb[i][j] = nb;
      }
    /* Else, we search the number of the concerned vertex.
     */
      else if (Poly[i].FacePoly[j] > 0)
      {
	PrevP = Poly[i].FacePoly[j];
	PrevF =
	  oneDIntEltPos (Poly[PrevP].FacePoly, 1, Poly[PrevP].FaceQty, i, 0);
	TFNb[i][j] = TFNb[PrevP][PrevF];
      }
  }

  return;
}

/* Recording of TVNb.
 */
void
IniTVNb (struct TESS Tess, struct POLY *Poly, int **TVNb)
{
  int PNb, VNb;			/* mute variables representing a polyhedron number and
				 * a polyhedron vertex.
				 */
  int nb = 0;			/* internal variable representing the number of real
				 * vertices.
				 */

  /* All vertices of all polyhedra are studied successively.
   */
  for (PNb = 1; PNb <= Tess.PolyQty; PNb++)
  {
    TVNb[PNb] = ut_alloc_1d_int (Poly[PNb].VerQty + 1);
    for (VNb = 1; VNb <= Poly[PNb].VerQty; VNb++)
      /* TreatVer returns 1 if the ver has not been recorded previously,
       * 0 else. If 1, the vertex number has to be created, else it has
       * to be searched.
       */
      if (TreatVer (Poly, PNb, VNb) == 1)
      {
	/* Creation of a new ver
	 */
	nb = AddVer (TVNb, PNb, VNb, nb);
      }
      else
      {
	/* Search of a previously-created ver
	 */
	SearchVer (Poly, PNb, VNb, TVNb);
      }
  }

  return;
}

/* Recording of Face germ
 */
void
IniTessFacePoly (struct TESS *pTess, struct POLY *Poly, int **TFNb)
{
  int i, j;			/* mute variables */
  int FNb;

  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      if (i < Poly[i].FacePoly[j] || Poly[i].FacePoly[j] < 0)
      {
	FNb = TFNb[i][j];
	(*pTess).FacePoly[FNb][0] = i;
	(*pTess).FacePoly[FNb][1] = Poly[i].FacePoly[j];
      }

  return;
}

/* Recording of Face eq
 */
void
IniTessFaceEq (struct TESS *pTess, struct POLY *Poly, int **TFNb)
{
  int i, j, k;
  int FNb;

  int first, second, third;
  double *a = ut_alloc_1d (4);
  double *b = ut_alloc_1d (4);
  double *c = ut_alloc_1d (4);

  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      if (i < Poly[i].FacePoly[j] || Poly[i].FacePoly[j] < 0)
      {
	FNb = TFNb[i][j];
	for (k = 0; k < 4; k++)
	  (*pTess).FaceEq[FNb][k] = Poly[i].FaceEq[j][k];

	/* Then, we check if the face is correctly oriented,    */
	/* (if not, modification eq. <-- -eq)                   */
	first = Poly[i].FaceVerNb[j][1];
	second = Poly[i].FaceVerNb[j][2];
	third = Poly[i].FaceVerNb[j][3];

	for (k = 1; k < 4; k++)
	{
	  a[k] = Poly[i].VerCoo[second][k - 1] - Poly[i].VerCoo[first][k - 1];
	  b[k] = Poly[i].VerCoo[third][k - 1] - Poly[i].VerCoo[second][k - 1];
	}
	VectorVectProd (a, b, c, 1);
	if (VectorScalProd ((*pTess).FaceEq[FNb], c, 1) < 0)
	  for (k = 0; k < 4; k++)
	    (*pTess).FaceEq[FNb][k] *= -1;
      }

  ut_free_1d (a);
  ut_free_1d (b);
  ut_free_1d (c);

  return;
}

/* Recording of Face ver qty
 */
void
IniTessFaceVerQty (struct TESS *pTess, struct POLY *Poly, int **TFNb)
{
  int i, j;
  int FNb;

  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].FaceQty; j++)
      if (Poly[i].FacePoly[j] > i || Poly[i].FacePoly[j] < 0)
      {
	FNb = TFNb[i][j];
	(*pTess).FaceVerQty[FNb] = Poly[i].FaceVerQty[j];
      }

  return;
}

/* Recording of the numbers of the ver parent germs
 */
void
IniTessVerGerm (struct TESS *pTess, struct POLY *Poly, int **TVNb)
{
  int i, j, k;
  int VNb;
  int ParF, ParG;
  int *Treat;

  Treat = ut_alloc_1d_int ((*pTess).VerQty + 1);

  /* For every poly, for every ver, if the (tess-eq) ver has not 
   * been treated yet (known thanks to Treat temporary array), the
   * 4 parent germs are recorded. The first one is positive, the
   * three others can be negative (poly initial faces).
   */
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].VerQty; j++)
    {
      VNb = TVNb[i][j];
      if (Treat[VNb] == 0)
      {
	(*pTess).VerGerm[VNb][0] = i;
	for (k = 1; k <= 3; k++)
	{
	  ParF = Poly[i].VerFace[j][k - 1];
	  ParG = Poly[i].FacePoly[ParF];
	  (*pTess).VerGerm[VNb][k] = ParG;
	}
	Treat[VNb] = 1;
      }
    }

  ut_free_1d_int (Treat);

  return;
}


/* Recording of the vertex coordinates
 */
void
IniTessVerCoo (struct TESS *pTess, struct POLY *Poly, int **TVNb)
{
  int i, j, k;
  int VNb;

  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 1; j <= Poly[i].VerQty; j++)
    {
      VNb = TVNb[i][j];
      for (k = 0; k < 3; k++)
	(*pTess).VerCoo[VNb][k] = Poly[i].VerCoo[j][k];
    }

  return;
}

/* TreatFaceVer returns 1 if the ver has not been recorded previously,
 * 0 else.
 */
int
TreatFaceVer (struct TESS *pTess, int FNb, int S1, int S2)
{
  int k;
  int *PFNb = ut_alloc_1d_int (3);
  int *PPNb = ut_alloc_1d_int (3);

  /* CommonGerms returns the numbers of the (always) 3 common parent
   * germs of the two studied vertices: S1 & S2. They are recorded in
   * PPNb & they are the 3 parent germs of the studied edge.
   * If the amount of parent germs is not 3, critical error 4 is generated.
   */
  if (CommonGerms (*pTess, S1, S2, PPNb) != 3)
  {
    printf ("number of common germs = %d != 3\n", CommonGerms (*pTess, S1, S2, PPNb));
    ut_error_reportbug ();
  }

  /* BissFace returns the bissecting face of segment based on the two
   * specified polyhedron centers.
   * PFNb[0...2] are the numbers of the three faces, if they all exist.
   * Else, the value is 0 or -1 (such cases are normal).
   */
  PFNb[0] = BissFace (*pTess, PPNb[0], PPNb[1]);
  PFNb[1] = BissFace (*pTess, PPNb[1], PPNb[2]);
  PFNb[2] = BissFace (*pTess, PPNb[2], PPNb[0]);

  /* If one of the parent faces of the studied edge has a number which is
   * positive (meaning the the face exists) and lower than that of the
   * current face, this means that the edge has already been recorded
   * -- during the treatment of this face. Its number is returned.
   * Else, that means that the edge must be recorded: 0 is returned.
   */

  int status = 0;

  for (k = 0; k < 3; k++)
    if (PFNb[k] < FNb && PFNb[k] > 0)
    {
      status = PFNb[k];
      break;
    }

  ut_free_1d_int (PFNb);
  ut_free_1d_int (PPNb);

  return status;
}

/* Creation of new edge nb through the incrementation of nb
 * and the writing of:
 * EdgeVerNb[nb][0,1]
 * FaceEdgeNb[FNb][S1]
 */
int
AddEdge (struct TESS *pTess, int FNb, int i, int S1, int S2, int nb)
{
  /* Incrementation of nb
   */
  nb++;

  (*pTess).EdgeVerNb = ut_realloc_2d_int_addline ((*pTess).EdgeVerNb, nb + 1, 2);

  /* Recording of EdgeVerNb
   */
  RecEdgeVerNb (pTess, nb, S1, S2);

  /* Recording of FaceEdgeNb
   */
  RecFaceEdgeNb (pTess, FNb, i, nb);

  return nb;
}

/* Search of the previously-given number of the edge
 * to record it in FaceEdgeNb.
 */
void
SearchEdge (struct TESS *pTess, int FNb, int PrevF, int S1, int S2, int p)
{
  int pos1, pos2;
  int nb;

  /* The positions of S1 & S2 into the previous face are recorded as
   * pos1 & pos2.
   * According to their values, the one to use to access the number of
   * the edge based on S1 and S2 is used (i.e. recorded as nb). 
   */
  pos1 =
    oneDIntEltPos ((*pTess).FaceVerNb[PrevF], 1, (*pTess).FaceVerQty[PrevF],
		   S1, 0);
  pos2 =
    oneDIntEltPos ((*pTess).FaceVerNb[PrevF], 1, (*pTess).FaceVerQty[PrevF],
		   S2, 0);

  /* nb is the minimum value between pos1 and pos2...
   */
  nb = IntMin (pos1, pos2);

  /* ... excepted is the following cases.
   */
  if (pos1 == 1 && pos2 != 2)
    nb = pos2;
  if (pos2 == 1 && pos1 != 2)
    nb = pos1;

  /* The edge number is recorded in face FNb.
   */
  (*pTess).FaceEdgeNb[FNb][p] = (*pTess).FaceEdgeNb[PrevF][nb];

  return;
}

/* Recording of FaceEdgeOri
 */
void
IniTessFaceEdgeOri (struct TESS *pTess)
{
  int i, j;
  int ver;
  int edge;

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceEdgeOri[i] = ut_alloc_1d_int ((*pTess).FaceVerQty[i] + 1);

    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
    {
      ver = (*pTess).FaceVerNb[i][j];
      edge = (*pTess).FaceEdgeNb[i][j];
      if ((*pTess).EdgeVerNb[edge][0] == ver)
	(*pTess).FaceEdgeOri[i][j] = 1;
      else if ((*pTess).EdgeVerNb[edge][1] == ver)
	(*pTess).FaceEdgeOri[i][j] = -1;
      else
	ut_error_reportbug ();
    }
  }

  return;
}

/* Recording of Tess.PolyEdgeX
 */
void
IniTessPolyEdge (struct TESS *pTess)
{
  int PNb;

  /* for every polyhedron, the edge numbers are recorded.
   */
  for (PNb = 1; PNb <= (*pTess).PolyQty; PNb++)
    (*pTess).PolyEdgeQty[PNb] = RecPolyEdges (pTess, PNb);

  return;
}


/* IniPolyFaceOri returns 1 if face i of polyhedron poly
   has an outgoing normal, and -1 else. */
int
IniPolyFaceOri (struct TESS *pTess, int poly, int i)
{
  int face;
  int d;

  face = (*pTess).PolyFaceNb[poly][i];

  d = -ut_space_planeside ((*pTess).FaceEq[face], (*pTess).CenterCoo[poly] - 1);

  return d;
}
