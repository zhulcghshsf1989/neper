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
neut_debug_nodes_name (char* filename, struct NODES Nodes)
{
  FILE* file = ut_file_open (filename, "w");
  neut_debug_nodes (file, Nodes);
  ut_file_close (file, filename, "w");

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
  fprintf (file, "EltType = %s\n", Mesh.EltType);
  fprintf (file, "EltQty = %d\n", Mesh.EltQty);
  if (Mesh.EltQty > 0)
  {
    fprintf (file, "EltNodes = \n");
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Mesh.EltNodes[i], eltnodeqty, "%d");
    }
    if (Mesh.EltElset != NULL)
      fprintf (file, "EltElset = \n");
    for (i = 1; i <= Mesh.EltQty; i++)
      fprintf (stdout, "%d\n", Mesh.EltElset[i]);
    fprintf (file, "ElsetQty = %d\n", Mesh.ElsetQty);
    fprintf (file, "Elsets = (quantity then ids of elements)\n");
    for (i = 1; i <= Mesh.ElsetQty; i++)
      ut_array_1d_int_fprintf (file, Mesh.Elsets[i], Mesh.Elsets[i][0] + 1, "%d");
  }

  fprintf (file, "NodeElts (elt qty then ids) = \n");
  if (Mesh.NodeElts == NULL)
    fprintf (file, "Mesh.NodeElts is NULL\n");
  else
  {
    fprintf (file, "Mesh.NodeQty = %d\n", Mesh.NodeQty);
    fprintf (file, "qty then ids\n");
    for (i = 1; i <=  Mesh.NodeQty; i++)
      ut_array_1d_int_fprintf (file, Mesh.NodeElts[i], Mesh.NodeElts[i][0] + 1, "%d");
  }

  fprintf (file, "====== End of Mesh =======================================\n");
  return;
}

void
neut_debug_mesh_name (char* filename, struct MESH Mesh)
{
  FILE* file = ut_file_open (filename, "w");
  neut_debug_mesh (file, Mesh);
  ut_file_close (file, filename, "w");

  return;
}

void
neut_debug_nset (FILE* file, struct NSET NSet)
{
  int i;

  fprintf (file, "====== Beginning of NSet =================================\n");
  fprintf (file, "NSet:\n");
  fprintf (file, "qty = %d\n", NSet.qty);

  fprintf (file, "names = \n");
  for (i = 1; i <= NSet.qty; i++)
    printf ("%s\n", NSet.names[i]);

  fprintf (file, "nodeqty = \n");
  for (i = 1; i <= NSet.qty; i++)
    printf ("%d\n", NSet.nodeqty[i]);

  fprintf (file, "nodes = \n");
  for (i = 1; i <= NSet.qty; i++)
    ut_array_1d_int_fprintf (stdout, NSet.nodes[i], NSet.nodeqty[i], "%d");

  fprintf (file, "====== End of NSet =======================================\n");
  return;
}

void
neut_debug_nset_name (char* filename, struct NSET NSet)
{
  FILE* file = ut_file_open (filename, "w");
  neut_debug_nset (file, NSet);
  ut_file_close (file, filename, "w");

  return;
}

