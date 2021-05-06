#include <bits/stdc++.h>
using namespace std;

/* find a minimum cost flow with value K. if k is infinity,
 * the problem is min-cost max-flow.
 * does shortest paths like edmonds-karp but using costs,
 * in this case uses spfa - extension of bellman-ford */

vector<vector<int>> g;
vector<vector<int>> cost, capacity;
int n, k, source, sink;

int minimum_cost() {
    int flow = 0, cst = 0;
    while(flow < k) {
        /* spfa to find shortest cost path in flow */
        vector<int> d(n, INT_MAX), p(n, -1); d[source] = 0; /* distance and parent */
        vector<bool> inqueue(n, false);
        queue<int> q; q.push(source);
        while(!q.empty()) {
            int v = q.front(); q.pop();
            inqueue[v] = false;
            for(int adj : g[v]) {
                if(capacity[v][adj] && d[adj] > d[v] + cost[v][adj]) {
                    d[adj] = d[v] + cost[v][adj];
                    p[adj] = v;
                    if(!inqueue[adj]) {
                        inqueue[adj] = true;
                        q.push(adj);
                    }
                }
            }
        }
        /* end spfa */
        if(p[sink] == -1) break; /* no more augmentation paths */
        /* find max flow not exceeding k - flow */
        int f = k - flow;
        int cur = sink;
        while(cur != source) {
            f = min(f, capacity[p[cur]][cur]);
            cur = p[cur];
        }
        /* apply flow */
        flow += f;
        cst += f * d[sink]; /* update cost function accordingly */
        cur = sink;
        while(cur != source) {
            capacity[p[cur]][cur] -= f;
            capacity[cur][p[cur]] += f;
            cur = p[cur];
        }
    }
    if(flow < k) return -1;
    else return cst;
}

/* add edge to flow */
void add(int u, int v, int cst, int cap) {
    g[u].push_back(v), g[v].push_back(u);
    cost[u][v] = cst, cost[v][u] = -cst;
    capacity[u][v] = cap, capacity[v][u] = 0;
}

int main() {
    cin >> n >> k;
    g.resize(n);
    cost.resize(n, vector<int> (n, 0));
    capacity.resize(n, vector<int> (n, 0));
    /* build the flow */
    int min_cost = minimum_cost();
    cout << min_cost;
}
