/**
 * @file dp.c
 * @brief Implementations of complete Dynamic Programming suite.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "dp.h"

#define MAX_DP_BUDGET 1000

/* -------------------------------------------------------------
 * 1. 0/1 KNAPSACK FOR STATION PLACEMENT
 * ------------------------------------------------------------- */

KnapsackResult knapsack_01_dp(const Station stations[], int count, int budget) {
    KnapsackResult res;
    res.max_benefit = 0; res.total_cost = 0; res.selected_count = 0;
    if (count <= 0 || budget <= 0) return res;

    int eff_b = (budget > MAX_DP_BUDGET) ? MAX_DP_BUDGET : budget;
    int **dp = (int **)malloc((size_t)(count + 1) * sizeof(int *));
    for (int i = 0; i <= count; i++) {
        dp[i] = (int *)malloc((size_t)(eff_b + 1) * sizeof(int));
    }

    for (int w = 0; w <= eff_b; w++) { dp[0][w] = 0; ops_increment(1); }
    for (int i = 0; i <= count; i++) { dp[i][0] = 0; ops_increment(1); }

    for (int i = 1; i <= count; i++) {
        int cost = stations[i - 1].install_cost;
        int val  = stations[i - 1].benefit_score;
        for (int w = 1; w <= eff_b; w++) {
            ops_increment(1);
            if (cost <= w) {
                int incl = dp[i - 1][w - cost] + val;
                int excl = dp[i - 1][w];
                dp[i][w] = (incl > excl) ? incl : excl;
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    res.max_benefit = dp[count][eff_b];
    int cur_w = eff_b;
    for (int i = count; i > 0 && cur_w > 0; i--) {
        ops_increment(1);
        if (dp[i][cur_w] != dp[i - 1][cur_w]) {
            res.selected_indices[res.selected_count++] = i - 1;
            res.total_cost += stations[i - 1].install_cost;
            cur_w -= stations[i - 1].install_cost;
        }
    }

    for (int i = 0; i <= count; i++) free(dp[i]);
    free(dp);
    return res;
}

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
        if (i == 0) printf(" Base  0 |");
        else printf(" S_%-4d |", stations[i - 1].id);
        for (int w = 0; w <= budget; w += 2) printf("%3d ", dp[i][w]);
        printf("\n");
    }
    printf("=====================================================================\n");
}

/* -------------------------------------------------------------
 * 2. MATRIX CHAIN MULTIPLICATION (MCM)
 * ------------------------------------------------------------- */

static void print_optimal_parens(int s[MCM_MAX][MCM_MAX], int i, int j, char *out) {
    if (i == j) {
        char name[8];
        snprintf(name, sizeof(name), "A%d", i);
        strcat(out, name);
    } else {
        strcat(out, "(");
        print_optimal_parens(s, i, s[i][j], out);
        print_optimal_parens(s, s[i][j] + 1, j, out);
        strcat(out, ")");
    }
}

/**
 * Solves Matrix Chain Multiplication.
 * p contains dimensions of n matrices: A1 is p[0]xp[1], ..., An is p[n-1]xp[n].
 * Recurrence: m[i][j] = min_{i<=k<j} { m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j] }.
 * Time: Theta(n^3), Space: Theta(n^2).
 */
MCMResult matrix_chain_order(const int p[], int n) {
    MCMResult res;
    res.min_scalar_mults = 0;
    res.parenthesization[0] = '\0';
    if (n <= 1) return res;

    int m[MCM_MAX][MCM_MAX] = {{0}};
    int num_matrices = n - 1;

    for (int L = 2; L <= num_matrices; L++) {
        for (int i = 1; i <= num_matrices - L + 1; i++) {
            int j = i + L - 1;
            m[i][j] = INF_DIST;
            for (int k = i; k <= j - 1; k++) {
                ops_increment(1);
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    res.s_table[i][j] = k;
                }
            }
        }
    }

    res.min_scalar_mults = m[1][num_matrices];
    print_optimal_parens(res.s_table, 1, num_matrices, res.parenthesization);
    return res;
}

/* -------------------------------------------------------------
 * 3. LONGEST COMMON SUBSEQUENCE (LCS)
 * ------------------------------------------------------------- */

