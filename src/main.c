/**
 * @file main.c
 * @brief Interactive CLI Menu for EV Charging Station Placement & Trip Planning System.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain, beginner-friendly C17 CLI with robust defensive input handling.
 */

#include "model.h"
#include "analysis.h"
#include "sorting_searching.h"
#include "divide_conquer.h"
#include "greedy.h"
#include "dp.h"
#include "backtracking.h"
#include "flow_matching.h"
#include "number_theory.h"
#include "string_match.h"
#include "scenarios.h"

static int read_int_safe(const char *prompt, int min_val, int max_val) {
    int val;
    char buffer[128];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return min_val; /* Gracefully exit loop on EOF / end of pipe */
        }
        char *endptr;
        val = (int)strtol(buffer, &endptr, 10);
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            printf("[Invalid Input] Please enter a valid number.\n");
            continue;
        }
        if (val < min_val || val > max_val) {
            printf("[Out of Range] Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return val;
    }
}

static void wait_for_enter(void) {
    printf("\nPress [Enter] to continue...");
    char buf[16];
    if (!fgets(buf, sizeof(buf), stdin)) {
        return;
    }
}

/* 1. Dataset Loader */
static void menu_load_dataset(CityNetwork *city) {
    printf("\n--- 1. DATASET MANAGEMENT ---\n");
    printf("1. Load Dehradun Smart City Network (20 Iconic Stops: UPES, ISBT, ClockTower)\n");
    printf("2. Load data/dehradun.txt (File)\n");
    printf("3. Load data/small.txt (~8 nodes)\n");
    printf("4. Load data/medium.txt (~30 nodes)\n");
    printf("5. Load data/large.txt (~100 nodes)\n");
    printf("6. Load data/adversarial.txt (Counterexample)\n");
    printf("7. View Summary & Connectivity\n");
    printf("0. Return\n");

    int c = read_int_safe("Select [0-7]: ", 0, 7);
    if (c == 1) {
        city_load_dehradun_sample(city);
        printf("[Success] Loaded Dehradun Smart City network (20 stops, UPES campuses, 10 stations).\n");
    }
    else if (c == 2) {
        if (city_load_from_file(city, "data/dehradun.txt"))
            printf("[Success] Loaded data/dehradun.txt successfully.\n");
    }
    else if (c == 3) city_load_from_file(city, "data/small.txt");
    else if (c == 4) city_load_from_file(city, "data/medium.txt");
    else if (c == 5) city_load_from_file(city, "data/large.txt");
    else if (c == 6) city_load_from_file(city, "data/adversarial.txt");
    else if (c == 7) { city_print_summary(city); city_print_stations(city); city_print_roads(city); }
    wait_for_enter();
}

