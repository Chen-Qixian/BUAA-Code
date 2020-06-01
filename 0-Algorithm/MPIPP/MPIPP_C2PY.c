/*
* @Author: kingofwolf
* @Date:   2019-03-17 19:47:22
* @Last Modified by:   kingofwolf
* @Last Modified time: 2019-03-17 21:06:43
*/
#include "map_MPIPP.h"

void MPIPP_MAPPING(char *arch_filename,int file_type,char *com_filename,int ct,int node,int core){
	tm_file_type_t arch_file_type=TM_FILE_TYPE_UNDEF;
	tm_metric_t metric = TM_METRIC_SUM_COM;
	Net_Node[NB_CT]=0;
	Net_Node[NB_NODE]=0;
	Net_Node[NB_CORE]=0;
	Net_Node[NB_FLAG]=0;
	if (file_type == 0) {
		arch_file_type=TM_FILE_TYPE_MATRIX;
		Net_Node[NB_CT]=ct;
		Net_Node[NB_NODE]=node;
		Net_Node[NB_CORE]=core;
		Net_Node[NB_FLAG]=1;
		Net_File=arch_filename;
	}
	else if (file_type == 1) arch_file_type=TM_FILE_TYPE_TGT;
	else if (file_type == 2) arch_file_type=TM_FILE_TYPE_XML;
	test_mapping(arch_filename, arch_file_type, com_filename, metric);
}
