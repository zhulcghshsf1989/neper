/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_debug.h"

void
neut_debug_nodes (FILE* file, struct NODES Nodes)
{
  fprintf (file, "====== Beginning of Nodes ================================\n");
  fprintf (file, "Nodes:\n");
  fprintf (file, "NodeQty = %d\n", Nodes.NodeQty);

  if (Nodes.NodeQty > 0)
  {
    fprintf (file, "NodeCoo =\n");
    ut_array_2d_fprintf (file, Nodes.NodeCoo + 1, Nodes.NodeQty, 3, "%f");
    fprintf (file, "NodeCl =\n");
    if (Nodes.NodeCl == NULL)
      fprintf (file, "null\n");
    else
      ut_array_1d_fprintf (file, Nodes.NodeCl + 1, Nodes.NodeQty, "%f\n");
  }
  
  fprintf (file, "====== End of Nodes ======================================\n");
  
  return;
}

void
neut_debug_mesh (FILE* file, struct MESH Mesh)
{
  int i;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  fprintf (file, "====== Beginning of Mesh =================================\n");
  fprintf (file, "Mesh:\n");
  fprintf (file, "Dimension = %d\n", Mesh.Dimension);
  fprintf (file, "EltOrder = %d\n", Mesh.EltOrder);
  fprintf (file, "EltQty = %d\n", Mesh.EltQty);
  if (Mesh.EltQty > 0)
  {
    fprintf (file, "EltNodes = \n");
    ut_array_2d_int_fprintf (file, Mesh.EltNodes + 1, Mesh.EltQty, eltnodeqty, "%d");
    fprintf (file, "ElsetQty = %d\n", Mesh.ElsetQty);
    fprintf (file, "Elsets = (quantity then ids of elements)\n");
    for (i = 1; i <= Mesh.ElsetQty; i++)
      ut_array_1d_int_fprintf (file, Mesh.Elsets[i], Mesh.Elsets[i][0] + 1, "%d");
  }

  fprintf (file, "====== End of Mesh =======================================\n");
  return;
}


void
neut_debug_fod (FILE* file, int** FoD)
{
  int i;

  fprintf (file, "====== Beginning of FoD ==================================\n");
  fprintf (file, "FoD = (quantity then ids of faces)\n");
  for (i = 1; i <= 6; i++)
    ut_array_1d_int_fprintf (file, FoD[i], FoD[i][0] + 1, "%d");

  fprintf (file, "====== End of FoD ========================================\n");

  return;
}


void
neut_debug_fodnodes (FILE* file, int** FoDNodes)
{
  int i;

  fprintf (file, "====== Beginning of FoDNodes =============================\n");
  fprintf (file, "FoDNodes = (quantity then ids of faces)\n");
  for (i = 1; i <= 6; i++)
    ut_array_1d_int_fprintf (file, FoDNodes[i], FoDNodes[i][0] + 1, "%d");

  fprintf (file, "====== End of FoDNodes ===================================\n");

  return;
}


