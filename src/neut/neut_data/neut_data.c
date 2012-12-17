/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_data.h"

void
neut_geodata_set_default (struct GEODATA* pGeoData)
{
  int idmax;

  neut_geodata_idmax (&idmax);

  (*pGeoData).verqty  = 0;
  (*pGeoData).edgeqty = 0;
  (*pGeoData).faceqty = 0;
  (*pGeoData).polyqty = 0;

  (*pGeoData).coldata     = ut_alloc_1d_ppdouble (idmax + 1);
  (*pGeoData).coldatatype = ut_alloc_1d_pchar    (idmax + 1);
  (*pGeoData).colscheme   = ut_alloc_1d_pchar    (idmax + 1);
  (*pGeoData).col         = ut_alloc_1d_ppint    (idmax + 1);

  (*pGeoData).raddata     = ut_alloc_1d_ppdouble (idmax + 1);
  (*pGeoData).raddatatype = ut_alloc_1d_pchar    (idmax + 1);
  (*pGeoData).rad         = ut_alloc_1d_pdouble  (idmax + 1);
  
  (*pGeoData).scalemin    = ut_alloc_1d_pchar    (idmax + 1);
  (*pGeoData).scalemax    = ut_alloc_1d_pchar    (idmax + 1);
  (*pGeoData).scaleticks  = ut_alloc_1d_pchar    (idmax + 1);

  return;
}

void
neut_geodata_free (struct GEODATA* pGeoData)
{
  int i, idmax, qty;

  neut_geodata_idmax (&idmax);

  for (i = 0; i <= idmax; i++)
  {
    neut_geodata_id_qty (*pGeoData, i, &qty);
    ut_free_2d ((*pGeoData).coldata[i], qty);
    ut_free_2d ((*pGeoData).raddata[i], qty);
    ut_free_2d_int ((*pGeoData).col[i], qty);
  }
  ut_free_1d_ppdouble ((*pGeoData).coldata);
  ut_free_1d_ppdouble ((*pGeoData).raddata);
  ut_free_1d_ppint    ((*pGeoData).col);

  ut_free_2d ((*pGeoData).rad, idmax + 1);

  ut_free_2d_char ((*pGeoData).coldatatype, idmax + 1);
  ut_free_2d_char ((*pGeoData).colscheme, idmax + 1);

  ut_free_2d_char ((*pGeoData).raddatatype, idmax + 1);
  
  ut_free_2d_char ((*pGeoData).scalemin, idmax + 1);
  ut_free_2d_char ((*pGeoData).scalemax, idmax + 1);
  ut_free_2d_char ((*pGeoData).scaleticks, idmax + 1);

  return;
}

int
neut_geodata_entity_id (char* entity, int* pid)
{
  (*pid) = -1;

  if (strcmp (entity, "poly") == 0)
    (*pid) = 0;
  else if (strcmp (entity, "face") == 0)
    (*pid) = 1;
  else if (strcmp (entity, "edge") == 0)
    (*pid) = 2;
  else if (strcmp (entity, "ver") == 0)
    (*pid) = 3;

  return (*pid >= 0) ? 0 : -1;
}

int
neut_geodata_id_entity (int id, char* entity)
{
  entity[0] = '\0';

  if (id == 0)
    strcpy (entity, "poly");
  else if (id == 1)
    strcpy (entity, "face");
  else if (id == 2)
    strcpy (entity, "edge");
  else if (id == 3)
    strcpy (entity, "ver");

  return (strlen (entity) > 0) ? 0 : -1;
}

int
neut_geodata_entity_qty (struct GEODATA GeoData, char* entity, int* pqty)
{
  int id;

  neut_geodata_entity_id (entity, &id);
  return neut_geodata_id_qty (GeoData, id, pqty);
}

int
neut_geodata_id_qty (struct GEODATA GeoData, int id, int* pqty)
{
  (*pqty) = -1;

  if (id == 0)
    (*pqty) = GeoData.polyqty;
  else if (id == 1)
    (*pqty) = GeoData.faceqty;
  else if (id == 2)
    (*pqty) = GeoData.edgeqty;
  else if (id == 3)
    (*pqty) = GeoData.verqty;

  return ((*pqty) >= 0) ? 0 : -1;
}

