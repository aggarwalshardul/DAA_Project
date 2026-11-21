/**
 * @file scenarios.c
 * @brief Implementation of the Three Required Practical Evaluation Scenarios.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "scenarios.h"

/* -------------------------------------------------------------
 * SCENARIO 1: NORMAL OPERATION
 * 20 nodes, 12 candidate sites, budget fits ~half.
 * Compare: Knapsack Greedy vs DP vs B&B; Dijkstra vs Bellman-Ford on 3 trips.
 * ------------------------------------------------------------- */
void run_scenario_1_normal(void) {
    printf("\n=========================================================================\n");
    printf("                SCENARIO 1: NORMAL OPERATION DEMONSTRATION               \n");
    printf("=========================================================================\n");
    printf(" City Model: 20 Junctions | 12 Candidate Stations | Capital Budget: 180\n\n");

    CityNetwork city;
    city_init(&city);
    city.num_nodes = 20;
    city.budget = 180;

    for (int i = 0; i < 20; i++) {
        city.nodes[i].id = i;
        snprintf(city.nodes[i].name, sizeof(city.nodes[i].name), "Zone_%02d", i);
    }
    for (int i = 0; i < 19; i++) {
        int d = (i % 5) * 4 + 8;
        city.adj_matrix[i][i + 1] = d; city.adj_matrix[i + 1][i] = d;
    }
    city.adj_matrix[19][0] = 15; city.adj_matrix[0][19] = 15;
    city.adj_matrix[0][10] = 22; city.adj_matrix[10][0] = 22;
    city.adj_matrix[5][15] = 25; city.adj_matrix[15][5] = 25;

    city.num_stations = 12;
    Station sites[12] = {
        {101, 0,  30, 150, 4, 85, 0}, {102, 2,  25, 100, 3, 65, 0},
        {103, 4,  40, 180, 5, 95, 0}, {104, 6,  20, 80,  2, 50, 0},
        {105, 8,  35, 120, 4, 75, 0}, {106, 10, 50, 200, 6, 115, 0},
        {107, 12, 28, 90,  3, 60, 0}, {108, 14, 45, 160, 5, 100, 0},
        {109, 16, 22, 70,  2, 48, 0}, {110, 18, 38, 140, 4, 88, 0},
        {111, 1,  32, 110, 3, 70, 0}, {112, 11, 26, 95,  3, 58, 0}
    };
    for (int i = 0; i < 12; i++) city.stations[i] = sites[i];

    /* PART A: Station Placement Algorithm Comparison */
    printf("--- PART A: STATION PLACEMENT STRATEGY COMPARISON ---\n");

    ops_reset();
    clock_t t0 = timer_start();
    KnapsackResult gr_res = knapsack_greedy_ratio(city.stations, 12, city.budget);
    double gr_time = timer_stop_ms(t0);
    long long gr_ops = ops_get();

    ops_reset();
    t0 = timer_start();
    KnapsackResult dp_res = knapsack_01_dp(city.stations, 12, city.budget);
    double dp_time = timer_stop_ms(t0);
    long long dp_ops = ops_get();

    ops_reset();
    BBResult bb_res = knapsack_branch_and_bound(city.stations, 12, city.budget, BB_LEAST_COST);
    long long bb_ops = ops_get();

    printf(" Strategy         | Total Benefit | Cost Used | Time (ms) | Operations | Optimality \n");
    printf("------------------+---------------+-----------+-----------+------------+------------\n");
    printf(" Greedy Ratio     | %-13d | %-9d | %-9.4f | %-10lld | Heuristic  \n",
           gr_res.max_benefit, gr_res.total_cost, gr_time, gr_ops);
    printf(" Dynamic Prog     | %-13d | %-9d | %-9.4f | %-10lld | OPTIMAL    \n",
           dp_res.max_benefit, dp_res.total_cost, dp_time, dp_ops);
    printf(" Branch & Bound LC| %-13d | %-9d | %-9.4f | %-10lld | OPTIMAL    \n",
           bb_res.max_benefit, bb_res.total_cost, bb_res.execution_time_ms, bb_ops);
    printf("-------------------------------------------------------------------------\n");

    /* PART B: Trip Planning Routing Comparison */
    printf("\n--- PART B: ROUTING ALGORITHM COMPARISON (3 SAMPLE TRIPS) ---\n");
    int test_trips[3][2] = {{0, 10}, {4, 16}, {1, 15}};

    for (int i = 0; i < 3; i++) {
        int u = test_trips[i][0];
        int v = test_trips[i][1];

        ops_reset();
        t0 = timer_start();
        DijkstraResult dij = dijkstra_array(&city, u, v);
        double dij_time = timer_stop_ms(t0);
        long long dij_ops = ops_get();

        ops_reset();
        t0 = timer_start();
        BellmanFordResult bf = bellman_ford(&city, u);
        double bf_time = timer_stop_ms(t0);
        long long bf_ops = ops_get();

        printf(" Trip %d (%s -> %s):\n", i + 1, city.nodes[u].name, city.nodes[v].name);
        printf("   Dijkstra Array:    Distance = %d km | Time = %.4f ms | Ops = %lld\n",
               dij.dist[v], dij_time, dij_ops);
        printf("   Bellman-Ford:      Distance = %d km | Time = %.4f ms | Ops = %lld\n",
               bf.dist[v], bf_time, bf_ops);
    }
    printf("=========================================================================\n");
}

