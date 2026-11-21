/**
 * @file dp.h
 * @brief Comprehensive Dynamic Programming suite for Optimization, Paths, and Sequences.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#ifndef DP_H
#define DP_H

#include "model.h"
#include "analysis.h"

/* -------------------------------------------------------------
 * 1. 0/1 KNAPSACK FOR STATION PLACEMENT
 * ------------------------------------------------------------- */
typedef struct {
    int max_benefit;
    int total_cost;
    int selected_count;
    int selected_indices[MAX_STATIONS];
} KnapsackResult;

KnapsackResult knapsack_01_dp(const Station stations[], int count, int budget);
void knapsack_print_table(const Station stations[], int count, int budget);

/* -------------------------------------------------------------
 * 2. MATRIX CHAIN MULTIPLICATION (MCM)
 * ------------------------------------------------------------- */
#define MCM_MAX 16

typedef struct {
    int min_scalar_mults;
    int s_table[MCM_MAX][MCM_MAX];
    char parenthesization[128];
} MCMResult;

MCMResult matrix_chain_order(const int p[], int n);

/* -------------------------------------------------------------
 * 3. LONGEST COMMON SUBSEQUENCE (LCS)
 * ------------------------------------------------------------- */
#define LCS_MAX 64

typedef struct {
    int lcs_length;
    int lcs_sequence[LCS_MAX];
} LCSResult;

LCSResult lcs_route_comparison(const int routeA[], int lenA,
                               const int routeB[], int lenB);

/* -------------------------------------------------------------
 * 4. OPTIMAL BINARY SEARCH TREE (OBST)
 * ------------------------------------------------------------- */
#define OBST_MAX 16

typedef struct {
    double min_cost;
    int root[OBST_MAX][OBST_MAX];
} OBSTResult;

OBSTResult optimal_bst(const int keys[], const double p[], const double q[], int n);

/* -------------------------------------------------------------
 * 5. ALL-PAIRS & NEGATIVE CYCLE PATHS (BELLMAN-FORD & FLOYD-WARSHALL)
 * ------------------------------------------------------------- */
typedef struct {
    int has_negative_cycle;
    int dist[MAX_NODES];
    int parent[MAX_NODES];
} BellmanFordResult;

BellmanFordResult bellman_ford(const CityNetwork *city, int src);

typedef struct {
    int dist[MAX_NODES][MAX_NODES];
    int next[MAX_NODES][MAX_NODES];
} FloydWarshallResult;

FloydWarshallResult floyd_warshall(const CityNetwork *city);
void floyd_reconstruct_path(const FloydWarshallResult *fw, int u, int v, int path[], int *len);

/* -------------------------------------------------------------
 * 6. TRAVELLING SALESPERSON PROBLEM (TSP BITMASK DP)
 * ------------------------------------------------------------- */
#define TSP_MAX_STOPS 10

typedef struct {
    int min_tour_cost;
    int num_stops;
    int tour_path[TSP_MAX_STOPS + 1];
} TSPResult;

TSPResult tsp_bitmask_dp(int n, const int cost_matrix[TSP_MAX_STOPS][TSP_MAX_STOPS]);

#endif /* DP_H */
