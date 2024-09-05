#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Structure to represent an edge in the graph (flight)
struct Flight {
    int destination;
    int distance; // Weight (could represent cost, time, etc.)
    Flight(int dest, int dist) : destination(dest), distance(dist) {}
};

// Comparator for the priority queue (min-heap)
struct Compare {
    bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) {
        return p1.second > p2.second;
    }
};

// Graph class for representing the flight network
class FlightGraph {
public:
    unordered_map<int, vector<Flight>> adjList; // Adjacency list

    // Add a flight between two cities (undirected graph)
    void addFlight(int source, int destination, int distance) {
        adjList[source].emplace_back(destination, distance);
        adjList[destination].emplace_back(source, distance); // If the flights are bidirectional
    }

    // Dijkstra's algorithm to find the shortest path from source to destination
    vector<int> findShortestRoute(int source, int destination) {
        unordered_map<int, int> dist; // Distance map
        unordered_map<int, int> parent; // To store the path
        priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> pq;

        // Initialize distances
        for (auto& pair : adjList) {
            dist[pair.first] = INT_MAX;
        }
        dist[source] = 0;
        pq.push({source, 0});
        parent[source] = -1;

        while (!pq.empty()) {
            int current = pq.top().first;
            pq.pop();

            // If we've reached the destination, break
            if (current == destination) {
                break;
            }

            // Explore all the neighbors
            for (Flight& flight : adjList[current]) {
                int next = flight.destination;
                int weight = flight.distance;
                int newDist = dist[current] + weight;

                // If a shorter path is found
                if (newDist < dist[next]) {
                    dist[next] = newDist;
                    pq.push({next, newDist});
                    parent[next] = current;
                }
            }
        }

        // Backtrack to find the path
        vector<int> path;
        for (int at = destination; at != -1; at = parent[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        return path;
    }
};

// Helper function to print the route
void printRoute(const vector<int>& route) {
    for (int city : route) {
        cout << city << " ";
    }
    cout << endl;
}

int main() {
    FlightGraph graph;

    // Add flights (example: cities represented as numbers)
    graph.addFlight(1, 2, 5);  // Flight from city 1 to city 2 with distance 5
    graph.addFlight(1, 3, 10); // Flight from city 1 to city 3 with distance 10
    graph.addFlight(2, 3, 3);  // Flight from city 2 to city 3 with distance 3
    graph.addFlight(2, 4, 8);  // Flight from city 2 to city 4 with distance 8
    graph.addFlight(3, 4, 2);  // Flight from city 3 to city 4 with distance 2

    int source = 1;
    int destination = 4;

    // Find the shortest route
    vector<int> route = graph.findShortestRoute(source, destination);

    // Print the route
    cout << "Shortest route from " << source << " to " << destination << ": ";
    printRoute(route);

    return 0;
}
