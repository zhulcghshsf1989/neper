/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_print_utils.h"

void
nevs_print_segment (FILE* file, double* coo1,
    double* coo2, char* edge_rad, char* edge_texture)
{
  if (edge_rad != NULL)
  {
    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo1[0], coo1[1], coo1[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo2[0], coo2[1], coo2[2]);
    fprintf (file, "              %s\n", edge_rad);

    if (edge_texture != NULL)
      fprintf (file, "    texture { %s }\n", edge_texture);
    fprintf (file, "  }\n");
  }
  
  return;
}

void
nevs_print_segment_wsph (FILE* file, double* coo1,
    double* coo2, char* edge_rad, char* edge_texture)
{
  if (edge_rad != NULL)
  {
    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo1[0], coo1[1], coo1[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo2[0], coo2[1], coo2[2]);
    fprintf (file, "              %s\n", edge_rad);

    if (edge_texture != NULL)
      fprintf (file, "    texture { %s }\n", edge_texture);
    fprintf (file, "  }\n");

    fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n",
	     coo1[0], coo1[1], coo1[2]);
    fprintf (file, "              %s\n", edge_rad);

    if (edge_texture != NULL)
      fprintf (file, "    texture { %s }\n", edge_texture);
    fprintf (file, "  }\n");

    fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n",
	     coo2[0], coo2[1], coo2[2]);
    fprintf (file, "              %s\n", edge_rad);

    if (edge_texture != NULL)
      fprintf (file, "    texture { %s }\n", edge_texture);
    fprintf (file, "  }\n");
  }
  
  return;
}

void
nevs_print_sphere (FILE* file, double* coo, char* rad, char* texture)
{
  if (rad != NULL)
  {
    fprintf (file, "    sphere {<%18.15g,%18.15g,%18.15g>,\n",
	     coo[0], coo[1], coo[2]);
    fprintf (file, "              %s\n", rad);

    if (texture != NULL)
      fprintf (file, "    texture { %s }\n", texture);
    fprintf (file, "  }\n");
  }
  
  return;
}

void
nevs_print_triangle_edge (FILE* file, double* coo1, double* coo2, double*
    coo3, char* edge_rad, char* edge_texture)
{
  if (edge_rad != NULL)
  {
    fprintf (file, "  union {\n");

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo1[0], coo1[1], coo1[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo2[0], coo2[1], coo2[2]);
    fprintf (file, "              %s }\n", edge_rad);

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo2[0], coo2[1], coo2[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo3[0], coo3[1], coo3[2]);
    fprintf (file, "              %s }\n", edge_rad);

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo3[0], coo3[1], coo3[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo1[0], coo1[1], coo1[2]);
    fprintf (file, "              %s }\n", edge_rad);

    if  (edge_texture != NULL)
      fprintf (file, "    texture { %s }\n", edge_texture);
    fprintf (file, "  }\n");
  }
  
  return;
}

void
nevs_print_mesh2d (FILE* file, struct NODES Nodes, struct MESH Mesh,
		       struct PRINT Print, int** rgb, char* coltype)
{
  int i;

  if (Nodes.NodeQty <= 0)
    return;

  fprintf (file, "mesh2 {\n");

