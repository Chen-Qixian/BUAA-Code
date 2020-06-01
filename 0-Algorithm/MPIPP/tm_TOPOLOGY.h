#ifndef __TM_TOPOLOGY_H
#define __TM_TOPOLOGY_H

#include <hwloc.h>

typedef struct {
  int *arity;             /* Arity of the nodes of each level*/
  int nb_levels;          /* Number of levels of the tree. Levels are numbered from top to bottom starting at 0*/
  size_t *nb_nodes;       /* Number of nodes of each level*/
  int physical_num;       /* Flag set to !=0 if se use physical numberig and set to 0 is we use logical numbering */
  int *node_id;           /* ID of the nodes of the tree of the last level*/
  int *node_rank ;        /* Rank of the nodes of the tree for the last level given its ID: this is the inverse tab of node_id*/

  size_t *nb_free_nodes;  /* Nb of available nodes of each level*/
  int **free_nodes;       /* array of node that are free: useful to simulate batch scheduler*/
  double *cost;           /* Cost of the communication depending on the distance:
          cost[i] is the cost for communicating at distance nb_levels-i*/
  
  int *constraints;       /* Array of constraints: id of the nodes where it is possible to map processes */
  int nb_constraints;     /* Size of the above array */
  int oversub_fact;       /* Maximum number of processes to be mapped on a given node */
  int nb_proc_units;      /* The real number of units used for computation */
}tm_topology_t;
/*type of topology files that can be read*/
typedef enum{
  TM_FILE_TYPE_UNDEF,
  TM_FILE_TYPE_XML,
  TM_FILE_TYPE_TGT,
  TM_FILE_TYPE_MATRIX
} tm_file_type_t;


tm_topology_t* hwloc_to_tm(char *filename);
int symetric(hwloc_topology_t topology);
tm_topology_t * tgt_to_tm(char *filename);
void tm_free_topology(tm_topology_t *topology);
tm_topology_t *tm_load_topology(char *arch_filename, tm_file_type_t arch_file_type);

#endif

/*
typedef struct {
  int * arity; / *每个级别的节点的Arity * /
  int nb_levels; / *树的级别数。级别从0开始从上到下编号* /
  size_t * nb_nodes; / *每个级别的节点数* /
  int physical_num; / *标志设置为！= 0如果使用物理编号并设置为0，我们使用逻辑编号* /
  int * node_id; / *最后一级树的节点ID * /
  int * node_rank; / *给定其ID的最后一级树的节点的等级：这是node_id的逆选项卡* /

  size_t * nb_free_nodes; / *每个级别的可用节点Nb * /
  int ** free_nodes; / *自由节点数组：用于模拟批处理调度程序* /
  double *cost; / *通信费用取决于距离：
          cost [i]是距离nb_levels-i * /的通信费用
  
  int *constraints; / *约束数组：可以映射进程的节点的ID * /
  int nb_constraints; / *上面数组的大小* /
  int oversub_fact; / *要在给定节点上映射的最大进程数* /
  int nb_proc_units; / *用于计算的实际单位数量* /
} tm_topology_t;
*/