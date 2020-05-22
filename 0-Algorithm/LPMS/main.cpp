/**
 * LPG Algorithm - main
 * Author: Chris.Chen
 * Date: 05/03/2020
 * Desc: Label Propagation based Process Grouping Algorithm
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#include <stdio.h>
#include <algorithm>
#include "global-var.h"
#include "utils.h"
#include "test-helper.h"
#include "file-io.h"
#include "lpis.h"
#include "label-update.h"
#include "strip.h"

using namespace std;

/*
	main entrance
	@param {void}
	@return {int} 0
*/
int main(void) {
	bool testEnabled = false; // enabling switch for test

	graph_init();
	readFile();
	run_test(testEnabled);
	lpis();
	update_global_deg();
	while (P_u.size() != 0) {
		label_update();
		strip();
		printf("P_u size = %d\n", P_u.size());
	}
	result_output();
	return 0;
}