  // vertex_vectors
  fprintf (file, "  vertex_vectors {\n");
  fprintf (file, "    %d,\n", Nodes.NodeQty);
  
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "    <%.12f,%.12f,%.12f>%c\n", Nodes.NodeCoo[i][0],
		       Nodes.NodeCoo[i][1],   Nodes.NodeCoo[i][2],
		     (i < Nodes.NodeQty) ? ',' : ' ');
  }

  fprintf (file, "  }\n");

  // texture_list
  fprintf (file, "  texture_list {\n");
  double ambient = (Print.showshadow == 1) ? 0.6 : 1;

  if (strcmp (coltype, "elt") == 0)
  {
    fprintf (file, "    %d,\n", Mesh.EltQty);

    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "    texture{pigment{rgb<%.12f,%.12f,%.12f>} finish {ambient %.1f}}\n", 
	  rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255., ambient);
    }
  }
  else if (strcmp (coltype, "node") == 0)
  {
    fprintf (file, "    %d,\n", Nodes.NodeQty);

    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      fprintf (file, "    texture{pigment{rgb<%.12f,%.12f,%.12f>} finish {ambient %.1f}}\n", 
	  rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255., ambient);
    }
  }
  else
    ut_error_reportbug ();

  fprintf (file, "  }\n");

  // face_indices
  fprintf (file, "  face_indices {\n");
  if (strcmp (Mesh.EltType, "tri") == 0)
    fprintf (file, "    %d,\n", Mesh.EltQty);
  else if (strcmp (Mesh.EltType, "quad") == 0)
    fprintf (file, "    %d,\n", Mesh.EltQty * 2);

  if (strcmp (coltype, "elt") == 0)
  {
    if (strcmp (Mesh.EltType, "tri") == 0)
      for (i = 1; i <= Mesh.EltQty; i++)
	fprintf (file, "    <%d,%d,%d>,%d%c\n", Mesh.EltNodes[i][0] - 1,
	      Mesh.EltNodes[i][1] - 1, Mesh.EltNodes[i][2] - 1,
		   i - 1, (i < Mesh.EltQty) ? ',' : ' ');
    else if (strcmp (Mesh.EltType, "quad") == 0)
      for (i = 1; i <= Mesh.EltQty; i++)
      {
	fprintf (file, "    <%d,%d,%d>,%d%c\n", Mesh.EltNodes[i][0] - 1,
	      Mesh.EltNodes[i][1] - 1, Mesh.EltNodes[i][2] - 1,
		   i - 1, (i < Mesh.EltQty) ? ',' : ',');
	fprintf (file, "    <%d,%d,%d>,%d%c\n", Mesh.EltNodes[i][0] - 1,
	      Mesh.EltNodes[i][2] - 1, Mesh.EltNodes[i][3] - 1,
		   i - 1, (i < Mesh.EltQty) ? ',' : ' ');
      }
  }
  else if (strcmp (coltype, "node") == 0)
  {
    if (strcmp (Mesh.EltType, "tri") == 0)
      for (i = 1; i <= Mesh.EltQty; i++)
	fprintf (file, "    <%d,%d,%d>,%d,%d,%d%c\n",
		  Mesh.EltNodes[i][0] - 1, Mesh.EltNodes[i][1] - 1,
		  Mesh.EltNodes[i][2] - 1, Mesh.EltNodes[i][0] - 1,
		  Mesh.EltNodes[i][1] - 1, Mesh.EltNodes[i][2] - 1,
		       (i < Mesh.EltQty) ? ',' : ' ');
    else if (strcmp (Mesh.EltType, "quad") == 0)
      for (i = 1; i <= Mesh.EltQty; i++)
      {
	fprintf (file, "    <%d,%d,%d>,%d,%d,%d%c\n",
		  Mesh.EltNodes[i][0] - 1, Mesh.EltNodes[i][1] - 1,
		  Mesh.EltNodes[i][2] - 1, Mesh.EltNodes[i][0] - 1,
		  Mesh.EltNodes[i][1] - 1, Mesh.EltNodes[i][2] - 1,
		       (i < Mesh.EltQty) ? ',' : ' ');
	fprintf (file, "    <%d,%d,%d>,%d,%d,%d%c\n",
		  Mesh.EltNodes[i][0] - 1, Mesh.EltNodes[i][2] - 1,
		  Mesh.EltNodes[i][3] - 1, Mesh.EltNodes[i][0] - 1,
		  Mesh.EltNodes[i][2] - 1, Mesh.EltNodes[i][3] - 1,
		       (i < Mesh.EltQty) ? ',' : ' ');
      }
  }
  else
    ut_error_reportbug ();

  fprintf (file, "  }\n");
  fprintf (file, "}\n");

  return;
}

void
nevs_print_triangle (FILE* file, double* coo1, double* coo2, double*
    coo3, char* texture, char* edge_rad, char* edge_texture)
{
  fprintf (file, "triangle {<%18.15g,%18.15g,%18.15g>,\n",
	   coo1[0], coo1[1], coo1[2]);
  fprintf (file, "          <%18.15g,%18.15g,%18.15g>,\n",
	   coo2[0], coo2[1], coo2[2]);
  fprintf (file, "          <%18.15g,%18.15g,%18.15g>\n",
	   coo3[0], coo3[1], coo3[2]);

  fprintf (file, "            texture { %s }\n", texture);
  fprintf (file, "  }\n");

  nevs_print_triangle_edge (file, coo1, coo2, coo3, edge_rad, edge_texture);
  
  return;
}
	
