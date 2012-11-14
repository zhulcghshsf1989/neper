/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_camera.h"

void
nevs_camera_init (struct GEO Geo, struct NODES Nodes,
		  struct MESH Mesh, struct MESHDATA MeshData,
		  struct PRINT* pPrint)
{
  int node_id;
  struct NODES Nodes2;
      
  neut_nodes_set_zero (&Nodes2);

  neut_meshdata_entity_id ("node", &node_id);

  if (MeshData.coodata[node_id] != NULL)
  {
    neut_nodes_nodes (Nodes, &Nodes2);

    if (! strcmp (MeshData.coodatatype[node_id], "coo"))
      ut_array_2d_memcpy (Nodes2.NodeCoo + 1, Nodes.NodeQty, 3,
			  MeshData.coodata[node_id] + 1);
    else if (! strcmp (MeshData.coodatatype[node_id], "disp"))
      ut_array_2d_add (Nodes2.NodeCoo + 1, 
		       MeshData.coodata[node_id] + 1,
		       Nodes.NodeQty, 3, Nodes2.NodeCoo + 1);
    else
      ut_error_reportbug ();
  }
  else
    Nodes2 = Nodes;

  nevs_camera_coo ((*pPrint).cameracooexpr[1], Geo, Nodes2, Mesh, pPrint, 1);
  nevs_camera_coo ((*pPrint).cameracooexpr[2], Geo, Nodes2, Mesh, pPrint, 2);
  nevs_camera_coo ((*pPrint).cameracooexpr[3], Geo, Nodes2, Mesh, pPrint, 3);

  nevs_camera_lookat ((*pPrint).cameralookatexpr[1], Geo, Nodes2, Mesh, pPrint, 1);
  nevs_camera_lookat ((*pPrint).cameralookatexpr[2], Geo, Nodes2, Mesh, pPrint, 2);
  nevs_camera_lookat ((*pPrint).cameralookatexpr[3], Geo, Nodes2, Mesh, pPrint, 3);
  
  if (MeshData.coodata[node_id] != NULL)
    neut_nodes_free (&Nodes2);

  return;
}

void
nevs_camera (char** argv, int* pi, struct GEO Geo, struct NODES Nodes,
             struct MESH Mesh, struct MESHDATA MeshData, struct PRINT* pPrint)
{
  int node_id;
  struct NODES Nodes2;

  neut_nodes_set_zero (&Nodes2);

  neut_meshdata_entity_id ("node", &node_id);

  if (MeshData.coodata[node_id] != NULL)
  {
    neut_nodes_nodes (Nodes, &Nodes2);
    
    if (! strcmp (MeshData.coodatatype[node_id], "coo"))
      ut_array_2d_memcpy (Nodes2.NodeCoo + 1, Nodes.NodeQty, 3,
			  MeshData.coodata[node_id] + 1);
    else if (! strcmp (MeshData.coodatatype[node_id], "disp"))
      ut_array_2d_add (Nodes2.NodeCoo + 1, 
		       MeshData.coodata[node_id] + 1,
		       Nodes.NodeQty, 3, Nodes2.NodeCoo + 1);
    else
      ut_error_reportbug ();
  }
  else
    Nodes2 = Nodes;

  if (strcmp (argv[*pi], "-cameracoox") == 0)
    nevs_camera_coo (argv[++(*pi)], Geo, Nodes2, Mesh, pPrint, 1);

  else if (strcmp (argv[*pi], "-cameracooy") == 0)
    nevs_camera_coo (argv[++(*pi)], Geo, Nodes2, Mesh, pPrint, 2);

  else if (strcmp (argv[*pi], "-cameracooz") == 0)
    nevs_camera_coo (argv[++(*pi)], Geo, Nodes2, Mesh, pPrint, 3);

  else if (strcmp (argv[*pi], "-cameracoo") == 0)
  {
    (*pi)++;
    nevs_camera_coo (argv[*pi], Geo, Nodes2, Mesh, pPrint, 1);
    nevs_camera_coo (argv[*pi], Geo, Nodes2, Mesh, pPrint, 2);
    nevs_camera_coo (argv[*pi], Geo, Nodes2, Mesh, pPrint, 3);
  }

  else if (strcmp (argv[*pi], "-cameralookatx") == 0)
    nevs_camera_lookat (argv[++(*pi)], Geo, Nodes2, Mesh, pPrint, 1);

  else if (strcmp (argv[*pi], "-cameralookaty") == 0)
    nevs_camera_lookat (argv[++(*pi)], Geo, Nodes2, Mesh, pPrint, 2);

  else if (strcmp (argv[*pi], "-cameralookatz") == 0)
    nevs_camera_lookat (argv[++(*pi)], Geo, Nodes2, Mesh, pPrint, 3);

  else if (strcmp (argv[*pi], "-cameralookat") == 0)
  {
    (*pi)++;
    nevs_camera_lookat (argv[*pi], Geo, Nodes2, Mesh, pPrint, 1);
    nevs_camera_lookat (argv[*pi], Geo, Nodes2, Mesh, pPrint, 2);
    nevs_camera_lookat (argv[*pi], Geo, Nodes2, Mesh, pPrint, 3);
  }

  else if (strcmp (argv[*pi], "-cameraangle") == 0)
    sscanf (argv[++(*pi)], "%lf", &((*pPrint).cameraangle));

  else if (strcmp (argv[*pi], "-cameraprojection") == 0)
    sprintf ((*pPrint).cameraprojection, "%s", argv[++(*pi)]);

  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  if (MeshData.coodata[node_id] != NULL)
    neut_nodes_free (&Nodes2);
      
  return;
}
