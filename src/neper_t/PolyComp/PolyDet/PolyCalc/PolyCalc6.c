/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyCalc.h"

/* FaceVerNb[Face] is rotated to put all the bad vertices at the end.
 * The program returns the number of bad vertices.
 */
int
RotFaceVerNb (struct POLYMOD *pPolymod, int *BadVer, int Face)
{
  int i, num = 0;		/* Mute variables */
  int pos, posb, tmp;		/* NbBadVer;      */

  /* While the first vertex of the face is a bad  vertex 
   *    or the last  vertex of the face is a good vertex,
   * FaceVerNb[Face] is rotated.
   */
  while (oneDIntEltPos
	 (BadVer, 1, BadVer[0], (*pPolymod).FaceVerNb[Face][1], 0) == -1
	 || oneDIntEltPos (BadVer, 1, BadVer[0],
			   (*pPolymod).
			   FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]],
			   0) != -1)
    oneDIntRotElts ((*pPolymod).FaceVerNb[Face], 1,
		    (*pPolymod).FaceVerQty[Face], -1);

  /*for(i=0;i<=(*pPolymod).FaceVerQty[Face];i++)
     printf("%d\n",oneDIntEltPos(BadVer,1,BadVer[0],(*pPolymod).FaceVerNb[Face][i],0));
   */
  /*(*pPolymod).FaceVerNb[Face] is sorted so that the suppressed vertices are situated at the end */
  pos =
    oneDIntEltPos (BadVer, 1, BadVer[0],
		   (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]],
		   0);
  posb =
    oneDIntEltPos (BadVer, 1, BadVer[0], (*pPolymod).FaceVerNb[Face][1], 0);
  while (pos == -1 || posb != -1)
  {
    tmp = (*pPolymod).FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]];
    for (i = (*pPolymod).FaceVerQty[Face]; i >= 2; i--)
      (*pPolymod).FaceVerNb[Face][i] = (*pPolymod).FaceVerNb[Face][i - 1];

    (*pPolymod).FaceVerNb[Face][1] = tmp;
    pos =
      oneDIntEltPos (BadVer, 1, BadVer[0],
		     (*pPolymod).
		     FaceVerNb[Face][(*pPolymod).FaceVerQty[Face]], 0);
    posb =
      oneDIntEltPos (BadVer, 1, BadVer[0], (*pPolymod).FaceVerNb[Face][1], 0);
  }

  for (i = (*pPolymod).FaceVerQty[Face]; i >= 0; i--)
    if (oneDIntEltPos
	(BadVer, 1, BadVer[0], (*pPolymod).FaceVerNb[Face][i], 0) != -1)
      num++;
    else
      break;

  return num;
}