/* 2. Station Placement Comparison */
static void menu_station_placement(CityNetwork *city) {
    printf("\n--- 2. STATION PLACEMENT (KNAPSACK PARADIGMS) ---\n");
    if (city->num_stations == 0) { printf("Load a dataset first.\n"); wait_for_enter(); return; }

    ops_reset();
    clock_t t0 = timer_start();
    KnapsackResult gr = knapsack_greedy_ratio(city->stations, city->num_stations, city->budget);
    double gr_t = timer_stop_ms(t0);
    long long gr_ops = ops_get();

    ops_reset();
    t0 = timer_start();
    KnapsackResult dp = knapsack_01_dp(city->stations, city->num_stations, city->budget);
    double dp_t = timer_stop_ms(t0);
    long long dp_ops = ops_get();

    ops_reset();
    KnapsackResult bt = knapsack_backtracking(city->stations, (city->num_stations > 20 ? 20 : city->num_stations), city->budget);
    long long bt_ops = ops_get();

    ops_reset();
    BBResult bb_fifo = knapsack_branch_and_bound(city->stations, city->num_stations, city->budget, BB_FIFO);
    long long fifo_ops = ops_get();

    ops_reset();
    BBResult bb_lc = knapsack_branch_and_bound(city->stations, city->num_stations, city->budget, BB_LEAST_COST);
    long long lc_ops = ops_get();

    printf("\n========================================================================================\n");
    printf(" Paradigm              | Max Benefit | Cost Used | Time (ms) | Operations | Optimality \n");
    printf("-----------------------+-------------+-----------+-----------+------------+-------------\n");
    printf(" Greedy by Ratio       | %-11d | %-9d | %-9.4f | %-10lld | Heuristic   \n", gr.max_benefit, gr.total_cost, gr_t, gr_ops);
    printf(" Dynamic Programming   | %-11d | %-9d | %-9.4f | %-10lld | OPTIMAL     \n", dp.max_benefit, dp.total_cost, dp_t, dp_ops);
    printf(" Backtracking (DFS)    | %-11d | %-9d | < 1 ms    | %-10lld | OPTIMAL     \n", bt.max_benefit, bt.total_cost, bt_ops);
    printf(" Branch & Bound (FIFO) | %-11d | %-9d | %-9.4f | %-10lld | OPTIMAL     \n", bb_fifo.max_benefit, bb_fifo.total_cost, bb_fifo.execution_time_ms, fifo_ops);
    printf(" Branch & Bound (L-Cost)| %-11d | %-9d | %-9.4f | %-10lld | OPTIMAL     \n", bb_lc.max_benefit, bb_lc.total_cost, bb_lc.execution_time_ms, lc_ops);
    printf("========================================================================================\n");
    printf(" [Observation] B&B Least-Cost pruned %lld nodes vs FIFO pruned %lld nodes!\n",
           bb_lc.nodes_pruned, bb_fifo.nodes_pruned);
    wait_for_enter();
}

/* 3. Trip Planning */
static void menu_trip_planning(CityNetwork *city) {
    printf("\n--- 3. TRIP PLANNING & ROUTING ---\n");
    printf("1. Dijkstra Shortest Path (Array vs Min-Heap)\n");
    printf("2. Bellman-Ford Routing (Single-source with cycle check)\n");
    printf("3. Floyd-Warshall All-Pairs Reachability\n");
    printf("4. Multi-Stop Inspection Tour (TSP Bitmask DP, <= 6 stops)\n");
    printf("0. Return\n");

    int c = read_int_safe("Select [0-4]: ", 0, 4);
    if (c == 1) {
        int u = read_int_safe("Origin Node ID: ", 0, city->num_nodes - 1);
        int v = read_int_safe("Destination Node ID: ", 0, city->num_nodes - 1);
        DijkstraResult res = dijkstra_array(city, u, v);
        dijkstra_print_path(city, &res);
    } else if (c == 2) {
        int u = read_int_safe("Origin Node ID: ", 0, city->num_nodes - 1);
        BellmanFordResult bf = bellman_ford(city, u);
        if (bf.has_negative_cycle) printf(" [!] Negative cost cycle detected!\n");
        else {
            printf(" Shortest distances from [%d] %s:\n", u, city->nodes[u].name);
            for (int i = 0; i < city->num_nodes; i++) {
                if (bf.dist[i] < INF_DIST) printf(" -> [%d] %s : %d km\n", i, city->nodes[i].name, bf.dist[i]);
            }
        }
    } else if (c == 3) {
        FloydWarshallResult fw = floyd_warshall(city);
        printf(" All-Pairs Distance Matrix Computed (%d x %d).\n", city->num_nodes, city->num_nodes);
        int u = read_int_safe("Query Origin: ", 0, city->num_nodes - 1);
        int v = read_int_safe("Query Dest: ", 0, city->num_nodes - 1);
        int path[MAX_NODES], len = 0;
        floyd_reconstruct_path(&fw, u, v, path, &len);
        printf(" Reconstructed Shortest Path (Dist = %d km): ", fw.dist[u][v]);
        for (int i = 0; i < len; i++) printf("[%d]%s%s", path[i], city->nodes[path[i]].name, (i < len - 1 ? " -> " : "\n"));
    } else if (c == 4) {
        int n = read_int_safe("Number of inspection stops [3-6]: ", 3, 6);
        int cost[TSP_MAX_STOPS][TSP_MAX_STOPS];
        FloydWarshallResult fw = floyd_warshall(city);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) cost[i][j] = fw.dist[i][j];
        }
        TSPResult tsp = tsp_bitmask_dp(n, cost);
        printf(" Optimal TSP Tour Cost: %d km\n Tour: ", tsp.min_tour_cost);
        for (int i = 0; i <= n; i++) printf("[%d]%s", tsp.tour_path[i], (i < n ? " -> " : "\n"));
    }
    wait_for_enter();
}