/**
 * Solves LCS between two EV route junction sequences.
 * Recurrence:
 *   L[i][j] = L[i-1][j-1] + 1         if routeA[i-1] == routeB[j-1]
 *   L[i][j] = max(L[i-1][j], L[i][j-1]) otherwise
 * Time: Theta(lenA * lenB), Space: Theta(lenA * lenB).
 */
LCSResult lcs_route_comparison(const int routeA[], int lenA,
                               const int routeB[], int lenB) {
    LCSResult res;
    res.lcs_length = 0;

    int L[LCS_MAX][LCS_MAX] = {{0}};
    for (int i = 1; i <= lenA; i++) {
        for (int j = 1; j <= lenB; j++) {
            ops_increment(1);
            if (routeA[i - 1] == routeB[j - 1]) {
                L[i][j] = L[i - 1][j - 1] + 1;
            } else {
                L[i][j] = (L[i - 1][j] > L[i][j - 1]) ? L[i - 1][j] : L[i][j - 1];
            }
        }
    }

    res.lcs_length = L[lenA][lenB];
    int i = lenA, j = lenB;
    int idx = res.lcs_length - 1;
    while (i > 0 && j > 0) {
        ops_increment(1);
        if (routeA[i - 1] == routeB[j - 1]) {
            res.lcs_sequence[idx--] = routeA[i - 1];
            i--; j--;
        } else if (L[i - 1][j] > L[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    return res;
}

/* -------------------------------------------------------------
 * 4. OPTIMAL BINARY SEARCH TREE (OBST)
 * ------------------------------------------------------------- */

/**
 * Computes Optimal Binary Search Tree for station query frequencies.
 * Time: Theta(n^3), Space: Theta(n^2).
 */
OBSTResult optimal_bst(const int keys[], const double p[], const double q[], int n) {
    (void)keys;
    OBSTResult res;
    res.min_cost = 0.0;
    double e[OBST_MAX][OBST_MAX] = {{0.0}};
    double w[OBST_MAX][OBST_MAX] = {{0.0}};

    for (int i = 1; i <= n + 1; i++) {
        e[i][i - 1] = q[i - 1];
        w[i][i - 1] = q[i - 1];
        ops_increment(2);
    }

    for (int L = 1; L <= n; L++) {
        for (int i = 1; i <= n - L + 1; i++) {
            int j = i + L - 1;
            e[i][j] = 1e9;
            w[i][j] = w[i][j - 1] + p[j] + q[j];

            for (int r = i; r <= j; r++) {
                ops_increment(1);
                double t = e[i][r - 1] + e[r + 1][j] + w[i][j];
                if (t < e[i][j]) {
                    e[i][j] = t;
                    res.root[i][j] = r;
                }
            }
        }
    }
    res.min_cost = e[1][n];
    return res;
}

/* -------------------------------------------------------------
 * 5. BELLMAN-FORD & FLOYD-WARSHALL
 * ------------------------------------------------------------- */

/**
 * Computes shortest paths from src with negative cycle detection.
 * Time: Theta(V * E), Space: Theta(V).
 */
BellmanFordResult bellman_ford(const CityNetwork *city, int src) {
    BellmanFordResult res;
    res.has_negative_cycle = 0;
    for (int i = 0; i < city->num_nodes; i++) {
        res.dist[i] = INF_DIST;
        res.parent[i] = -1;
    }
    res.dist[src] = 0;

    /* Relax all edges V - 1 times */
    for (int step = 1; step <= city->num_nodes - 1; step++) {
        for (int u = 0; u < city->num_nodes; u++) {
            if (res.dist[u] == INF_DIST) continue;
            for (int v = 0; v < city->num_nodes; v++) {
                ops_increment(1);
                int weight = city->adj_matrix[u][v];
                if (weight > 0 && weight < INF_DIST) {
                    if (res.dist[u] + weight < res.dist[v]) {
                        res.dist[v] = res.dist[u] + weight;
                        res.parent[v] = u;
                    }
                }
            }
        }
    }

    /* Check for negative-weight cycles */
    for (int u = 0; u < city->num_nodes; u++) {
        if (res.dist[u] == INF_DIST) continue;
        for (int v = 0; v < city->num_nodes; v++) {
            ops_increment(1);
            int weight = city->adj_matrix[u][v];
            if (weight > 0 && weight < INF_DIST) {
                if (res.dist[u] + weight < res.dist[v]) {
                    res.has_negative_cycle = 1;
                    return res;
                }
            }
        }
    }
    return res;
}

/**
 * Computes All-Pairs Shortest Paths using Floyd-Warshall algorithm.
 * Time: Theta(V^3), Space: Theta(V^2).
 */
FloydWarshallResult floyd_warshall(const CityNetwork *city) {
    FloydWarshallResult fw;
    int n = city->num_nodes;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fw.dist[i][j] = city->adj_matrix[i][j];
            if (i != j && city->adj_matrix[i][j] < INF_DIST) {
                fw.next[i][j] = j;
            } else {
                fw.next[i][j] = -1;
            }
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ops_increment(1);
                if (fw.dist[i][k] < INF_DIST && fw.dist[k][j] < INF_DIST) {
                    if (fw.dist[i][k] + fw.dist[k][j] < fw.dist[i][j]) {
                        fw.dist[i][j] = fw.dist[i][k] + fw.dist[k][j];
                        fw.next[i][j] = fw.next[i][k];
                    }
                }
            }
        }
    }
    return fw;
}

