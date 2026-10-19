#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
using namespace std;

const int INF = 1000000000;

struct Node {
    vector<vector<int>> matrix;
    vector<int> path;
    int level;
    int bound;
    int cost;
};

struct Compare {
    bool operator()(const Node& a, const Node& b) const {
        return a.bound > b.bound;
    }
};

int reduceMatrix(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int reductionCost = 0;

    for (int i = 0; i < n; i++) {
        int minimum = INF;

        for (int j = 0; j < n; j++) {
            minimum = min(minimum, matrix[i][j]);
        }

        if (minimum != INF && minimum > 0) {
            reductionCost += minimum;

            for (int j = 0; j < n; j++) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= minimum;
                }
            }
        }
    }

    for (int j = 0; j < n; j++) {
        int minimum = INF;

        for (int i = 0; i < n; i++) {
            minimum = min(minimum, matrix[i][j]);
        }

        if (minimum != INF && minimum > 0) {
            reductionCost += minimum;

            for (int i = 0; i < n; i++) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= minimum;
                }
            }
        }
    }

    return reductionCost;
}

bool containsCity(const vector<int>& path, int city) {
    return find(path.begin(), path.end(), city) != path.end();
}

int main() {
    int n;

    cout << "===== TSP - Least Cost Branch and Bound =====\n";
    cout << "Enter number of cities: ";
    cin >> n;

    if (n < 2 || n > 15) {
        cout << "Enter between 2 and 15 cities.\n";
        return 0;
    }

    vector<string> cityNames(n);
    vector<vector<int>> cost(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        cout << "Enter name of city " << i + 1 << ": ";
        cin >> cityNames[i];
    }

    cout << "\nEnter the cost matrix (-1 for no direct route):\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "Cost from " << cityNames[i] << " to "
                 << cityNames[j] << ": ";
            cin >> cost[i][j];

            if (i == j) {
                cost[i][j] = INF;
            } else if (cost[i][j] == -1) {
                cost[i][j] = INF;
            } else if (cost[i][j] < 0) {
                cout << "Invalid cost.\n";
                return 0;
            }
        }
    }

    int start;
    cout << "\nChoose starting city (1-" << n << "): ";
    cin >> start;

    if (start < 1 || start > n) {
        cout << "Invalid starting city.\n";
        return 0;
    }

    start--;

    vector<vector<int>> rootMatrix = cost;
    int initialBound = reduceMatrix(rootMatrix);

    Node root;
    root.matrix = rootMatrix;
    root.path = {start};
    root.level = 0;
    root.bound = initialBound;
    root.cost = 0;

    priority_queue<Node, vector<Node>, Compare> pq;
    pq.push(root);

    int bestCost = INF;
    vector<int> bestPath;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.bound >= bestCost) {
            continue;
        }

        int u = current.path.back();

        if (current.level == n - 1) {
            if (cost[u][start] != INF &&
                current.cost + cost[u][start] < bestCost) {
                bestCost = current.cost + cost[u][start];
                bestPath = current.path;
                bestPath.push_back(start);
            }
            continue;
        }

        for (int v = 0; v < n; v++) {
            if (containsCity(current.path, v) ||
                cost[u][v] == INF ||
                current.matrix[u][v] == INF) {
                continue;
            }

            Node child;
            child.matrix = current.matrix;
            child.path = current.path;
            child.path.push_back(v);
            child.level = current.level + 1;
            child.cost = current.cost + cost[u][v];

            for (int j = 0; j < n; j++) {
                child.matrix[u][j] = INF;
                child.matrix[j][v] = INF;
            }

            child.matrix[v][start] = INF;

            int reductionCost = reduceMatrix(child.matrix);
            child.bound = child.cost + reductionCost;

            if (child.bound < bestCost) {
                pq.push(child);
            }
        }
    }

    cout << "\n===== Optimal Delivery Route =====\n";

    if (bestPath.empty()) {
        cout << "No complete tour exists for the given cost matrix.\n";
        return 0;
    }

    for (int i = 0; i < (int)bestPath.size(); i++) {
        cout << cityNames[bestPath[i]];

        if (i + 1 < (int)bestPath.size()) {
            cout << " -> ";
        }
    }

    cout << "\nMinimum Total Cost: " << bestCost << '\n';
    cout << "Number of Cities Visited: " << n << '\n';

    cout << "\nAlgorithm: Least Cost Branch and Bound\n";
    cout << "Worst-Case Time Complexity: O(N! * N^2)\n";

    return 0;
}
