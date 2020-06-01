#ifndef __TM_MAPPING_H__
#define __TM_MAPPING_H__

#include "tm_TOPOLOGY.h"
#include "tm_MALLOC.h"
#include "tm_TIMINGS.h"

typedef struct {
  double ** mat;
  double *  sum_row;
  int order;
  long int nnz; /* number of non zero entries */
} tm_affinity_mat_t;

tm_affinity_mat_t * new_affinity_mat(double **mat, double *sum_row, int order, long int nnz);
void tm_free_affinity_mat(tm_affinity_mat_t *aff_mat);
tm_affinity_mat_t *tm_load_aff_mat(char *filename);

#endif