void
neut_debug_geo (FILE* file, struct GEO Geo)
{
  int i;

  fprintf (file, "====== Beginning of Geo ==================================\n");
  if (Geo.version != NULL)
    fprintf (file, "version = %s\n", Geo.version);
  else
    fprintf (file, "version = NULL\n");

  fprintf (file, "N = %d\n", Geo.N);
  fprintf (file, "Id = %d\n", Geo.Id);

  if (Geo.morpho != NULL)
    fprintf (file, "morpho = %s\n", Geo.morpho);
  else
    fprintf (file, "morpho = NULL\n");

  fprintf (file, "Type = %s\n", Geo.Type);

  fprintf (file, "VerQty = %d\n", Geo.VerQty);
  fprintf (file, "EdgeQty = %d\n", Geo.EdgeQty);
  fprintf (file, "FaceQty = %d\n", Geo.FaceQty);
  fprintf (file, "PolyQty = %d\n", Geo.PolyQty);

  fprintf (file, "== Vertices =================\n");
  fprintf (file, "[id] VerCoo =\n");
  if (Geo.VerCoo == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.VerQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_fprintf (file, Geo.VerCoo[i], 3, "%f");
    }

  fprintf (file, "[id] VerEdgeQty then VerEdgeNb =\n");
  if (Geo.VerEdgeQty == NULL || Geo.VerEdgeNb == NULL)
    fprintf (file, "one is NULL\n");
  else 
    for (i = 1; i <= Geo.VerQty; i++)
    {
      fprintf (file, "%d %d ", i, Geo.VerEdgeQty[i]);
      ut_array_1d_int_fprintf (file, Geo.VerEdgeNb[i], Geo.VerEdgeQty[i], "%d");
    }

  fprintf (file, "[id] VerDom =\n");
  if (Geo.VerDom == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.VerQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Geo.VerDom[i], 2, "%d");
    }
  
  fprintf (file, "[id] VerState =\n");
  if (Geo.VerState == NULL)
    fprintf (file, "is NULL\n");
  else
  for (i = 1; i <= Geo.VerQty; i++)
    fprintf (file, "%d %d\n", i, Geo.VerState[i]);

  fprintf (file, "== Edges =================\n");

  fprintf (file, "[id] EdgeVerNb =\n");
  if (Geo.EdgeVerNb == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.EdgeQty; i++)
      fprintf (file, "%d %d %d\n", i, Geo.EdgeVerNb[i][0], Geo.EdgeVerNb[i][1]);

  fprintf (file, "[id] EdgeFaceQty then EdgeFaceNb =\n");
  if (Geo.EdgeFaceQty == NULL || Geo.EdgeFaceNb == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.EdgeQty; i++)
    {
      fprintf (file, "%d %d ", i, Geo.EdgeFaceQty[i]);
      ut_array_1d_int_fprintf (file, Geo.EdgeFaceNb[i], Geo.EdgeFaceQty[i], "%d");
    }

  fprintf (file, "[id] EdgeLength =\n");
  if (Geo.EdgeLength == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.EdgeQty; i++)
      fprintf (file, "%d %f\n", i, Geo.EdgeLength[i]);

  fprintf (file, "[id] EdgeState =\n");
  if (Geo.EdgeState == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.EdgeQty; i++)
      fprintf (file, "%d %d\n", i, Geo.EdgeState[i]);

  fprintf (file, "[id] EdgeDel =\n");
  if (Geo.EdgeDel == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.EdgeQty; i++)
      fprintf (file, "%d %d\n", i, Geo.EdgeDel[i]);

  fprintf (file, "[id] EdgeDom =\n");
  if (Geo.EdgeDom == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.EdgeQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Geo.EdgeDom[i], 2, "%d");
    }

  fprintf (file, "== Faces =================\n");

  fprintf (file, "[id] FacePoly =\n");
  if (Geo.FacePoly == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
      fprintf (file, "%d %d %d\n", i, Geo.FacePoly[i][0], Geo.FacePoly[i][1]);

  fprintf (file, "[id] FaceVerQty then FaceVerNb =\n");
  if (Geo.FaceVerQty == NULL || Geo.FaceVerNb == NULL)
    fprintf (file, "one is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Geo.FaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Geo.FaceVerNb[i] + 1, Geo.FaceVerQty[i], "%d");
    }

  fprintf (file, "[id] FaceEdgeQty then FaceEdgeNb =\n");
  if (Geo.FaceVerQty == NULL || Geo.FaceEdgeNb == NULL)
    fprintf (file, "one is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Geo.FaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Geo.FaceEdgeNb[i] + 1, Geo.FaceVerQty[i], "%d");
    }

  fprintf (file, "[id] FaceEdgeQty then Ori =\n");
  if (Geo.FaceVerQty == NULL || Geo.FaceEdgeOri == NULL)
    fprintf (file, "one is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Geo.FaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Geo.FaceEdgeOri[i] + 1, Geo.FaceVerQty[i], "%d");
    }

  fprintf (file, "[id] FaceEq =\n");
  if (Geo.FaceEq == NULL)
    fprintf (file, "is NULL\n");
  else
  for (i = 1; i <= Geo.FaceQty; i++)
    ut_array_1d_fprintf (file, Geo.FaceEq[i], 4, "%f");

  fprintf (file, "[id] FaceState =\n");
  if (Geo.FaceState == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
      fprintf (file, "%d %d\n", i, Geo.FaceState[i]);

  fprintf (file, "[id] FacePt =\n");
  if (Geo.FacePt == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
      fprintf (file, "%d %d\n", i, Geo.FacePt[i]);

  fprintf (file, "[id] FacePtCoo =\n");
  if (Geo.FacePtCoo == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_fprintf (file, Geo.FacePtCoo[i], 3, "%f");
    }

  fprintf (file, "[id] FaceFF =\n");
  if (Geo.FaceFF == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
      fprintf (file, "%d %f\n", i, Geo.FaceFF[i]);

  fprintf (file, "[id] FaceDom =\n");
  if (Geo.FaceDom == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.FaceQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Geo.FaceDom[i], 2, "%d");
    }

  fprintf (file, "== Polys =================\n");

  fprintf (file, "[id] CenterCoo =\n");
  if (Geo.CenterCoo == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.PolyQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_fprintf (file, Geo.CenterCoo[i], 3, "%f");
    }

  fprintf (file, "[id] PolyFaceQty then PolyFaceNb =\n");
  if (Geo.PolyFaceQty == NULL || Geo.PolyFaceNb == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.PolyQty; i++)
    {
      fprintf (file, "%d %d\n", i, Geo.PolyFaceQty[i]);
      ut_array_1d_int_fprintf (file, Geo.PolyFaceNb[i] + 1, Geo.PolyFaceQty[i], "%d");
    }

  fprintf (file, "[id] PolyFaceQty then PolyFaceOri =\n");
  if (Geo.PolyFaceQty == NULL || Geo.PolyFaceOri == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.PolyQty; i++)
    {
      fprintf (file, "%d %d\n", i, Geo.PolyFaceQty[i]);
      ut_array_1d_int_fprintf (file, Geo.PolyFaceOri[i] + 1, Geo.PolyFaceQty[i], "%d");
    }

  fprintf (file, "[id] PolyTrue =\n");
  if (Geo.PolyTrue == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.PolyQty; i++)
    {
      fprintf (file, "%d %d\n", i, Geo.PolyTrue[i]);
    }

  fprintf (file, "[id] PolyBody =\n");
  if (Geo.PolyBody == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Geo.PolyQty; i++)
    {
      fprintf (file, "%d %d\n", i, Geo.PolyBody[i]);
    }

  fprintf (file, "== Domain =================\n");

  // Domain
  if (Geo.DomType != NULL)
    fprintf (file, "DomType = %s\n", Geo.DomType);

  // Domain ver
  fprintf (file, "DomVerQty = %d\n", Geo.DomVerQty);
  fflush (file);

  fprintf (file, "[id] DomVerEdgeQty then DomVerEdgeNb\n");
  for (i = 1; i <= Geo.DomVerQty; i++)
  {
    fprintf (file, "%d %d ", i, Geo.DomVerEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Geo.DomVerEdgeNb[i], Geo.DomVerEdgeQty[i], "%d");
  }

  fprintf (file, "[id] DomTessVerNb\n");
  for (i = 1; i <= Geo.DomVerQty; i++)
    fprintf (file, "%d %d\n", i, Geo.DomTessVerNb[i]);

  // Domain edge
  fprintf (file, "DomEdgeQty = %d\n", Geo.DomEdgeQty);
  fflush (file);

  fprintf (file, "[id] DomEdgeVerNb\n");
  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    fprintf (file, "%d ", i);
    ut_array_1d_int_fprintf (file, Geo.DomEdgeVerNb[i], 2, "%d");
  }

  fprintf (file, "[id] DomEdgeFaceNb\n");
  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    fprintf (file, "%d ", i);
    ut_array_1d_int_fprintf (file, Geo.DomEdgeFaceNb[i], 2, "%d");
  }

  fprintf (file, "[id] DomTessEdgeQty then DomTessEdgeNb\n");
  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    fprintf (file, "%d %d ", i, Geo.DomTessEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Geo.DomTessEdgeNb[i] + 1, Geo.DomTessEdgeQty[i], "%d");
  }

  // Domain face
  fprintf (file, "DomFaceQty = %d\n", Geo.DomFaceQty);
  fflush (file);

  fprintf (file, "DomFaceEq =\n");
  if (Geo.DomFaceEq != NULL)
    for (i = 1; i <= Geo.DomFaceQty; i++)
      ut_array_1d_fprintf (file, Geo.DomFaceEq[i], 4, "%f");
  else
    printf ("is NULL\n");
  fflush (file);

  fprintf (file, "[id] DomFaceVerQty then DomFaceVerNb =\n");
  if (Geo.DomFaceVerQty != NULL && Geo.DomFaceVerNb != NULL)
    for (i = 1; i <= Geo.DomFaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Geo.DomFaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Geo.DomFaceVerNb[i] + 1, Geo.DomFaceVerQty[i], "%d");
    }
  else
    printf ("is NULL\n");
  fflush (file);

  fprintf (file, "[id] DomTessFaceQty then DomTessFaceNb =\n");
  if (Geo.DomTessFaceQty != NULL)
    for (i = 1; i <= Geo.DomFaceQty; i++)
    {
      printf ("%d %d ", i, Geo.DomTessFaceQty[i]);
      ut_array_1d_int_fprintf (file, Geo.DomTessFaceNb[i] + 1, Geo.DomTessFaceQty[i], "%d");
    }
  else
    printf ("is NULL\n");
  fflush (file);

  fprintf (file, "====== End of Geo ========================================\n");
  fflush (file);

  return;
}


