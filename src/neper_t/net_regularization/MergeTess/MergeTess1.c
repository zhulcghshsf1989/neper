/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"MergeTess.h"

int
MergeTess (struct TESS *pTess, struct REG Reg, int* pDelId)
{
  int i, loop, trash, testtess;
  int edge, edgeb, newver;
  double lratio1;
  double tmp;
  double maxff, minmaxff = 0;
  int minid = 0;
  int ThisDelQty, DelQty = 0;
  struct TESS TessC;
  struct TESS TessCC;
  double lratio2;

  int verbosity = 0;

  neut_tess_set_zero (&TessC);
  neut_tess_set_zero (&TessCC);

  char* string = ut_alloc_1d_char (100);
  char* prevstring = ut_alloc_1d_char (100);
  for (loop = 1; loop <= Reg.mloop; loop++)
  {
    edge = NextEdgeToDel (*pTess, Reg, &lratio1);
    
    while (edge != -1)
    {
      if ((*pDelId) >= Reg.maxedgedelqty)
	break;

      ThisDelQty = 0;

      neut_tess_tess (*pTess, &TessC);

      testtess = DeleteEdge (&TessC, edge, &newver, &maxff);

      /* if (testtess == 0 and) maxff < max, the edge is deleted */
      /* else (maxff > max), we try with another edge */
      /* even if testtess == -1, the deletion continues:  we let the
       * program a chance to find a suitable solution, and generally it
       * does.
       */

      /* if the max ff is acceptable, we record the modif into Tess
       * else, we re-initialize it.
       */
      if ((! strcmp ((*pTess).DomType, "cube") && (testtess == 0 || testtess == -1))
       || (! strcmp ((*pTess).DomType, "poly") && (testtess == 0 || testtess == -1))
       || (  strcmp ((*pTess).DomType, "cube") && (testtess == 0)))
      {
	if (maxff < Reg.maxff)
	{
	  ThisDelQty = 1;
	  neut_tess_tess (TessC, pTess);
	}
	else
	{
	  if (verbosity >= 2)
	  {
	    printf ("newver = %d\n", newver);
	    printf ("Deleting neighbouring edges, in turn: %d: ", TessC.VerEdgeQty[newver]);
	    abort ();
	    for (i = 0; i <= TessC.VerEdgeQty[newver] - 1; i++)
	      printf ("%d ", TessC.VerEdgeNb[newver][i]);
	    printf ("\n");
	  }

	  minmaxff = 180;
	  minid = 0;
	  for (i = 0; i < TessC.VerEdgeQty[newver]; i++)
	  {
	    edgeb = TessC.VerEdgeNb[newver][i];
	    
	    neut_tess_edge_selratio (*pTess, Reg, edgeb, &lratio2);

	    int status = 0;
	    int ver1, ver2;
	    ver1 = TessC.EdgeVerNb[edgeb][0];
	    ver2 = TessC.EdgeVerNb[edgeb][1];

	    if (TessC.EdgeDom[edgeb][0] == 2)
	    {
	      // means that the 2 vertices are on (different) domain edges
	      // the edge would shrink to a third vertex (which is a domain
	      // vertex), which is not handle properly.
	      if (TessC.VerDom[ver1][0] == 1 && TessC.VerDom[ver2][0] == 1)
		status = -1;
	    }
	    else if (TessC.EdgeDom[edgeb][0] == 1)
	    {
	      if (TessC.VerDom[ver1][0] == 0 && TessC.VerDom[ver2][0] == 0)
		status = -1;
	    }

	    if (status == 0 && lratio1 + lratio2 < 2)
	    {
	      neut_tess_tess (TessC, &TessCC);
	      testtess = DeleteEdge (&TessCC, edgeb, &trash, &tmp);

	      if (testtess == 0 && tmp < minmaxff)
	      {
		minmaxff = tmp;
		minid = edgeb;
	      }
	    }
	    else if (verbosity >= 3)
	    {
	      ut_print_lineheader (0);
	      printf ("edgedel %d=-1 --> skipping\n", edgeb);
	    }
	  }

	  if (minmaxff < Reg.maxff)
	  {
	    ThisDelQty = 2;

	    DeleteEdge (pTess, edge, &newver, &tmp);
	    DeleteEdge (pTess, minid, &newver, &tmp);


	    /* this node has been deleted: edgedel <- 1 */
	    (*pTess).EdgeDel[minid] = 1;
	  }
	}
      }

      /* in all cases, edgedel <- 1 */
      (*pTess).EdgeDel[edge] = 1;

      DelQty += ThisDelQty;
      (*pDelId) += ThisDelQty;

      if (verbosity >= 1)
	PrintDeletion (loop, lratio1, ThisDelQty, edge, minid, maxff,
		       minmaxff);

      /* Searching next edge to delete */
      edge = NextEdgeToDel (*pTess, Reg, &lratio1);
      
      sprintf (string, "loop=%d/%d %%3.0f%%%% del=%d",
		       loop, Reg.mloop, DelQty);
      ut_print_progress_nonl (stdout, 100 * lratio1, 100, string, prevstring);

      neut_tess_free (&TessC);
      neut_tess_free (&TessCC);
    }
      
    /* ForceMerge deletes the edges specified manually 
     * ForceMerge(pTess,Reg);
     */

    for (i = 1; i <= (*pTess).EdgeQty; i++)
      (*pTess).EdgeDel[i] = 0;

    if (DelQty == 0)
      break;
  }

  neut_tess_free (&TessC);
  neut_tess_free (&TessCC);

  return DelQty;
}