void floyd_reconstruct_path(const FloydWarshallResult *fw, int u, int v, int path[], int *len) {
    *len = 0;
    if (fw->next[u][v] == -1) return;
    path[(*len)++] = u;
    while (u != v) {
        u = fw->next[u][v];
        path[(*len)++] = u;
    }
}

/* -------------------------------------------------------------
 * 6. TSP BITMASK DYNAMIC PROGRAMMING (HELD-KARP)
 * ------------------------------------------------------------- */

/**
 * Solves Travelling Salesperson Problem for n <= 10 stops.
 * State: dp[mask][i] = min distance visiting set of vertices in 'mask' ending at 'i'.
 * Recurrence: dp[mask | (1<<j)][j] = min { dp[mask][i] + dist[i][j] }.
 * Time: Theta(n^2 * 2^n), Space: Theta(n * 2^n).
 */
TSPResult tsp_bitmask_dp(int n, const int cost[TSP_MAX_STOPS][TSP_MAX_STOPS]) {
    TSPResult res;
    res.min_tour_cost = INF_DIST;
    res.num_stops = n;

    if (n <= 1 || n > TSP_MAX_STOPS) {
        res.min_tour_cost = 0;
        return res;
    }

    int total_masks = 1 << n;
    int dp[1 << TSP_MAX_STOPS][TSP_MAX_STOPS];
    int parent[1 << TSP_MAX_STOPS][TSP_MAX_STOPS];

    for (int m = 0; m < total_masks; m++) {
        for (int i = 0; i < n; i++) {
            dp[m][i] = INF_DIST;
            parent[m][i] = -1;
        }
    }

    /* Base case: start at vertex 0 */
    dp[1][0] = 0;

    for (int mask = 1; mask < total_masks; mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u)) || dp[mask][u] == INF_DIST) continue;

            for (int v = 0; v < n; v++) {
                ops_increment(1);
                if (!(mask & (1 << v))) {
                    int next_mask = mask | (1 << v);
                    int new_cost = dp[mask][u] + cost[u][v];
                    if (new_cost < dp[next_mask][v]) {
                        dp[next_mask][v] = new_cost;
                        parent[next_mask][v] = u;
                    }
                }
            }
        }
    }

    /* Close the tour by returning to vertex 0 */
    int final_mask = (1 << n) - 1;
    int best_last = -1;
    for (int u = 1; u < n; u++) {
        ops_increment(1);
        if (dp[final_mask][u] < INF_DIST && cost[u][0] < INF_DIST) {
            int tour_cost = dp[final_mask][u] + cost[u][0];
            if (tour_cost < res.min_tour_cost) {
                res.min_tour_cost = tour_cost;
                best_last = u;
            }
        }
    }

    /* Reconstruct tour path */
    if (best_last != -1) {
        int curr_mask = final_mask;
        int curr_node = best_last;
        res.tour_path[n] = 0;
        for (int step = n - 1; step >= 1; step--) {
            res.tour_path[step] = curr_node;
            int prev = parent[curr_mask][curr_node];
            curr_mask ^= (1 << curr_node);
            curr_node = prev;
        }
        res.tour_path[0] = 0;
    }
    return res;
}
