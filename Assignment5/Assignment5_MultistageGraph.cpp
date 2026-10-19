#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

const double INF = numeric_limits<double>::infinity();

struct Edge {
    int to;
    double base_cost;
    double cur_cost;
};

int main() {
    int S;

    cout << "Enter number of stages: ";
    cin >> S;

    if (S < 2) {
        cout << "A multistage graph must have at least 2 stages.\n";
        return 0;
    }

    vector<int> stage_count(S);
    vector<int> stage_start(S + 1, 0);

    cout << "Enter number of nodes in each stage ("
         << S << " values): ";

    for (int i = 0; i < S; i++) {
        cin >> stage_count[i];

        if (stage_count[i] <= 0) {
            cout << "Each stage must contain at least one node.\n";
            return 0;
        }

        stage_start[i + 1] = stage_start[i] + stage_count[i];
    }

    int N = stage_start[S];

    vector<int> stage_of(N);

    for (int s = 0; s < S; s++) {
        for (int i = stage_start[s]; i < stage_start[s + 1]; i++) {
            stage_of[i] = s;
        }
    }

    vector<vector<Edge>> adj(N);
    vector<vector<int>> rev_adj(N);

    int M;
    cout << "Enter number of edges: ";
    cin >> M;

    if (M < 0) {
        cout << "Number of edges cannot be negative.\n";
        return 0;
    }

    cout << "Enter each edge as: u v cost\n";

    for (int i = 0; i < M; i++) {
        int u, v;
        double cost;
        cin >> u >> v >> cost;

        if (u < 0 || u >= N || v < 0 || v >= N ||
            stage_of[v] != stage_of[u] + 1 || cost < 0) {
            cout << "Invalid edge. Use valid node IDs, "
                 << "non-negative costs, and edges between "
                 << "consecutive stages.\n";
            return 0;
        }

        adj[u].push_back({v, cost, cost});
        rev_adj[v].push_back(u);
    }

    vector<double> best_cost(N, INF);
    vector<int> next_node(N, -1);

    for (int i = stage_start[S - 1]; i < N; i++) {
        best_cost[i] = 0.0;
    }

    for (int s = S - 2; s >= 0; s--) {
        for (int u = stage_start[s];
             u < stage_start[s + 1]; u++) {

            for (const Edge &e : adj[u]) {
                if (best_cost[e.to] == INF)
                    continue;

                double candidate = e.cur_cost + best_cost[e.to];

                if (candidate < best_cost[u]) {
                    best_cost[u] = candidate;
                    next_node[u] = e.to;
                }
            }
        }
    }

    auto print_stage0_costs = [&]() {
        cout << fixed << setprecision(6);
        cout << "\nBest costs from Stage-0 nodes:\n";

        for (int i = stage_start[0];
             i < stage_start[1]; i++) {

            cout << "Node " << i << ": cost = ";

            if (best_cost[i] == INF)
                cout << "INF\n";
            else
                cout << best_cost[i] << '\n';
        }
    };

    print_stage0_costs();

    int src;
    cout << "\nEnter a source node id (in stage 0) "
         << "to print path, or -1 to skip: ";
    cin >> src;

    if (src != -1) {
        if (src < stage_start[0] || src >= stage_start[1]) {
            cout << "Invalid source. Choose a node from stage 0.\n";
        } else if (best_cost[src] == INF) {
            cout << "No path from source " << src
                 << " to the final stage.\n";
        } else {
            cout << "Path from " << src << " : ";

            int current = src;
            cout << current;

            while (next_node[current] != -1) {
                current = next_node[current];
                cout << " -> " << current;
            }

            cout << "\nTotal route cost (sum edges): "
                 << best_cost[src] << '\n';
        }
    }

    int Q;
    cout << "\nEnter number of live updates to edge costs "
         << "(0 to finish): ";
    cin >> Q;

    for (int q = 0; q < Q; q++) {
        int u, v;
        double multiplier;

        cout << "Enter edge update (u v multiplier): ";
        cin >> u >> v >> multiplier;

        if (u < 0 || u >= N || v < 0 || v >= N ||
            multiplier < 0) {
            cout << "Invalid update.\n";
            continue;
        }

        bool found = false;

        for (Edge &e : adj[u]) {
            if (e.to == v) {
                e.cur_cost = e.base_cost * multiplier;
                found = true;
            }
        }

        if (!found) {
            cout << "Edge " << u << " -> " << v
                 << " not found.\n";
            continue;
        }

        
        queue<int> pending;
        vector<bool> in_queue(N, false);

        auto recompute_node = [&](int node) {
            double new_cost = INF;
            int new_next = -1;

            for (const Edge &e : adj[node]) {
                if (best_cost[e.to] == INF)
                    continue;

                double candidate = e.cur_cost + best_cost[e.to];

                if (candidate < new_cost) {
                    new_cost = candidate;
                    new_next = e.to;
                }
            }

            bool changed = (new_cost != best_cost[node]);

            best_cost[node] = new_cost;
            next_node[node] = new_next;

            return changed;
        };

        pending.push(u);
        in_queue[u] = true;

        while (!pending.empty()) {
            int node = pending.front();
            pending.pop();
            in_queue[node] = false;

            bool changed = recompute_node(node);

            if (changed) {
                for (int predecessor : rev_adj[node]) {
                    if (!in_queue[predecessor]) {
                        pending.push(predecessor);
                        in_queue[predecessor] = true;
                    }
                }
            }
        }
    }

    if (Q > 0) {
        cout << "\nAfter updates, ";
        print_stage0_costs();
    }

    return 0;
}
