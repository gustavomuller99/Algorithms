#include <bits/stdc++.h>
using namespace std;

/* given a flow, dinics solves the standard max flow problem
 * very quickly - it works by building a layered graph each iteration and
 * finding a blocking flow each iteration. */

vector<vector<int>> g;
vector<vector<int>> capacity;
vector<int> level;
int n, source, sink;

/* build layered graph */
bool bfs() {
    level = vector<int> (n, -1);
    level[source] = 0;
    queue<int> q; q.push(source);
    while(!q.empty()) {
        int u = q.front(); q.pop();
        for(int adj : g[u]) {
            if(capacity[u][adj] && level[adj] == -1) {
                level[adj] = level[u] + 1;
                q.push(adj);
            }
        }
    }
    return level[sink] != -1;
}

/* find any path to sink */
pair<int,bool> dfs(int u = source, int pushed = INT_MAX) {
    if(u == sink) return {true, pushed};
    for(int adj : g[u]) {
        if(capacity[u][adj] && level[adj] == level[u] + 1) {
            auto r = dfs(adj, min(pushed, capacity[u][adj]));
            if(r.second) {
                capacity[u][adj] -= r.first;
                capacity[adj][u] += r.first;
                return r;
            }
        }
    }
    return {false, 0};
}

int dinics() {
    int flow = 0;
    bfs();
    while(bfs()) {
        while(true) { /* while a blocking flow is not found */
            auto r = dfs();
            if(!r.second) break;
            flow += r.first;
        }
    }
    return flow;
}

/* add edge to flow */
void add(int u, int v, int cap) {
    g[u].push_back(v), g[v].push_back(u);
    capacity[u][v] = cap; capacity[v][u] = 0;
}

int main() {
    cin >> n;
    g.resize(n);
    capacity.resize(n, vector<int>(n));
    /* build flow */
    int max_flow = dinics();
    cout << max_flow;
}
