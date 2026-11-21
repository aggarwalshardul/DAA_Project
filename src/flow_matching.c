/**
 * @file flow_matching.c
 * @brief Implementation of Ford-Fulkerson Max Flow, Min-Cut, and Maximum Bipartite Matching.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "flow_matching.h"

/* -------------------------------------------------------------
 * 1. FORD-FULKERSON MAXIMUM FLOW & MIN-CUT
 * ------------------------------------------------------------- */

static int dfs_augmenting_path(int n, int r_graph[FLOW_MAX_NODES][FLOW_MAX_NODES],
                               int s, int t, int visited[], int parent[]) {
    visited[s] = 1;
    ops_increment(1);
    if (s == t) return 1;

    for (int v = 0; v < n; v++) {
        if (!visited[v] && r_graph[s][v] > 0) {
            parent[v] = s;
            if (dfs_augmenting_path(n, r_graph, v, t, visited, parent)) {
                return 1;
            }
        }
    }
    return 0;
}

static void dfs_min_cut(int n, int r_graph[FLOW_MAX_NODES][FLOW_MAX_NODES],
                        int s, int visited[]) {
    visited[s] = 1;
    for (int v = 0; v < n; v++) {
        if (!visited[v] && r_graph[s][v] > 0) {
            dfs_min_cut(n, r_graph, v, visited);
        }
    }
}

/**
 * Computes Maximum Flow and Minimum Cut transmission lines using Ford-Fulkerson.
 * Time: O(E * max_flow), Space: O(V^2).
 */
MaxFlowResult ford_fulkerson_max_flow(int n, const int capacity[FLOW_MAX_NODES][FLOW_MAX_NODES],
                                      int source, int sink) {
    MaxFlowResult res;
    res.max_flow = 0;
    res.min_cut_edge_count = 0;

    int r_graph[FLOW_MAX_NODES][FLOW_MAX_NODES];
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            r_graph[u][v] = capacity[u][v];
        }
    }

    int parent[FLOW_MAX_NODES];
    int visited[FLOW_MAX_NODES];

    for (int i = 0; i < n; i++) visited[i] = 0;
    while (dfs_augmenting_path(n, r_graph, source, sink, visited, parent)) {
        int path_flow = INF_DIST;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            if (r_graph[u][v] < path_flow) path_flow = r_graph[u][v];
            ops_increment(1);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            r_graph[u][v] -= path_flow;
            r_graph[v][u] += path_flow;
            ops_increment(2);
        }

        res.max_flow += path_flow;
        for (int i = 0; i < n; i++) visited[i] = 0;
    }

    /* Find Minimum Cut edges (from S to T) */
    for (int i = 0; i < n; i++) visited[i] = 0;
    dfs_min_cut(n, r_graph, source, visited);

    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (visited[u] && !visited[v] && capacity[u][v] > 0) {
                res.min_cut_edges[res.min_cut_edge_count].u = u;
                res.min_cut_edges[res.min_cut_edge_count].v = v;
                res.min_cut_edges[res.min_cut_edge_count].weight = capacity[u][v];
                res.min_cut_edge_count++;
            }
        }
    }
    return res;
}

/* -------------------------------------------------------------
 * 2. MAXIMUM BIPARTITE MATCHING (VEHICLES <-> CHARGING BAYS)
 * ------------------------------------------------------------- */

static int bpm_dfs(int u, int num_bays,
                   const int compat[MATCH_MAX_ITEMS][MATCH_MAX_ITEMS],
                   int seen[], int match_bay_to_veh[]) {
    for (int v = 0; v < num_bays; v++) {
        ops_increment(1);
        if (compat[u][v] && !seen[v]) {
            seen[v] = 1;
            /* If bay v is free OR the assigned vehicle can find another bay */
            if (match_bay_to_veh[v] < 0 ||
                bpm_dfs(match_bay_to_veh[v], num_bays, compat, seen, match_bay_to_veh)) {
                match_bay_to_veh[v] = u;
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Computes maximum assignment of vehicles to charging bays using augmenting paths.
 * Time: O(V * E), Space: O(V).
 */
BipartiteMatchResult max_bipartite_matching(int num_vehicles, int num_bays,
                                           const int compat[MATCH_MAX_ITEMS][MATCH_MAX_ITEMS]) {
    BipartiteMatchResult res;
    res.match_count = 0;
    for (int i = 0; i < MATCH_MAX_ITEMS; i++) {
        res.vehicle_assigned_bay[i] = -1;
    }

    int match_bay_to_veh[MATCH_MAX_ITEMS];
    for (int i = 0; i < MATCH_MAX_ITEMS; i++) match_bay_to_veh[i] = -1;

    for (int u = 0; u < num_vehicles; u++) {
        int seen[MATCH_MAX_ITEMS] = {0};
        if (bpm_dfs(u, num_bays, compat, seen, match_bay_to_veh)) {
            res.match_count++;
        }
    }

    /* Convert bay->veh mapping to veh->bay */
    for (int v = 0; v < num_bays; v++) {
        if (match_bay_to_veh[v] >= 0) {
            res.vehicle_assigned_bay[match_bay_to_veh[v]] = v;
        }
    }
    return res;
}
