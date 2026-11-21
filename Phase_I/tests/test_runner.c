/**
 * @file test_runner.c
 * @brief Automated Test Suite for Phase I Core Algorithms.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Tests normal, boundary, invalid, stress, and adversarial cases.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../src/model.h"
#include "../src/analysis.h"
#include "../src/sorting_searching.h"
#include "../src/greedy.h"
#include "../src/dp.h"

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
 * TEST 1: Model Loading & Validation
 * ------------------------------------------------------------- */
void test_model_loading_and_validation(void) {
    printf("\n[TEST MODULE 1] Model Loading and Validation\n");
    CityNetwork city;

    /* 1.1 Default sample */
    city_add_default_sample(&city);
    TEST_ASSERT(city.num_nodes == 8, "Default sample initializes 8 nodes");
    TEST_ASSERT(city_validate(&city) == 1, "Default sample passes domain validation");

    /* 1.2 Non-existent file */
    int res_missing = city_load_from_file(&city, "data/non_existent_file.txt");
    TEST_ASSERT(res_missing == 0, "Non-existent file returns error safely without crash");

    /* 1.3 Small dataset file */
    int res_small = city_load_from_file(&city, "data/small.txt");
    TEST_ASSERT(res_small == 1, "small.txt loads and validates successfully");
    TEST_ASSERT(city.num_stations == 6, "small.txt contains exactly 6 candidate stations");

    /* 1.4 Invalid data detection */
    city.budget = -50;
    TEST_ASSERT(city_validate(&city) == 0, "Validation rejects negative budget");
}

/* -------------------------------------------------------------
 * TEST 2: Merge Sort (Normal, Boundary, Duplicates)
 * ------------------------------------------------------------- */
void test_merge_sort(void) {
    printf("\n[TEST MODULE 2] Merge Sort (Handwritten)\n");

    /* 2.1 Normal random array */
    int arr1[6] = {45, 12, 89, 23, 7, 56};
    merge_sort_int(arr1, 0, 5);
    int sorted1 = 1;
    for (int i = 0; i < 5; i++) {
        if (arr1[i] > arr1[i + 1]) sorted1 = 0;
    }
    TEST_ASSERT(sorted1 == 1, "Merge sort correctly sorts normal random array");

    /* 2.2 Already sorted (best-case) */
    int arr2[5] = {10, 20, 30, 40, 50};
    merge_sort_int(arr2, 0, 4);
    TEST_ASSERT(arr2[0] == 10 && arr2[4] == 50, "Merge sort handles already-sorted input");

    /* 2.3 Reverse sorted */
    int arr3[5] = {90, 70, 50, 30, 10};
    merge_sort_int(arr3, 0, 4);
    TEST_ASSERT(arr3[0] == 10 && arr3[4] == 90, "Merge sort handles reverse-sorted input");

    /* 2.4 Many duplicates */
    int arr4[6] = {5, 2, 5, 2, 5, 1};
    merge_sort_int(arr4, 0, 5);
    TEST_ASSERT(arr4[0] == 1 && arr4[1] == 2 && arr4[5] == 5, "Merge sort handles duplicate elements");

    /* 2.5 Single element boundary */
    int arr5[1] = {42};
    merge_sort_int(arr5, 0, 0);
    TEST_ASSERT(arr5[0] == 42, "Merge sort handles single-element boundary gracefully");
}

/* -------------------------------------------------------------
 * TEST 3: Quick Sort (Normal, Adversarial, Duplicates)
 * ------------------------------------------------------------- */
void test_quick_sort(void) {
    printf("\n[TEST MODULE 3] Quick Sort (Handwritten)\n");

    /* 3.1 Normal array */
    int arr1[7] = {34, 7, 23, 32, 5, 62, 19};
    quick_sort_int(arr1, 0, 6);
    int ok = 1;
    for (int i = 0; i < 6; i++) {
        if (arr1[i] > arr1[i + 1]) ok = 0;
    }
    TEST_ASSERT(ok == 1, "Quick sort sorts random array correctly");

    /* 3.2 Adversarial case: already sorted */
    int arr2[5] = {1, 2, 3, 4, 5};
    quick_sort_int(arr2, 0, 4);
    TEST_ASSERT(arr2[0] == 1 && arr2[4] == 5, "Quick sort handles already-sorted adversarial input safely");

    /* 3.3 Station struct sorting by cost */
    Station s[3] = {
        {101, 0, 40, 100, 4, 80, 0},
        {102, 1, 15, 60,  2, 40, 0},
        {103, 2, 25, 80,  3, 60, 0}
    };
    quick_sort_stations(s, 0, 2, SORT_BY_COST);
    TEST_ASSERT(s[0].install_cost == 15 && s[1].install_cost == 25 && s[2].install_cost == 40,
                "Quick sort correctly sorts Station structs by installation cost");
}