void
nevs_print_tet_edge (FILE* file, double* coo1, double* coo2, double*
    coo3, double* coo4, char* edge_rad, char* edge_texture)
{
  if (edge_rad != NULL)
  {
    fprintf (file, "  union {\n");

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo1[0], coo1[1], coo1[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo2[0], coo2[1], coo2[2]);
    fprintf (file, "              %s }\n", edge_rad);

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo2[0], coo2[1], coo2[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo3[0], coo3[1], coo3[2]);
    fprintf (file, "              %s }\n", edge_rad);

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo3[0], coo3[1], coo3[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo1[0], coo1[1], coo1[2]);
    fprintf (file, "              %s }\n", edge_rad);

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo4[0], coo4[1], coo4[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo1[0], coo1[1], coo1[2]);
    fprintf (file, "              %s }\n", edge_rad);

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo4[0], coo4[1], coo4[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo2[0], coo2[1], coo2[2]);
    fprintf (file, "              %s }\n", edge_rad);

    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo4[0], coo4[1], coo4[2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
             coo3[0], coo3[1], coo3[2]);
    fprintf (file, "              %s }\n", edge_rad);

    if  (edge_texture != NULL)
      fprintf (file, "    texture { %s }\n", edge_texture);
    fprintf (file, "  }\n");
  }
  
  return;
}
	
void
nevs_print_polygon_edge (FILE* file, int qty, double** coo,
    char* edge_rad, char* edge_texture)
{
  int i;

  if (edge_rad != NULL)
  {
    fprintf (file, "  union {\n");

    for (i = 0; i < qty - 1; i++)
    {
      fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	       coo[i][0], coo[i][1], coo[i][2]);
      fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	       coo[i + 1][0], coo[i + 1][1], coo[i + 1][2]);
      fprintf (file, "              %s }\n", edge_rad);
    }
    fprintf (file, "    cylinder {<%18.15g,%18.15g,%18.15g>,\n",
	     coo[qty - 1][0], coo[qty - 1][1], coo[qty - 1][2]);
    fprintf (file, "              <%18.15g,%18.15g,%18.15g>,\n",
	     coo[0][0], coo[0][1], coo[0][2]);
    fprintf (file, "              %s }\n", edge_rad);

    if  (edge_texture != NULL)
      fprintf (file, "    texture { %s }\n", edge_texture);
    fprintf (file, "  }\n");
  }
  
  return;
}
	
void
nevs_print_polygon (FILE* file, int qty, double** coo,
    char* texture, char* edge_rad, char* edge_texture,
    double* p, char* iedge_rad, char* iedge_texture)
{
  int i;
  double* c = ut_alloc_1d (3);

  if (p == NULL)
  {
    for (i = 0; i < qty; i++)
      ut_array_1d_add (c, coo[i], 3, c);
    ut_array_1d_scale (c, 3, 1. / qty);
  }
  else
    ut_array_1d_memcpy (c, 3, p);

  // drawing face
  for (i = 0; i < qty - 1; i++)
    nevs_print_triangle (file, c, coo[i], coo[i + 1], texture, NULL, NULL);
  nevs_print_triangle (file, c, coo[qty - 1], coo[0], texture, NULL, NULL);

  nevs_print_polygon_edge (file, qty, coo, edge_rad, edge_texture);

  // drawing segment face interpolation 
  if (iedge_rad != NULL)
    for (i = 0; i < qty; i++)
      nevs_print_segment (file, c, coo[i], iedge_rad, iedge_texture);

  ut_free_1d (c);

  return;
}
	
void
nevs_print_tet (FILE* file, double* coo1, double* coo2, double*
    coo3, double* coo4, char* texture, char* edge_rad, char* edge_texture)
{
  nevs_print_triangle (file, coo1, coo2, coo3, texture, NULL, NULL);
  nevs_print_triangle (file, coo1, coo3, coo4, texture, NULL, NULL);
  nevs_print_triangle (file, coo1, coo4, coo2, texture, NULL, NULL);
  nevs_print_triangle (file, coo2, coo3, coo4, texture, NULL, NULL);

  nevs_print_tet_edge (file, coo1, coo2, coo3, coo4, edge_rad, edge_texture);

  return;
}

