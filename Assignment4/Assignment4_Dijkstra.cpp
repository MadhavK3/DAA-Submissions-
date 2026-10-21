#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

class TrafficNetwork
{
private:
    int V;
    vector<vector<pair<int, int>>> graph;
    vector<int> hospitals;

public:
    TrafficNetwork(int vertices)
    {
        V = vertices;
        graph.resize(V);
    }

    void addEdge(int u, int v, int weight)
    {
        graph[u].push_back({v, weight});
        graph[v].push_back({u, weight});
    }

    void updateEdgeWeight(int u, int v, int newWeight)
    {
        bool found = false;

        for (auto &edge : graph[u])
        {
            if (edge.first == v)
            {
                edge.second = newWeight;
                found = true;
            }
        }

        for (auto &edge : graph[v])
        {
            if (edge.first == u)
            {
                edge.second = newWeight;
            }
        }

        if (found)
            cout << "Edge weight updated successfully.\n";
        else
            cout << "Edge not found.\n";
    }

    void addHospital(int node)
    {
        hospitals.push_back(node);
    }

    bool isHospital(int node)
    {
        for (int hospital : hospitals)
        {
            if (hospital == node)
                return true;
        }

        return false;
    }

    void shortestPathToNearestHospital(int source)
    {
        vector<int> dist(V, INT_MAX);
        vector<int> parent(V, -1);

        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        dist[source] = 0;
        pq.push({0, source});

        int nearestHospital = -1;

        while (!pq.empty())
        {
            int currentDistance = pq.top().first;
            int u = pq.top().second;

            pq.pop();

            if (currentDistance > dist[u])
                continue;

            if (isHospital(u))
            {
                nearestHospital = u;
                break;
            }

            for (auto edge : graph[u])
            {
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] != INT_MAX &&
                    dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;

                    pq.push({dist[v], v});
                }
            }
        }

        if (nearestHospital == -1)
        {
            cout << "No hospital reachable.\n";
            return;
        }

        vector<int> path;
        int current = nearestHospital;

        while (current != -1)
        {
            path.push_back(current);
            current = parent[current];
        }

        reverse(path.begin(), path.end());

        cout << "Shortest travel time: "
             << dist[nearestHospital]
             << " minutes\n";

        cout << "Nearest hospital: "
             << nearestHospital << endl;

        cout << "Path: ";

        for (int i = 0; i < path.size(); i++)
        {
            cout << path[i];

            if (i != path.size() - 1)
                cout << " -> ";
        }

        cout << endl;
    }
};

int main()
{
    int V, E;

    cout << "=============================================\n";
    cout << "       SMART CITY TRAFFIC MANAGEMENT\n";
    cout << "             DIJKSTRA'S ALGORITHM\n";
    cout << "=============================================\n";

    cout << "\nEnter number of intersections (vertices): ";
    cin >> V;

    if (V <= 0)
    {
        cout << "Invalid number of vertices.\n";
        return 0;
    }

    TrafficNetwork city(V);

    cout << "Enter number of roads (edges): ";
    cin >> E;

    cout << "\nEnter edges (source destination travel_time):\n";

    for (int i = 0; i < E; i++)
    {
        int u, v, w;

        cin >> u >> v >> w;

        if (u < 0 || u >= V || v < 0 || v >= V || w < 0)
        {
            cout << "Invalid edge. Please enter valid values.\n";
            return 0;
        }

        city.addEdge(u, v, w);
    }

    int numHospitals;

    cout << "\nEnter number of hospitals: ";
    cin >> numHospitals;

    cout << "Enter hospital locations: ";

    for (int i = 0; i < numHospitals; i++)
    {
        int hospital;
        cin >> hospital;

        if (hospital < 0 || hospital >= V)
        {
            cout << "Invalid hospital location.\n";
            return 0;
        }

        city.addHospital(hospital);
    }

    int ambulanceLocation;

    cout << "\nEnter ambulance starting location: ";
    cin >> ambulanceLocation;

    if (ambulanceLocation < 0 || ambulanceLocation >= V)
    {
        cout << "Invalid ambulance location.\n";
        return 0;
    }

    cout << "\n---------------------------------------------\n";
    cout << "INITIAL SHORTEST ROUTE\n";
    cout << "---------------------------------------------\n";

    city.shortestPathToNearestHospital(ambulanceLocation);

    int u, v, newWeight;

    cout << "\nEnter edge to update (u v newWeight): ";
    cin >> u >> v >> newWeight;

    if (u < 0 || u >= V || v < 0 || v >= V || newWeight < 0)
    {
        cout << "Invalid edge update.\n";
        return 0;
    }

    city.updateEdgeWeight(u, v, newWeight);

    cout << "\n---------------------------------------------\n";
    cout << "AFTER REAL-TIME TRAFFIC UPDATE\n";
    cout << "---------------------------------------------\n";

    city.shortestPathToNearestHospital(ambulanceLocation);

    cout << "\n=============================================\n";
    cout << "          ROUTE COMPUTATION COMPLETED\n";
    cout << "=============================================\n";

    return 0;
}