void
neut_debug_tess (FILE* file, struct TESS Tess)
{
  int i;

  fprintf (file, "====== Beginning of Tess ==================================\n");
  fprintf (file, "N = %d\n", Tess.N);
  fprintf (file, "Id = %d\n", Tess.Id);

  if (Tess.morpho != NULL)
    fprintf (file, "morpho = %s\n", Tess.morpho);
  else
    fprintf (file, "morpho = NULL\n");

  fprintf (file, "Type = %s\n", Tess.Type);

  fprintf (file, "VerQty = %d\n", Tess.VerQty);
  fprintf (file, "EdgeQty = %d\n", Tess.EdgeQty);
  fprintf (file, "FaceQty = %d\n", Tess.FaceQty);
  fprintf (file, "PolyQty = %d\n", Tess.PolyQty);

  fprintf (file, "== Vertices =================\n");
  fprintf (file, "[id] VerCoo =\n");
  if (Tess.VerCoo == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.VerQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_fprintf (file, Tess.VerCoo[i], 3, "%f");
    }

  fprintf (file, "[id] VerEdgeQty then VerEdgeNb =\n");
  if (Tess.VerEdgeQty == NULL || Tess.VerEdgeNb == NULL)
    fprintf (file, "one is NULL\n");
  else 
    for (i = 1; i <= Tess.VerQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.VerEdgeQty[i]);
      ut_array_1d_int_fprintf (file, Tess.VerEdgeNb[i], Tess.VerEdgeQty[i], "%d");
    }

  fprintf (file, "[id] VerDom =\n");
  if (Tess.VerDom == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.VerQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Tess.VerDom[i], 2, "%d");
    }
  
  fprintf (file, "[id] VerState =\n");
  if (Tess.VerState == NULL)
    fprintf (file, "is NULL\n");
  else
  for (i = 1; i <= Tess.VerQty; i++)
    fprintf (file, "%d %d\n", i, Tess.VerState[i]);

  fprintf (file, "== Edges =================\n");

  fprintf (file, "[id] EdgeVerNb =\n");
  if (Tess.EdgeVerNb == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.EdgeQty; i++)
      fprintf (file, "%d %d %d\n", i, Tess.EdgeVerNb[i][0], Tess.EdgeVerNb[i][1]);

  fprintf (file, "[id] EdgeFaceQty then EdgeFaceNb =\n");
  if (Tess.EdgeFaceQty == NULL || Tess.EdgeFaceNb == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.EdgeQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.EdgeFaceQty[i]);
      ut_array_1d_int_fprintf (file, Tess.EdgeFaceNb[i], Tess.EdgeFaceQty[i], "%d");
    }

  fprintf (file, "[id] EdgeLength =\n");
  if (Tess.EdgeLength == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.EdgeQty; i++)
      fprintf (file, "%d %f\n", i, Tess.EdgeLength[i]);

  fprintf (file, "[id] EdgeState =\n");
  if (Tess.EdgeState == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.EdgeQty; i++)
      fprintf (file, "%d %d\n", i, Tess.EdgeState[i]);

  fprintf (file, "[id] EdgeDel =\n");
  if (Tess.EdgeDel == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.EdgeQty; i++)
      fprintf (file, "%d %d\n", i, Tess.EdgeDel[i]);

  fprintf (file, "[id] EdgeDom =\n");
  if (Tess.EdgeDom == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.EdgeQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Tess.EdgeDom[i], 2, "%d");
    }

  fprintf (file, "== Faces =================\n");

  fprintf (file, "[id] FacePoly =\n");
  if (Tess.FacePoly == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
      fprintf (file, "%d %d %d\n", i, Tess.FacePoly[i][0], Tess.FacePoly[i][1]);

  fprintf (file, "[id] FaceVerQty then FaceVerNb =\n");
  if (Tess.FaceVerQty == NULL || Tess.FaceVerNb == NULL)
    fprintf (file, "one is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.FaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Tess.FaceVerNb[i] + 1, Tess.FaceVerQty[i], "%d");
    }

  fprintf (file, "[id] FaceEdgeQty then FaceEdgeNb =\n");
  if (Tess.FaceVerQty == NULL || Tess.FaceEdgeNb == NULL)
    fprintf (file, "one is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.FaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Tess.FaceEdgeNb[i] + 1, Tess.FaceVerQty[i], "%d");
    }

  fprintf (file, "[id] FaceEdgeQty then Ori =\n");
  if (Tess.FaceVerQty == NULL || Tess.FaceEdgeOri == NULL)
    fprintf (file, "one is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.FaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Tess.FaceEdgeOri[i] + 1, Tess.FaceVerQty[i], "%d");
    }

  fprintf (file, "[id] FaceEq =\n");
  if (Tess.FaceEq == NULL)
    fprintf (file, "is NULL\n");
  else
  for (i = 1; i <= Tess.FaceQty; i++)
    ut_array_1d_fprintf (file, Tess.FaceEq[i], 4, "%f");

  fprintf (file, "[id] FaceState =\n");
  if (Tess.FaceState == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
      fprintf (file, "%d %d\n", i, Tess.FaceState[i]);

  fprintf (file, "[id] FacePt =\n");
  if (Tess.FacePt == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
      fprintf (file, "%d %d\n", i, Tess.FacePt[i]);

  fprintf (file, "[id] FacePtCoo =\n");
  if (Tess.FacePtCoo == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_fprintf (file, Tess.FacePtCoo[i], 3, "%f");
    }

  fprintf (file, "[id] FaceFF =\n");
  if (Tess.FaceFF == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
      fprintf (file, "%d %f\n", i, Tess.FaceFF[i]);

  fprintf (file, "[id] FaceDom =\n");
  if (Tess.FaceDom == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_int_fprintf (file, Tess.FaceDom[i], 2, "%d");
    }

  fprintf (file, "== Polys =================\n");

  fprintf (file, "[id] CenterCoo =\n");
  if (Tess.CenterCoo == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      fprintf (file, "%d ", i);
      ut_array_1d_fprintf (file, Tess.CenterCoo[i], 3, "%f");
    }

  fprintf (file, "[id] PolyFaceQty then PolyFaceNb =\n");
  if (Tess.PolyFaceQty == NULL || Tess.PolyFaceNb == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      fprintf (file, "%d %d\n", i, Tess.PolyFaceQty[i]);
      ut_array_1d_int_fprintf (file, Tess.PolyFaceNb[i] + 1, Tess.PolyFaceQty[i], "%d");
    }

  fprintf (file, "[id] PolyFaceQty then PolyFaceOri =\n");
  if (Tess.PolyFaceQty == NULL || Tess.PolyFaceOri == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      fprintf (file, "%d %d\n", i, Tess.PolyFaceQty[i]);
      ut_array_1d_int_fprintf (file, Tess.PolyFaceOri[i] + 1, Tess.PolyFaceQty[i], "%d");
    }

  fprintf (file, "[id] PolyTrue =\n");
  if (Tess.PolyTrue == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      fprintf (file, "%d %d\n", i, Tess.PolyTrue[i]);
    }

  fprintf (file, "[id] PolyBody =\n");
  if (Tess.PolyBody == NULL)
    fprintf (file, "is NULL\n");
  else
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      fprintf (file, "%d %d\n", i, Tess.PolyBody[i]);
    }

  fprintf (file, "== Domain =================\n");

  // Domain
  if (Tess.DomType != NULL)
    fprintf (file, "DomType = %s\n", Tess.DomType);

  // Domain ver
  fprintf (file, "DomVerQty = %d\n", Tess.DomVerQty);
  fflush (file);

  fprintf (file, "[id] DomVerEdgeQty then DomVerEdgeNb\n");
  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    fprintf (file, "%d %d ", i, Tess.DomVerEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Tess.DomVerEdgeNb[i], Tess.DomVerEdgeQty[i], "%d");
  }

  fprintf (file, "[id] DomTessVerNb\n");
  for (i = 1; i <= Tess.DomVerQty; i++)
    fprintf (file, "%d %d\n", i, Tess.DomTessVerNb[i]);

  // Domain edge
  fprintf (file, "DomEdgeQty = %d\n", Tess.DomEdgeQty);
  fflush (file);

  fprintf (file, "[id] DomEdgeVerNb\n");
  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    fprintf (file, "%d ", i);
    ut_array_1d_int_fprintf (file, Tess.DomEdgeVerNb[i], 2, "%d");
  }

  fprintf (file, "[id] DomEdgeFaceNb\n");
  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    fprintf (file, "%d ", i);
    ut_array_1d_int_fprintf (file, Tess.DomEdgeFaceNb[i], 2, "%d");
  }

  fprintf (file, "[id] DomTessEdgeQty then DomTessEdgeNb\n");
  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    fprintf (file, "%d %d ", i, Tess.DomTessEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Tess.DomTessEdgeNb[i] + 1, Tess.DomTessEdgeQty[i], "%d");
  }

  // Domain face
  fprintf (file, "DomFaceQty = %d\n", Tess.DomFaceQty);
  fflush (file);

  fprintf (file, "DomFaceEq =\n");
  if (Tess.DomFaceEq != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
      ut_array_1d_fprintf (file, Tess.DomFaceEq[i], 4, "%f");
  else
    printf ("is NULL\n");
  fflush (file);

  fprintf (file, "[id] DomFaceVerQty then DomFaceVerNb =\n");
  if (Tess.DomFaceVerQty != NULL && Tess.DomFaceVerNb != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      fprintf (file, "%d %d ", i, Tess.DomFaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomFaceVerNb[i] + 1, Tess.DomFaceVerQty[i], "%d");
    }
  else
    printf ("is NULL\n");
  fflush (file);

  fprintf (file, "[id] DomTessFaceQty then DomTessFaceNb =\n");
  if (Tess.DomTessFaceQty != NULL)
    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      printf ("%d %d ", i, Tess.DomTessFaceQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomTessFaceNb[i] + 1, Tess.DomTessFaceQty[i], "%d");
    }
  else
    printf ("is NULL\n");
  fflush (file);

  fprintf (file, "====== End of Tess ========================================\n");
  fflush (file);

  return;
}