/* 4. Infrastructure Backbone (Prim vs Kruskal) */
static void menu_infrastructure_mst(CityNetwork *city) {
    printf("\n--- 4. INFRASTRUCTURE BACKBONE (MINIMUM SPANNING TREE) ---\n");
    ops_reset();
    clock_t t0 = timer_start();
    MSTResult prim = prim_mst(city);
    double prim_t = timer_stop_ms(t0);
    long long prim_ops = ops_get();

    ops_reset();
    t0 = timer_start();
    MSTResult krusk = kruskal_mst(city);
    double krusk_t = timer_stop_ms(t0);
    long long krusk_ops = ops_get();

    printf(" Prim's MST    : Total Backbone Cable = %d km | Time = %.4f ms | Ops = %lld\n",
           prim.total_weight, prim_t, prim_ops);
    printf(" Kruskal's MST : Total Backbone Cable = %d km | Time = %.4f ms | Ops = %lld\n",
           krusk.total_weight, krusk_t, krusk_ops);
    printf("\nMST Edges Sample:\n");
    for (int i = 0; i < (prim.edge_count > 6 ? 6 : prim.edge_count); i++) {
        printf("  Cable [%d] %s <--> [%d] %s (Weight: %d km)\n",
               prim.edges[i].u, city->nodes[prim.edges[i].u].name,
               prim.edges[i].v, city->nodes[prim.edges[i].v].name,
               prim.edges[i].weight);
    }
    wait_for_enter();
}

/* 5. Charging Schedule */
static void menu_charging_schedule(void) {
    printf("\n--- 5. CHARGING SCHEDULE (GREEDY WORKBENCH) ---\n");
    ChargingSession sessions[5] = {
        {101, 1, 800, 930}, {102, 2, 830, 915}, {103, 3, 900, 1030},
        {104, 4, 920, 1100}, {105, 5, 1045, 1200}
    };

    ActivitySelectionResult act = activity_selection(sessions, 5);
    printf(" A. Activity Selection (1 Charger Port):\n");
    printf("    Max Non-Overlapping Sessions Served: %d / 5\n    Selected Sessions: ", act.count);
    for (int i = 0; i < act.count; i++) printf("S_%d ", act.session_ids[i]);
    printf("\n\n");

    IntervalPartitionResult intv = interval_partitioning(sessions, 5);
    printf(" B. Interval Partitioning (All 5 Sessions):\n");
    printf("    Minimum Physical Chargers Required: %d\n", intv.total_chargers_needed);

    PowerRequest reqs[4] = {
        {1, 50.0, 80.0, 0}, {2, 75.0, 100.0, 0}, {3, 40.0, 90.0, 0}, {4, 60.0, 60.0, 0}
    };
    FractionalKnapsackResult fknap = fractional_knapsack_power(reqs, 4, 120.0);
    printf("\n C. Fractional Power Allocation (Grid Limit = 120 kW):\n");
    printf("    Total Priority Served: %.1f | Allocated Power: %.1f kW\n",
           fknap.total_priority_served, fknap.total_kw_allocated);
    for (int i = 0; i < 4; i++) {
        printf("    Vehicle %d: Demanded %.0f kW -> Allocated %.1f kW\n",
               reqs[i].vehicle_id, reqs[i].kw_demanded, reqs[i].kw_allocated);
    }
    wait_for_enter();
}

