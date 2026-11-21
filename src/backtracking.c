/**
 * @file backtracking.c
 * @brief Implementation of Backtracking and Branch & Bound algorithms.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "backtracking.h"

/* -------------------------------------------------------------
 * 1. N-QUEENS PROBLEM
 * ------------------------------------------------------------- */

static int is_queen_safe(const int board[], int row, int col) {
    for (int i = 0; i < row; i++) {
        ops_increment(1);
        if (board[i] == col || abs(board[i] - col) == abs(i - row)) {
            return 0;
        }
    }
    return 1;
}

static void n_queens_dfs(int row, int n, int board[], NQueensResult *res) {
    ops_increment(1);
    if (row == n) {
        res->total_solutions++;
        if (!res->has_solution) {
            for (int i = 0; i < n; i++) res->board[i] = board[i];
            res->has_solution = 1;
        }
        return;
    }

    for (int col = 0; col < n; col++) {
        if (is_queen_safe(board, row, col)) {
            board[row] = col;
            n_queens_dfs(row + 1, n, board, res);
        }
    }
}

/**
 * Solves N-Queens problem on n x n grid.
 * Time: O(n!), Space: O(n) call stack.
 */
NQueensResult solve_n_queens(int n) {
    NQueensResult res;
    res.n = n;
    res.total_solutions = 0;
    res.has_solution = 0;
    if (n < 1 || n > NQUEENS_MAX) return res;

    int board[NQUEENS_MAX] = {0};
    n_queens_dfs(0, n, board, &res);
    return res;
}

/* -------------------------------------------------------------
 * 2. SUM OF SUBSETS (EXACT BUDGET MATCHING)
 * ------------------------------------------------------------- */

static void subset_sum_dfs(const int costs[], int n, int idx, int cur_sum,
                           int target, int total_rem, int chosen[],
                           int chosen_len, SubsetSumResult *res) {
    ops_increment(1);
    if (cur_sum == target) {
        res->total_solutions_found++;
        if (res->solution_count == 0) {
            for (int i = 0; i < chosen_len; i++) {
                res->solution_indices[i] = chosen[i];
            }
            res->solution_count = chosen_len;
        }
        return;
    }

    if (idx >= n || cur_sum + costs[idx] > target || cur_sum + total_rem < target) {
        return; /* Prune branch */
    }

    /* Include costs[idx] */
    chosen[chosen_len] = idx;
    subset_sum_dfs(costs, n, idx + 1, cur_sum + costs[idx], target,
                   total_rem - costs[idx], chosen, chosen_len + 1, res);

    /* Exclude costs[idx] */
    subset_sum_dfs(costs, n, idx + 1, cur_sum, target,
                   total_rem - costs[idx], chosen, chosen_len, res);
}

/**
 * Finds combinations of station costs that sum exactly to target budget.
 * Time: O(2^n) worst-case, heavily pruned in practice. Space: O(n).
 */
SubsetSumResult solve_subset_sum(const int costs[], int n, int target_budget) {
    SubsetSumResult res;
    res.total_solutions_found = 0;
    res.solution_count = 0;

    int total_sum = 0;
    for (int i = 0; i < n; i++) total_sum += costs[i];
    if (target_budget <= 0 || target_budget > total_sum) return res;

    int chosen[MAX_STATIONS];
    subset_sum_dfs(costs, n, 0, 0, target_budget, total_sum, chosen, 0, &res);
    return res;
}

/* -------------------------------------------------------------
 * 3. 0/1 KNAPSACK BY BACKTRACKING
 * ------------------------------------------------------------- */

static void knapsack_bt_dfs(const Station stations[], int count, int budget,
                            int idx, int cur_w, int cur_v, int chosen[],
                            int chosen_len, KnapsackResult *best) {
    ops_increment(1);
    if (idx == count) {
        if (cur_v > best->max_benefit) {
            best->max_benefit = cur_v;
            best->total_cost = cur_w;
            best->selected_count = chosen_len;
            for (int i = 0; i < chosen_len; i++) {
                best->selected_indices[i] = chosen[i];
            }
        }
        return;
    }

    /* Option 1: Include item if within budget */
    if (cur_w + stations[idx].install_cost <= budget) {
        chosen[chosen_len] = idx;
        knapsack_bt_dfs(stations, count, budget, idx + 1,
                        cur_w + stations[idx].install_cost,
                        cur_v + stations[idx].benefit_score,
                        chosen, chosen_len + 1, best);
    }

    /* Option 2: Exclude item */
    knapsack_bt_dfs(stations, count, budget, idx + 1, cur_w, cur_v,
                    chosen, chosen_len, best);
}

