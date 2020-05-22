/**
 * APHiD - bottom-up-aphid
 * Authors: Chris.Chen
 * Date: 10/03/2020
 * Desc: Algorithm for Placing process Hierarchically on D-ary trees
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef APHID_APHID1_H
#define APHID_APHID1_H

void update(int node, int &cnt, vector<int> &tmp ,bool mask[]) {
	if(mask[node]) {
		tmp.push_back(node);
		mask[node] = false;
		cnt ++;
		if(cnt == D) {
			groups.push_back(tmp);
			tmp.clear();
			cnt = 0;
		}
	}
}

void aphid_init() {
	groups.clear();
	picks.clear();
	for(int i = 0 ; i < taskNum - 1 ; i ++) {
		for(int j = i + 1 ; j < taskNum ; j ++) {
			if(tg[i][j] != INFINITE) {
				P p;
				p.a = i;
				p.b = j;
				p.w = tg[i][j];
				picks.push_back(p);
			}
		}
	}
	// picks_test();
	sort(picks.begin(), picks.end());
	bool mask[taskNum];
	for(int i = 0 ; i < taskNum ; i ++) {
		mask[i] = true;
	}
	int size = picks.size();
	int cnt = 0;
	vector<int> tmp;
	for(int i = 0 ; i < size ; i ++) {
		P p = picks[i];
		update(p.a, cnt, tmp, mask);
		update(p.b, cnt, tmp, mask);
	}
	if(tmp.size() != 0) {
		groups.push_back(tmp);
	}
	// group_test();	
}

int get_group_weight(int a, int b) {
	int lena = groups[a].size();
	int lenb = groups[b].size();
	int ret = 0;
	for(int i = 0 ; i < lena ; i ++) {
		for(int j = 0 ; j < lenb ; j ++) {
			int x = groups[a][i];
			int y = groups[b][j];
			if(tg[x][y] != INFINITE) {
				ret += tg[x][y];
			}
		}
	}
	return ret;
}

void merge_update(int id, vector<int> &tmp, bool mask[], int &cnt) {
	if(mask[id]) {
		vector<int> a = groups[id];
		merge_vector(tmp, a);
		cnt ++;
		mask[id] = false;
		if(cnt == D) {
			cnt = 0;
			tmpGroups.push_back(tmp);
			tmp.clear();
		}
	}
}

void merge() {
	int len = groups.size();
	picks.clear();
	for(int i = 0 ; i < len - 1 ; i ++) {
		for(int j = i + 1 ; j < len ; j ++) {
			P p;
			p.a = i;
			p.b = j;
			p.w = get_group_weight(i, j);
			picks.push_back(p);
		}
	}
	sort(picks.begin(), picks.end());
	picks_test();
	bool mask[len];
	for(int i = 0 ; i < len ; i ++) {
		mask[i] = true;
	}
	int size = picks.size();
	vector<int> tmp;
	int cnt = 0;
	tmpGroups.clear();
	for(int i = 0 ; i < size ; i ++) {
		P p = picks[i];
		merge_update(p.a, tmp, mask, cnt);
		merge_update(p.b, tmp, mask, cnt);
	}
	if(tmp.size() != 0) {
		tmpGroups.push_back(tmp);
	}
	groups.clear();
	groups = tmpGroups;
	// group_test();
}

void bottom_up_aphid() {
	aphid_init();
	
	while(groups.size() > 1) {
		merge();
	}
	result1 = groups[0];
	vector_test((char*)"result1", result1);
}

#endif // APHID_APHID1_H