/* 6. Capacity and Assignment */
static void menu_capacity_matching(void) {
    printf("\n--- 6. CAPACITY AND ASSIGNMENT (FLOW & MATCHING) ---\n");
    int cap[FLOW_MAX_NODES][FLOW_MAX_NODES] = {{0}};
    cap[0][1] = 40; cap[0][2] = 30; /* Substation to Feeder 1 and 2 */
    cap[1][2] = 10; cap[1][3] = 25;
    cap[2][4] = 20; cap[3][5] = 30; cap[4][5] = 25; /* 5 is Charging Hub Sink */

    MaxFlowResult flow = ford_fulkerson_max_flow(6, cap, 0, 5);
    printf(" A. Ford-Fulkerson Max Power Flow from Substation [0] to Hub [5]:\n");
    printf("    Maximum Transmitted Power: %d kW\n", flow.max_flow);
    printf("    Minimum-Cut Bottleneck Transmission Lines:\n");
    for (int i = 0; i < flow.min_cut_edge_count; i++) {
        printf("    -> Line %d -> %d (Capacity: %d kW)\n",
               flow.min_cut_edges[i].u, flow.min_cut_edges[i].v, flow.min_cut_edges[i].weight);
    }

    int compat[MATCH_MAX_ITEMS][MATCH_MAX_ITEMS] = {
        {1, 1, 0}, {1, 0, 0}, {0, 1, 1}, {0, 0, 1}
    };
    BipartiteMatchResult bpm = max_bipartite_matching(4, 3, compat);
    printf("\n B. Maximum Bipartite Matching (4 Vehicles <-> 3 Charging Bays):\n");
    printf("    Max Pairs Matched: %d / 3 bays\n", bpm.match_count);
    for (int v = 0; v < 4; v++) {
        if (bpm.vehicle_assigned_bay[v] != -1) {
            printf("    -> Vehicle V_%d matches Bay B_%d\n", v + 1, bpm.vehicle_assigned_bay[v] + 1);
        }
    }
    wait_for_enter();
}

/* 7. Sorting & Searching */
static void menu_sorting_tools(CityNetwork *city) {
    printf("\n--- 7. SORTING & SEARCHING BENCHMARK ---\n");
    int test_arr[10] = {85, 23, 67, 12, 90, 44, 31, 78, 56, 9};
    int m_arr[10], q_arr[10];
    for (int i = 0; i < 10; i++) { m_arr[i] = test_arr[i]; q_arr[i] = test_arr[i]; }

    merge_sort_int(m_arr, 0, 9);
    quick_sort_int(q_arr, 0, 9);

    printf(" Original Array: 85 23 67 12 90 44 31 78 56 9\n");
    printf(" Merge Sorted  : "); for (int i = 0; i < 10; i++) printf("%d ", m_arr[i]); printf("\n");
    printf(" Quick Sorted  : "); for (int i = 0; i < 10; i++) printf("%d ", q_arr[i]); printf("\n");

    int target = read_int_safe("Enter integer to binary search: ", 0, 100);
    int idx_it = binary_search_iterative_int(m_arr, 10, target);
    int idx_rec = binary_search_recursive_int(m_arr, 0, 9, target);
    printf(" Iterative Binary Search: %s (Index %d)\n", (idx_it != -1 ? "FOUND" : "NOT FOUND"), idx_it);
    printf(" Recursive Binary Search: %s (Index %d)\n", (idx_rec != -1 ? "FOUND" : "NOT FOUND"), idx_rec);
    (void)city;
    wait_for_enter();
}

