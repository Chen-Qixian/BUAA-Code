/**
 * QTLS Algorithm - merge
 * Author: Chris.Chen
 * Date: 06/03/2020
 * Desc: Quadratic Time Complexity Locality Detection Algorithm for Shared HPC Systems
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef QTLS_MERGE_H
#define QTLS_MERGE_H
#include <utility>

using namespace std;

/*
	generate all merge schemes, save to list merge
	@param {void}
	@return {vector<PII>} list contains pairs of merge scheme
*/
vector<PII> generate_merge_scheme() {
	vector<PII> ret;
	// P_g has a max size of procNum, size == 0 when it is not a root for a group
	for(int i = 0 ; i < procNum ; i ++) {
		if(P_g[i].size() == 0) continue;
		for(int j = i + 1; j < procNum ; j ++) {
			if(P_g[j].size() == 0) continue;
			ret.push_back(make_pair(i, j));
		}
	}

	return ret;
}

/*
	calculate e_ij
	@param {PII} the pair of two sub graphs i, j
	@return {int} e_ij = edge number between two sub graphs i, j
*/
int get_e_ij(PII mergeI) {
	vector<int> group1 = P_g[mergeI.first];
	vector<int> group2 = P_g[mergeI.second];
	int size1 = group1.size();
	int size2 = group2.size();
	int e_ij = 0;
	// vector_test((char*)"group 1:" , group1);
	// vector_test((char*)"group 2:" , group2);
	for(int i = 0 ; i < size1 ; i ++) {
		for(int j = 0 ; j < size2 ; j ++) {
			if(graph[group1[i]][group2[j]] > 0) {
				e_ij ++;
			}
		}
	}
	return e_ij;
}

/*
	calculate a_i
	@param {int} the i sub graph
	@return {int} a_i = edge number of graph i to all other subgraphs
*/
int get_a(int groupId) {
	int a = 0;
	for(int i = 0 ; i < procNum ; i ++) {
		// skip non group and self
		if(P_g[i].size() == 0 || i == groupId) continue;
		a += get_e_ij(make_pair(groupId, i));
	}
	return a;
}

/*
	note: return value is the absolute of delta Q
	@param {PII} scheme pairs i, j
	@return {int} absolute of Delta Q
*/
int calculate_delta_Q(PII mergeI) {
	int e_ij = get_e_ij(mergeI);
	// printf("\ne_ij = %d\n", e_ij);
	int a_i = get_a(mergeI.first);
	int a_j = get_a(mergeI.second);
	// printf("\na_i = %d\n", a_i);
	// printf("\na_j = %d\n", a_j);
	return a_i * a_j - e_ij;
}

/*
	merge P_g[b] to P_g[a] and erase P_g[b]
	save origin P_g[a] and P_g[b] info
	@param {PII} scheme pairs i, j to be merged
	@return {void}
*/
void merge_group(PII maxMerge) {
	int a = maxMerge.first;
	int b = maxMerge.second;
	int sizea = P_g[a].size();
	int sizeb = P_g[b].size();
	lastPga.clear();
	lastPgb.clear();
	for(int i = 0 ; i < sizea ; i ++) {
		lastPga.push_back(P_g[a][i]);
	}
	for(int i = 0 ; i < sizeb ; i ++) {
		lastPgb.push_back(P_g[b][i]);
		P_g[a].push_back(P_g[b][i]);
	}
	P_g[b].clear(); // reset P_g[b] to NULL
}

/*
	merge method in iteration
	@param {vector<PII>} available merge scheme list generated
	@return {PII} maxMerge
*/
PII iter_merge(vector<PII> merge) {
	// merge_scheme_test(merge);
	int size = merge.size();
	// int deltaQ = calculate_delta_Q(merge[0]);
	int maxDeltaQ = calculate_delta_Q(merge[0]);
	PII maxMerge = merge[0];
	for(int i = 0 ; i < size ; i ++) {
		int deltaQ = calculate_delta_Q(merge[i]);
		// note: maxDeltaQ is the min absolute of deltaQ
		if(deltaQ < maxDeltaQ) {
			maxDeltaQ = deltaQ;
			maxMerge = merge[i];
		}
		// printf("deltQ[%d] = %d\n", i, deltaQ);
	}
	// printf("***max merge = %d, %d; max DeltaQ= %d\n", maxMerge.first, maxMerge.second, maxDeltaQ);
	merge_group(maxMerge);
	// vector_test((char *)"P_g[a]", P_g[maxMerge.first]);
	// printf("\nP_g[%d] = %d",maxMerge.second, P_g[maxMerge.second].size());
	return maxMerge;
}

#endif // QTLS_MERGE_H


