void
neut_meshdata_set_default (struct MESHDATA* pMeshData)
{
  int idmax;

  neut_meshdata_idmax (&idmax);
  
  (*pMeshData).elset0dqty = 0;
  (*pMeshData).elset1dqty = 0;
  (*pMeshData).elset2dqty = 0;
  (*pMeshData).elset3dqty = 0;

  (*pMeshData).elt0dqty = 0;
  (*pMeshData).elt1dqty = 0;
  (*pMeshData).elt2dqty = 0;
  (*pMeshData).elt3dqty = 0;
  (*pMeshData).nodeqty  = 0;

  (*pMeshData).coldata     = ut_alloc_1d_ppdouble (idmax + 1);
  (*pMeshData).coldatatype = ut_alloc_1d_pchar    (idmax + 1);
  (*pMeshData).colscheme   = ut_alloc_1d_pchar    (idmax + 1);
  (*pMeshData).col         = ut_alloc_1d_ppint    (idmax + 1);

  (*pMeshData).raddata     = ut_alloc_1d_ppdouble (idmax + 1);
  (*pMeshData).raddatatype = ut_alloc_1d_pchar    (idmax + 1);
  (*pMeshData).rad         = ut_alloc_1d_pdouble  (idmax + 1);
  
  (*pMeshData).scalemin    = ut_alloc_1d_pchar    (idmax + 1);
  (*pMeshData).scalemax    = ut_alloc_1d_pchar    (idmax + 1);
  (*pMeshData).scaleticks  = ut_alloc_1d_pchar    (idmax + 1);

  (*pMeshData).coodata     = ut_alloc_1d_ppdouble (idmax + 1);
  (*pMeshData).coodatatype = ut_alloc_1d_pchar    (idmax + 1);
  (*pMeshData).coo         = ut_alloc_1d_ppdouble (idmax + 1);

  (*pMeshData).coofact     = ut_alloc_1d          (idmax + 1);
  ut_array_1d_set ((*pMeshData).coofact + 1, idmax, 1.);

  return;
}

void
neut_meshdata_free (struct MESHDATA* pMeshData)
{
  int i, idmax, qty;

  neut_meshdata_idmax (&idmax);

  for (i = 0; i <= idmax; i++)
  {
    neut_meshdata_id_qty (*pMeshData, i,    &qty);

    ut_free_2d     ((*pMeshData).coldata[i], qty + 1);
    ut_free_2d_int ((*pMeshData).col[i]    , qty + 1);
    ut_free_2d     ((*pMeshData).coodata[i], qty + 1);
    ut_free_2d     ((*pMeshData).coo[i]    , qty + 1);
    ut_free_2d     ((*pMeshData).raddata[i], qty + 1);
  }
  ut_free_1d_ppdouble ((*pMeshData).coldata);
  ut_free_1d_ppint    ((*pMeshData).col);
  ut_free_1d_ppdouble ((*pMeshData).coodata);
  ut_free_1d_ppdouble ((*pMeshData).coo);
  ut_free_1d_ppdouble ((*pMeshData).raddata);

  ut_free_2d      ((*pMeshData).rad, idmax + 1);

  ut_free_2d_char ((*pMeshData).coldatatype, idmax + 1);
  ut_free_2d_char ((*pMeshData).colscheme  , idmax + 1);

  ut_free_2d_char ((*pMeshData).raddatatype, idmax + 1);
  
  ut_free_2d_char ((*pMeshData).scalemin   , idmax + 1);
  ut_free_2d_char ((*pMeshData).scalemax   , idmax + 1);
  ut_free_2d_char ((*pMeshData).scaleticks , idmax + 1);
  ut_free_2d_char ((*pMeshData).coodatatype, idmax + 1);

  ut_free_1d ((*pMeshData).coofact);
  
  (*pMeshData).elset0dqty = 0;
  (*pMeshData).elset1dqty = 0;
  (*pMeshData).elset2dqty = 0;
  (*pMeshData).elset3dqty = 0;

  (*pMeshData).elt0dqty = 0;
  (*pMeshData).elt1dqty = 0;
  (*pMeshData).elt2dqty = 0;
  (*pMeshData).elt3dqty = 0;
  (*pMeshData).nodeqty  = 0;

  return;
}

