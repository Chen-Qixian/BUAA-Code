/**
 * QTLS Algorithm - strip
 * Author: Chris.Chen
 * Date: 06/03/2020
 * Desc: Quadratic Time Complexity Locality Detection Algorithm for Shared HPC Systems
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */
#ifndef QTLS_STRIP_H
#define QTLS_STRIP_H

using namespace std;

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
			e.erase(it);
			it --;
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
	vector<int> curGroup = P_g[groupId];
	int size = curGroup.size();
	for(int i = 0 ; i < size ; i ++) {
		for(int j = 0 ; j < procNum ; j ++) {
			// node belongs to P_f itself should be clear to UNDEFINED
			if(curGroup[i] == j) {
				edge[j].erase(edge[j].begin(), edge[j].end());
				continue;
			}
			// two nodes should have connection
			else if(graph[curGroup[i]][j] != INFINITE) {
				graph[curGroup[i]][j] = INFINITE;
				graph[j][curGroup[i]] = INFINITE;
				eliminate_edge(edge[j], curGroup[i]);
			}
		}
	}
	P_g[groupId].clear(); // remove P_f from merge scheme selection
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
	set up roll back new groups
	@param {PII} max merge
	@return {void}
*/
void set_new_groups(vector<int> g) {
	int size = g.size();
	if(size == 0) return;
	int dst = g[0];
	for(int i = 0 ; i < size ; i ++) {
		P_g[dst].push_back(g[i]);
	}
}

/*
	roll back main function
	@param {vector<int>} remove node list
	@return {void}
*/
void roll_back(vector<int> rmNodes) {
	int size = rmNodes.size();
	vector<int> g1;
	vector<int> g2;
	for(int i = 0 ; i < size ; i ++) {
		if(find(lastPga.begin(), lastPga.end(), rmNodes[i]) != lastPga.end()) {
			g1.push_back(rmNodes[i]);
		}
		else if(find(lastPgb.begin(), lastPgb.end(), rmNodes[i]) != lastPgb.end()) {
			g2.push_back(rmNodes[i]);
		}
	}
	set_new_groups(g1);
	set_new_groups(g2);
}

/*
	add P_f in v to result[dst]
	@param {vector<int>} P_f
	@param {int} destination result group
	@return {void}
*/
void add_to_result(vector<int> v, int dst) {
	int size = v.size();
	for(int i = 0 ; i < size ; i ++) {
		result[dst].push_back(v[i]);
	}
}

/*
	update sub graph(remove P_f) and rollback removed nodes
	@param {vector<int>} removed nodes list
	@param {int} groupId
	@return {void}
*/
void justify_sub_graph(vector<int> rmNodes,int groupId) {
	update_graph(groupId);
	// printf("allocate to:%d\n", m0.id);
	// vector_test((char*)"result:", result[m0.id]);
	if(find(rmNodes.begin(), rmNodes.end(), groupId) != rmNodes.end()) {
		int dst = P_g[groupId][0];
		int size = P_g[groupId].size();
		for(int i = 0 ; i < size ; i ++) {
			P_g[dst].push_back(P_g[groupId][i]);
		}
		P_g[groupId].clear();
	}
	roll_back(rmNodes);
	pg_test();
	graph_test();
}

/*
	remove nodes according to weight contribution sort result
	@param {vector<WeiCon>} weight contribution
	@param {vector<int>&} remove node list(needs to be modified)
	@param {vector<int>} current P_g
	@param {int} max core number of calculation node list
	@param {int} current P_g id
	@return {void}
*/
void remove_nodes(vector<WeiCon> weightContribution, vector<int>& rmNodes, vector<int> curGroup, int limit, int groupId) {
	int t = curGroup.size();
	int rmNum = t - limit;
	vector<int> minWcNodes; // for randomly choose a node to eliminate when their weight contribution is equal
	minWcNodes.clear();
	rmNodes.clear();
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
		// printf("\n===randomly choose remove element: %d; size = %d\n", rmEle, minWcNodes.size());
		eliminate_ele(minWcNodes, rmEle);  // remove rmEle from minWcNodes list
		eliminate_ele(P_g[groupId], rmEle); // remove rmEle from P_g
		rmNodes.push_back(rmEle);
	}
}

/*
	strip sub graph
	@param {PII} max merge
	@param {Cores} calculation node with most cores
	@return {void}
*/
void strip_sub_graph(PII maxMerge, Cores m0) {
	int groupId = maxMerge.first;
	vector<int> curGroup = P_g[groupId];
	vector<WeiCon> weightContribution;
	vector<int> rmNodes; // nodes to be removed from this group
	weightContribution = get_inter_weight_contribution(curGroup);

	sort(weightContribution.begin(), weightContribution.end());
	
	remove_nodes(weightContribution, rmNodes, curGroup, m0.num, groupId);
	add_to_result(P_g[groupId], m0.id);
	justify_sub_graph(rmNodes, groupId);
}

/*
	strip main function
	@param {PII} max merge
	@return {void}
*/
void strip(PII maxMerge) {
	int groupId = maxMerge.first;
	vector<int> curGroup = P_g[groupId]; // merged group root
	int size = curGroup.size();
	if(size > m[0].num) { // need to strip
		// printf("greater\n");
		strip_sub_graph(maxMerge, m[0]);
		m.erase(m.begin());
	}
	else if (size < m[m.size()-1].num) { // do nothing
		// printf("strip do nothing\n");
		return;	
	}
	else{ // remove sub graph
		for(auto it = m.begin(); it != m.end() ; it ++) {
			if(size == it->num) {
				add_to_result(curGroup, it->id);
				// printf("\ngroup:%d\n", it->id);
				// vector_test((char*)"@strip: result", result[it->id]);
				update_graph(groupId);
				// graph_test();
				m.erase(it);
				it --;
			}
		}
	}
}

#endif // QTLS_STRIP_H
