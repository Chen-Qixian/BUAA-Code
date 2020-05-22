/**
 * LPG Algorithm - strip
 * Author: Chris.Chen
 * Date: 05/03/2020
 * Desc: Label Propagation based Process Grouping Algorithm
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef LPMS_STRIP_H
#define LPMS_STRIP_H

using namespace std;

void strip();

/*
	eliminate edges of full subgraph
	@param {vector<Edge>} current start point edges
	@param {int} current node to be eliminated
	@return {void} 
*/
void eliminate_edge(vector<Edge>& e, int node) {
	if(e.size() == 0) return;
	for(auto it = e.begin(); it != e.end() ; it ++) {
		if(it -> nextNode == node) {
			if(e.size() > 1) {
				e.erase(it);
			}
			// NOTE that unable to earase a vector to NULL
			else {
				it -> nextNode = UNDEFINED;
				it -> cost = INFINITE;
			}
			break;	
		}
	}
}

/*
	P = P - P_f prunning full sub-graph from origin graph and update origin graph
	@param {int} group id : P_f
	@return {void} 
*/
void update_graph(int groupId) {
	vector<int> curGroup = groupResult[groupId];
	int size = curGroup.size();
	for(int i = 0 ; i < size ; i ++) {
		for(int j = 0 ; j < procNum ; j ++) {
			// node belongs to P_f itself should be clear to UNDEFINED
			if(curGroup[i] == j) {
				edge[j].erase(edge[j].begin() + 1, edge[j].end());
				edge[j][0].nextNode = UNDEFINED;
				edge[j][0].cost = INFINITE;
				continue;
			}
			// two nodes should have connection
			if(graph[curGroup[i]][j] != INFINITE) {
				eliminate_edge(edge[j], curGroup[i]);
			}
		}
	}
	update_global_deg(); // always update global degree after eliminate full subgraph
	eliminate_ele(availableGroup, groupId); // eliminate current group from available group set
}

/*
	get nodes' weight contribution in target group
	@param {vector<int> v} node list of target group
	@return {vector<WeiCon>} struct list contains node_id and its weight contribution 
*/
vector<WeiCon> get_inter_weight_contribution(vector<int> v) {
	vector<WeiCon> ret;
	int size = v.size();
	for(int i = 0 ; i < size ; i ++) {
		int weightContribution = 0;
		for(int j = 0 ; j < size ; j ++) {
			if(v[i] == v[j] || graph[v[i]][v[j]] == INFINITE) continue;
			weightContribution += graph[v[i]][v[j]];
		}
		WeiCon wc;
		wc.node = v[i];
		wc.con = weightContribution;
		ret.push_back(wc);
	}
	return ret;
}

/*
	stripe group when more process than core in a group
	@param {int} the id of group to be strip
	@return {void} 
*/
void group_strip(int groupId) {
	vector<int> curGroup = groupResult[groupId];
	vector<WeiCon> weightContribution;
	weightContribution = get_inter_weight_contribution(curGroup);

	sort(weightContribution.begin(), weightContribution.end());
	
	int t = curGroup.size();
	int rmNum = t - m;
	vector<int> minWcNodes; // for randomly choose a node to eliminate when their weight contribution is equal
	minWcNodes.clear();
	for(int i = 0 ; i < rmNum ; i ++) {
		// detecting equal weight contribution nodes
		if(minWcNodes.size() == 0) {
			int j = i;
			while(j < weightContribution.size() && weightContribution[j].con == weightContribution[i].con) {
				minWcNodes.push_back(weightContribution[j].node);
				j ++;
			}
		}
		
		int rmEle = random_choose(minWcNodes);
		eliminate_ele(minWcNodes, rmEle);
		eliminate_ele(P_l, rmEle);
		eliminate_ele(groupResult[groupId], rmEle);
		P_u.push_back(rmEle);
	}
	update_graph(groupId);
}

/*
	strip the graph
	@param {void}
	@return {void} 
*/
void strip() {
	for(int i = 0 ; i < n ; i ++) {
		if(groupResult[i].size() > m) {
			group_strip(i);
		}
		else if(groupResult[i].size() == m) {
			update_graph(i);
		}
	}
}


#endif // LPMS_STRIP_H