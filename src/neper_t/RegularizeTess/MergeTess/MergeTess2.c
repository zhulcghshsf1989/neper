/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"MergeTess.h"

void
PrintDeletion (int loop, double LengthRatio, int ThisDelQty, int edge,
	       int minid, double maxff, double minmaxff)
{
  if (ThisDelQty == 1)		/* 1 edge deleted */
  {
    ut_print_lineheader (0);
    printf
      ("[%d/%4.2f] %4d: ff=%5.1f ....................... %4d deleted.\n",
       loop, LengthRatio, edge, maxff, edge);
  }
  else if (ThisDelQty == 2)	/* 2 edges deleted */
  {
    ut_print_lineheader (0);
    printf
      ("[%d/%4.2f] %4d: ff=%5.1f + %4d: ff=%5.1f ...... %4d deleted.\n",
       loop, LengthRatio, edge, maxff, minid, minmaxff, edge);
    printf ("\t\t\t\t\t\t\t  %4d deleted.\n", minid);
  }
  else if (ThisDelQty == 0 && minid != 0)	/* none edge del. bcse ff too high */
  {
    ut_print_lineheader (0);
    printf
      ("[%d/%4.2f] %4d: ff=%5.1f + %4d: ff=%5.1f ......  ff too high.\n",
       loop, LengthRatio, edge, maxff, minid, minmaxff);
  }
  else if (ThisDelQty == 0 && minid == 0)	/* none edge del. bcse no 2nd edge */
  {
    ut_print_lineheader (0);
    printf
      ("[%d/%4.2f] %4d: ff=%5.1f + none: ff=%5.1f ......  ff too high.\n",
       loop, LengthRatio, edge, maxff, maxff);
  }
  else				/* there is a pb */
    abort ();

  return;
}

/* NextEdgeToDel returns the smallest edge id --excluding the forbidden edges. */
int
NextEdgeToDel (struct TESS Tess, struct TESSPARA TessPara, double *plengthratio)
{
  int i;			/* mute variable */
  int edgenb = -1;		/* returned value */
  double lratio;

  (*plengthratio) = 1.;

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeState[i] == 0 && Tess.EdgeDel[i] == 0)
    {
      neut_tess_edge_selratio (Tess, TessPara, i, &lratio);

      if (lratio < *plengthratio)
      {
	(*plengthratio) = lratio;
	edgenb = i;
      }
    }

  return edgenb;
}

/*
   ForceMerge deletes the edges specified manually 
void ForceMerge(struct TESS* pTess,struct TESSPARA TessPara)
{
    int i;
    
    for(i=1;i<=TessPara.forcemergeqty;i++)
	DeleteEdge(pTess,TessPara.forcemerge[i]);

    return;
}
*/
