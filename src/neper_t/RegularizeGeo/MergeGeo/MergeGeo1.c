/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"MergeGeo.h"

int
MergeGeo (struct GEO *pGeo, struct GEOPARA GeoPara, struct IN In, int
    *pDelId, int loop)
{
  int i, trash, testgeo;
  int edge, edgeb, newver;
  double lratio1;
  double tmp;
  double maxff, minmaxff = 0;
  int minid = 0;
  int ThisDelQty, DelQty = 0;
  struct GEO GeoC;
  struct GEO GeoCC;
  double lratio2;

  int verbosity = 0;

  neut_geo_set_zero (&GeoC);
  neut_geo_set_zero (&GeoCC);

  edge = NextEdgeToDel (*pGeo, GeoPara, &lratio1);
  
  if (isatty (1))
    ut_print_message (0, 2, "                        ");
  else
    ut_print_message (0, 2, "  ");

  fflush (stdout);
    
  if (isatty (1))
    fprintf (stdout, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b %d     %4.0f%%    %6d", loop, 0., DelQty);
  else
    fprintf (stdout, " %d     %4.0f%%    %6d", loop, 0., DelQty);

  while (edge != -1)
  {
    if ((*pDelId) >= In.maxedgedelqty)
      break;

    ThisDelQty = 0;

    neut_geo_geo (*pGeo, &GeoC);

    testgeo = DeleteEdge (&GeoC, edge, &newver, &maxff);

    /* if (testgeo == 0 and) maxff < max, the edge is deleted */
    /* else (maxff > max), we try with another edge */
    /* even if testgeo == -1, the deletion continues:  we let the
     * program a chance to find a suitable solution, and generally it
     * does.
     */

    /* if the max ff is acceptable, we record the modif into Geo
     * else, we re-initialize it.
     */
    if ((! strcmp ((*pGeo).DomType, "cube") && (testgeo == 0 || testgeo == -1))
     || (  strcmp ((*pGeo).DomType, "cube") && (testgeo == 0)))
    {
      if (maxff < GeoPara.maxff)
      {
	ThisDelQty = 1;
	neut_geo_geo (GeoC, pGeo);
      }
      else
      {
	if (verbosity >= 2)
	{
	  printf ("newver = %d\n", newver);
	  printf ("Deleting neighbouring edges, in turn: %d: ", GeoC.VerEdgeQty[newver]);
	  abort ();
	  for (i = 0; i <= GeoC.VerEdgeQty[newver] - 1; i++)
	    printf ("%d ", GeoC.VerEdgeNb[newver][i]);
	  printf ("\n");
	}

	minmaxff = 180;
	minid = 0;
	for (i = 0; i < GeoC.VerEdgeQty[newver]; i++)
	{
	  edgeb = GeoC.VerEdgeNb[newver][i];
	  
	  neut_geo_edge_selratio (*pGeo, GeoPara, edgeb, &lratio2);

	  int status = 0;
	  int ver1, ver2;
	  ver1 = GeoC.EdgeVerNb[edgeb][0];
	  ver2 = GeoC.EdgeVerNb[edgeb][1];

	  if (GeoC.EdgeDom[edgeb][0] == 2)
	  {
	    // means that the 2 vertices are on (different) domain edges
	    // the edge would shrink to a third vertex (which is a domain
	    // vertex), which is not handle properly.
	    if (GeoC.VerDom[ver1][0] == 1 && GeoC.VerDom[ver2][0] == 1)
	      status = -1;
	  }
	  else if (GeoC.EdgeDom[edgeb][0] == 1)
	  {
	    if (GeoC.VerDom[ver1][0] == 0 && GeoC.VerDom[ver2][0] == 0)
	      status = -1;
	  }

	  if (status == 0 && lratio1 + lratio2 < 2)
	  {
	    neut_geo_geo (GeoC, &GeoCC);
	    testgeo = DeleteEdge (&GeoCC, edgeb, &trash, &tmp);

	    if (testgeo == 0 && tmp < minmaxff)
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

	if (minmaxff < GeoPara.maxff)
	{
	  ThisDelQty = 2;

	  DeleteEdge (pGeo, edge, &newver, &tmp);
	  DeleteEdge (pGeo, minid, &newver, &tmp);


	  /* this node has been deleted: edgedel <- 1 */
	  (*pGeo).EdgeDel[minid] = 1;
	}
      }
    }

    /* in all cases, edgedel <- 1 */
    (*pGeo).EdgeDel[edge] = 1;

    DelQty += ThisDelQty;
    (*pDelId) += ThisDelQty;

    if (verbosity >= 1)
      PrintDeletion (loop, lratio1, ThisDelQty, edge, minid, maxff,
		     minmaxff);

    /* Searching next edge to delete */
    edge = NextEdgeToDel (*pGeo, GeoPara, &lratio1);
    
    if (isatty (1))
      printf ("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b %d     %4.0f%%    %6d", loop, 100 * lratio1, DelQty);
    else
      printf (" %d     %4.0f%%    %6d", loop, 100 * lratio1, DelQty);

    fflush (stdout);

    neut_geo_free (&GeoC);
    neut_geo_free (&GeoCC);
  }
  if (isatty (1))
    printf ("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b %d     %4.0f%%    %6d", loop, 100., DelQty);
  else
    printf (" %d     %4.0f%%    %6d", loop, 100., DelQty);

  printf ("\n");

  /* ForceMerge deletes the edges specified manually 
   * ForceMerge(pGeo,GeoPara);
   */

  /* at the end,  edgedel is reset to 0. */
  for (i = 1; i <= (*pGeo).EdgeQty; i++)
    (*pGeo).EdgeDel[i] = 0;

  /* printf("end of MergeGeo\n"); */
  neut_geo_free (&GeoC);
  neut_geo_free (&GeoCC);

  return DelQty;
}