/* -------------------------------------------------------------
 * SCENARIO 2: CONSTRAINT SHOCK
 * Key station goes offline AND budget cut by 30%.
 * Show: change in benefit, trips becoming infeasible, bipartite bay reduction.
 * ------------------------------------------------------------- */
void run_scenario_2_shock(void) {
    printf("\n=========================================================================\n");
    printf("               SCENARIO 2: CONSTRAINT SHOCK DEMONSTRATION                \n");
    printf("=========================================================================\n");

    CityNetwork city;
    city_add_default_sample(&city);
    int original_budget = city.budget; /* 100 */
    KnapsackResult orig_plan = knapsack_01_dp(city.stations, city.num_stations, original_budget);

    printf(" Normal Status:\n");
    printf("   Budget: %d units | Planned Benefit: %d | Active Sites: %d\n",
           original_budget, orig_plan.max_benefit, orig_plan.selected_count);

    /* SHOCK APPLIED:
     * 1. Station ID 101 at node 0 goes completely OFFLINE (damaged transformer).
     * 2. Budget is slashed by 30%: 100 -> 70.
     */
    printf("\n [!] CONSTRAINT SHOCK TRIGGERED:\n");
    printf("   - Station 101 at Node 0 (Downtown Hub) is OFFLINE (Transformer Failure)!\n");
    printf("   - Capital Budget reduced by 30%%: from %d to %d units!\n",
           original_budget, (int)(original_budget * 0.70));

    city.stations[0].install_cost = INF_DIST; /* Cannot be selected */
    city.stations[0].benefit_score = 0;
    int shocked_budget = (int)(original_budget * 0.70);

    KnapsackResult shocked_plan = knapsack_01_dp(city.stations, city.num_stations, shocked_budget);

    printf("\n Post-Shock Re-Optimization Results:\n");
    printf("   New Capital Budget     : %d units\n", shocked_budget);
    printf("   Achieved Benefit       : %d (Loss: %d points, -%.1f%%)\n",
           shocked_plan.max_benefit,
           orig_plan.max_benefit - shocked_plan.max_benefit,
           ((double)(orig_plan.max_benefit - shocked_plan.max_benefit) / orig_plan.max_benefit) * 100.0);
    printf("   New Installed Sites    : %d stations\n", shocked_plan.selected_count);

    /* Check Trip Infeasibility under Reduced Charging Network */
    printf("\n Impact on Vehicle Trips (Fleet Range Constraints):\n");
    /* Vehicle with low battery needing recharge at Hub 0 */
    Vehicle ev = city.vehicles[1]; /* City-Mini, 150 km range, 20% charge -> 30 km */
    ev.current_charge_pct = 20.0;
    double usable_range = ev.range_km * (ev.current_charge_pct / 100.0); /* 30 km */

    DijkstraResult r = dijkstra_array(&city, 4, 7); /* Suburbs to Mall */
    printf("   Trip: %s -> %s (Distance = %d km)\n", city.nodes[4].name, city.nodes[7].name, r.dist[7]);
    printf("   Vehicle %s Usable Battery: %.1f km\n", ev.model_name, usable_range);
    if (r.dist[7] <= usable_range) {
        printf("   Trip Status: FEASIBLE\n");
    } else {
        printf("   Trip Status: [INFEASIBLE] Deficit of %.1f km! Vehicle stranded due to station outage!\n",
               r.dist[7] - usable_range);
    }

    /* Bipartite Matching Shock: Available bays reduced from 4 to 2 */
    printf("\n Re-running Bipartite Charger Matching under bay reduction:\n");
    int compat[MATCH_MAX_ITEMS][MATCH_MAX_ITEMS] = {
        {1, 1}, {1, 0}, {0, 1}, {1, 1} /* 4 vehicles, only 2 operational bays */
    };
    BipartiteMatchResult bpm = max_bipartite_matching(4, 2, compat);
    printf("   Vehicles Queued: 4 | Operational Bays: 2 | Total Matched: %d\n", bpm.match_count);
    for (int v = 0; v < 4; v++) {
        if (bpm.vehicle_assigned_bay[v] != -1) {
            printf("   -> Vehicle V_%d ASSIGNED to Bay B_%d\n", v + 1, bpm.vehicle_assigned_bay[v] + 1);
        } else {
            printf("   -> Vehicle V_%d [DENIED/QUEUED] No bay available!\n", v + 1);
        }
    }
    printf("=========================================================================\n");
}

/* -------------------------------------------------------------
 * SCENARIO 3: LARGE-DATA STRESS TEST
 * 100 nodes, 40 stations, large request logs.
 * Compare: Merge vs Quick Sort, Dijkstra Array vs Heap, Kruskal vs Prim, KMP vs Naive.
 * ------------------------------------------------------------- */
