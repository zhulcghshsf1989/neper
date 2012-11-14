/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_show.h"
#include<unistd.h>

void
nevs_show (char **argv, int *pi, struct GEO Geo, struct NODES Nodes,
	   struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
	   struct MESH Mesh3D, struct PRINT *pPrint)
{
  int i, j, k, l, m, n;
  FILE *file;
  double *coo = ut_alloc_1d (3);
  int id;
  
  if (strcmp (argv[(*pi)], "-showtess") == 0)
  {
    (*pi)++;
    sscanf (argv[(*pi)], "%d", &((*pPrint).showtess));
  }
  else if (strcmp (argv[(*pi)], "-showmesh") == 0)
  {
    (*pi)++;
    sscanf (argv[(*pi)], "%d", &((*pPrint).showmesh));
  }
  else if (strcmp (argv[(*pi)], "-showslice") == 0)
  {
    (*pi)++;
    (*pPrint).showslice = ut_alloc_1d_char (100);
    sscanf (argv[(*pi)], "%s", ((*pPrint).showslice));
  }
  else if (strcmp (argv[(*pi)], "-showpoly") == 0)
  {
    (*pi)++;
    (*pPrint).showpoly = ut_realloc_1d_int ((*pPrint).showpoly, Geo.PolyQty + 1);
    ut_array_1d_int_set ((*pPrint).showpoly + 1, Geo.PolyQty, 0);

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showpoly + 1, Geo.PolyQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).showpoly[id] = 1;
      ut_file_close (file, argv[(*pi)] + 1, "r");
    }
    else
    {
      int var_qty = 8;
      char **vars = ut_alloc_2d_char (var_qty, 15);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res = 0;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "volume");
      sprintf (vars[4], "true");
      sprintf (vars[5], "body");
      sprintf (vars[6], "id");
      sprintf (vars[7], "faceqty");

      for (i = 1; i <= Geo.PolyQty; i++)
      {
	ut_array_1d_memcpy (vals, 3, Geo.CenterCoo[i]);
	neut_geo_poly_volume (Geo, i, &(vals[3]));
	vals[4] = Geo.PolyTrue[i];
	vals[5] = Geo.PolyBody[i];
	vals[6] = i;
	vals[7] = Geo.PolyFaceQty[i];

#ifdef HAVE_LIBMATHEVAL
	status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
  abort ();
#endif
	if (status == -1)
	  abort ();
	(*pPrint).showpoly[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
    }

    (*pPrint).showpoly[0] = ut_array_1d_int_sum ((*pPrint).showpoly + 1,
						 Geo.PolyQty);

    if ((*pPrint).showverinit == 0)
    {
      char** locargv = ut_alloc_2d_char (2, 100);
      sprintf (locargv[0], "-showver");
      sprintf (locargv[1], "poly_shown");
      int loci = 0;

      nevs_show (locargv, &loci, Geo, Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, pPrint);

      ut_free_2d_char (locargv, 2);
    }
    
    if ((*pPrint).showedgeinit == 0)
    {
      char** locargv = ut_alloc_2d_char (100, 2);
      sprintf (locargv[0], "-showedge");
      sprintf (locargv[1], "poly_shown");
      int loci = 0;

      nevs_show (locargv, &loci, Geo, Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, pPrint);

      ut_free_2d_char (locargv, 2);
    }
  }
  
  else if (strcmp (argv[(*pi)], "-showshadow") == 0)
  {
    (*pi)++;
    sscanf (argv[(*pi)], "%d", &((*pPrint).showshadow));
  }

  else if (strcmp (argv[(*pi)], "-showfaceinter") == 0)
  {
    (*pi)++;
    sscanf (argv[(*pi)], "%d", &((*pPrint).showfaceinter));
  }

  else if (strcmp (argv[(*pi)], "-showface") == 0)
  {
    (*pi)++;
    (*pPrint).showface = ut_realloc_1d_int ((*pPrint).showface, Geo.FaceQty + 1);
    ut_array_1d_int_set ((*pPrint).showface + 1, Geo.FaceQty, 0);
    (*pPrint).showface[0] = -1;

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showface + 1, Geo.FaceQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).showface[id] = 1;
      ut_file_close (file, argv[(*pi) + 1], "r");
    }
    else
    {
      int var_qty = 11;
      char **vars = ut_alloc_2d_char (var_qty, 15);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "area");
      sprintf (vars[4], "true");
      sprintf (vars[5], "body");
      sprintf (vars[6], "id");
      sprintf (vars[7], "poly_shown");
      sprintf (vars[8], "ff");
      sprintf (vars[9], "edgeqty");
      sprintf (vars[10], "verqty");

      for (i = 1; i <= Geo.FaceQty; i++)
      {
	neut_geo_face_centre (Geo, i, vals);
	neut_geo_face_area   (Geo, i, &(vals[3]));
	vals[4] = neut_geo_face_polytruelevelmax (Geo, i);
	vals[5] = neut_geo_face_polybodylevelmax (Geo, i);
	vals[6] = i;
	vals[7] = 0;
	for (j = 0; j < 2; j++)
	  if (Geo.FacePoly[i][j] > 0 && (*pPrint).showpoly[Geo.FacePoly[i][j]] == 1)
	  {
	    vals[7] = 1;
	    break;
	  }
	vals[8] = Geo.FaceFF[i];
	vals[9] = Geo.FaceVerQty[i];
	vals[10] = Geo.FaceVerQty[i];

#ifdef HAVE_LIBMATHEVAL
	status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
  abort ();
#endif
	if (status == -1)
	  abort ();
	(*pPrint).showface[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
    }

    (*pPrint).showface[0] = ut_array_1d_int_sum ((*pPrint).showface + 1,
						 Geo.FaceQty);
  }
  
  else if (strcmp (argv[(*pi)], "-showedge") == 0)
  {
    (*pPrint).showedgeinit = 1;

    (*pi)++;
    (*pPrint).showedge = ut_realloc_1d_int ((*pPrint).showedge, Geo.EdgeQty + 1);
    ut_array_1d_int_set ((*pPrint).showedge + 1, Geo.EdgeQty, 0);
    (*pPrint).showedge[0] = -1;

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showedge + 1, Geo.EdgeQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).showedge[id] = 1;
      ut_file_close (file, argv[(*pi) + 1], "r");
    }
    else
    {
      int var_qty = 10;
      char **vars = ut_alloc_2d_char (var_qty, 15);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res;
      int polyqty;
      int *poly = NULL;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "length");
      sprintf (vars[4], "true");
      sprintf (vars[5], "body");
      sprintf (vars[6], "id");
      sprintf (vars[7], "poly_shown");
      sprintf (vars[8], "face_shown");
      sprintf (vars[9], "cyl");

      for (i = 1; i <= Geo.EdgeQty; i++)
      {
	neut_geo_edge_centre (Geo, i, vals);
	vals[3] = Geo.EdgeLength[i];
	vals[4] = neut_geo_edge_polytruelevelmax (Geo, Geo.PolyTrue, i);
	vals[5] = neut_geo_edge_polybodylevelmax (Geo, Geo.PolyBody, i);
	vals[6] = i;
	vals[7] = 0;
	neut_geo_edge_polys (Geo, i, &poly, &polyqty);
	for (j = 0; j < polyqty; j++)
	  if (poly[j] > 0 && (*pPrint).showpoly[poly[j]] == 1)
	  {
	    vals[7] = 1;
	    break;
	  }
	vals[8] = 0;
	for (j = 1; j <= Geo.EdgeFaceQty[i]; j++)
	  if ((*pPrint).showface[Geo.EdgeFaceNb[i][j - 1]] == 1)
	  {
	    vals[8] = 1;
	    break;
	  }

	vals[9] = 0;
	if (! strcmp (Geo.DomType, "cylinder"))
	{
	  int domedge, face, domface1, domface2, poly1, poly2;
	  if (Geo.EdgeDom[i][0] == 1)
	  {
	    domedge = Geo.EdgeDom[i][1];
	    domface1 = Geo.DomEdgeFaceNb[domedge][0];
	    domface2 = Geo.DomEdgeFaceNb[domedge][1];

	    poly1 = -1;
	    poly2 = -1;
	    for (j = 0; j < Geo.EdgeFaceQty[i]; j++)
	    {
	      face = Geo.EdgeFaceNb[i][j];
	      if (Geo.FaceDom[face][0] == 2)
	      {
		if (Geo.FaceDom[face][1] == domface1)
		  poly1 = Geo.FacePoly[face][0];
		if (Geo.FaceDom[face][1] == domface2)
		  poly2 = Geo.FacePoly[face][0];
	      }
	    }

	    if (poly1 < 0 || poly2 < 0)
	      ut_error_reportbug ();

	    if (domface1 > 2 && domface2 > 2 && poly1 == poly2)
	      vals[9] = 1;
	  }
	}

#ifdef HAVE_LIBMATHEVAL
	status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
  abort ();
#endif
	if (status == -1)
	  abort ();
	(*pPrint).showedge[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
      ut_free_1d_int (poly);
    }

    (*pPrint).showedge[0] = ut_array_1d_int_sum ((*pPrint).showedge + 1,
						 Geo.EdgeQty);
  }

  else if (strcmp (argv[(*pi)], "-showver") == 0)
  {
    (*pPrint).showverinit = 1;

    (*pi)++;
    (*pPrint).showver = ut_realloc_1d_int ((*pPrint).showver, Geo.VerQty + 1);
    ut_array_1d_int_set ((*pPrint).showver + 1, Geo.VerQty, 0);
    (*pPrint).showver[0] = -1;

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showver + 1, Geo.VerQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).showver[id] = 1;
      ut_file_close (file, argv[(*pi) + 1], "r");
    }
    else
    {
      int var_qty = 9;
      char **vars = ut_alloc_2d_char (var_qty, 15);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res;
      int faceqty, polyqty;
      int *poly = NULL;
      int *face = NULL;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "true");
      sprintf (vars[4], "body");
      sprintf (vars[5], "id");
      sprintf (vars[6], "poly_shown");
      sprintf (vars[7], "face_shown");
      sprintf (vars[8], "edge_shown");

      for (i = 1; i <= Geo.VerQty; i++)
      {
	ut_array_1d_memcpy (vals, 3, Geo.VerCoo[i]);
	vals[3] = neut_geo_ver_polytruelevelmax (Geo, Geo.PolyTrue, i);
	vals[4] = neut_geo_ver_polybodylevelmax (Geo, Geo.PolyBody, i);
	vals[5] = i;
	vals[6] = 0;
	neut_geo_ver_polys (Geo, i, &poly, &polyqty);
	for (j = 0; j < polyqty; j++)
	{
	  if (poly[j] > 0 && (*pPrint).showpoly[poly[j]] == 1)
	  {
	    vals[6] = 1;
	    break;
	  }
	}
	vals[7] = 0;
	neut_geo_ver_faces (Geo, i, &face, &faceqty);
	for (j = 0; j < faceqty; j++)
	  if ((*pPrint).showface[face[j]] == 1)
	  {
	    vals[7] = 1;
	    break;
	  }
	vals[8] = 0;
	for (j = 0; j < Geo.VerEdgeQty[i]; j++)
	  if ((*pPrint).showedge[Geo.VerEdgeNb[i][j]] == 1)
	  {
	    vals[8] = 1;
	    break;
	  }

#ifdef HAVE_LIBMATHEVAL
	status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
  abort ();
#endif
	if (status == -1)
	  abort ();
	(*pPrint).showver[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
      ut_free_1d_int (poly);
      ut_free_1d_int (face);
    }

    (*pPrint).showver[0] = ut_array_1d_int_sum ((*pPrint).showver + 1,
						 Geo.VerQty);
  }

  // show elements -----------------------------------------------------
  else if (strcmp (argv[(*pi)], "-showelt") == 0)
  {
    (*pi)++;
    (*pPrint).showelt = ut_realloc_1d_int ((*pPrint).showelt, Mesh3D.EltQty + 1);
    ut_array_1d_int_set ((*pPrint).showelt + 1, Mesh3D.EltQty, 0);

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showelt + 1, Mesh3D.EltQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).showelt[id] = 1;
      ut_file_close (file, argv[(*pi) + 1], "r");
    }
    else
    {
      int var_qty = 8;
      char **vars = ut_alloc_2d_char (var_qty, 15);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "volume");
      sprintf (vars[4], "elset_true");
      sprintf (vars[5], "elset_body");
      sprintf (vars[6], "id");
      sprintf (vars[7], "elset_id");

      for (i = 1; i <= Mesh3D.EltQty; i++)
      {
	neut_mesh_eltcentre (Mesh3D, Nodes, i, vals);
	neut_mesh_elt_volume (Nodes, Mesh3D, i, &(vals[3]));
	vals[4] = Geo.PolyTrue[Mesh3D.EltElset[i]];
	vals[5] = Geo.PolyBody[Mesh3D.EltElset[i]];
	vals[6] = i;
	vals[7] = Mesh3D.EltElset[i];

	int done = 0;
	for (j = 0; j < var_qty; j++)
	  if (strcmp (argv[(*pi)], vars[j]) == 0)
	  {
	    res = vals[j];
	    done = 1;
	    status = 1;
	    break;
	  }

	if (done == 0 && strcmp (argv[(*pi)], "(-ceny+cenz)<0.4") == 0)
	{
	  res = (-vals[1]+vals[2] < 0.4) ? 1: 0;
	  done = 1;
	  status = 1;
	}

	if (done == 0)
	{
#ifdef HAVE_LIBMATHEVAL
	status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
  abort ();
#endif
	if (status == -1)
	  abort ();
	}
	(*pPrint).showelt[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
    }

    (*pPrint).showelt[0] = ut_array_1d_int_sum ((*pPrint).showelt + 1,
						Mesh3D.EltQty);
  }
  
  else if (strcmp (argv[(*pi)], "-showelt1d") == 0)
  {
    (*pi)++;
    (*pPrint).showelt1d = ut_realloc_1d_int ((*pPrint).showelt1d, Mesh1D.EltQty + 1);
    ut_array_1d_int_set ((*pPrint).showelt1d + 1, Mesh1D.EltQty, 0);

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showelt1d + 1, Mesh1D.EltQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).showelt1d[id] = 1;
      ut_file_close (file, argv[(*pi) + 1], "r");
    }
    else
    {
      int var_qty = 7;
      char **vars = ut_alloc_2d_char (var_qty, 100);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res;
      int *elt1delts3d = ut_alloc_1d_int (1000);
      int elt1delt3dqty;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "length");
      sprintf (vars[4], "id");
      sprintf (vars[5], "elt3d_shown");
      sprintf (vars[6], "cyl");

      for (i = 1; i <= Mesh1D.EltQty; i++)
      {
	neut_mesh_eltcentre (Mesh1D, Nodes, i, vals);
	neut_mesh_eltlength (Nodes, Mesh1D, i, &(vals[3]));
	vals[4] = i;

	vals[5] = 0;
	neut_mesh_elt1delts3d (Mesh1D, i, Mesh2D, Mesh3D, elt1delts3d,
			       &elt1delt3dqty);
	for (j = 0; j < elt1delt3dqty; j++)
	  if ((*pPrint).showelt[elt1delts3d[j]] == 1)
	  {
	    vals[5] = 1;
	    break;
	  }

	vals[6] = 0;
	if (! strcmp (Geo.DomType, "cylinder"))
	{
	  int edge, domedge, domface1, domface2, face, poly1, poly2;
	  edge = Mesh1D.EltElset[i];

	  if (Geo.EdgeDom[edge][0] == 1)
	  {
	    domedge = Geo.EdgeDom[edge][1];
	    domface1 = Geo.DomEdgeFaceNb[domedge][0];
	    domface2 = Geo.DomEdgeFaceNb[domedge][1];
	    
	    poly1 = -1;
	    poly2 = -1;
	    for (j = 0; j < Geo.EdgeFaceQty[edge]; j++)
	    {
	      face = Geo.EdgeFaceNb[edge][j];
	      if (Geo.FaceDom[face][0] == 2)
	      {
		if (Geo.FaceDom[face][1] == domface1)
		  poly1 = Geo.FacePoly[face][0];
		if (Geo.FaceDom[face][1] == domface2)
		  poly2 = Geo.FacePoly[face][0];
	      }
	    }

	    if (poly1 < 0 || poly2 < 0)
	      ut_error_reportbug ();

	    if (domface1 > 2 && domface2 > 2 && poly1 == poly2)
	      vals[6] = 1;
	  }
	}

	int done = 0;
	for (j = 0; j < var_qty; j++)
	  if (strcmp (argv[(*pi)], vars[j]) == 0)
	  {
	    res = vals[j];
	    done = 1;
	    status = 1;
	    break;
	  }

	if (done == 0)
	{
#ifdef HAVE_LIBMATHEVAL
	  status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
	  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
	  abort ();
#endif
	}

	if (status == -1)
	  abort ();

	(*pPrint).showelt1d[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
      ut_free_1d_int (elt1delts3d);
    }

    (*pPrint).showelt1d[0] = ut_array_1d_int_sum ((*pPrint).showelt1d + 1,
						  Mesh1D.EltQty);
  }
  
  else if (strcmp (argv[(*pi)], "-showelt2d") == 0)
  {
    (*pi)++;
    (*pPrint).showelt2d = ut_realloc_1d_int ((*pPrint).showelt2d, Mesh2D.EltQty + 1);
    ut_array_1d_int_set ((*pPrint).showelt2d + 1, Mesh2D.EltQty, 0);

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showelt2d + 1, Mesh2D.EltQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).showelt2d[id] = 1;
      ut_file_close (file, argv[(*pi) + 1], "r");
    }
    else
    {
      int var_qty = 8;
      char **vars = ut_alloc_2d_char (var_qty, 100);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res;
      int *elt2delts3d = ut_alloc_1d_int (1000);
      int elt2delt3dqty;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "length");
      sprintf (vars[4], "id");
      sprintf (vars[5], "elt3d_shown");

      for (i = 1; i <= Mesh2D.EltQty; i++)
      {
	neut_mesh_eltcentre (Mesh2D, Nodes, i, vals);
	neut_mesh_eltlength (Nodes, Mesh2D, i, &(vals[3]));
	vals[4] = i;

	vals[5] = 0;
	neut_mesh_elt2delts3d (Mesh2D, i, Mesh3D, elt2delts3d,
			       &elt2delt3dqty);
	for (j = 0; j < elt2delt3dqty; j++)
	  if ((*pPrint).showelt[elt2delts3d[j]] == 1)
	  {
	    vals[5] = 1;
	    break;
	  }

	int done = 0;
	for (j = 0; j < var_qty; j++)
	  if (strcmp (argv[(*pi)], vars[j]) == 0)
	  {
	    res = vals[j];
	    done = 1;
	    status = 1;
	    break;
	  }

	if (done == 0)
	{
#ifdef HAVE_LIBMATHEVAL 
	  status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
	  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
	  abort ();
#endif
	}

	if (status == -1)
	  abort ();

	(*pPrint).showelt2d[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
      ut_free_1d_int (elt2delts3d);
    }

    (*pPrint).showelt2d[0] = ut_array_1d_int_sum ((*pPrint).showelt2d + 1,
						  Mesh2D.EltQty);
  }

  else if (strcmp (argv[(*pi)], "-showelt0d") == 0)
  {
    (*pi)++;
    (*pPrint).showelt0d = ut_realloc_1d_int ((*pPrint).showelt0d, Mesh0D.EltQty + 1);
    ut_array_1d_int_set ((*pPrint).showelt0d + 1, Mesh0D.EltQty, 0);

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showelt0d + 1, Mesh0D.EltQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).showelt0d[id] = 1;
      ut_file_close (file, argv[(*pi) + 1], "r");
    }
    else
    {
      int var_qty = 8;
      char **vars = ut_alloc_2d_char (var_qty, 100);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res;
      int *elt0delts3d = ut_alloc_1d_int (1000);
      int elt0delt3dqty;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "length");
      sprintf (vars[4], "id");
      sprintf (vars[5], "elt3d_shown");

      for (i = 1; i <= Mesh0D.EltQty; i++)
      {
	neut_mesh_eltcentre (Mesh1D, Nodes, i, vals);
	neut_mesh_eltlength (Nodes, Mesh1D, i, &(vals[3]));
	vals[4] = i;

	vals[5] = 0;
	neut_mesh_elt0delts3d (Mesh0D, i, Mesh1D, Mesh2D, Mesh3D, elt0delts3d,
			       &elt0delt3dqty);
	for (j = 0; j < elt0delt3dqty; j++)
	  if ((*pPrint).showelt[elt0delts3d[j]] == 1)
	  {
	    vals[5] = 1;
	    break;
	  }

	int done = 0;
	for (j = 0; j < var_qty; j++)
	  if (strcmp (argv[(*pi)], vars[j]) == 0)
	  {
	    res = vals[j];
	    done = 1;
	    status = 1;
	    break;
	  }

	if (done == 0)
	{
#ifdef HAVE_LIBMATHEVAL
	  status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
	  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
	  abort ();
#endif
	}

	if (status == -1)
	  abort ();

	(*pPrint).showelt0d[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
      ut_free_1d_int (elt0delts3d);
    }

    (*pPrint).showelt0d[0] = ut_array_1d_int_sum ((*pPrint).showelt0d + 1,
						  Mesh0D.EltQty);
  }

  else if (strcmp (argv[(*pi)], "-shownode") == 0)
  {
    (*pi)++;
    (*pPrint).shownode = ut_realloc_1d_int ((*pPrint).shownode, Nodes.NodeQty + 1);
    ut_array_1d_int_set ((*pPrint).shownode + 1, Nodes.NodeQty, 0);

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).shownode + 1, Nodes.NodeQty, 1);

    else if (argv[(*pi)][0] == '@')
    {
      file = ut_file_open (argv[(*pi)] + 1, "r");
      while (fscanf (file, "%d", &id) != EOF)
	(*pPrint).shownode[id] = 1;
      ut_file_close (file, argv[(*pi) + 1], "r");
    }
    else
    {
      int var_qty = 10;
      char **vars = ut_alloc_2d_char (var_qty, 100);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res;

      sprintf (vars[0], "x");
      sprintf (vars[1], "y");
      sprintf (vars[2], "z");
      sprintf (vars[3], "elset_true");
      sprintf (vars[4], "elset_body");
      sprintf (vars[5], "id");
      sprintf (vars[6], "elt3d_shown");
      sprintf (vars[7], "elt2d_shown");
      sprintf (vars[8], "elt1d_shown");
      sprintf (vars[9], "eltdim");

      for (i = 1; i <= Nodes.NodeQty; i++)
      {
	ut_array_1d_memcpy (vals, 3, Nodes.NodeCoo[i]);
	vals[4] = 0;
	vals[5] = i;

	vals[6] = 0;
	for (j = 1; j <= Mesh3D.NodeElts[i][0]; j++)
	  if ((*pPrint).showelt[Mesh3D.NodeElts[i][j]] == 1)
	  {
	    vals[6] = 1;
	    break;
	  }

	vals[7] = 0;
	for (j = 1; j <= Mesh2D.NodeElts[i][0]; j++)
	  if ((*pPrint).showelt2d[Mesh2D.NodeElts[i][j]] == 1)
	  {
	    vals[7] = 1;
	    break;
	  }

	vals[8] = 0;
	for (j = 1; j <= Mesh1D.NodeElts[i][0]; j++)
	  if ((*pPrint).showelt1d[Mesh1D.NodeElts[i][j]] == 1)
	  {
	    vals[8] = 1;
	    break;
	  }

	vals[9] = neut_nodes_eltdim (Mesh0D, Mesh1D, Mesh2D, Mesh3D, i);

	int done = 0;
	for (j = 0; j < var_qty; j++)
	  if (strcmp (argv[(*pi)], vars[j]) == 0)
	  {
	    res = vals[j];
	    done = 1;
	    status = 1;
	    break;
	  }

	if (done == 0)
	{
#ifdef HAVE_LIBMATHEVAL 
	  status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
	  ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
	  abort ();
#endif
	}

	if (status == -1)
	  abort ();

	(*pPrint).shownode[i] = ut_num_equal (res, 1, 1e-6);
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
    }

    (*pPrint).shownode[0] = ut_array_1d_int_sum ((*pPrint).shownode + 1,
						  Nodes.NodeQty);
  }

  else if (strcmp (argv[(*pi)], "-showelt1din") == 0)
  {
    int* nelts = ut_alloc_1d_int (1000);
    int* cnodes = ut_alloc_1d_int (20);
    int cnodeqty;
    int* celts = ut_alloc_1d_int (20);
    int* nodepair = ut_alloc_1d_int (2);
    int celtqty;

    (*pPrint).showelt1dallqty = 0;
    (*pPrint).showelt1dall = ut_alloc_1d_pint (1);
    for (i = 1; i <= Mesh3D.EltQty; i++)
      if ((*pPrint).showelt[i] == 1)
      {
	neut_mesh_elt_neighelts (Mesh3D, i, nelts + 1, &(nelts[0])); 

	for (j = 1; j <= nelts[0]; j++)
	  if ((*pPrint).showelt[nelts[j]] == 1)
	    if (Mesh3D.EltElset[i] != Mesh3D.EltElset[nelts[j]])
	    {
	      cnodeqty = 0;
	      for (k = 0; k < 4; k++)
		if (ut_array_1d_int_eltpos (Mesh3D.EltNodes[i], 4,
		      Mesh3D.EltNodes[nelts[j]][k]) != -1)
		  cnodes[cnodeqty++] = Mesh3D.EltNodes[nelts[j]][k];

	      if (cnodeqty > 3 || cnodeqty < 1)
		abort ();
	      
	      for (k = 0; k < cnodeqty; k++)
	      {
		nodepair[0] = cnodes[k];
		for (l = k + 1; l < cnodeqty; l++)
		{
		  nodepair[1] = cnodes[l];
		  neut_nodes_commonelts (Mesh3D, nodepair, 2, celts, &celtqty);
		  for (m = 0; m < celtqty; m++)
		    if ((*pPrint).showelt[celts[m]] == 0)
		    {
		      int record = 1;
		      for (n = 0; n < (*pPrint).showelt1dallqty; n++)
			if ((*pPrint).showelt1dall[n][0] == nodepair[0]
			 && (*pPrint).showelt1dall[n][1] == nodepair[1])
			{
			  record = 0;
			  break;
			}

		      if (record == 1)
		      {
			(*pPrint).showelt1dallqty++;
			(*pPrint).showelt1dall =
			  ut_realloc_2d_int_addline
			  ((*pPrint).showelt1dall,
			   (*pPrint).showelt1dallqty, 2);

			(*pPrint).showelt1dall[(*pPrint).showelt1dallqty - 1][0] = nodepair[0];
			(*pPrint).showelt1dall[(*pPrint).showelt1dallqty - 1][1] = nodepair[1];
		      }
		    }
		}
	      }
	    }
      }

    ut_free_1d_int (nelts);
    ut_free_1d_int (cnodes);
    ut_free_1d_int (celts);
    ut_free_1d_int (nodepair);
  }

  // show element sets -------------------------------------------------
  else if (strcmp (argv[(*pi)], "-showelset") == 0)
  {
    (*pi)++;
    (*pPrint).showelt = ut_realloc_1d_int ((*pPrint).showelt, Mesh3D.EltQty + 1);
    ut_array_1d_int_set ((*pPrint).showelt + 1, Mesh3D.EltQty, 0);

    if (strcmp (argv[(*pi)], "all") == 0)
      ut_array_1d_int_set ((*pPrint).showelt + 1, Mesh3D.EltQty, 1);
    else if (argv[(*pi)][0] == '@')
    {
      int elsetqty = ut_file_nbwords (argv[(*pi)] + 1);

      file = ut_file_open (argv[(*pi)] + 1, "r");
      for (i = 0; i < elsetqty; i++)
      {
	fscanf (file, "%d", &id);
	if (id > Mesh3D.ElsetQty)
	{
	  ut_print_message (2, 0, "Elset id exceeds the maximum.\n");
	  abort ();
	}
	for (j = 1; j <= Mesh3D.Elsets[id][0]; j++)
	  (*pPrint).showelt[Mesh3D.Elsets[id][j]] = 1;
      }
      ut_file_close (file, argv[(*pi)] + 1, "r");
    }
    else
    {
      int var_qty = 8;
      char **vars = ut_alloc_2d_char (var_qty, 10);
      double *vals = ut_alloc_1d (var_qty);
      int status;
      double res = 0;

      sprintf (vars[0], "cenx");
      sprintf (vars[1], "ceny");
      sprintf (vars[2], "cenz");
      sprintf (vars[3], "vol");
      sprintf (vars[4], "true");
      sprintf (vars[5], "body");
      sprintf (vars[6], "id");
      sprintf (vars[7], "eltqty");

      for (i = 1; i <= Mesh3D.ElsetQty; i++)
      {
	neut_mesh_elset_centre (Nodes, Mesh3D, i, vals);
	neut_mesh_elset_volume (Nodes, Mesh3D, i, &(vals[3]));
	vals[4] = Geo.PolyTrue[i];
	vals[5] = Geo.PolyBody[i];
	vals[6] = i;
	vals[7] = Mesh3D.Elsets[i][0];

	status = -1;
	for (j = 0; j < var_qty; j++)
	  if (strcmp (argv[(*pi)], vars[j]) == 0)
	  {
	    res = vals[j];
	    status = 0;
	  }

	if (status == -1)
	{
#ifdef HAVE_LIBMATHEVAL
	status = ut_math_eval (argv[(*pi)], var_qty, vars, vals, &res);
#else
	ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
	abort ();
#endif
	}

	if (status == -1)
	  abort ();

	if (ut_num_equal (res, 1, 1e-6))
	  for (j = 1; j <= Mesh3D.Elsets[i][0]; j++)
	    (*pPrint).showelt[Mesh3D.Elsets[i][j]] = 1;
      }

      ut_free_2d_char (vars, var_qty);
      ut_free_1d (vals);
    }

    (*pPrint).showelt[0] = ut_array_1d_int_sum ((*pPrint).showelt + 1,
						Mesh3D.EltQty);
  }

  // error -------------------------------------------------------------
  /*
  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    fflush (stdout);
    abort ();
  }
  */
  
  ut_free_1d (coo);

  return;
}
