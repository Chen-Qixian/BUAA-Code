/*
* @Author: kingofwolf
* @Date:   2019-03-09 14:54:00
* @Last Modified by:   kingofwolf
* @Last Modified time: 2019-03-09 19:48:24
*/
#include <stdio.h>
#include "tm_TOPOLOGY.h"
#include "tm_MAPPING.h"

void print_topology(tm_topology_t *topology)
{
	int i = 0;
	for (i = 0; i < topology->nb_levels; i++){
		printf("int arity[%d]:%d\n", i, topology->arity[i]);
	}
	printf("int nb_levels:%d\n",topology->nb_levels);
	for (i = 0; i < topology->nb_levels; i++){
		printf("int nb_nodes[%d]:%lu\n", i, topology->nb_nodes[i]);
	}
	printf("node_id:\n");
	for (i = 0; i < topology->nb_nodes[topology->nb_levels-1]; i++)
	{
		printf("%d_", topology->node_id[i]);
	}
	printf("\nnode_rank:\n");
	for (i = 0; i < topology->nb_nodes[topology->nb_levels-1]; i++)
	{
		printf("%d_", topology->node_rank[i]);
	}
	// for (i = 0; i < topology->nb_levels; i++){
	// 	printf("size_t nb_free_nodes[%d]:%lu\n", i, topology->nb_free_nodes[i]);
	// }
	printf("\n");
	for (i = 0; i < topology->nb_levels; i++){
		printf("double cost[%d]:%f\n", i, topology->cost[i]);
	}
	// for (int i = 0; i < topology->nb_constraints; i++)
	// {
	// 	printf("constraints:%d\n", topology->constraints[i]);
	// }
	// printf("nb_constraints:%d\n", topology->nb_constraints);
	// printf("oversub_fact:%d\n", topology->oversub_fact);
	// printf("nb_proc_units:%d\n", topology->nb_proc_units);
}

int main(int argc, char const *argv[])
{
	tm_topology_t *topology;
	tm_file_type_t filetype = TM_FILE_TYPE_TGT;
	char path[101] = "../../../examples/topologies/128.tgt";

	topology = tm_load_topology(path,filetype);

	print_topology(topology);

	return 0;
}