void
neut_debug_germset (FILE* file, struct GERMSET GermSet)
{
  int i;

  fprintf (file, "morpho = %s\n", GermSet.morpho);
  fprintf (file, "N = %d\n", GermSet.N);
  fprintf (file, "N1d = %d\n", GermSet.N1d);
  fprintf (file, "Id = %d\n", GermSet.Id);
  fprintf (file, "Random = %ld\n", GermSet.Random);

  fprintf (file, "GermsCoo =\n");
  for (i = 1; i <= GermSet.N; i++)
    ut_array_1d_fprintf (file, GermSet.GermsCoo[i] + 1, 3, "%f");

  fprintf (file, "ttype = %s\n", GermSet.ttype);
  fprintf (file, "NDensity = %d\n", GermSet.NDensity);
  fprintf (file, "randomize = %f\n", GermSet.randomize);
  fprintf (file, "randomize2 = %d\n", GermSet.randomize2);

  return;
}

/*
void
neut_debug_geodata (FILE* file, struct GEODATA GeoData)
{
  int i;

  // Poly stuff
  fprintf (file, "polydataqty = %d\n", GeoData.polydataqty);
    
  if (GeoData.polydatatype != NULL)
  {
    fprintf (file, "polydatatype =\n");
    for (i = 0; i < GeoData.polydataqty; i++)
      fprintf (file, "%d: %s\n", i, GeoData.polydatatype[i]);
  }
  else
    fprintf (file, "polydatatype = NULL\n");
    
  if (GeoData.polydatasize != NULL)
  {
    fprintf (file, "polydatasize =\n");
    for (i = 0; i < GeoData.polydataqty; i++)
      fprintf (file, "%d: %d %d\n", i, GeoData.polydatasize[i][0], GeoData.polydatasize[i][1]);
  }
  else
    fprintf (file, "polydatasize = NULL\n");

  if (GeoData.polydata != NULL)
  {
    for (i = 0; i < GeoData.polydataqty; i++)
    {
      fprintf (file, "polydata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, GeoData.polydata[i] + 1,
			   GeoData.polydatasize[i][0], GeoData.polydatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "polydata = NULL\n");

  if (GeoData.polycol != NULL)
  {
    fprintf (file, "polycol =\n");
    ut_array_2d_int_fprintf (file, GeoData.polycol + 1,
                             GeoData.polyqty, 3, "%d");
  }
  else
    fprintf (file, "polycol = NULL\n");
 
  if (GeoData.polycolourscheme != NULL)
    fprintf (file, "polycolourscheme = %s\n", GeoData.polycolourscheme);
  else
    fprintf (file, "polycolourscheme = NULL\n");
  
  // Face stuff
  fprintf (file, "facedataqty = %d\n", GeoData.facedataqty);
    
  if (GeoData.facedatatype != NULL)
  {
    fprintf (file, "facedatatype =\n");
    for (i = 0; i < GeoData.facedataqty; i++)
      fprintf (file, "%d: %s\n", i, GeoData.facedatatype[i]);
  }
  else
    fprintf (file, "facedatatype = NULL\n");
    
  if (GeoData.facedatasize != NULL)
  {
    fprintf (file, "facedatasize =\n");
    for (i = 0; i < GeoData.facedataqty; i++)
      fprintf (file, "%d: %d %d\n", i, GeoData.facedatasize[i][0], GeoData.facedatasize[i][1]);
  }
  else
    fprintf (file, "facedatasize = NULL\n");

  if (GeoData.facedata != NULL)
  {
    for (i = 0; i < GeoData.facedataqty; i++)
    {
      fprintf (file, "facedata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, GeoData.facedata[i] + 1,
			   GeoData.facedatasize[i][0], GeoData.facedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "facedata = NULL\n");
  
  if (GeoData.facewidth != NULL)
  {
    fprintf (file, "facewidth =\n");
    ut_array_1d_fprintf (file, GeoData.facewidth + 1,
			 GeoData.faceqty, "%f");
  }
  else
    fprintf (file, "facewidth = NULL\n");

  if (GeoData.facecol != NULL)
  {
    fprintf (file, "facecol =\n");
    ut_array_2d_int_fprintf (file, GeoData.facecol + 1,
                             GeoData.faceqty, 3, "%d");
  }
  else
    fprintf (file, "facecol = NULL\n");
 
  if (GeoData.facecolourscheme != NULL)
    fprintf (file, "facecolourscheme = %s\n", GeoData.facecolourscheme);
  else
    fprintf (file, "facecolourscheme = NULL\n");
  
  // Edge stuff
  fprintf (file, "edgedataqty = %d\n", GeoData.edgedataqty);
    
  if (GeoData.edgedatatype != NULL)
  {
    fprintf (file, "edgedatatype =\n");
    for (i = 0; i < GeoData.edgedataqty; i++)
      fprintf (file, "%d: %s\n", i, GeoData.edgedatatype[i]);
  }
  else
    fprintf (file, "edgedatatype = NULL\n");
    
  if (GeoData.edgedatasize != NULL)
  {
    fprintf (file, "edgedatasize =\n");
    for (i = 0; i < GeoData.edgedataqty; i++)
      fprintf (file, "%d: %d %d\n", i, GeoData.edgedatasize[i][0], GeoData.edgedatasize[i][1]);
  }
  else
    fprintf (file, "edgedatasize = NULL\n");

  if (GeoData.edgedata != NULL)
  {
    for (i = 0; i < GeoData.edgedataqty; i++)
    {
      fprintf (file, "edgedata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, GeoData.edgedata[i] + 1,
			   GeoData.edgedatasize[i][0], GeoData.edgedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "edgedata = NULL\n");
  
  if (GeoData.edgerad != NULL)
  {
    fprintf (file, "edgerad =\n");
    ut_array_1d_fprintf (file, GeoData.edgerad + 1,
			 GeoData.edgeqty, "%f");
  }
  else
    fprintf (file, "edgerad = NULL\n");
 

  if (GeoData.edgecol != NULL)
  {
    fprintf (file, "edgecol =\n");
    ut_array_2d_int_fprintf (file, GeoData.edgecol + 1,
                             GeoData.edgeqty, 3, "%d");
  }
  else
    fprintf (file, "edgecol = NULL\n");
 
  if (GeoData.edgecolourscheme != NULL)
    fprintf (file, "edgecolourscheme = %s\n", GeoData.edgecolourscheme);
  else
    fprintf (file, "edgecolourscheme = NULL\n");
  
  // Ver stuff
  fprintf (file, "verdataqty = %d\n", GeoData.edgedataqty);
    
  if (GeoData.verdatatype != NULL)
  {
    fprintf (file, "verdatatype =\n");
    for (i = 0; i < GeoData.verdataqty; i++)
      fprintf (file, "%d: %s\n", i, GeoData.verdatatype[i]);
  }
  else
    fprintf (file, "verdatatype = NULL\n");
    
  if (GeoData.verdatasize != NULL)
  {
    fprintf (file, "verdatasize =\n");
    for (i = 0; i < GeoData.verdataqty; i++)
      fprintf (file, "%d: %d %d\n", i, GeoData.verdatasize[i][0], GeoData.edgedatasize[i][1]);
  }
  else
    fprintf (file, "verdatasize = NULL\n");

  if (GeoData.verdata != NULL)
  {
    for (i = 0; i < GeoData.verdataqty; i++)
    {
      fprintf (file, "verdata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, GeoData.verdata[i] + 1,
			   GeoData.verdatasize[i][0], GeoData.edgedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "verdata = NULL\n");
  
  if (GeoData.verrad != NULL)
  {
    fprintf (file, "verrad =\n");
    ut_array_1d_fprintf (file, GeoData.verrad + 1,
			 GeoData.verqty, "%f");
  }
  else
    fprintf (file, "verrad = NULL\n");
 

  if (GeoData.vercol != NULL)
  {
    fprintf (file, "vercol =\n");
    ut_array_2d_int_fprintf (file, GeoData.vercol + 1,
                             GeoData.verqty, 3, "%d");
  }
  else
    fprintf (file, "vercol = NULL\n");
 
  if (GeoData.vercolourscheme != NULL)
    fprintf (file, "vercolourscheme = %s\n", GeoData.edgecolourscheme);
  else
    fprintf (file, "vercolourscheme = NULL\n");
  
  return;
}
*/