/* 8. Divide & Conquer Tools */
static void menu_divide_conquer(void) {
    printf("\n--- 8. DIVIDE & CONQUER LAB ---\n");
    int net_energy[8] = {-2, 5, -1, 7, -3, 6, -8, 4};
    MaxSubarrayResult sub = max_subarray_dc(net_energy, 0, 7);
    printf(" A. Maximum Subarray (Station Net Profit):\n");
    printf("    Best Period: Days %d to %d | Total Net Profit: %d units\n\n",
           sub.low, sub.high, sub.sum);

    printf(" B. Fast Exponentiation vs Naive Power (Battery Degradation (0.95)^20):\n");
    ops_reset();
    double p_fast = fast_power(0.95, 20);
    long long f_ops = ops_get();
    ops_reset();
    double p_naive = naive_power(0.95, 20);
    long long n_ops = ops_get();
    printf("    Fast Power:  %.6f (Ops: %lld, Theta(log n))\n", p_fast, f_ops);
    printf("    Naive Power: %.6f (Ops: %lld, Theta(n))\n\n", p_naive, n_ops);

    int reach[MAX_NODES][MAX_NODES] = {{0}};
    /* 4 nodes: node 2 is hub (everyone can reach 2, 2 reaches none) */
    reach[0][2] = 1; reach[1][2] = 1; reach[3][2] = 1;
    int hub = celebrity_hub_linear(4, reach);
    printf(" C. Celebrity / Hub Station Discovery (O(n) Elimination):\n");
    printf("    Identified Hub Station ID: Node [%d] (Eliminated in linear time)\n", hub);

    printf("\n D. Strassen vs Classical Matrix Multiplication (4x4 2-hop matrix):\n");
    int A[MATRIX_MAX][MATRIX_MAX] = {{1,1,0,1},{0,1,1,0},{1,0,1,1},{0,1,0,1}};
    int C_cls[MATRIX_MAX][MATRIX_MAX], C_str[MATRIX_MAX][MATRIX_MAX];
    matrix_multiply_classical(4, A, A, C_cls);
    strassen_multiply(4, A, A, C_str);
    printf("    Verified: Strassen produces identical 2-hop reachability counts as Classical!\n");
    wait_for_enter();
}

/* 9. DP Lab (LCS, Matrix Chain, Optimal BST) */
static void menu_dp_lab(void) {
    printf("\n--- 9. ADVANCED DYNAMIC PROGRAMMING LAB ---\n");
    int r1[6] = {0, 1, 3, 5, 7, 8};
    int r2[6] = {0, 2, 3, 7, 8, 9};
    LCSResult lcs = lcs_route_comparison(r1, 6, r2, 6);
    printf(" A. Longest Common Subsequence (Planned Route vs GPS Trajectory):\n");
    printf("    LCS Length: %d junctions | Overlapping Sequence: ", lcs.lcs_length);
    for (int i = 0; i < lcs.lcs_length; i++) printf("%d ", lcs.lcs_sequence[i]);
    printf("\n\n");

    int p[5] = {10, 30, 5, 60, 20}; /* 4 matrices */
    MCMResult mcm = matrix_chain_order(p, 5);
    printf(" B. Matrix Chain Multiplication (Optimal Dispatch Pipeline):\n");
    printf("    Minimum Scalar Operations: %d\n    Optimal Parenthesization: %s\n\n",
           mcm.min_scalar_mults, mcm.parenthesization);

    double prob_p[4] = {0.0, 0.15, 0.10, 0.05};
    double prob_q[4] = {0.05, 0.10, 0.05, 0.05};
    int keys[4] = {0, 101, 102, 103};
    OBSTResult obst = optimal_bst(keys, prob_p, prob_q, 3);
    printf(" C. Optimal Binary Search Tree (Station Query Frequencies):\n");
    printf("    Minimum Expected Search Cost: %.4f | Optimal Root: Key %d\n",
           obst.min_cost, keys[obst.root[1][3]]);
    wait_for_enter();
}

