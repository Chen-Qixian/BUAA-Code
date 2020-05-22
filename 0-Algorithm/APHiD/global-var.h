/**
 * APHiD - global-var
 * Authors: Chris.Chen
 * Date: 10/03/2020
 * Desc: Algorithm for Placing process Hierarchically on D-ary trees
 * Copyright (c) 2020 chenqixian@buaa.edu.cn. All rights reserved.
 */

#ifndef APHID_GLOBAL_VAR_H
#define APHID_GLOBAL_VAR_H
#include <vector>
#include <cstring>

#define N 1024
#define INFINITE -1

#define BOTTOM_UP 1
#define TOP_DOWN 2

using namespace std;

typedef pair<int, int> PII;

struct Edge {
	int nextNode;
	int cost;
};

struct P {
	int a; // task a
	int b; // task b
	int w; // weight

	bool operator < (const P &p) const {
		return p.w < w;
	}

};

int tg[N][N]; // task graph
vector<Edge> Tg[N]; // save adjacent
int taskNum;
int D;
vector<int> result1;
vector<int> result2;
vector<P> picks;
vector<vector<int> > groups; // 2D result groups
vector<vector<int> > tmpGroups; // tmp 2D result groups

#endif //APHID_GLOBAL_VAR_H
