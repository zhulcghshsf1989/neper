/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Partition.h"

#ifdef HAVE_LIBSCOTCH

void
PartitionElts_Match (struct IN In, struct MESH *pMesh, struct PART *pPart)
{
  int i, j, partition, maxpnodeqty, eltnodeqty;
  int nodemaxpqty, nodeqty, k;
  int *eltnodepart = ut_alloc_1d_int (In.partqty);
  int *pos = ut_alloc_1d_int (In.partqty);
  int *peltqty = ut_alloc_1d_int (In.partqty);
  SCOTCH_Num partnbr = In.partqty;

  eltnodeqty =
    neut_elt_nodeqty ("tri", (*pMesh).Dimension, (*pMesh).EltOrder);

  /* each elt is assigned to a partition according to:
   * an elt is assigned to the partition of which most of its nodes
   * belong.  In case of ambiguity, it is assigned to the partition with
   * the least number of elements. */

  for (nodeqty = eltnodeqty; nodeqty >= 1; nodeqty--)
    for (i = 1; i <= (*pMesh).EltQty; i++)
      if ((*pPart).elt_parts[i] == -1)
	{
	  /* if the element has not been assigned to a partition yet */
	  ut_array_1d_int_zero (eltnodepart, partnbr);

	  /* recording eltnodepart: the number of nodes in every partition */
	  for (j = 0; j < eltnodeqty; j++)
	    eltnodepart[(*pPart).node_parts[(*pMesh).EltNodes[i][j]]]++;

	  /* maxpnodeqty: max number of nodes in the same partition
	   * nodemaxpqty: number of partitions for which it is the case */
	  maxpnodeqty = ut_array_1d_int_max (eltnodepart, partnbr);
	  nodemaxpqty =
	    ut_array_1d_int_eltpos_all (eltnodepart, partnbr, maxpnodeqty,
					pos);
	  if (nodemaxpqty < 1)
	    ut_error_reportbug ();

	  /* if most of the nodes are in one partition or no ambiguity:
	   * assigning the element to the partition */
	  if (maxpnodeqty == nodeqty)
	    {
	      partition = -1;

	      if (nodemaxpqty == 1)
		partition = pos[0];
	      else
		{
		  for (k = 0; k < nodemaxpqty; k++)
		    {
		      peltqty[k] = (*pPart).eltqty[pos[k]];
		      partition =
			pos[ut_array_1d_int_min_index (peltqty, nodemaxpqty)];
		    }
		}

	      if (partition == -1)
		ut_error_reportbug ();

	      (*pPart).elt_parts[i] = partition;
	      (*pPart).eltqty[partition]++;
	    }
	}

  if (ut_array_1d_int_eltpos ((*pPart).elt_parts + 1, (*pMesh).EltQty, -1) !=
      -1)
    ut_error_reportbug ();

  ut_free_1d_int (pos);
  ut_free_1d_int (eltnodepart);
  ut_free_1d_int (peltqty);

  return;
}

int
PartitionElts_Balancing (struct MESH *pMesh, struct PART *pPart, double level)
{
  int i;
  int **Q = NULL;
  int BoundEltQty;
  int sum;

  /* recording elements whose nodes are in > 1 partitions */

  /* first loop for allocation */
  /* BoundEltQty = number of boundary elements (i.e. which have node in
   * different partitions */

  BoundEltQty = 0;
  Q = ut_alloc_2d_int (BoundEltQty, 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
    if (neut_mesh_elt_isbound (*pMesh, i, (*pPart).node_parts) == 1)
      {
	BoundEltQty++;
	Q = ut_realloc_2d_int_addline (Q, BoundEltQty, 3);
	Q[BoundEltQty - 1][0] = i;
      }

  sum = PartitionElts_Balancing_Q (*pMesh, pPart, level, BoundEltQty, Q);

  if (sum != 0)
    ut_print_message (1, 3, "Balancing is off by %d elements.\n", sum);

  ut_free_2d_int (Q, BoundEltQty);

  return sum;
}

void
PartitionElts_ByNumber (struct IN In, struct MESH *pMesh, struct PART *pPart)
{
  int j;

  for (j = 1; j <= (*pMesh).EltQty; j++)
    {
      (*pPart).elt_parts[j] = In.partqty * (j - 1) / (*pMesh).EltQty;
      (*pPart).eltqty[(*pPart).elt_parts[j]]++;
    }

  return;
}

#endif
