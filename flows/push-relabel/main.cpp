#include <bits/stdc++.h>
using namespace std;

/* given a flow, push-relabel solves the standard max flow problem
 * this is the improved version, where only greatest height vertices are
 * considered for operations push and relabel
 * every iteration find greatest heights vertices with excess, from these try to push flow to
 * neighbour vertices, if impossible relabel it (increase height)
 * flow can only go through edges connecting vertices with h1 - h2 == 1
 * while ford-fulkerson always maintains a valid flow and converges to the max flow,
 * push-relabel always maintains the max flow and converges to a valid flow. */

vector<vector<int>> g;
vector<vector<int>> capacity; /* actually holds residual flow capacities */
vector<int> height, excess;
int n, source, sink;

/* push flow through a vertex with excess */
void push(int u, int v) {
    int d = min(excess[u], capacity[u][v]); /* minimum between excess and residual capacity */
    capacity[u][v] -= d;
    capacity[v][u] += d;
    excess[u] -= d;
    excess[v] += d;
}

/* relabel vertex - height = 1 + min(height[adj], capacity[u][adj] > 0) */
void relabel(int u) {
    int d = INT_MAX;
    for(int adj : g[u]) {
        if(capacity[u][adj]) {
            d = min(d, height[adj]);
        }
    }
    if(d < INT_MAX) height[u] = d + 1;
}

/* find current vertices with max height */
vector<int> get_max_height_vertices() {
    vector<int> max_height;
    for (int i = 0; i < n; ++i) {
        if(i != source && i != sink && excess[i] > 0) {
            if(!max_height.empty() && height[i] > height[max_height[0]])
                max_height = vector<int> ();
            if(max_height.empty() || height[i] == height[max_height[0]])
                max_height.push_back(i);
        }
    }
    return max_height;
}

int push_relabel() {
    height[source] = n;
    excess[source] = INT_MAX;
    for(int adj : g[source]) push(source, adj);

    vector<int> max_height;
    while(!(max_height = get_max_height_vertices()).empty()) {
        for(int i : max_height) {
            bool pushed = false;
            for(int adj : g[i]) {
                if(!excess[i]) break;
                if(capacity[i][adj] && height[i] == height[adj] + 1) {
                    push(i, adj);
                    pushed = true;
                }
            }
            if(!pushed) {
                relabel(i);
                break; /* once a vertex is relabeled we need to recompute max_height */
            }
        }
    }

    int flow = 0;
    for(int adj : g[source]) {
        flow += capacity[adj][source]; /* flow exiting the source == inverse edges in residual flow */
    }
    return flow;
}

/* add edge to flow */
void add(int u, int v, int cap) {
    g[u].push_back(v), g[v].push_back(u);
    capacity[u][v] = cap, capacity[v][u] = 0;
}

int main() {
    cin >> n;
    g.resize(n);
    height.resize(n); excess.resize(n);
    capacity.resize(n, vector<int>(n));
    /* build flow */
    int max_flow = push_relabel();
    cout << max_flow;
}
