/**
 * QTLS Algorithm - main
 * Author: Chris.Chen
 * Date: 06/03/2020
 * Desc: Quadratic Time Complexity Locality Detection Algorithm for Shared HPC Systems
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#include <stdio.h>
#include <algorithm>
#include "global-var.h"
#include "utils.h"
#include "file-io.h"
#include "test-helper.h"
#include "merge.h"
#include "strip.h"

using namespace std;

void iteration() {
	vector<PII> merge = generate_merge_scheme();
	PII maxMerge = iter_merge(merge); // return current iteration's max merge
	strip(maxMerge); 
}

int main(void) {
	global_var_init();
	read_file();
	group_initialize();
	while(m.size() != 0) {
		iteration();
	}
	run_test(false);
	result_test();
	write_result();
	return 0;
}
