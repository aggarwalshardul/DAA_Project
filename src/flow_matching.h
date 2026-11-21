/**
 * @file flow_matching.h
 * @brief Ford-Fulkerson Maximum Flow and Bipartite Matching for EV Power & Bay Allocation.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#ifndef FLOW_MATCHING_H
#define FLOW_MATCHING_H

#include "model.h"
#include "analysis.h"
#include "greedy.h"

#define FLOW_MAX_NODES 32

/* Result struct for Ford-Fulkerson Max Flow & Min-Cut */
typedef struct {
    int max_flow;
    int min_cut_edge_count;
    GraphEdge min_cut_edges[FLOW_MAX_NODES * FLOW_MAX_NODES];
} MaxFlowResult;

MaxFlowResult ford_fulkerson_max_flow(int n, const int capacity[FLOW_MAX_NODES][FLOW_MAX_NODES],
                                      int source, int sink);

/* Result struct for Maximum Bipartite Matching */
#define MATCH_MAX_ITEMS 32

typedef struct {
    int match_count;
    int vehicle_assigned_bay[MATCH_MAX_ITEMS]; /* [v] = bay */
} BipartiteMatchResult;

BipartiteMatchResult max_bipartite_matching(int num_vehicles, int num_bays,
                                           const int compat_matrix[MATCH_MAX_ITEMS][MATCH_MAX_ITEMS]);

#endif /* FLOW_MATCHING_H */