/**
 * Solves 0/1 Knapsack using recursive backtracking.
 * Time: O(2^n), Space: O(n) call stack.
 */
KnapsackResult knapsack_backtracking(const Station stations[], int count, int budget) {
    KnapsackResult res;
    res.max_benefit = 0;
    res.total_cost = 0;
    res.selected_count = 0;

    int chosen[MAX_STATIONS];
    knapsack_bt_dfs(stations, count, budget, 0, 0, 0, chosen, 0, &res);
    return res;
}

/* -------------------------------------------------------------
 * 4. BRANCH & BOUND FOR KNAPSACK (FIFO, LIFO, LEAST-COST)
 * ------------------------------------------------------------- */

typedef struct {
    int level;
    int profit;
    int weight;
    double bound;
} BBNode;

/**
 * Computes upper bound on profit using greedy fractional relaxation.
 */
static double compute_bound(BBNode u, int n, int W, const Station s[]) {
    if (u.weight >= W) return 0.0;
    double profit_bound = u.profit;
    int j = u.level + 1;
    int totweight = u.weight;

    while (j < n && totweight + s[j].install_cost <= W) {
        totweight += s[j].install_cost;
        profit_bound += s[j].benefit_score;
        j++;
    }
    if (j < n) {
        profit_bound += (W - totweight) * ((double)s[j].benefit_score / s[j].install_cost);
    }
    return profit_bound;
}

#define BB_QUEUE_MAX 4096

/**
 * Solves 0/1 Knapsack using Branch and Bound (FIFO, LIFO, or Least-Cost).
 * Compares nodes explored and nodes pruned.
 * Time: O(2^n) worst case, fast with bounds. Space: O(Queue_size).
 */
BBResult knapsack_branch_and_bound(const Station stations[], int count, int budget, BBStrategy strategy) {
    BBResult res;
    res.max_benefit = 0;
    res.total_cost = 0;
    res.nodes_explored = 0;
    res.nodes_pruned = 0;

    clock_t t0 = timer_start();
    if (count <= 0 || budget <= 0) {
        res.execution_time_ms = timer_stop_ms(t0);
        return res;
    }

    BBNode pool[BB_QUEUE_MAX];
    int head = 0, tail = 0;

    BBNode u, v;
    u.level = -1; u.profit = 0; u.weight = 0;
    u.bound = compute_bound(u, count, budget, stations);

    pool[tail++] = u;

    while (head < tail) {
        /* Dequeue node depending on strategy */
        int selected_idx = head;
        if (strategy == BB_LIFO) {
            selected_idx = tail - 1; /* Stack top */
            u = pool[selected_idx];
            tail--;
        } else if (strategy == BB_LEAST_COST) {
            /* Best-First: select node with maximum upper bound */
            selected_idx = head;
            for (int i = head + 1; i < tail; i++) {
                if (pool[i].bound > pool[selected_idx].bound) {
                    selected_idx = i;
                }
            }
            u = pool[selected_idx];
            pool[selected_idx] = pool[head++];
        } else {
            /* FIFO Queue */
            u = pool[head++];
        }

        res.nodes_explored++;
        ops_increment(1);

        if (u.bound <= res.max_benefit) {
            res.nodes_pruned++;
            continue;
        }

        if (u.level == count - 1) continue;

        /* Child 1: Include item */
        v.level = u.level + 1;
        v.weight = u.weight + stations[v.level].install_cost;
        v.profit = u.profit + stations[v.level].benefit_score;

        if (v.weight <= budget && v.profit > res.max_benefit) {
            res.max_benefit = v.profit;
            res.total_cost = v.weight;
        }

        v.bound = compute_bound(v, count, budget, stations);
        if (v.bound > res.max_benefit && tail < BB_QUEUE_MAX - 1) {
            pool[tail++] = v;
        } else {
            res.nodes_pruned++;
        }

        /* Child 2: Exclude item */
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = compute_bound(v, count, budget, stations);
        if (v.bound > res.max_benefit && tail < BB_QUEUE_MAX - 1) {
            pool[tail++] = v;
        } else {
            res.nodes_pruned++;
        }
    }

    res.execution_time_ms = timer_stop_ms(t0);
    return res;
}
