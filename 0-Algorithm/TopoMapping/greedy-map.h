/**
 * TopoMapping - greedy-map
 * Authors: Chris.Chen
 * Date: 09/03/2020
 * Desc: Automatic Topology Mapping of Diverse Large-scale Parallel Applications
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef TOPO_GREEDYMAP_H
#define TOPO_GREEDYMAP_H

#include <limits.h>

using namespace std;

// mark processor list as all available
vector<int> init_ret() {
	vector<int> r;
	for(int i = 0 ; i < net_ct ; i ++) {
		r.push_back(-1);
	}
	return r;
}

void init_free_processors(int nl, vector<int> S, vector<int>& K) {
	K.clear();
	PII range = get_core_range(nl);
	// printf("\n@init_free_processors: %d, %d\n", range.first, range.second);
	for(int i = range.first ; i < range.second ; i ++) {
		if(S[i] == -1) {
			K.push_back(i);
		}
	}
}

vector<int> get_node_free_cores(vector<int> S, int pnode) {
	vector<int> ret;
	PII range = get_core_range(pnode);
	for(int i = range.first ; i < range.second ; i ++) {
		if(S[i] == -1) {
			ret.push_back(i);
		}
	}
	return ret;
}

vector<int> closest_free_node(vector<int> S, int pnode) {
	vector<int> ret;
	int minDist = INT_MAX;
	for(int i = 0 ; i < net_node ; i ++) {
		if(i == pnode) continue;
		vector<int> freeCores = get_node_free_cores(S, i);
		int dist = get_node_distance(pnode, i);
		if(freeCores.size() > 0) {
			if(dist < minDist) {
				ret.clear();
				ret.push_back(i);
				minDist = dist;
			}
			else if(dist == minDist) {
				ret.push_back(i);
			}
		}
	}	
	return ret;
}

void add_cores(vector<int>& cores,vector<vector<int> >& nodeCores, vector<int> S, int pnode) {
	vector<int> pcores = get_node_free_cores(S, pnode);
	int pSize = pcores.size();
	for(int k = 0 ; k < pSize ; k ++) {
		cores.push_back(pcores[k]);
	}
	if(pSize > 0) {
		nodeCores.push_back(pcores);
	}
}



vector<int> greedy_map(bool packNodeFirst, int taskListMode) {
	vector<int> S = init_ret();
	vector<int> tList = taskList[taskListMode];
	int tSize = tList.size();
	int nl = get_node_order(0);
	vector<int> K;
	// tSize = 1;
	// vector_test((char *)"taskList",tList);
	for(int i = 0 ; i < tSize ; i ++) {
		int t = tList[i];
		init_free_processors(nl, S, K);
		// vector_test((char *)"K", K);
		if(!packNodeFirst || K.size() == 0) {
			K.clear();
			vector<int> cores;
			vector<vector<int> > nodeCores;
			vector<int> closestFreeNodes = closest_free_node(S, nl);
			int cSize = closestFreeNodes.size();
			for(int j = 0; j < cSize ; j ++) {
				int pnode = closestFreeNodes[j];
				add_cores(cores, nodeCores, S, pnode);
			}
			if(cSize == 0 && !packNodeFirst) {
				add_cores(cores, nodeCores, S, nl);
			}
			if(cores.size() == 0) {
				printf("\nNo more free cores!\n");
				return S;
			}
			vector<int> costs;
			int nSize = nodeCores.size();
			for(int j = 0 ; j < nSize ; j ++) {
				int pcore = nodeCores[j][0];
				// printf("%d ", pcore);
				vector<int> Stmp = S;
				Stmp[pcore] = t;
				int curCost = cost_function(Stmp, t);
				costs.push_back(curCost);
				// printf("%d ", curCost);
			}
			sort(costs.begin(), costs.end());
			int minCost = costs[0];
			int costSize = costs.size();
			for(int j = 0 ; j < costSize ; j ++) {
				if(costs[j] == minCost) {
					merge_vector(K, nodeCores[j]);
				}
				else {
					break;
				}
			}
			// vector_test((char *)"K", K);
		}
		int dstCore = random_choose(K);
		// printf("===picked dstCore: %d===", dstCore);
		S[dstCore] = t;
		nl = get_node_order(dstCore);
	}


	return S;
}

#endif //TOPO_GREEDYMAP_H