void
neut_debug_tess_name (char* filename, struct TESS Tess)
{
  FILE* file = ut_file_open (filename, "w");
  neut_debug_tess (file, Tess);
  ut_file_close (file, filename, "w");

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

  fprintf (file, "GermCoo =\n");
  for (i = 1; i <= GermSet.N; i++)
    ut_array_1d_fprintf (file, GermSet.GermCoo[i], 3, "%f");

  fprintf (file, "NDensity = %d\n", GermSet.NDensity);

  return;
}

void
neut_debug_germset_name (char* filename, struct GERMSET GSet)
{
  FILE* file = ut_file_open (filename, "w");
  neut_debug_germset (file, GSet);
  ut_file_close (file, filename, "w");

  return;
}

/*
void
neut_debug_tessdata (FILE* file, struct TESSDATA TessData)
{
  int i;

  // Poly stuff
  fprintf (file, "polydataqty = %d\n", TessData.polydataqty);
    
  if (TessData.polydatatype != NULL)
  {
    fprintf (file, "polydatatype =\n");
    for (i = 0; i < TessData.polydataqty; i++)
      fprintf (file, "%d: %s\n", i, TessData.polydatatype[i]);
  }
  else
    fprintf (file, "polydatatype = NULL\n");
    
  if (TessData.polydatasize != NULL)
  {
    fprintf (file, "polydatasize =\n");
    for (i = 0; i < TessData.polydataqty; i++)
      fprintf (file, "%d: %d %d\n", i, TessData.polydatasize[i][0], TessData.polydatasize[i][1]);
  }
  else
    fprintf (file, "polydatasize = NULL\n");

  if (TessData.polydata != NULL)
  {
    for (i = 0; i < TessData.polydataqty; i++)
    {
      fprintf (file, "polydata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, TessData.polydata[i] + 1,
			   TessData.polydatasize[i][0], TessData.polydatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "polydata = NULL\n");

  if (TessData.polycol != NULL)
  {
    fprintf (file, "polycol =\n");
    ut_array_2d_int_fprintf (file, TessData.polycol + 1,
                             TessData.polyqty, 3, "%d");
  }
  else
    fprintf (file, "polycol = NULL\n");
 
  if (TessData.polycolourscheme != NULL)
    fprintf (file, "polycolourscheme = %s\n", TessData.polycolourscheme);
  else
    fprintf (file, "polycolourscheme = NULL\n");
  
  // Face stuff
  fprintf (file, "facedataqty = %d\n", TessData.facedataqty);
    
  if (TessData.facedatatype != NULL)
  {
    fprintf (file, "facedatatype =\n");
    for (i = 0; i < TessData.facedataqty; i++)
      fprintf (file, "%d: %s\n", i, TessData.facedatatype[i]);
  }
  else
    fprintf (file, "facedatatype = NULL\n");
    
  if (TessData.facedatasize != NULL)
  {
    fprintf (file, "facedatasize =\n");
    for (i = 0; i < TessData.facedataqty; i++)
      fprintf (file, "%d: %d %d\n", i, TessData.facedatasize[i][0], TessData.facedatasize[i][1]);
  }
  else
    fprintf (file, "facedatasize = NULL\n");

  if (TessData.facedata != NULL)
  {
    for (i = 0; i < TessData.facedataqty; i++)
    {
      fprintf (file, "facedata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, TessData.facedata[i] + 1,
			   TessData.facedatasize[i][0], TessData.facedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "facedata = NULL\n");
  
  if (TessData.facewidth != NULL)
  {
    fprintf (file, "facewidth =\n");
    ut_array_1d_fprintf (file, TessData.facewidth + 1,
			 TessData.faceqty, "%f");
  }
  else
    fprintf (file, "facewidth = NULL\n");

  if (TessData.facecol != NULL)
  {
    fprintf (file, "facecol =\n");
    ut_array_2d_int_fprintf (file, TessData.facecol + 1,
                             TessData.faceqty, 3, "%d");
  }
  else
    fprintf (file, "facecol = NULL\n");
 
  if (TessData.facecolourscheme != NULL)
    fprintf (file, "facecolourscheme = %s\n", TessData.facecolourscheme);
  else
    fprintf (file, "facecolourscheme = NULL\n");
  
  // Edge stuff
  fprintf (file, "edgedataqty = %d\n", TessData.edgedataqty);
    
  if (TessData.edgedatatype != NULL)
  {
    fprintf (file, "edgedatatype =\n");
    for (i = 0; i < TessData.edgedataqty; i++)
      fprintf (file, "%d: %s\n", i, TessData.edgedatatype[i]);
  }
  else
    fprintf (file, "edgedatatype = NULL\n");
    
  if (TessData.edgedatasize != NULL)
  {
    fprintf (file, "edgedatasize =\n");
    for (i = 0; i < TessData.edgedataqty; i++)
      fprintf (file, "%d: %d %d\n", i, TessData.edgedatasize[i][0], TessData.edgedatasize[i][1]);
  }
  else
    fprintf (file, "edgedatasize = NULL\n");

  if (TessData.edgedata != NULL)
  {
    for (i = 0; i < TessData.edgedataqty; i++)
    {
      fprintf (file, "edgedata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, TessData.edgedata[i] + 1,
			   TessData.edgedatasize[i][0], TessData.edgedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "edgedata = NULL\n");
  
  if (TessData.edgerad != NULL)
  {
    fprintf (file, "edgerad =\n");
    ut_array_1d_fprintf (file, TessData.edgerad + 1,
			 TessData.edgeqty, "%f");
  }
  else
    fprintf (file, "edgerad = NULL\n");
 

  if (TessData.edgecol != NULL)
  {
    fprintf (file, "edgecol =\n");
    ut_array_2d_int_fprintf (file, TessData.edgecol + 1,
                             TessData.edgeqty, 3, "%d");
  }
  else
    fprintf (file, "edgecol = NULL\n");
 
  if (TessData.edgecolourscheme != NULL)
    fprintf (file, "edgecolourscheme = %s\n", TessData.edgecolourscheme);
  else
    fprintf (file, "edgecolourscheme = NULL\n");
  
  // Ver stuff
  fprintf (file, "verdataqty = %d\n", TessData.edgedataqty);
    
  if (TessData.verdatatype != NULL)
  {
    fprintf (file, "verdatatype =\n");
    for (i = 0; i < TessData.verdataqty; i++)
      fprintf (file, "%d: %s\n", i, TessData.verdatatype[i]);
  }
  else
    fprintf (file, "verdatatype = NULL\n");
    
  if (TessData.verdatasize != NULL)
  {
    fprintf (file, "verdatasize =\n");
    for (i = 0; i < TessData.verdataqty; i++)
      fprintf (file, "%d: %d %d\n", i, TessData.verdatasize[i][0], TessData.edgedatasize[i][1]);
  }
  else
    fprintf (file, "verdatasize = NULL\n");

  if (TessData.verdata != NULL)
  {
    for (i = 0; i < TessData.verdataqty; i++)
    {
      fprintf (file, "verdata =\n");
      fprintf (file, "%d:\n", i);
      ut_array_2d_fprintf (file, TessData.verdata[i] + 1,
			   TessData.verdatasize[i][0], TessData.edgedatasize[i][1], "%f");
    }
  }
  else
    fprintf (file, "verdata = NULL\n");
  
  if (TessData.verrad != NULL)
  {
    fprintf (file, "verrad =\n");
    ut_array_1d_fprintf (file, TessData.verrad + 1,
			 TessData.verqty, "%f");
  }
  else
    fprintf (file, "verrad = NULL\n");
 

  if (TessData.vercol != NULL)
  {
    fprintf (file, "vercol =\n");
    ut_array_2d_int_fprintf (file, TessData.vercol + 1,
                             TessData.verqty, 3, "%d");
  }
  else
    fprintf (file, "vercol = NULL\n");
 
  if (TessData.vercolourscheme != NULL)
    fprintf (file, "vercolourscheme = %s\n", TessData.edgecolourscheme);
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
    fprintf (file, "%d: ", Poly.FaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Poly.FaceVerNb[i] + 1, Poly.FaceVerQty[i], "%d");
  }

  fprintf (file, "====== End of Poly ======================================\n");

  return;
}

void
neut_debug_poly_name (char* filename, struct POLY Poly)
{
  FILE* file = ut_file_open (filename, "w");
  neut_debug_poly (file, Poly);
  ut_file_close (file, filename, "w");

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

void
neut_debug_polymod_name (char* filename, struct POLYMOD Polymod)
{
  FILE* file = ut_file_open (filename, "w");
  neut_debug_polymod (file, Polymod);
  ut_file_close (file, filename, "w");

  return;
}