/* -------------------------------------------------------------
 * TEST 4: Binary Search Iterative & Recursive Oracle
 * ------------------------------------------------------------- */
void test_binary_search(void) {
    printf("\n[TEST MODULE 4] Binary Search (Iterative vs Recursive)\n");
    int sorted[8] = {11, 22, 33, 44, 55, 66, 77, 88};

    /* 4.1 Target present */
    int idx_it = binary_search_iterative_int(sorted, 8, 44);
    int idx_rec = binary_search_recursive_int(sorted, 0, 7, 44);
    TEST_ASSERT(idx_it == 3, "Iterative binary search finds element in middle");
    TEST_ASSERT(idx_rec == 3, "Recursive binary search finds element in middle");
    TEST_ASSERT(idx_it == idx_rec, "Oracle agreement: Iterative and recursive match on present target");

    /* 4.2 Target at boundaries (first and last) */
    TEST_ASSERT(binary_search_iterative_int(sorted, 8, 11) == 0, "Iterative finds first element (index 0)");
    TEST_ASSERT(binary_search_recursive_int(sorted, 0, 7, 88) == 7, "Recursive finds last element (index 7)");

    /* 4.3 Target absent */
    TEST_ASSERT(binary_search_iterative_int(sorted, 8, 50) == -1, "Iterative returns -1 for absent value");
    TEST_ASSERT(binary_search_recursive_int(sorted, 0, 7, 50) == -1, "Recursive returns -1 for absent value");
}

/* -------------------------------------------------------------
 * TEST 5: 0/1 Knapsack DP Optimality vs Standard Instance
 * ------------------------------------------------------------- */
void test_knapsack_dp(void) {
    printf("\n[TEST MODULE 5] 0/1 Knapsack Dynamic Programming\n");

    /* Textbook instance:
     * Weights: 10, 20, 30 | Values: 60, 100, 120 | Capacity: 50
     * Optimal subset: items 2 and 3 (weights 20+30=50, values 100+120=220) */
    Station items[3] = {
        {1, 0, 10, 50, 2, 60, 0},
        {2, 1, 20, 50, 2, 100, 0},
        {3, 2, 30, 50, 2, 120, 0}
    };
    KnapsackResult res = knapsack_01_dp(items, 3, 50);
    TEST_ASSERT(res.max_benefit == 220, "DP achieves guaranteed theoretical optimal benefit (220)");
    TEST_ASSERT(res.total_cost == 50, "DP consumes exact optimal budget (50)");
    TEST_ASSERT(res.selected_count == 2, "DP selects exactly 2 items for optimal packing");

    /* Boundary: zero budget */
    KnapsackResult zero_res = knapsack_01_dp(items, 3, 0);
    TEST_ASSERT(zero_res.max_benefit == 0, "Zero budget yields zero benefit");

    /* Boundary: empty candidate list */
    KnapsackResult empty_res = knapsack_01_dp(items, 0, 50);
    TEST_ASSERT(empty_res.max_benefit == 0, "Empty candidate list yields zero benefit");
}

/* -------------------------------------------------------------
 * TEST 6: Knapsack Greedy Suboptimality Counterexample
 * ------------------------------------------------------------- */
void test_knapsack_counterexample(void) {
    printf("\n[TEST MODULE 6] Knapsack Counterexample (Greedy Suboptimality Proof)\n");

    /* Counterexample where greedy ratio fails:
     * Budget: 50
     * S1: Cost 30, Benefit 33 -> Ratio 1.10
     * S2: Cost 25, Benefit 25 -> Ratio 1.00
     * S3: Cost 25, Benefit 25 -> Ratio 1.00
     * Greedy picks S1 (benefit 33, remaining budget 20, stops).
     * DP picks S2 + S3 (cost 50 <= 50, benefit 50). */
    Station items[3] = {
        {101, 0, 30, 100, 4, 33, 0},
        {102, 1, 25, 100, 4, 25, 0},
        {103, 2, 25, 100, 4, 25, 0}
    };
    KnapsackResult dp_res = knapsack_01_dp(items, 3, 50);
    KnapsackResult gr_res = knapsack_greedy_ratio(items, 3, 50);

    TEST_ASSERT(dp_res.max_benefit == 50, "DP selects optimal combination (Benefit = 50)");
    TEST_ASSERT(gr_res.max_benefit == 33, "Greedy ratio is trapped in local optimum (Benefit = 33)");
    TEST_ASSERT(dp_res.max_benefit > gr_res.max_benefit,
                "PROVEN: DP is strictly superior to Greedy heuristic on counterexample");
}

