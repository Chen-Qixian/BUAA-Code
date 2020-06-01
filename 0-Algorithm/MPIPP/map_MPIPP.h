#ifndef _MAP_MPIPP_H_
#define _MAP_MPIPP_H_
#include "tm_TOPOLOGY.h"
/* different metrics to evaluate the solution */
typedef enum{
  TM_METRIC_SUM_COM  = 1,
  TM_METRIC_MAX_COM  = 2,
  TM_METRIC_HOP_BYTE = 3
} tm_metric_t;
typedef enum{
  NB_CT=0,
  NB_NODE=1,
  NB_CORE=2,
  NB_FLAG=3
} net_node_index;
int Net_Node[4];
char * Net_File;
int test_mapping(char *arch_filename,tm_file_type_t arch_file_type,char *com_filename,tm_metric_t metric);
void file_2_arch(double **arch);

#endif