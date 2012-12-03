/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Partition.h"
#ifdef HAVE_LIBSCOTCH

void
Partition (struct IN In, struct NODES *pNodes, struct MESH *pMesh0D,
	   struct MESH *pMesh1D, struct MESH *pMesh2D, struct MESH *pMesh,
	   struct PART *pPart)
{
  FILE *file;
  int i, total;
  SCOTCH_Mesh SCMesh;
  SCOTCH_Arch Arch;
  SCOTCH_archInit (&Arch);

  if (In.partmode != 0)
    {
      file = ut_file_open (In.partarchfile, "r");
      SCOTCH_archLoad (&Arch, file);
      ut_file_close (file, In.partarchfile, "r");
      In.partqty = SCOTCH_archSize (&Arch);
    }

  (*pPart).qty = In.partqty;

  (*pPart).nodeqty = ut_alloc_1d_int (In.partqty);
  (*pPart).eltqty = ut_alloc_1d_int (In.partqty);
  (*pPart).targeteltqty = ut_alloc_1d_int (In.partqty);

  total = 0;
  for (i = 0; i < In.partqty; i++)
    {
      (*pPart).targeteltqty[i] =
	(*pMesh).EltQty * (double) (i + 1) / In.partqty - total;
      total += (*pPart).targeteltqty[i];
    }
  assert (total == (*pMesh).EltQty);

  ut_print_message (0, 2, "Preparing data ...\n");

  if ((*pMesh).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh, (*pNodes).NodeQty);

  SCOTCH_meshInit (&SCMesh);
  neut_mesh_scotchmesh ((*pMesh), (*pNodes).NodeQty, &SCMesh);

  /* SCOTCH_meshOrder (&SCMesh, &Strat, permtab, ); */

  ut_print_message (0, 2, "Partitioning nodes ...\n");
  PartitionNodes (In, &SCMesh, &Arch, pNodes, pMesh0D, pMesh1D, pMesh2D,
		  pMesh, pPart);

  ut_print_message (0, 2, "Partitioning elements ...\n");
  PartitionElts (In, pMesh, pPart);

  ut_print_message (0, 2, "Partition properties:\n");
  PartitionStats (In.partqty, *pMesh, *pPart);

  return;
}

#endif
