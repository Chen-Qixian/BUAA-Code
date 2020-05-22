/**
 * TopoMapping - par-mapper
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef TOPO_PARMAPPER_H
#define TOPO_PARMAPPER_H

using namespace std;

int get_hop_bytes_avg(vector<HopBytes> h) {
	int res = 0;
	int size = h.size();
	for(int i = 0 ; i < size ; i ++) {
		res += (h[i].hopbytes / size);
	}
	return res;
}

int get_hop_bytes_max(vector<HopBytes> h) {
	int res = 0;
	int size = h.size();
	for(int i = 0 ; i < size ; i ++) {
		if(res < h[i].hopbytes) {
			res = h[i].hopbytes;
		}
	}
	return res;
}

void strategy(int alpha) {
	vector<Pick> avg;
	vector<Pick> max;

	for(int i = 0 ; i < PROCESS_NUM ; i ++) {
		vector<HopBytes> hopbytes = hop_bytes(Slist[i]);
		int avgHopBytes = get_hop_bytes_avg(hopbytes);
		int maxHopBytes = get_hop_bytes_max(hopbytes);
		Pick p;
		p.id = i;
		p.val = avgHopBytes;
		avg.push_back(p);
		p.val = maxHopBytes;
		max.push_back(p);
	}

	sort(avg.begin(), avg.end());
	sort(max.begin(), max.end());

	int expectResult = max[0].id;

	// using scale to prevent overflow int limit
	if(max[0].val / SCALE < avg[0].val / SCALE * alpha) {
		result = Slist[max[0].id];
		vector_test((char*)"result", result);
	}
	else {
		printf("\n===max: %d, avg: %d===\n", max[0].val, avg[0].val);
		printf("\n@strategy: Error! Do not find a Pareto result with aplha = %d\n", alpha);
	}

}

void par_mapper() {
	for(int i = 0 ; i < PROCESS_NUM ; i ++) {
		bool packNodeFirst = (i % 2 == 0) ? true : false;
		int taskListMode = i % TASKORDERS;
		Slist[i] = greedy_map(packNodeFirst, taskListMode);
		vector_test((char *)"Slist test", Slist[i]);
	}
	strategy(ALPHA);
}

#endif //TOPO_PARMAPPER_H