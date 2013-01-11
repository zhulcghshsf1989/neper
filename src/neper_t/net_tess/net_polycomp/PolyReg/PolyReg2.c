/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyReg.h"

void
Pm2PComp (struct POLYMOD Polymod, int *CompVer, int *CompVerInv,
	  int *CompFace, int *CompFaceInv)
{
  /* Determination of CompVer and CompVerInv
   */
  Pm2PCompVerNVerInv (Polymod, CompVer, CompVerInv);
  /* Determination of CompFace and CompFaceInv
   */
  Pm2PCompFaceNFaceInv (Polymod, CompFace, CompFaceInv);

  return;
}

/* Writing of VerQty.
 */
void
Pm2PVerQty (struct POLY *Poly, int Center, int *CompVer)
{
  Poly[Center].VerQty = CompVer[0];

  return;
}

/* Writing of FaceQty.
 */
void
Pm2PFaceQty (struct POLY *Poly, int Center, int *CompFace)
{
  Poly[Center].FaceQty = CompFace[0];

  return;
}

/* Writing of VerFace.
 */
void
Pm2PVerFace (struct POLYMOD Polymod, struct POLY *Poly, int *CompVer,
	     int *CompFaceInv, int Center)
{
  int i, j;

  for (i = 1; i <= Poly[Center].VerQty; i++)
    for (j = 0; j <= 2; j++)
      Poly[Center].VerFace[i][j] =
	CompFaceInv[Polymod.VerFace[CompVer[i]][j]];

  return;
}

/* Writing of VerCoo.
 */
void
Pm2PVerCoo (struct POLYMOD Polymod, struct POLY *Poly, int *CompVer,
	    int Center)
{
  int i, j;

  for (i = 1; i <= Poly[Center].VerQty; i++)
    for (j = 0; j <= 2; j++)
      Poly[Center].VerCoo[i][j] = Polymod.VerCoo[CompVer[i]][j];

  return;
}

/* Writing of FacePoly
 */
void
Pm2PFacePoly (struct POLYMOD Polymod, struct POLY *Poly, int *CompFace,
	      int Center)
{
  int i;

  for (i = 1; i <= Poly[Center].FaceQty; i++)
    Poly[Center].FacePoly[i] = Polymod.FacePoly[CompFace[i]];

  return;
}

/* Writing of FaceEq
 */
void
Pm2PFaceEq (struct POLYMOD Polymod, struct POLY *Poly, int *CompFace,
	    int Center)
{
  int i, j;

  for (i = 1; i <= Poly[Center].FaceQty; i++)
    for (j = 0; j <= 3; j++)
      Poly[Center].FaceEq[i][j] = Polymod.FaceEq[CompFace[i]][j];

  return;
}

/* Writing of FaceVerQty
 */
void
Pm2PFaceVerQty (struct POLYMOD Polymod, struct POLY *Poly, int *CompFace,
		int Center)
{
  int i;

  for (i = 1; i <= Poly[Center].FaceQty; i++)
    Poly[Center].FaceVerQty[i] = Polymod.FaceVerQty[CompFace[i]];

  return;
}

/* Writing of FaceVerNb
 */
void
Pm2PFaceVerNb (struct POLYMOD Polymod, struct POLY *Poly, int *CompFace,
	       int *CompVerInv, int Center)
{
  int i, j;

  for (i = 1; i <= Poly[Center].FaceQty; i++)
  {
    Poly[Center].FaceVerNb[i] =
      ut_alloc_1d_int (Poly[Center].FaceVerQty[i] + 1);
    for (j = 1; j <= Poly[Center].FaceVerQty[i]; j++)
      Poly[Center].FaceVerNb[i][j] =
	CompVerInv[Polymod.FaceVerNb[CompFace[i]][j]];
  }

  return;
}