int
neut_meshdata_entity_id (char* entity, int* pid)
{
  (*pid) = -1;

  if (strcmp (entity, "elt3d") == 0)
    (*pid) = 0;
  else if (strcmp (entity, "elt") == 0)
    (*pid) = 0;
  else if (strcmp (entity, "elt2d") == 0)
    (*pid) = 1;
  else if (strcmp (entity, "elt1d") == 0)
    (*pid) = 2;
  else if (strcmp (entity, "elt0d") == 0)
    (*pid) = 3;
  else if (strcmp (entity, "node") == 0)
    (*pid) = 4;
  else if (strcmp (entity, "node2elt") == 0)
    (*pid) = 5;
  else if (strcmp (entity, "elt3dedge") == 0)
    (*pid) = 6;
  else if (strcmp (entity, "elset3d") == 0)
    (*pid) = 7;
  else if (strcmp (entity, "elset1d") == 0)
    (*pid) = 8;
  else if (strcmp (entity, "elset3dedge") == 0)
    (*pid) = 9;

  return (*pid >= 0) ? 0 : -1;
}

int
neut_meshdata_id_entity (int id, char* entity)
{
  entity[0] = '\0';

  if (id == 0)
    strcpy (entity, "elt3d");
  else if (id == 1)
    strcpy (entity, "elt2d");
  else if (id == 2)
    strcpy (entity, "elt1d");
  else if (id == 3)
    strcpy (entity, "elt0d");
  else if (id == 4)
    strcpy (entity, "node");
  else if (id == 5)
    strcpy (entity, "node2elt");
  else if (id == 6)
    strcpy (entity, "elt3dedge");
  else if (id == 7)
    strcpy (entity, "elset3d");
  else if (id == 8)
    strcpy (entity, "elset1d");
  else if (id == 9)
    strcpy (entity, "elset3dedge");

  return (strlen (entity) > 0) ? 0 : -1;
}

int
neut_meshdata_idmax (int* pidmax)
{
  (*pidmax) = 9;

  return 0; 
}

int
neut_geodata_idmax (int* pidmax)
{
  (*pidmax) = 3;

  return 0; 
}

int
neut_meshdata_entity_qty (struct MESHDATA MeshData, char* entity, int* pqty)
{
  int id;

  neut_meshdata_entity_id (entity, &id);
  return neut_meshdata_id_qty (MeshData, id, pqty);
}

int
neut_meshdata_id_qty (struct MESHDATA MeshData, int id, int* pqty)
{
  (*pqty) = -1;

  if (id == 0)
    (*pqty) = MeshData.elt3dqty;
  else if (id == 1)
    (*pqty) = MeshData.elt2dqty;
  else if (id == 2)
    (*pqty) = MeshData.elt1dqty;
  else if (id == 3)
    (*pqty) = MeshData.elt0dqty;
  else if (id == 4)
    (*pqty) = MeshData.nodeqty;
  else if (id == 5)
    (*pqty) = MeshData.nodeqty;
  else if (id == 6)
    (*pqty) = MeshData.elt3dqty;
  else if (id == 7)
    (*pqty) = MeshData.elset3dqty;
  else if (id == 8)
    (*pqty) = MeshData.elset1dqty;
  else if (id == 9)
    (*pqty) = MeshData.elset3dqty;

  return ((*pqty) >= 0) ? 0 : -1;
}

int
neut_meshdata_type_qty (char* type, int* pqty)
{
  (*pqty) = -1;

  if (strcmp (type, "col") == 0)
    (*pqty) = 3;
  else if (strcmp (type, "orie" ) == 0
        || strcmp (type, "oriek") == 0
        || strcmp (type, "orier") == 0
        || strcmp (type, "oriR") == 0)
    (*pqty) = 3;
  else if (strcmp (type, "oriq" ) == 0
        || strcmp (type, "orirtheta") == 0)
    (*pqty) = 4;
  else if (strcmp (type, "m" ) == 0)
    (*pqty) = 6;
  else if (strcmp (type, "g" ) == 0)
    (*pqty) = 9;
  else if (strcmp (type, "scal") == 0)
    (*pqty) = 1;
  else if (strcmp (type, "coo") == 0)
    (*pqty) = 3;
  else if (strcmp (type, "coofact") == 0)
    (*pqty) = 1;

  return (*pqty >= 0) ? 0 : -1;
}

void
neut_meshdata_mesh2slice (struct NODES Nodes, struct MESH Mesh, struct MESHDATA MeshData,
		          struct NODES SNodes, struct MESH SMesh,
	                  int* elt_newold, int** node_newold, double* node_fact,
			  struct MESHDATA* pSMeshData)
{
  int i, j, k;
  int qty;
  int idmax;