void
neut_debug_poly (FILE* file, struct POLY Poly)
{
  int i;

  fprintf (file, "====== Beginning of Poly ================================\n");

  fprintf (file, "VerQty = %d\n", Poly.VerQty);
  fprintf (file, "FaceQty = %d\n", Poly.FaceQty);

  fprintf (file, "VerFace = \n");
  ut_array_2d_int_fprintf (file, Poly.VerFace + 1, Poly.VerQty, 3, "%d");
  
  fprintf (file, "VerCoo = \n");
  ut_array_2d_fprintf (file, Poly.VerCoo + 1, Poly.VerQty, 3, "%f");

  fprintf (file, "FacePoly = \n");
  ut_array_1d_int_fprintf (file, Poly.FacePoly + 1, Poly.FaceQty, "%d");

  fprintf (file, "FaceEq = \n");
  ut_array_2d_fprintf (file, Poly.FaceEq + 1, Poly.FaceQty, 4, "%f");

  fprintf (file, "FaceVerQty: FaceVerNb = \n");
  for (i = 1; i <= Poly.FaceQty; i++)
  {
    printf ("%d: ", Poly.FaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Poly.FaceVerNb[i] + 1, Poly.FaceVerQty[i], "%d");
  }

  fprintf (file, "====== End of Poly ======================================\n");

  return;
}