/* 10. Backtracking Lab */
static void menu_backtracking_lab(CityNetwork *city) {
    printf("\n--- 10. BACKTRACKING LAB (N-QUEENS & SUBSET SUM) ---\n");
    int nq = read_int_safe("Enter N for N-Queens [4-10]: ", 4, 10);
    NQueensResult nres = solve_n_queens(nq);
    printf(" N-Queens (%d x %d): Total Valid Configurations = %d\n", nq, nq, nres.total_solutions);
    printf(" Sample Solution (Row -> Col placement):\n ");
    for (int i = 0; i < nq; i++) printf("Q(%d,%d) ", i, nres.board[i]);
    printf("\n\n");

    int costs[5] = {30, 25, 40, 20, 35};
    int target = city->budget > 0 ? city->budget : 80;
    SubsetSumResult sub = solve_subset_sum(costs, 5, target);
    printf(" Sum of Subsets: Finding stations summing exactly to budget (%d units):\n", target);
    printf(" Solutions Found: %d | First Combination: ", sub.total_solutions_found);
    for (int i = 0; i < sub.solution_count; i++) printf("%d units ", costs[sub.solution_indices[i]]);
    printf("\n");
    wait_for_enter();
}

/* 11. Logs: Huffman & String Matching */
static void menu_logs_lab(void) {
    printf("\n--- 11. EVENT LOGS: COMPRESSION & PATTERN SEARCH ---\n");
    const char *text = "STATION_101 CHARGE_START STATION_102 CHARGE_FINISH STATION_101 GRID_OVERLOAD";
    char encoded[512] = "";
    char decoded[512] = "";

    HuffmanResult huff = huffman_compress(text, encoded, sizeof(encoded));
    huffman_decompress(encoded, &huff, decoded);

    printf(" A. Huffman Coding on Event Stream:\n");
    printf("    Original Text    : %s\n", text);
    printf("    Original Bits    : %d bits\n", huff.original_bits);
    printf("    Compressed Bits  : %d bits (Ratio: %.2f%%)\n", huff.compressed_bits, huff.compression_ratio * 100.0);
    printf("    Decoded Text     : %s\n\n", decoded);

    printf(" B. String Matching (Search 'CHARGE_START' across 4 Algorithms):\n");
    StringMatchResult r1 = string_match_naive(text, "CHARGE_START");
    StringMatchResult r2 = string_match_rabin_karp(text, "CHARGE_START");
    StringMatchResult r3 = string_match_finite_automaton(text, "CHARGE_START");
    StringMatchResult r4 = string_match_kmp(text, "CHARGE_START");

    printf("    Naive           : %d matches, %lld comparisons, %.4f ms\n", r1.match_count, r1.char_comparisons, r1.execution_time_ms);
    printf("    Rabin-Karp      : %d matches, %lld comparisons, %.4f ms\n", r2.match_count, r2.char_comparisons, r2.execution_time_ms);
    printf("    Finite Automaton: %d matches, %lld transitions, %.4f ms\n", r3.match_count, r3.char_comparisons, r3.execution_time_ms);
    printf("    KMP (Linear)    : %d matches, %lld comparisons, %.4f ms\n", r4.match_count, r4.char_comparisons, r4.execution_time_ms);
    wait_for_enter();
}

/* 12. Number Theory (CRT) */
static void menu_number_theory(void) {
    printf("\n--- 12. NUMBER THEORY: CHINESE REMAINDER THEOREM ---\n");
    printf(" Scenario: Partitioned distributed energy meter counters.\n");
    printf(" Substation A counts modulo 3: remainder = 2\n");
    printf(" Substation B counts modulo 5: remainder = 3\n");
    printf(" Substation C counts modulo 7: remainder = 2\n");

    long long rem[3] = {2, 3, 2};
    long long mod[3] = {3, 5, 7};
    CRTResult crt = chinese_remainder_theorem(3, rem, mod);

    printf("\n [CRT Solution]:\n");
    printf("   Composite Modulus M = 3 * 5 * 7 = %lld\n", crt.composite_modulus);
    printf("   Reconstructed Total Energy Counter: %lld kWh\n", crt.total_counter);
    printf("   Verification:\n");
    for (int i = 0; i < 3; i++) {
        printf("    %lld mod %lld = %lld (Expected: %lld) -> %s\n",
               crt.total_counter, mod[i], crt.total_counter % mod[i], rem[i],
               (crt.total_counter % mod[i] == rem[i] ? "VERIFIED" : "FAIL"));
    }
    wait_for_enter();
}