/* -------------------------------------------------------------
 * TEST 7: Dijkstra Shortest Path (Array vs Min-Heap Oracle)
 * ------------------------------------------------------------- */
void test_dijkstra_routing(void) {
    printf("\n[TEST MODULE 7] Dijkstra Shortest Path (Array vs Min-Heap)\n");

    CityNetwork city;
    city_init(&city);
    city.num_nodes = 4;
    /* Triangle with detour shortcut:
     * 0 -> 1 : 10
     * 1 -> 2 : 15
     * 0 -> 2 : 35  (detour 0->1->2 is 25, shorter than direct 35!)
     * 2 -> 3 : 20
     */
    city.adj_matrix[0][1] = 10; city.adj_matrix[1][0] = 10;
    city.adj_matrix[1][2] = 15; city.adj_matrix[2][1] = 15;
    city.adj_matrix[0][2] = 35; city.adj_matrix[2][0] = 35;
    city.adj_matrix[2][3] = 20; city.adj_matrix[3][2] = 20;

    DijkstraResult res_arr = dijkstra_array(&city, 0, 2);
    DijkstraResult res_heap = dijkstra_heap(&city, 0, 2);

    TEST_ASSERT(res_arr.is_reachable == 1, "Dijkstra Array finds destination reachable");
    TEST_ASSERT(res_heap.is_reachable == 1, "Dijkstra Heap finds destination reachable");
    TEST_ASSERT(res_arr.dist[2] == 25, "Shortest path chooses 2-hop route (dist = 25) over direct edge (35)");
    TEST_ASSERT(res_arr.dist[2] == res_heap.dist[2], "Oracle agreement: Array and Heap produce identical distance");

    /* Unreachable node test */
    CityNetwork disc_city;
    city_init(&disc_city);
    disc_city.num_nodes = 3;
    disc_city.adj_matrix[0][1] = 10; disc_city.adj_matrix[1][0] = 10;
    /* Node 2 is completely isolated */
    DijkstraResult disc_res = dijkstra_array(&disc_city, 0, 2);
    TEST_ASSERT(disc_res.is_reachable == 0, "Dijkstra detects disconnected/unreachable destination");
}

/* -------------------------------------------------------------
 * TEST 8: EV Battery Feasibility Checking
 * ------------------------------------------------------------- */
void test_ev_feasibility(void) {
    printf("\n[TEST MODULE 8] EV Battery Feasibility Verification\n");

    CityNetwork city;
    city_add_default_sample(&city);

    DijkstraResult route = dijkstra_array(&city, 0, 3); /* Downtown to Airport */
    TEST_ASSERT(route.is_reachable == 1, "Airport is reachable from Downtown");

    double battery_used = 0.0;
    /* Case A: Vehicle with large range (250 km) */
    int feasible_high = is_path_ev_feasible(&city, &route, 250.0, &battery_used);
    TEST_ASSERT(feasible_high == 1, "Vehicle with 250 km range successfully completes route");

    /* Case B: Vehicle with depleted battery (range only 10 km) */
    int feasible_low = is_path_ev_feasible(&city, &route, 10.0, &battery_used);
    TEST_ASSERT(feasible_low == 0, "Vehicle with insufficient battery fails feasibility check safely");
}

/* -------------------------------------------------------------
 * MAIN TEST RUNNER
 * ------------------------------------------------------------- */
int main(void) {
    printf("=========================================================\n");
    printf("      EV SYSTEM AUTOMATED TEST SUITE (PHASE I CORE)      \n");
    printf("=========================================================\n");

    test_model_loading_and_validation();
    test_merge_sort();
    test_quick_sort();
    test_binary_search();
    test_knapsack_dp();
    test_knapsack_counterexample();
    test_dijkstra_routing();
    test_ev_feasibility();

    printf("\n=========================================================\n");
    printf(" FINAL TEST SUMMARY: %d / %d Tests PASSED\n", g_tests_passed, g_tests_run);
    if (g_tests_passed == g_tests_run) {
        printf(" ALL TESTS PASSED SUCCESSFULLY! (100%% Green)\n");
    } else {
        printf(" WARNING: %d test(s) failed!\n", g_tests_run - g_tests_passed);
    }
    printf("=========================================================\n");

    return (g_tests_passed == g_tests_run) ? 0 : 1;
}
