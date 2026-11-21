/**
 * @file greedy.h
 * @brief Greedy algorithms: Greedy Knapsack and Dijkstra Shortest Path.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 */

#ifndef GREEDY_H
#define GREEDY_H

#include "model.h"
#include "analysis.h"
#include "dp.h"

/**
 * @brief Shortest path result structure for Dijkstra.
 */
typedef struct {
    int source;
    int dest;
    int dist[MAX_NODES];
    int parent[MAX_NODES];
    int path[MAX_NODES];
    int path_length;
    int is_reachable;
} DijkstraResult;

/* Greedy Knapsack for Station Placement (ratio-based heuristic) */
KnapsackResult knapsack_greedy_ratio(const Station stations[], int count, int budget);

/* Dijkstra Shortest Path - Array based O(V^2) */
DijkstraResult dijkstra_array(const CityNetwork *city, int src, int dest);

/* Dijkstra Shortest Path - Min-Heap based O((V + E) log V) */
DijkstraResult dijkstra_heap(const CityNetwork *city, int src, int dest);

/* Path utility */
void dijkstra_print_path(const CityNetwork *city, const DijkstraResult *res);

/* EV Feasibility checker for a computed path */
int is_path_ev_feasible(const CityNetwork *city,
                        const DijkstraResult *route,
                        double vehicle_range,
                        double *out_battery_used);

#endif /* GREEDY_H */
