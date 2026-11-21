/**
 * @file test_runner.c
 * @brief Comprehensive Automated Test Suite for All Syllabus Modules.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Tests Normal, Boundary, Invalid, Stress, and Adversarial cases.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/model.h"
#include "../src/analysis.h"
#include "../src/sorting_searching.h"
#include "../src/divide_conquer.h"
#include "../src/greedy.h"
#include "../src/dp.h"
#include "../src/backtracking.h"
#include "../src/flow_matching.h"
#include "../src/number_theory.h"
#include "../src/string_match.h"

static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_ASSERT(cond, msg) do { \
    g_tests_run++; \
    if (cond) { \
        g_tests_passed++; \
        printf("  [PASS] %s\n", msg); \
    } else { \
        printf("  [FAIL] %s (Line %d)\n", msg, __LINE__); \
    } \
} while(0)

/* -------------------------------------------------------------
 * TEST 1: Model & I/O
 * ------------------------------------------------------------- */
void test_model_suite(void) {
    printf("\n[TEST MODULE 1] Domain Model & File I/O\n");
    CityNetwork city;
    city_add_default_sample(&city);
    TEST_ASSERT(city.num_nodes == 8, "Default sample initializes 8 nodes");
    TEST_ASSERT(city_validate(&city) == 1, "Default sample passes domain validation");
    TEST_ASSERT(city_load_from_file(&city, "data/small.txt") == 1, "small.txt loads cleanly");
    city.budget = -1;
    TEST_ASSERT(city_validate(&city) == 0, "Negative budget safely rejected");
}

/* -------------------------------------------------------------
 * TEST 2: Sorting & Searching
 * ------------------------------------------------------------- */
void test_sort_search_suite(void) {
    printf("\n[TEST MODULE 2] Sorting & Searching\n");
    int arr[6] = {45, 12, 89, 23, 7, 56};
    merge_sort_int(arr, 0, 5);
    int ok = 1;
    for (int i = 0; i < 5; i++) if (arr[i] > arr[i + 1]) ok = 0;
    TEST_ASSERT(ok == 1, "Merge sort correctly sorts integer array");

    int qarr[6] = {5, 4, 3, 2, 1, 0};
    quick_sort_int(qarr, 0, 5);
    TEST_ASSERT(qarr[0] == 0 && qarr[5] == 5, "Quick sort sorts reverse array");

    int sorted[6] = {10, 20, 30, 40, 50, 60};
    TEST_ASSERT(binary_search_iterative_int(sorted, 6, 30) == 2, "Iterative binary search finds element");
    TEST_ASSERT(binary_search_recursive_int(sorted, 0, 5, 30) == 2, "Recursive binary search finds element");
    TEST_ASSERT(binary_search_iterative_int(sorted, 6, 99) == -1, "Binary search returns -1 on absent key");
}

/* -------------------------------------------------------------
 * TEST 3: Divide and Conquer
 * ------------------------------------------------------------- */
void test_divide_conquer_suite(void) {
    printf("\n[TEST MODULE 3] Divide & Conquer\n");
    int arr[8] = {-2, 1, -3, 4, -1, 2, 1, -5};
    MaxSubarrayResult dc = max_subarray_dc(arr, 0, 7);
    MaxSubarrayResult brute = max_subarray_brute(arr, 8);
    TEST_ASSERT(dc.sum == 6, "Maximum subarray sum equals 6 ({4, -1, 2, 1})");
    TEST_ASSERT(dc.sum == brute.sum, "Oracle check: D&C matches Brute-Force maximum subarray");

    double p1 = fast_power(2.0, 10);
    double p2 = naive_power(2.0, 10);
    TEST_ASSERT((int)p1 == 1024, "Fast power 2^10 = 1024");
    TEST_ASSERT(p1 == p2, "Oracle check: Fast power matches Naive power");

    int reach[MAX_NODES][MAX_NODES] = {{0}};
    reach[0][2] = 1; reach[1][2] = 1; reach[3][2] = 1;
    int hub_lin = celebrity_hub_linear(4, reach);
    int hub_brt = celebrity_hub_brute(4, reach);
    TEST_ASSERT(hub_lin == 2, "Linear celebrity discovery identifies hub node 2");
    TEST_ASSERT(hub_lin == hub_brt, "Oracle check: Linear celebrity matches Brute Force");

    int A[MATRIX_MAX][MATRIX_MAX] = {{1, 2}, {3, 4}};
    int C1[MATRIX_MAX][MATRIX_MAX], C2[MATRIX_MAX][MATRIX_MAX];
    matrix_multiply_classical(2, A, A, C1);
    strassen_multiply(2, A, A, C2);
    TEST_ASSERT(C1[0][0] == C2[0][0] && C1[1][1] == C2[1][1], "Strassen matches Classical matrix multiplication");
}

