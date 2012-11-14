/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_data.h"

void
nevs_data (char** argv, int* pi, struct GEO Geo, struct NODES Nodes, struct MESH Mesh1D,
           struct MESH Mesh, struct GEODATA* pGeoData, struct MESHDATA* pMeshData)
{
  char* entity = ut_alloc_1d_char (100);
  char* type   = ut_alloc_1d_char (100);

  nevs_data_string_entity_type (argv[(*pi)], entity, type);

  if (strcmp (entity, "poly") == 0
   || strcmp (entity, "edge") == 0
   || strcmp (entity, "face") == 0
   || strcmp (entity, "ver" ) == 0)
    nevs_geodata_fscanf (Geo, entity, type, argv[++(*pi)], pGeoData);

  else if (
      strncmp (entity, "elt"  , 3) == 0
   || strncmp (entity, "node" , 4) == 0
   || strncmp (entity, "elset", 5) == 0)
    nevs_meshdata_fscanf (Nodes, Mesh1D, Mesh, entity, type, argv[++(*pi)], pMeshData);

  else
    ut_print_message (1, 0, "Unknown entity = %s - skipping.\n", entity);

  ut_free_1d_char (entity);
  ut_free_1d_char (type);

  return;
}

void
nevs_data_init (struct GEO Geo, struct GEODATA* pGeoData,
                struct NODES Nodes, struct MESH Mesh3D,
                struct MESHDATA* pMeshData)
{
  if ((*pGeoData).polyqty > 0)
    nevs_geodata_init (Geo, pGeoData);
  
  if ((*pMeshData).elt3dqty > 0)
    nevs_meshdata_init (Nodes, Mesh3D, pMeshData);

  return;
}