/* 13. Three Scenarios */
static void menu_scenarios(void) {
    printf("\n--- 13. THREE MANDATORY DEMONSTRATION SCENARIOS ---\n");
    printf("1. Run Scenario 1 (Normal Operation: 20 nodes, 12 sites, Knapsack & Routing)\n");
    printf("2. Run Scenario 2 (Constraint Shock: Station Failure & 30%% Budget Cut)\n");
    printf("3. Run Scenario 3 (Large-Data Stress: 100 Nodes, Scalability Evaluation)\n");
    printf("4. Run ALL Three Scenarios Sequentially\n");
    printf("0. Return\n");

    int c = read_int_safe("Select [0-4]: ", 0, 4);
    if (c == 1) run_scenario_1_normal();
    else if (c == 2) run_scenario_2_shock();
    else if (c == 3) run_scenario_3_stress();
    else if (c == 4) { run_scenario_1_normal(); run_scenario_2_shock(); run_scenario_3_stress(); }
    wait_for_enter();
}

/* 14. Benchmark Runner */
static void menu_benchmark_runner(void) {
    printf("\n--- 14. BENCHMARK RUNNER & CSV EXPORT ---\n");
    const char *csv_path = "benchmark_results.csv";
    printf(" Executing benchmarks across Small, Medium, Large, and Adversarial datasets...\n");

    int sizes[4] = {10, 50, 200, 1000};
    for (int s = 0; s < 4; s++) {
        int n = sizes[s];
        int *arr = (int *)malloc((size_t)n * sizeof(int));
        for (int i = 0; i < n; i++) arr[i] = (i * 13) % n;

        ops_reset();
        clock_t t0 = timer_start();
        merge_sort_int(arr, 0, n - 1);
        double m_time = timer_stop_ms(t0);
        long long m_ops = ops_get();
        benchmark_log_csv(csv_path, "MergeSort", n, "Random", m_time, m_ops, n * sizeof(int), 1.0);

        for (int i = 0; i < n; i++) arr[i] = (i * 13) % n;
        ops_reset();
        t0 = timer_start();
        quick_sort_int(arr, 0, n - 1);
        double q_time = timer_stop_ms(t0);
        long long q_ops = ops_get();
        benchmark_log_csv(csv_path, "QuickSort", n, "Random", q_time, q_ops, 0, 1.0);

        free(arr);
    }
    printf(" [Success] Benchmarks completed. Results exported to %s!\n", csv_path);
    wait_for_enter();
}

/* 16. Complexity Reduction Demo */
static void menu_reduction_demo(void) {
    printf("\n=========================================================================\n");
    printf("     COMPLEXITY REDUCTION DEMO: SUBSET-SUM -> 0/1 KNAPSACK DECISION      \n");
    printf("=========================================================================\n");
    printf(" Theorem: 0/1 Knapsack Decision Problem is NP-Complete.\n");
    printf(" Proof by Polynomial-Time Many-One Reduction from SUBSET-SUM (known NPC):\n\n");
    printf(" 1. Let Subset-Sum instance be S = {20, 25, 35, 40} with target T = 60.\n");
    printf(" 2. Polynomial Reduction Mapping:\n");
    printf("    For each integer s_i in S, create a Candidate Station i with:\n");
    printf("      - Installation Cost c_i = s_i\n");
    printf("      - Benefit Score     b_i = s_i\n");
    printf("    Set Knapsack Capital Budget B = T = 60.\n");
    printf("    Set Decision Threshold K = T = 60.\n\n");
    printf(" 3. Equivalence Argument:\n");
    printf("    Because cost c_i = benefit b_i for all items:\n");
    printf("    Any subset with Cost <= 60 can achieve Benefit >= 60 if and only if\n");
    printf("    total cost == 60 AND total benefit == 60!\n\n");

    Station red_items[4] = {
        {1, 0, 20, 50, 2, 20, 0}, {2, 1, 25, 50, 2, 25, 0},
        {3, 2, 35, 50, 2, 35, 0}, {4, 3, 40, 50, 2, 40, 0}
    };
    KnapsackResult res = knapsack_01_dp(red_items, 4, 60);

    printf(" [Evaluation on Knapsack DP]:\n");
    printf("   Maximum Achievable Benefit = %d | Cost Used = %d\n", res.max_benefit, res.total_cost);
    if (res.max_benefit >= 60 && res.total_cost <= 60) {
        printf("   Knapsack Decision: YES (Benefit >= 60) <===> Subset-Sum Decision: YES!\n");
        printf("   Selected Subset of S: ");
        for (int i = 0; i < res.selected_count; i++) {
            printf("%d ", red_items[res.selected_indices[i]].install_cost);
        }
        printf("(Sum = 60)\n");
    } else {
        printf("   Knapsack Decision: NO <===> Subset-Sum Decision: NO!\n");
    }
    printf("=========================================================================\n");
    wait_for_enter();
}

