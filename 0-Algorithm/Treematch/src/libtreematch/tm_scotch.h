#ifndef __TM_SCOTCH_H__
#define __TM_SCOTCH_H__

#include "treematch.h"
#include "tm_kpartitioning.h"


#if HAVE_LIBSCOTCH
#include <scotch.h>
//SCOTCH_Graph* com_mat_to_scotch_graph(com_mat_t *com_mat, int n);
SCOTCH_Num *scotch_partitionning(SCOTCH_Arch archdat, tm_affinity_mat_t *aff_mat, SCOTCH_Num *parttab_len, int oversub_fact);
tm_solution_t *build_mapping_with_scotch(tm_topology_scotch_t *topology, tm_affinity_mat_t *aff_mat);
SCOTCH_Graph* com_mat_to_scotch_graph(com_mat_t *com_mat, int n);
#endif

#endif