  (*pSMeshData).elset0dqty = 0;
  (*pSMeshData).elset1dqty = 0;
  (*pSMeshData).elset2dqty = 0;
  (*pSMeshData).elset3dqty = 0;

  (*pSMeshData).elt0dqty = 0;
  (*pSMeshData).elt1dqty = 0;
  (*pSMeshData).elt2dqty = SMesh.EltQty;
  (*pSMeshData).elt3dqty = 0;
  
  (*pSMeshData).nodeqty  = SNodes.NodeQty;

  neut_meshdata_idmax (&idmax);

  Mesh  = Mesh;
  SMesh = SMesh;
  Nodes = Nodes;

  int qty0;
  int** id_newold = ut_alloc_2d_int (2, 2);
  neut_meshdata_entity_id ("node2elt", &(id_newold[0][0]));
  id_newold[0][1] = id_newold[0][0];
  neut_meshdata_entity_id ("elt2d", &(id_newold[1][0]));
  neut_meshdata_entity_id ("elt3d", &(id_newold[1][1]));

  for (i = 0; i <= 1; i++)
  {
    if (MeshData.coldatatype[id_newold[i][1]] == NULL)
      continue;

    (*pSMeshData).coldatatype[id_newold[i][0]]
      = ut_alloc_1d_char (strlen (MeshData.coldatatype[id_newold[i][1]]) + 1);
    strcpy ((*pSMeshData).coldatatype[id_newold[i][0]], MeshData.coldatatype[id_newold[i][1]]);
	    
    neut_meshdata_id_qty   ((*pSMeshData), id_newold[i][0], &qty0);
    neut_meshdata_type_qty (MeshData.coldatatype[id_newold[i][1]], &qty);

    (*pSMeshData).coldata[id_newold[i][0]] = ut_alloc_2d (qty0 + 1, qty);

    for (j = 1; j <= qty0; j++)
    {
      char* ent = ut_alloc_1d_char (100);
      neut_meshdata_id_entity (id_newold[i][0], ent);

      if (! strncmp (ent, "elt", 3))
      {
	for (k = 0; k < qty; k++)
	  (*pSMeshData).coldata[id_newold[i][0]][j][k] =
	    MeshData.coldata[id_newold[i][1]][elt_newold[j]][k];
      }
      else if (! strncmp (ent, "node", 4))
	for (k = 0; k < qty; k++)
	  (*pSMeshData).coldata[id_newold[i][0]][j][k] =
		(1 - node_fact[j]) * MeshData.coldata[id_newold[i][1]][node_newold[j][0]][k]
		 +   node_fact[j]    * MeshData.coldata[id_newold[i][1]][node_newold[j][1]][k];
    }
  
    if (MeshData.scalemin[id_newold[i][1]] != NULL)
    {
      (*pSMeshData).scalemin[id_newold[i][0]]
	= ut_alloc_1d_char (strlen (MeshData.scalemin[id_newold[i][1]]) + 1);
      strcpy ((*pSMeshData).scalemin[id_newold[i][0]],
		   MeshData.scalemin[id_newold[i][1]]);
    }

    if (MeshData.scalemax[id_newold[i][1]] != NULL)
    {
      (*pSMeshData).scalemax[id_newold[i][0]]
	= ut_alloc_1d_char (strlen (MeshData.scalemax[id_newold[i][1]]) + 1);
      strcpy ((*pSMeshData).scalemax[id_newold[i][0]],
	           MeshData.scalemax[id_newold[i][1]]);
    }

    if (MeshData.scaleticks[id_newold[i][1]] != NULL)
    {
      (*pSMeshData).scaleticks[id_newold[i][0]]
	= ut_alloc_1d_char (strlen (MeshData.scaleticks[id_newold[i][1]]) + 1);
      strcpy ((*pSMeshData).scaleticks[id_newold[i][0]],
	           MeshData.scaleticks[id_newold[i][1]]);
    }

    if (MeshData.colscheme[id_newold[i][1]] != NULL)
    {
      (*pSMeshData).colscheme[id_newold[i][0]]
	= ut_alloc_1d_char (strlen (MeshData.colscheme[id_newold[i][1]]) + 1);
      strcpy ((*pSMeshData).colscheme[id_newold[i][0]],
	           MeshData.colscheme[id_newold[i][1]]);
    }
  }

  return;
}
