/**
 * @file dp.h
 * @brief Dynamic Programming algorithms for Station Placement and Optimization.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 */

#ifndef DP_H
#define DP_H

#include "model.h"
#include "analysis.h"

/**
 * @brief Result structure for Knapsack Station Placement.
 */
typedef struct {
    int max_benefit;
    int total_cost;
    int selected_count;
    int selected_indices[MAX_STATIONS];
} KnapsackResult;

/* Core 0/1 Knapsack DP for EV Station Placement */
KnapsackResult knapsack_01_dp(const Station stations[], int count, int budget);

/* Prints the DP table for educational/viva tracing */
void knapsack_print_table(const Station stations[], int count, int budget);

#endif /* DP_H */
