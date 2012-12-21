/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyReg.h"

/* The polyhedron description is as poly[Center].
 */
void
PolyhedronRecording (int Center, struct POLYMOD Polymod, struct POLY *Poly)
{
  int *CompVer;
  int *CompVerInv;
  int *CompFace;
  int *CompFaceInv;
  /* ver i in poly     == ver CompVer[i]      in polymod
   * ver i in polymod  == ver CompVerInv[i]   in poly
   */
  /* face i in poly    == face CompFace[i]    in polymod
   * face i in polymod == face CompFaceInv[i] in poly
   */

  /* Memory allocation of CompVer, CompVerInv, CompFace & CompFaceInv.
   */
  CompVer = ut_alloc_1d_int (Polymod.VerQty + 1);
  CompVerInv = ut_alloc_1d_int (Polymod.VerQty + 1);
  CompFace = ut_alloc_1d_int (Polymod.FaceQty + 1);
  CompFaceInv = ut_alloc_1d_int (Polymod.FaceQty + 1);

  /* Initialization of CompVer, CompVerInv, CompFace & CompFaceInv.
   */
  Pm2PComp (Polymod, CompVer, CompVerInv, CompFace, CompFaceInv);

  /* Writing of VerQty.
   */
  Pm2PVerQty (Poly, Center, CompVer);
  /* Writing of FaceQty.
   */
  Pm2PFaceQty (Poly, Center, CompFace);

  /* Memory allocation of VerFace & VerCoo.
   */
  Poly[Center].VerFace = ut_alloc_2d_int (CompVer[0] + 1, 3);
  Poly[Center].VerCoo = ut_alloc_2d (CompVer[0] + 1, 3);

  /* Writing of VerFace & VerCoo
   */
  Pm2PVerFace (Polymod, Poly, CompVer, CompFaceInv, Center);
  Pm2PVerCoo (Polymod, Poly, CompVer, Center);

  /* Memory allocation of Face****.
   */
  Poly[Center].FacePoly = ut_alloc_1d_int (Poly[Center].FaceQty + 1);
  Poly[Center].FaceEq = ut_alloc_2d (Poly[Center].FaceQty + 1, 4);
  Poly[Center].FaceVerQty = ut_alloc_1d_int (Poly[Center].FaceQty + 1);
  Poly[Center].FaceVerNb = ut_alloc_1d_pint (Poly[Center].FaceQty + 1);

  /* Writing of FacePoly
   */
  Pm2PFacePoly (Polymod, Poly, CompFace, Center);
  /* Writing of FaceEq
   */
  Pm2PFaceEq (Polymod, Poly, CompFace, Center);
  /* Writing of FaceVerQty
   */
  Pm2PFaceVerQty (Polymod, Poly, CompFace, Center);
  /* Writing of FaceVerNb
   */
  Pm2PFaceVerNb (Polymod, Poly, CompFace, CompVerInv, Center);

  /* Memory allocation of temporary used arrays.
   */
  ut_free_1d_int (CompVer);
  ut_free_1d_int (CompVerInv);
  ut_free_1d_int (CompFace);
  ut_free_1d_int (CompFaceInv);

  return;
}
