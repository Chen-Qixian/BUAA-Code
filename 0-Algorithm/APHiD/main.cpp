/**
 * APHiD Algorithm - main
 * Authors: Chris.Chen
 * Date: 10/03/2020
 * Desc: Algorithm for Placing process Hierarchically on D-ary trees
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#include <stdio.h>
#include <algorithm>
#include "global-var.h"
#include "utils.h"
#include "test-helper.h"
#include "file-io.h"
#include "aphid1.h"
#include "aphid2.h"

#define APHID_MODE 2 // 1- BOTTOM_UP 2-TOP_DOWN

using namespace std;

void aphid() {
	printf("\n=====bottom-up-aphid begin=====\n");
	bottom_up_aphid();
	groups.clear();
	printf("\n=====top-down-aphid begin=====\n");
	top_down_aphid();
}

int main(void) {
	global_var_init();
	read_task_graph();
	aphid();
	write_result();
	return 0;
}
