#include <bits/stdc++.h>
using namespace std;

/* given a flow graph and the capacities, edmonds-karp solves
 * the max-flow problem with no extra constraints */
vector<vector<int>> g; /* graph */
vector<vector<int>> c; /* capacities, this will actually hold the residual capacity during execution */
int n, source, sink;

int edmonds_karp() {
    int flow = 0;
    while(true) {
        vector<int> p(n, -1); /* parent array */
        bool found = false;
        int new_flow = INT_MAX;
        /* bfs to find shortest path from source to sink: */
        queue<pair<int,int>> q; q.push({source, INT_MAX});
        while(!q.empty()) {
            int cur = q.front().first, cur_min = q.front().second;
            q.pop();
            for(int adj : g[cur]) {
                if(p[adj] == -1 && c[cur][adj]) {
                    p[adj] = cur;
                    new_flow = min(cur_min, c[cur][adj]); /* bottleneck flow from source to adj */
                    if(adj == sink) {
                        found = true;
                        break;
                    }
                    q.push({adj, new_flow});
                }
            }
            if(found) break;
        }
        /* end bfs */
        if(!found) break;
        flow += new_flow;
        int cur = sink;
        /* update residual capacities (subtract from edges on the path and add to their reverse) */
        while(cur != source) {
            int prev = p[cur];
            c[prev][cur] -= new_flow;
            c[cur][prev] += new_flow;
            cur = prev;
        }
    }
    return flow;
}

/* add edge to flow */
void add(int u, int v, int capacity) {
    g[u].push_back(v); g[v].push_back(u);
    c[u][v] = capacity;
    c[v][u] = 0;
}

int main() {
    cin >> n;
    g.resize(n);
    c.resize(n, vector<int> (n));
    /* build flow here */
    int maxflow = edmonds_karp();
    cout << maxflow;
}