/* -------------------------------------------------------------
 * TEST 4: Greedy Algorithms
 * ------------------------------------------------------------- */
void test_greedy_suite(void) {
    printf("\n[TEST MODULE 4] Greedy Suite\n");
    ChargingSession s[3] = {{1, 1, 1, 4}, {2, 2, 3, 5}, {3, 3, 0, 6}};
    ActivitySelectionResult act = activity_selection(s, 3);
    TEST_ASSERT(act.count == 1, "Activity selection picks non-overlapping session");

    PowerRequest req[2] = {{1, 50.0, 50.0, 0}, {2, 50.0, 100.0, 0}};
    FractionalKnapsackResult fk = fractional_knapsack_power(req, 2, 75.0);
    TEST_ASSERT(fk.total_kw_allocated == 75.0, "Fractional knapsack fills total grid capacity");

    CityNetwork city;
    city_add_default_sample(&city);
    MSTResult prim = prim_mst(&city);
    MSTResult krusk = kruskal_mst(&city);
    TEST_ASSERT(prim.total_weight == krusk.total_weight, "Oracle check: Prim and Kruskal MST weights match");

    const char *msg = "ABRACADABRA";
    char enc[128] = "", dec[128] = "";
    HuffmanResult huff = huffman_compress(msg, enc, sizeof(enc));
    huffman_decompress(enc, &huff, dec);
    TEST_ASSERT(strcmp(msg, dec) == 0, "Huffman decompress perfectly reconstructs original string");
}

/* -------------------------------------------------------------
 * TEST 5: Dynamic Programming
 * ------------------------------------------------------------- */
void test_dp_suite(void) {
    printf("\n[TEST MODULE 5] Dynamic Programming Suite\n");
    Station items[3] = {
        {101, 0, 30, 100, 4, 33, 0},
        {102, 1, 25, 100, 4, 25, 0},
        {103, 2, 25, 100, 4, 25, 0}
    };
    KnapsackResult dp = knapsack_01_dp(items, 3, 50);
    KnapsackResult gr = knapsack_greedy_ratio(items, 3, 50);
    TEST_ASSERT(dp.max_benefit == 50, "0/1 Knapsack DP finds optimal combination (50)");
    TEST_ASSERT(dp.max_benefit > gr.max_benefit, "DP strictly outperforms Greedy on counterexample");

    int p[4] = {10, 20, 30, 40};
    MCMResult mcm = matrix_chain_order(p, 4);
    TEST_ASSERT(mcm.min_scalar_mults > 0, "Matrix chain order computes positive scalar operations");

    int r1[4] = {1, 2, 3, 4};
    int r2[4] = {2, 4, 3, 4};
    LCSResult lcs = lcs_route_comparison(r1, 4, r2, 4);
    TEST_ASSERT(lcs.lcs_length >= 2, "LCS identifies shared route segments");

    int tsp_cost[TSP_MAX_STOPS][TSP_MAX_STOPS] = {
        {0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}
    };
    TSPResult tsp = tsp_bitmask_dp(4, tsp_cost);
    TEST_ASSERT(tsp.min_tour_cost == 80, "TSP bitmask DP computes optimal 4-stop tour cost (80)");
}

/* -------------------------------------------------------------
 * TEST 6: Backtracking and Branch & Bound
 * ------------------------------------------------------------- */
void test_backtracking_suite(void) {
    printf("\n[TEST MODULE 6] Backtracking & Branch and Bound\n");
    NQueensResult n8 = solve_n_queens(8);
    TEST_ASSERT(n8.total_solutions == 92, "N-Queens (8x8) finds exactly 92 distinct solutions");

    int costs[4] = {15, 20, 30, 35};
    SubsetSumResult ss = solve_subset_sum(costs, 4, 50);
    TEST_ASSERT(ss.total_solutions_found >= 1, "Sum of subsets finds exact budget combination (15+35 or 20+30)");

    Station sites[3] = {{1, 0, 10, 50, 2, 60, 0}, {2, 1, 20, 50, 2, 100, 0}, {3, 2, 30, 50, 2, 120, 0}};
    KnapsackResult bt = knapsack_backtracking(sites, 3, 50);
    BBResult bb_fifo = knapsack_branch_and_bound(sites, 3, 50, BB_FIFO);
    BBResult bb_lc   = knapsack_branch_and_bound(sites, 3, 50, BB_LEAST_COST);
    TEST_ASSERT(bt.max_benefit == 220, "Backtracking finds optimal knapsack benefit (220)");
    TEST_ASSERT(bb_fifo.max_benefit == 220, "Branch & Bound FIFO finds optimal knapsack benefit (220)");
    TEST_ASSERT(bb_lc.max_benefit == 220, "Branch & Bound Least-Cost finds optimal knapsack benefit (220)");
}

