/**
 * APHiD - top-down-aphid
 * Authors: Chris.Chen
 * Date: 10/03/2020
 * Desc: Algorithm for Placing process Hierarchically on D-ary trees
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef APHID_APHID2_H
#define APHID_APHID2_H

void shuffle(vector<int> &s) {
	int size = s.size();
	for(int i = size - 1 ; i > 0 ; i --) {
		int j = randint(i);
		int tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}
}

int get_total_weight(vector<int> v[N]) {
	int ret = 0;
	for(int i = 0 ; i < D ; i ++) {
		int size = v[i].size();
		for(int j = 0 ; j < size - 1 ; j ++) {
			for(int k = j + 1 ; k < size ; k ++) {
				if(tg[v[i][j]][v[i][k]] != INFINITE) {
					ret += tg[v[i][j]][v[i][k]];
				}
			}	
		}
	}
	return ret;
}

void forward(vector<int> v[N], int cnt) {
	int beg = v[0][cnt];
	int i;
	for(i = 0 ; i < D - 1 ; i ++) {
		if(v[i + 1].size() <= cnt) break;
		v[i][cnt] = v[i + 1][cnt];
	}
	v[i][cnt] = beg;
}

void roll_back(vector<int> v[N], int cnt) {
	int end;
	int i;
	for(i = D - 1 ; i > 0 ; i --) {
		if(v[i].size() <= cnt) continue;
		end = v[i][cnt];
		break;
	}
	for(; i > 0 ; i --) {
		v[i][cnt] = v[i - 1][cnt];
	}
	v[0][cnt] = end;
}



void adjust_groups(vector<int> v[N]) {
	int curTotal, nextTotal;
	int cnt = 0;
	do {
		do {
			curTotal = get_total_weight(v);
			forward(v, cnt);
			nextTotal = get_total_weight(v);
		} while(curTotal < nextTotal);
		roll_back(v, cnt);
		cnt ++;
	} while(cnt < v[0].size() - 1);
}

vector<int> t;
void divide(vector<int> s) {
	int size = s.size();
	vector<int> dGroups[N]; // divide groups
	
	if(size == D) {
		groups.push_back(s);
		return;
	}
	else if(size < D) {
		merge_vector(t, s);
		if(t.size() == D) {
			groups.push_back(t);
			t.clear();
		}
		else if(t.size() > D) {
			vector<int> tt;
			for(int i = 0 ; i < D ; i ++) {
				tt.push_back(t[i]);
			}
			groups.push_back(tt);
			t.erase(t.begin(), t.begin() + D);
		}
		return;
	}
	else {
		shuffle(s);
		for(int i = 0 ; i < D ; i ++) {
			dGroups[i].clear();
		}
		for(int i = 0, j = 0 ; i < size ; i ++ , j ++) {
			dGroups[j % D].push_back(s[i]);
		}
		adjust_groups(dGroups);
		v_test(dGroups);
		for(int i = 0 ; i < D ; i ++) {
			divide(dGroups[i]);
		}
	}
}

void top_down_aphid() {
	vector<int> s;
	for(int i = 0 ; i < taskNum ; i ++) {
		s.push_back(i);
	}
	divide(s);
	// group_test();
	int size=  groups.size();
	for(int i = 0 ; i < size ; i ++) {
		merge_vector(result2, groups[i]);
	}
	vector_test((char *)"result2", result2);
}

#endif // APHID_APHID2_H