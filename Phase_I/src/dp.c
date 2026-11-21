/**
 * @file dp.c
 * @brief Dynamic Programming implementation for 0/1 Knapsack Station Placement.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "dp.h"

#define MAX_DP_BUDGET 1000

/**
 * Solves 0/1 Knapsack for EV Station Placement using Dynamic Programming.
 * DP Recurrence:
 *   dp[i][w] = dp[i-1][w]                                    if cost[i] > w
 *   dp[i][w] = max(dp[i-1][w], dp[i-1][w-cost[i]] + val[i])  otherwise
 *
 * Time Complexity:  O(n * W) pseudo-polynomial.
 * Space Complexity: O(n * W) auxiliary table.
 */
KnapsackResult knapsack_01_dp(const Station stations[], int count, int budget) {
    KnapsackResult res;
    res.max_benefit = 0;
    res.total_cost = 0;
    res.selected_count = 0;

    if (count <= 0 || budget <= 0) {
        return res;
    }

    /* Cap budget to avoid excessive static/stack allocation */
    int effective_budget = (budget > MAX_DP_BUDGET) ? MAX_DP_BUDGET : budget;

    /* Allocate 2D DP table: (count + 1) rows x (effective_budget + 1) cols */
    int **dp = (int **)malloc((size_t)(count + 1) * sizeof(int *));
    for (int i = 0; i <= count; i++) {
        dp[i] = (int *)malloc((size_t)(effective_budget + 1) * sizeof(int));
    }

    /* Base case: 0 items or 0 capacity gives 0 benefit */
    for (int w = 0; w <= effective_budget; w++) {
        dp[0][w] = 0;
        ops_increment(1);
    }
    for (int i = 0; i <= count; i++) {
        dp[i][0] = 0;
        ops_increment(1);
    }

    /* Build DP table in bottom-up manner */
    for (int i = 1; i <= count; i++) {
        int cost = stations[i - 1].install_cost;
        int val  = stations[i - 1].benefit_score;

        for (int w = 1; w <= effective_budget; w++) {
            ops_increment(1); /* comparison & table update */
            if (cost <= w) {
                int incl = dp[i - 1][w - cost] + val;
                int excl = dp[i - 1][w];
                dp[i][w] = (incl > excl) ? incl : excl;
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    res.max_benefit = dp[count][effective_budget];

    /* Backtrack to reconstruct selected stations */
    int cur_w = effective_budget;
    for (int i = count; i > 0 && cur_w > 0; i--) {
        ops_increment(1);
        if (dp[i][cur_w] != dp[i - 1][cur_w]) {
            res.selected_indices[res.selected_count++] = i - 1;
            res.total_cost += stations[i - 1].install_cost;
            cur_w -= stations[i - 1].install_cost;
        }
    }

    /* Free DP table memory */
    for (int i = 0; i <= count; i++) {
        free(dp[i]);
    }
    free(dp);

    return res;
}

/**
 * Prints the DP table for small instances (count <= 10, budget <= 30) for viva tracing.
 * Time: O(count * budget), Space: O(count * budget).
 */
void knapsack_print_table(const Station stations[], int count, int budget) {
    if (count > 8 || budget > 25) {
        printf("Table display truncated for viva brevity (only shown for small subproblems <= 8 items, budget <= 25).\n");
        return;
    }

    int dp[9][26] = {{0}};

    for (int i = 1; i <= count; i++) {
        int cost = stations[i - 1].install_cost;
        int val  = stations[i - 1].benefit_score;
        for (int w = 1; w <= budget; w++) {
            if (cost <= w) {
                int incl = dp[i - 1][w - cost] + val;
                int excl = dp[i - 1][w];
                dp[i][w] = (incl > excl) ? incl : excl;
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    printf("\n=== 0/1 Knapsack DP Table (Rows: Items 0..%d, Cols: Budget 0..%d) ===\n", count, budget);
    printf("Item\\Cap |");
    for (int w = 0; w <= budget; w += 2) printf("%3d ", w);
    printf("\n---------+");
    for (int w = 0; w <= budget; w += 2) printf("----");
    printf("\n");

    for (int i = 0; i <= count; i++) {
        if (i == 0) {
            printf(" Base  0 |");
        } else {
            printf(" S_%-4d |", stations[i - 1].id);
        }
        for (int w = 0; w <= budget; w += 2) {
            printf("%3d ", dp[i][w]);
        }
        printf("\n");
    }
    printf("=====================================================================\n");
}