void run_scenario_3_stress(void) {
    printf("\n=========================================================================\n");
    printf("               SCENARIO 3: LARGE-DATA STRESS TEST (SCALABILITY)          \n");
    printf("=========================================================================\n");

    CityNetwork city;
    if (!city_load_from_file(&city, "data/large.txt")) {
        printf("[Notice] Loading 100-node stress test network...\n");
        city_init(&city);
        city.num_nodes = 100;
        city.budget = 800;
        for (int i = 0; i < 99; i++) {
            city.adj_matrix[i][i+1] = (i % 7) + 5;
            city.adj_matrix[i+1][i] = (i % 7) + 5;
        }
    }

    printf(" Test 1: Sorting 1000 Elements (Random vs Adversarial Sorted)\n");
    int test_data[1000];
    int copy_m[1000], copy_q[1000];
    for (int i = 0; i < 1000; i++) test_data[i] = (i * 37) % 1000;

    for (int i = 0; i < 1000; i++) { copy_m[i] = test_data[i]; copy_q[i] = test_data[i]; }
    ops_reset();
    clock_t t0 = timer_start();
    merge_sort_int(copy_m, 0, 999);
    double m_time = timer_stop_ms(t0);
    long long m_ops = ops_get();

    ops_reset();
    t0 = timer_start();
    quick_sort_int(copy_q, 0, 999);
    double q_time = timer_stop_ms(t0);
    long long q_ops = ops_get();

    printf("   Merge Sort (1000 random items): Time = %.4f ms | Ops = %lld\n", m_time, m_ops);
    printf("   Quick Sort (1000 random items): Time = %.4f ms | Ops = %lld\n", q_time, q_ops);

    printf("\n Test 2: Routing on 100-Node Network (Dijkstra Array vs Min-Heap)\n");
    ops_reset();
    t0 = timer_start();
    DijkstraResult da = dijkstra_array(&city, 0, 99);
    double da_time = timer_stop_ms(t0);
    long long da_ops = ops_get();

    ops_reset();
    t0 = timer_start();
    DijkstraResult dh = dijkstra_heap(&city, 0, 99);
    double dh_time = timer_stop_ms(t0);
    long long dh_ops = ops_get();

    printf("   Dijkstra Array (V^2):       Time = %.4f ms | Ops = %lld | Distance = %d km\n",
           da_time, da_ops, da.dist[99]);
    printf("   Dijkstra Min-Heap (E logV): Time = %.4f ms | Ops = %lld | Distance = %d km\n",
           dh_time, dh_ops, dh.dist[99]);

    printf("\n Test 3: Infrastructure Backbone MST (Prim vs Kruskal)\n");
    ops_reset();
    t0 = timer_start();
    MSTResult prim = prim_mst(&city);
    double prim_time = timer_stop_ms(t0);
    long long prim_ops = ops_get();

    ops_reset();
    t0 = timer_start();
    MSTResult krusk = kruskal_mst(&city);
    double krusk_time = timer_stop_ms(t0);
    long long krusk_ops = ops_get();

    printf("   Prim MST (Dense scan)   : Weight = %d km | Time = %.4f ms | Ops = %lld\n",
           prim.total_weight, prim_time, prim_ops);
    printf("   Kruskal MST (Union-Find): Weight = %d km | Time = %.4f ms | Ops = %lld\n",
           krusk.total_weight, krusk_time, krusk_ops);

    printf("\n Test 4: Pattern Search in Operational Log (Naive vs KMP)\n");
    const char *sample_log = "2026-11-20T08:00:01 [INFO] STATION_101 VEHICLE_001 CHARGE_START Bay=1 Power=50kW "
                             "2026-11-20T08:02:15 [INFO] STATION_102 VEHICLE_002 CHARGE_START Bay=2 Power=35kW "
                             "2026-11-20T08:14:22 [WARN] STATION_101 VEHICLE_003 QUEUE_DELAY BayWait=12min "
                             "2026-11-20T08:25:30 [INFO] STATION_103 VEHICLE_004 CHARGE_START Bay=1 Power=75kW "
                             "2026-11-20T08:50:00 [ALARM] STATION_105 GRID_OVERLOAD VoltageDrop=4.2% SheddingLoad=15kW "
                             "2026-11-20T10:30:45 [INFO] STATION_105 RESTORE_NORMAL GridStabilized PowerMax=120kW";

    ops_reset();
    StringMatchResult n_res = string_match_naive(sample_log, "CHARGE_START");
    ops_reset();
    StringMatchResult k_res = string_match_kmp(sample_log, "CHARGE_START");

    printf("   Naive String Search: Matches = %d | Comparisons = %lld | Time = %.4f ms\n",
           n_res.match_count, n_res.char_comparisons, n_res.execution_time_ms);
    printf("   KMP String Search  : Matches = %d | Comparisons = %lld | Time = %.4f ms\n",
           k_res.match_count, k_res.char_comparisons, k_res.execution_time_ms);

    printf("=========================================================================\n");
}
