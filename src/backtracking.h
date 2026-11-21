/**
 * @file backtracking.h
 * @brief Backtracking and Branch & Bound algorithms for exact combinatorial decisions.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "model.h"
#include "analysis.h"
#include "dp.h"

/* -------------------------------------------------------------
 * 1. N-QUEENS (CONSTRAINT SATISFACTION VALIDATION)
 * ------------------------------------------------------------- */
#define NQUEENS_MAX 10

typedef struct {
    int n;
    int total_solutions;
    int board[NQUEENS_MAX]; /* board[row] = col */
    int has_solution;
} NQueensResult;

NQueensResult solve_n_queens(int n);

/* -------------------------------------------------------------
 * 2. SUM OF SUBSETS (EXACT BUDGET COMBINATIONS)
 * ------------------------------------------------------------- */
typedef struct {
    int total_solutions_found;
    int solution_indices[MAX_STATIONS];
    int solution_count;
} SubsetSumResult;

SubsetSumResult solve_subset_sum(const int costs[], int n, int target_budget);

/* -------------------------------------------------------------
 * 3. 0/1 KNAPSACK BY BACKTRACKING
 * ------------------------------------------------------------- */
KnapsackResult knapsack_backtracking(const Station stations[], int count, int budget);

/* -------------------------------------------------------------
 * 4. BRANCH & BOUND (FIFO, LIFO, LEAST-COST)
 * ------------------------------------------------------------- */
typedef enum {
    BB_FIFO = 0,
    BB_LIFO,
    BB_LEAST_COST
} BBStrategy;

typedef struct {
    int max_benefit;
    int total_cost;
    long long nodes_explored;
    long long nodes_pruned;
    double execution_time_ms;
} BBResult;

BBResult knapsack_branch_and_bound(const Station stations[], int count, int budget, BBStrategy strategy);

#endif /* BACKTRACKING_H */