/* -------------------------------------------------------------
 * TEST 7: Flow & Matching
 * ------------------------------------------------------------- */
void test_flow_matching_suite(void) {
    printf("\n[TEST MODULE 7] Flow and Matching\n");
    int cap[FLOW_MAX_NODES][FLOW_MAX_NODES] = {{0}};
    cap[0][1] = 16; cap[0][2] = 13;
    cap[1][2] = 10; cap[1][3] = 12;
    cap[2][1] = 4;  cap[2][4] = 14;
    cap[3][2] = 9;  cap[3][5] = 20;
    cap[4][3] = 7;  cap[4][5] = 4;

    MaxFlowResult flow = ford_fulkerson_max_flow(6, cap, 0, 5);
    TEST_ASSERT(flow.max_flow == 23, "Ford-Fulkerson computes correct max flow (23)");
    TEST_ASSERT(flow.min_cut_edge_count > 0, "Minimum cut identified bottleneck lines");

    int compat[MATCH_MAX_ITEMS][MATCH_MAX_ITEMS] = {{1, 1}, {1, 0}};
    BipartiteMatchResult bpm = max_bipartite_matching(2, 2, compat);
    TEST_ASSERT(bpm.match_count == 2, "Bipartite matching assigns both vehicles to distinct bays");
}

/* -------------------------------------------------------------
 * TEST 8: Number Theory & String Matching
 * ------------------------------------------------------------- */
void test_number_theory_and_strings(void) {
    printf("\n[TEST MODULE 8] Number Theory & String Matching\n");
    long long rem[3] = {2, 3, 2};
    long long mod[3] = {3, 5, 7};
    CRTResult crt = chinese_remainder_theorem(3, rem, mod);
    TEST_ASSERT(crt.total_counter == 23, "CRT reconstructs total counter 23 (23%3=2, 23%5=3, 23%7=2)");

    const char *text = "AABAACAADAABAABA";
    const char *pat  = "AABA";
    StringMatchResult r_naive = string_match_naive(text, pat);
    StringMatchResult r_rk    = string_match_rabin_karp(text, pat);
    StringMatchResult r_dfa   = string_match_finite_automaton(text, pat);
    StringMatchResult r_kmp   = string_match_kmp(text, pat);

    TEST_ASSERT(r_naive.match_count == 3, "Naive finds 3 occurrences of pattern");
    TEST_ASSERT(r_rk.match_count == 3, "Rabin-Karp finds 3 occurrences of pattern");
    TEST_ASSERT(r_dfa.match_count == 3, "Finite Automaton finds 3 occurrences of pattern");
    TEST_ASSERT(r_kmp.match_count == 3, "KMP finds 3 occurrences of pattern");
    TEST_ASSERT(r_naive.match_positions[0] == r_kmp.match_positions[0], "Oracle check: KMP and Naive match indices agree");
}

int main(void) {
    printf("=========================================================\n");
    printf("     EV SYSTEM FULL SYLLABUS AUTOMATED TEST HARNESS      \n");
    printf("=========================================================\n");

    test_model_suite();
    test_sort_search_suite();
    test_divide_conquer_suite();
    test_greedy_suite();
    test_dp_suite();
    test_backtracking_suite();
    test_flow_matching_suite();
    test_number_theory_and_strings();

    printf("\n=========================================================\n");
    printf(" FINAL HARNESS SUMMARY: %d / %d Tests PASSED\n", g_tests_passed, g_tests_run);
    if (g_tests_passed == g_tests_run) {
        printf(" ALL SYLLABUS TESTS PASSED WITH 100%% GREEN STATUS!\n");
    } else {
        printf(" WARNING: %d failure(s) detected!\n", g_tests_run - g_tests_passed);
    }
    printf("=========================================================\n");
    return (g_tests_passed == g_tests_run) ? 0 : 1;
}