void
neut_debug_polymod (FILE* file, struct POLYMOD Polymod)
{
  int i;

  fprintf (file, "====== Beginning of Polymod =============================\n");

  fprintf (file, "VerQty = %d\n", Polymod.VerQty);
  fprintf (file, "FaceQty = %d\n", Polymod.FaceQty);

  fprintf (file, "VerUse = \n");
  ut_array_1d_int_fprintf (file, Polymod.VerUse + 1, Polymod.VerQty, "%d");
  
  fprintf (file, "VerCoo = \n");
  ut_array_2d_fprintf (file, Polymod.VerCoo + 1, Polymod.VerQty, 3, "%f");

  fprintf (file, "VerFace = \n");
  ut_array_2d_int_fprintf (file, Polymod.VerFace + 1, Polymod.VerQty, 3, "%d");

  fprintf (file, "FaceUse = \n");
  ut_array_1d_int_fprintf (file, Polymod.FaceUse + 1, Polymod.FaceQty, "%d");

  fprintf (file, "FacePoly = \n");
  ut_array_1d_int_fprintf (file, Polymod.FacePoly + 1, Polymod.FaceQty, "%d");

  fprintf (file, "FaceEq = \n");
  ut_array_2d_fprintf (file, Polymod.FaceEq + 1, Polymod.FaceQty, 4, "%f");

  fprintf (file, "FaceVerQty: FaceVerNb = \n");
  for (i = 1; i <= Polymod.FaceQty; i++)
  {
    printf ("%d: ", Polymod.FaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Polymod.FaceVerNb[i] + 1, Polymod.FaceVerQty[i], "%d");
  }

  fprintf (file, "====== End of Polymod ===================================\n");

  return;
}