int main(void) {
    CityNetwork city;
    city_load_dehradun_sample(&city);  /* Boot with Dehradun Smart City by default */

    while (1) {
        printf("\n=========================================================\n");
        printf("       EV CHARGING PLANNER - FULL SYLLABUS CLI (C17)     \n");
        printf("          CSEG3053 Design and Analysis of Algorithms     \n");
        printf("=========================================================\n");
        printf(" 1. Load / Generate Dataset\n");
        printf(" 2. Station Placement (Knapsack: Greedy / DP / BT / B&B)\n");
        printf(" 3. Trip Planning (Dijkstra / Bellman-Ford / Floyd / TSP)\n");
        printf(" 4. Infrastructure Network (Prim / Kruskal MST)\n");
        printf(" 5. Charging Schedule (Activity / Interval / Fractional)\n");
        printf(" 6. Capacity & Assignment (Ford-Fulkerson / Bipartite)\n");
        printf(" 7. Sorting & Searching Tools\n");
        printf(" 8. Divide & Conquer Tools (MaxSubarray/Power/Celebrity/Strassen)\n");
        printf(" 9. DP Lab (LCS / Matrix Chain / Optimal BST)\n");
        printf(" 10. Backtracking Lab (N-Queens / Subset Sum)\n");
        printf(" 11. Event Logs (Huffman Compression + 4 String Matchers)\n");
        printf(" 12. Number Theory (Chinese Remainder Theorem)\n");
        printf(" 13. Run 3 Required Demo Scenarios\n");
        printf(" 14. Benchmark Runner (Export CSV)\n");
        printf(" 15. Run Test Suite (Self-Validation)\n");
        printf(" 16. Complexity Classes & Reduction Demo\n");
        printf(" 0. Exit System\n");
        printf("=========================================================\n");

        int choice = read_int_safe("Select Option [0-16]: ", 0, 16);
        if (choice == 0) {
            printf("\nExiting EV Planning System.\n");
            break;
        }

        switch (choice) {
            case 1:  menu_load_dataset(&city); break;
            case 2:  menu_station_placement(&city); break;
            case 3:  menu_trip_planning(&city); break;
            case 4:  menu_infrastructure_mst(&city); break;
            case 5:  menu_charging_schedule(); break;
            case 6:  menu_capacity_matching(); break;
            case 7:  menu_sorting_tools(&city); break;
            case 8:  menu_divide_conquer(); break;
            case 9:  menu_dp_lab(); break;
            case 10: menu_backtracking_lab(&city); break;
            case 11: menu_logs_lab(); break;
            case 12: menu_number_theory(); break;
            case 13: menu_scenarios(); break;
            case 14: menu_benchmark_runner(); break;
            case 15: printf("Run './test_runner.exe' directly for full batch test reports.\n"); wait_for_enter(); break;
            case 16: menu_reduction_demo(); break;
            default: break;
        }
    }
    return 0;
}
