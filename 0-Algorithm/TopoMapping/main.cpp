/**
 * TopoMapping - main
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#include <stdio.h>
#include <algorithm>
#include "global-var.h"
#include "utils.h"
#include "test-helper.h"
#include "file-io.h"
#include "task-graph.h"
#include "cost-function.h"
#include "greedy-map.h"
#include "par-mapper.h"

using namespace std;

int main(void) {
	init_global_var();
	read_task_graph();
	read_net_graph();
	taskList[OOLIST] = OO();
	taskList[BFSLIST] = BFS(0);
	taskList[DFSLIST] = BFS_DFS(0);
	par_mapper();
	write_result();
	return 0;
}