void
nevs_print_scale (FILE* file, char* colscheme, char* minexpr, char* maxexpr, char* ticks)
{
  int i;
  struct NODES N;
  struct MESH M;
  double* coo = ut_alloc_1d (3);
  int binqty = 100;
  int* eltnodes = ut_alloc_1d_int (3);

  double height = 1;

  double* nodescal = ut_alloc_1d (2 * (binqty + 1) + 1);

  double min, max;

  sscanf (minexpr, "%lf", &min);
  sscanf (maxexpr, "%lf", &max);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  M.Dimension = 2;
  M.EltType = ut_alloc_1d_char (4);
  strcpy (M.EltType, "tri");
  M.EltOrder = 1;

  // adding bottom nodes
  coo[2] = 0;
  coo[0] = 0;
  neut_nodes_addnode (&N, coo, 0);
  nodescal[N.NodeQty] = min;

  coo[0] = -0.1;
  neut_nodes_addnode (&N, coo, 0);
  nodescal[N.NodeQty] = min;

  // adding other nodes
  for (i = 1; i <= binqty; i++)
  {
    coo[2] = height * i / (double) binqty;

    coo[0] = 0;
    neut_nodes_addnode (&N, coo, 0);
    nodescal[N.NodeQty] = min + (max - min) * i / (double) binqty;

    coo[0] = -0.1;
    neut_nodes_addnode (&N, coo, 0);
    nodescal[N.NodeQty] = nodescal[N.NodeQty - 1];

    eltnodes[0] = N.NodeQty - 1;
    eltnodes[1] = N.NodeQty - 2;
    eltnodes[2] = N.NodeQty - 3;

    neut_mesh_addelt (&M, eltnodes);

    eltnodes[0] = N.NodeQty;
    eltnodes[1] = N.NodeQty - 2;
    eltnodes[2] = N.NodeQty - 1;

    neut_mesh_addelt (&M, eltnodes);
  }
  
  neut_mesh_addelset (&M, NULL, M.EltQty);

  neut_nodes_scale (&N, height, height, height);

  struct PRINT Print;
  neut_print_set_default (&Print);
  Print.cameralookat[0] = 0;
  Print.cameralookat[1] = 0;
  Print.cameralookat[2] = height / 2;
  Print.cameracoo[0] =  0;
  Print.cameracoo[1] = -5;
  Print.cameracoo[2] = height / 2;
  Print.cameraangle = 6;
  sprintf (Print.cameraprojection, "orthographic");

  Print.showshadow = 1;

  int** rgb = ut_alloc_2d_int (N.NodeQty + 1, 3);
  if (colscheme != NULL)
    for (i = 1; i <= N.NodeQty; i++)
      ut_color_scheme_val_color (colscheme, min, max, nodescal[i], rgb[i]);
  else
    for (i = 1; i <= N.NodeQty; i++)
      ut_color_scheme_val_color ("blue,cyan,yellow,red", min, max, nodescal[i], rgb[i]);

  nevs_print_header (file, Print);
  nevs_print_mesh2d (file, N, M, Print, rgb, "elt");
  nevs_print_foot (file, Print);

  fprintf (file, "union {\n");
  fprintf (file, "cylinder {<-0.1,0,0>,<-0.1,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n", height);
  fprintf (file, "cylinder {<0,0,0>,<0,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n", height);
    fprintf (file, "cylinder {<-0.1,0,0>,<0,0,0>,0.0025 texture {pigment{rgb<0,0,0>}}}\n");
    fprintf (file, "cylinder {<-0.1,0,%f>,<0,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n", height, height);

  int valqty;
  double z;
  double* val = NULL;
  char** valstring = NULL;
  if (ticks == NULL)
  {
    valqty = 11;
    val = ut_alloc_1d (valqty);
    valstring = ut_alloc_2d_char (valqty, 11);
    for (i = 0; i < valqty; i++)
    {
      val[i] = min + ((double) i / (valqty - 1)) * (max - min);
      sprintf (valstring[i], "%f", val[i]);
    }
  }
  else
  {
    ut_string_separate (ticks, ',', &valstring, &valqty);
    val = ut_alloc_1d (valqty);
    for (i = 0; i < valqty; i++)
    {
      ut_string_fnr (valstring[i], '_', ' ');
      sscanf (valstring[i], "%lf", &(val[i]));
    }
  }

  fprintf (file, "union {\n");

  for (i = 0; i < valqty; i++)
  {
    z = height * (val[i] - min) / (max - min);

    fprintf (file, "cylinder {<-0.1,0,%f>,<0,0,%f>,0.0025 texture {pigment{rgb<0,0,0>}}}\n", z, z);

    fprintf (file, "text {ttf \"timrom.ttf\" \"%s\" 0.1, 0\n", valstring[i]);
    fprintf (file, "pigment {rgb<0,0,0>}\n");
    fprintf (file, "scale 0.06\n");
    fprintf (file, "rotate <90,0,0>\n");
    fprintf (file, "translate <0,0,%f>\n", z);
    fprintf (file, "translate <0.02,0,-0.02>\n");
    fprintf (file, "}\n");
  }

  ut_free_1d (val);
  ut_free_2d_char (valstring, valqty);

  fprintf (file, "}\n");

  fprintf (file, "rotate<-90, 0, 0>\nscale<1,1,-1>}\n");

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  ut_free_1d (coo);
  ut_free_1d_int (eltnodes);
  ut_free_1d (nodescal);

  return;
}
