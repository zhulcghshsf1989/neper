/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_print_tess.h"

void
nevs_print_tess (FILE* file, struct PRINT Print, struct GEO Geo, struct GEODATA GeoData)
{
  int i, j;
  double** coo = NULL;
  int* polylist = NULL;
  int** polyfacelist = NULL;
  int* facelist = NULL;
  int* edgelist = NULL;
  int* verlist = NULL;
  int neighqty;
  int* neigh = NULL;
  int poly, allneigh, show, facepoly, face, edge, ver;
  char* texture = ut_alloc_1d_char (100);

  int poly_id = 0;
  int face_id = 1;
  int edge_id = 2;
  int ver_id  = 3;

  // Writing pov file --------------------------------------------------
  // nevs_print_tess_header (file, GeoData);

  // compressing poly to print
  polylist = ut_alloc_1d_int (Geo.PolyQty + 1);

  if (Print.showpoly[0] > 0)
    for (i = 1; i <= Geo.PolyQty; i++)
      if (Print.showpoly[i] == 1)
      {
	neut_geo_poly_neighpoly (Geo, i, &neigh, &neighqty);
	
	if (neighqty < Geo.PolyFaceQty[i])
	  polylist[++polylist[0]] = i;
	else
	{
	  allneigh = 1;
	  for (j = 0; j < neighqty; j++)
	    if (Print.showpoly[neigh[j]] == 0)
	    {
	      allneigh = 0;
	      break;
	    }
	  if (allneigh == 0)
	    polylist[++polylist[0]] = i;
	}
      }

  // from the polylist, determining the face list
  int polyfaceqty = 0;
  polyfacelist = ut_alloc_2d_int (Geo.FaceQty + 1, 2);

  for (i = 1; i <= Geo.FaceQty; i++)
  {
    show = 0;
    facepoly = 0;
    for (j = 0; j < 2; j++)
      if (Geo.FacePoly[i][j] > 0 && ut_array_1d_int_eltpos
		    (polylist + 1, polylist[0], Geo.FacePoly[i][j]) != -1)
      {
	show++;
	facepoly = Geo.FacePoly[i][j];
      }

    if (show == 1)
    {
      polyfaceqty++;
      polyfacelist[polyfaceqty - 1][0] = i;
      polyfacelist[polyfaceqty - 1][1] = facepoly;
    }
  }

  verlist = ut_alloc_1d_int (Geo.EdgeQty + 1);
  
  for (i = 1; i <= Geo.VerQty; i++)
    if (Print.showver[i] == 1)
      verlist[++verlist[0]] = i;

  edgelist = ut_alloc_1d_int (Geo.EdgeQty + 1);
  
  for (i = 1; i <= Geo.EdgeQty; i++)
    if (Print.showedge[i] == 1)
      edgelist[++edgelist[0]] = i;

  facelist = ut_alloc_1d_int (Geo.FaceQty + 1);
  
  for (i = 1; i <= Geo.FaceQty; i++)
    if (Print.showface[i] == 1)
      facelist[++facelist[0]] = i;

  double* p = ut_alloc_1d (3);
  double* p2 = NULL;
  double ambient = (Print.showshadow == 1) ? 0.6 : 1;

  // Writing poly faces
  for (i = 0; i < polyfaceqty; i++)
  {
    face = polyfacelist[i][0];
    if (ut_array_1d_int_eltpos (facelist + 1, facelist[0], face) != -1)
      continue;

    poly = polyfacelist[i][1];

    fprintf (file,
 "#declare grain%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
      poly,
      GeoData.col[poly_id][poly][0] / 255., \
      GeoData.col[poly_id][poly][1] / 255., \
      GeoData.col[poly_id][poly][2] / 255.,
      ambient);

    sprintf (texture, "grain%d", poly);

    // vertex coordinates
    coo = ut_alloc_2d (Geo.FaceVerQty[face], 3);

    for (j = 1; j <= Geo.FaceVerQty[face]; j++)
      ut_array_1d_memcpy (coo[j - 1], 3, Geo.VerCoo[Geo.FaceVerNb[face][j]]);

    fprintf (file, "// poly face %d\n", face);

    nevs_print_polygon (file, Geo.FaceVerQty[face], coo, texture, \
			NULL, NULL, p2, NULL, NULL);

    ut_free_2d (coo, Geo.FaceVerQty[face]);
  }

  // Writing faces
  for (i = 1; i <= facelist[0]; i++)
  {
    face = facelist[i];

    fprintf (file,
 "#declare face%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
      face,
      GeoData.col[face_id][face][0] / 255., \
      GeoData.col[face_id][face][1] / 255., \
      GeoData.col[face_id][face][2] / 255., \
      ambient);

    sprintf (texture, "face%d", face);

    // vertex coordinates
    coo = ut_alloc_2d (Geo.FaceVerQty[face], 3);

    for (j = 1; j <= Geo.FaceVerQty[face]; j++)
      ut_array_1d_memcpy (coo[j - 1], 3, Geo.VerCoo[Geo.FaceVerNb[face][j]]);

    // interpolation point
    if (Print.showfaceinter == 1 && Geo.FaceState[face] > 0)
    {
      if (Geo.FacePt[face] == 0)
      {
	ut_array_1d_set (p, 3, 0);
	for (j = 0; j < Geo.FaceVerQty[face]; j++)
	  ut_array_1d_add (p, coo[j], 3, p);
	ut_array_1d_scale (p, 3, 1./Geo.FaceVerQty[face]);
      }
      else if (Geo.FacePt[face] > 0)
	ut_array_1d_memcpy (p, 3, Geo.FacePtCoo[face]);

      p2 = p;
    }
    else
      p2 = NULL;

    fprintf (file, "// face %d\n", face);

    nevs_print_polygon (file, Geo.FaceVerQty[face], coo, texture, \
			NULL, NULL, p2, NULL, NULL);

    if (Print.showfaceinter == 1 && Geo.FaceState[face] > 0)
      for (j = 0; j < Geo.FaceVerQty[face]; j++)
	if (j +1 != Geo.FacePt[face])
	  nevs_print_segment_wsph (file, p, coo[j], "faceinter_rad", "faceinter_texture");

    ut_free_2d (coo, Geo.FaceVerQty[face]);
  }

  ut_free_1d (p);

  // Writing edges
  for (i = 1; i <= edgelist[0]; i++)
  {
    edge = edgelist[i];

    fprintf (file,
 "#declare edge%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
      edge,
      GeoData.col[edge_id][edge][0] / 255., \
      GeoData.col[edge_id][edge][1] / 255., \
      GeoData.col[edge_id][edge][2] / 255., \
      ambient);
    
    sprintf (texture, "edge%d", edge);

    coo = ut_alloc_2d (2, 3);

    ut_array_1d_memcpy (coo[0], 3, Geo.VerCoo[Geo.EdgeVerNb[edge][0]]);
    ut_array_1d_memcpy (coo[1], 3, Geo.VerCoo[Geo.EdgeVerNb[edge][1]]);

    char* string = ut_alloc_1d_char (100);
    sprintf (string, "%.12f", GeoData.rad[edge_id][edge]);
    nevs_print_segment_wsph (file, coo[0], coo[1],
			  string, texture);
    ut_free_1d_char (string);

    ut_free_2d (coo, 2);
  }

  // Writing vers
  for (i = 1; i <= verlist[0]; i++)
  {
    ver = verlist[i];

    fprintf (file,
 "#declare ver%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
      ver,
      GeoData.col[ver_id][ver][0] / 255., \
      GeoData.col[ver_id][ver][1] / 255., \
      GeoData.col[ver_id][ver][2] / 255., \
      ambient);
    
    sprintf (texture, "ver%d", ver);

    char* string = ut_alloc_1d_char (100);
    sprintf (string, "%.12f", GeoData.rad[ver_id][ver]);
    nevs_print_sphere (file,  Geo.VerCoo[ver], string, texture);
    ut_free_1d_char (string);
  }

  ut_free_1d_char (texture);
  ut_free_1d_int (neigh);

  return;
}
