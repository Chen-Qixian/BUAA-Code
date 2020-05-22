/**
 * TopoMapping - cost-function
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef TOPO_COST_FUNCTION_H
#define TOPO_COST_FUNCTION_H

using namespace std;

vector<HopBytes> hop_bytes(vector<int> S) {
	vector<HopBytes> ret;
	int size = S.size();
	// printf("@hop_bytes: size = %d\n", size);
	for(int i = 0 ; i < size ; i ++) {
		if(S[i] == -1) continue;
		int hb = 0; // hopbytes
		// printf("@hop_bytes: task = %d\n", S[i]);
		for(int j = 0 ; j < size ; j ++) {
			if(S[j] == -1 || i == j) continue;
			int weight = get_weight(S[i], S[j]);
			int distance = get_node_distance(get_node_order(i), get_node_order(j));
			// printf("@hop_bytes: i = %d,j = %d, weight = %d, distance = %d\n", i, j, weight, distance);
			hb += weight * distance;
		}
		HopBytes h;
		h.task = S[i];
		h.hopbytes = hb;
		ret.push_back(h);
	}
	return ret;
} 

int cost_function(vector<int> S, int t) {
	vector<HopBytes> hopBytesList = hop_bytes(S);
	for(vector<HopBytes>::iterator it = hopBytesList.begin() ; it != hopBytesList.end() ; it ++) {
		if(it->task == t) {
			return it->hopbytes;
		}
	}
	return -1;
}

#endif //TOPO_COST_FUNCTION_H







