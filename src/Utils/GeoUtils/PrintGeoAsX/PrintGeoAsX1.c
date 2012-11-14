/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PrintGeoAsX.h"

void
PrintGeoStuff (char* printentity, int *list, struct GEO Geo, int**
    pPolyList, int** pFaceList, int **pEdgeList, int **pVerList)
{
  (*pPolyList) = ut_alloc_1d_int (Geo.PolyQty + 1);
  (*pFaceList) = ut_alloc_1d_int (Geo.FaceQty + 1);
  (*pEdgeList) = ut_alloc_1d_int (Geo.EdgeQty + 1);
  (*pVerList) = ut_alloc_1d_int (Geo.VerQty + 1);

  if (strcmp (printentity, "tessdomainface") == 0)
  {
    PrintGeoFaceToPlot (Geo, list, *pFaceList);
    PrintGeoEdgeToPlot (Geo, *pFaceList, *pEdgeList);
    PrintGeoVerToPlot (Geo, *pFaceList, *pVerList);
  }
  else if (strcmp (printentity, "poly") == 0)
  {
    PrintGeoFaceToPlot_poly (Geo, list, *pFaceList);
    PrintGeoEdgeToPlot (Geo, *pFaceList, *pEdgeList);
    PrintGeoVerToPlot (Geo, *pFaceList, *pVerList);
  }
  else
    abort ();
  
  return;
}

void
PrintGeoAsImg (struct GEO Geo, int* PolyList, int* FaceList, int
    *EdgeList, int *VerList, char *format, char *filename)
{
  int i, j, edge, face, ver, ver1, ver2;
  double* rgb = NULL;
  FILE *out;

  PolyList[0] = PolyList[0];

  out = ut_file_open (filename, "w");

  if (strcmp (format, "asy") == 0)
  {
    fprintf (out, "import three;\n");
    fprintf (out, "currentprojection=obliqueX;\n");
    fprintf (out, "unitsize(10cm);\n");

    for (i = 1; i <= VerList[0]; i++)
    {
      ver = VerList[i];
      fprintf (out, "triple p%d =(%f, %f, %f);\n", VerList[i],
	       Geo.VerCoo[ver][0], Geo.VerCoo[ver][1], Geo.VerCoo[ver][2]);
    }
    for (i = 1; i <= EdgeList[0]; i++)
    {
      edge = EdgeList[i];
      fprintf (out, "draw (p%d--p%d);\n", Geo.EdgeVerNb[edge][0],
	       Geo.EdgeVerNb[edge][1]);
    }
  }
  else if (strcmp (format, "mast") == 0)
  {
    fprintf (out, "****master\n");
    fprintf (out, " ***geometry\n");
    for (i = 1; i <= VerList[0]; i++)
    {
      ver = VerList[i];
      fprintf (out, "  **point p%d\n", VerList[i]);
      fprintf (out, "   *position ");
      for (j = 0; j < 3; j++)
	fprintf (out, " %.12f", Geo.VerCoo[ver][j]);
      fprintf (out, "\n");
    }
    for (i = 1; i <= EdgeList[0]; i++)
    {
      edge = EdgeList[i];
      fprintf (out, "  **line l%d\n", edge);
      fprintf (out, "   *p1 p%d\n", Geo.EdgeVerNb[edge][0]);
      fprintf (out, "   *p2 p%d\n", Geo.EdgeVerNb[edge][1]);
    }

    fprintf (out, " ***plotting\n");
    fprintf (out, "  **default_options 1 1 1\n");
    fprintf (out, " ***lines3d\n");
    fprintf (out, "  **enable 0\n");
    fprintf (out, "****return\n");
  }
  else if (strcmp (format, "gnuplot") == 0)
  {
    for (i = 1; i <= EdgeList[0]; i++)
    {
      edge = EdgeList[i];
      ver1 = Geo.EdgeVerNb[edge][0];
      ver2 = Geo.EdgeVerNb[edge][1];

      ut_array_1d_fprintf (out, Geo.VerCoo[ver1], 3, "%.12f");
      ut_array_1d_fprintf (out, Geo.VerCoo[ver2], 3, "%.12f");
      fprintf (out, "\n");
    }
    /* residue for gnuplot */
    fprintf (out, "%.12f %.12f %.12f\n", 0., 0., 0.);
  }
  else if (strcmp (format, "povray") == 0)
  {
    rgb = ut_alloc_1d (3);
    srand48 (1);

    fprintf (out, "background {color rgb<1,1,1>}\n\n");

    fprintf (out, "#declare dist = 1.6;\n");
    fprintf (out, "camera {\n");
    fprintf (out, "location <1 * dist, 0.75 * dist, 0.75 * dist>\n");
    fprintf (out, "look_at <0, 0, 0>\n");
    fprintf (out, "                   }\n");
    fprintf (out, "light_source { <10, 10, 10> color rgb<1, 1, 1> }\n");

    for (i = 1; i <= FaceList[0]; i++)
    {
      face = FaceList[i];
      rgb[0] = drand48 ();
      rgb[1] = drand48 ();
      rgb[2] = drand48 ();

      fprintf (out,"#declare face%d = texture {\n", face);
      fprintf (out,"    pigment { color rgb<%.12f, %.12f, %.12f> }\n", rgb[0],rgb[1],rgb[2]);
      fprintf (out,"    finish { ambient 0.2 diffuse 0.5 }\n");
      fprintf (out,"    }\n");
    }

    for (i = 1; i <= FaceList[0]; i++)
    {
      face = FaceList[i];
      fprintf (out,"polygon {\n");
      fprintf (out,"%d,\n", Geo.FaceVerQty[face]);
      for (j = 1; j <= Geo.FaceVerQty[face]; j++)
      {
	ver = Geo.FaceVerNb[face][j];
	fprintf (out,"<%.12f,%.12f,%.12f>", Geo.VerCoo[ver][0], Geo.VerCoo[ver][1], Geo.VerCoo[ver][2]);
	if (j < Geo.FaceVerQty[face])
	  fprintf (out,", ");
      }
      fprintf (out,"texture { face%d }\n", face);
      fprintf (out,"}\n");
    }
    
    ut_free_1d (rgb);
  }

  ut_file_close (out, filename, "w");

  return;
}
