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
#include "greedy.h"
#include "dp.h"

/**
 * Safely reads an integer from stdin within [min_val, max_val].
 * Flushes buffer on invalid/non-numeric input so program never crashes.
 */
static int read_int_safe(const char *prompt, int min_val, int max_val) {
    int val;
    char buffer[128];

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            continue;
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

/**
 * Pauses until user presses Enter.
 */
static void wait_for_enter(void) {
    printf("\nPress [Enter] to continue...");
    char buf[16];
    fgets(buf, sizeof(buf), stdin);
}

/**
 * Menu Option 1: Load or inspect dataset.
 */
static void menu_load_dataset(CityNetwork *city) {
    printf("\n--- DATASET MANAGEMENT ---\n");
    printf("1. Load Default Built-in Sample (8 nodes)\n");
    printf("2. Load small.txt (~8 nodes)\n");
    printf("3. Load medium.txt (~30 nodes)\n");
    printf("4. Load large.txt (~100 nodes)\n");
    printf("5. Load adversarial.txt (Knapsack counterexample)\n");
    printf("6. View Current Network Summary\n");
    printf("7. View Candidate Stations List\n");
    printf("8. View Road Network Connections\n");
    printf("0. Return to Main Menu\n");

    int choice = read_int_safe("Select choice [0-8]: ", 0, 8);
    switch (choice) {
        case 1:
            city_add_default_sample(city);
            printf("[Success] Loaded 8-node default sample city.\n");
            break;
        case 2:
            if (city_load_from_file(city, "data/small.txt"))
                printf("[Success] Loaded data/small.txt\n");
            break;
        case 3:
            if (city_load_from_file(city, "data/medium.txt"))
                printf("[Success] Loaded data/medium.txt\n");
            break;
        case 4:
            if (city_load_from_file(city, "data/large.txt"))
                printf("[Success] Loaded data/large.txt\n");
            break;
        case 5:
            if (city_load_from_file(city, "data/adversarial.txt"))
                printf("[Success] Loaded data/adversarial.txt\n");
            break;
        case 6:
            city_print_summary(city);
            break;
        case 7:
            city_print_stations(city);
            break;
        case 8:
            city_print_roads(city);
            break;
        default:
            break;
    }
    wait_for_enter();
}

/**
 * Menu Option 2: Station Placement (0/1 Knapsack DP vs Greedy).
 */
static void menu_station_placement(CityNetwork *city) {
    printf("\n--- STATION PLACEMENT OPTIMIZATION (KNAPSACK) ---\n");
    printf("Available Budget: %d units | Candidate Sites: %d\n",
           city->budget, city->num_stations);
    if (city->num_stations == 0) {
        printf("Please load a dataset with candidate stations first.\n");
        wait_for_enter();
        return;
    }

    /* 1. Run Dynamic Programming Knapsack */
    ops_reset();
    clock_t t0 = timer_start();
    KnapsackResult dp_res = knapsack_01_dp(city->stations, city->num_stations, city->budget);
    double dp_time = timer_stop_ms(t0);
    long long dp_ops = ops_get();

    /* 2. Run Greedy Ratio Knapsack */
    ops_reset();
    t0 = timer_start();
    KnapsackResult gr_res = knapsack_greedy_ratio(city->stations, city->num_stations, city->budget);
    double gr_time = timer_stop_ms(t0);
    long long gr_ops = ops_get();

    printf("\n=========================================================================\n");
    printf("                  PLACEMENT STRATEGY COMPARISON                          \n");
    printf("=========================================================================\n");
    printf(" Metric                | DP (0/1 Optimal)       | Greedy (Benefit/Cost) \n");
    printf("-----------------------+------------------------+------------------------\n");
    printf(" Total Benefit Achieved| %-22d | %-22d \n", dp_res.max_benefit, gr_res.max_benefit);
    printf(" Budget Consumed       | %-22d | %-22d \n", dp_res.total_cost, gr_res.total_cost);
    printf(" Stations Installed    | %-22d | %-22d \n", dp_res.selected_count, gr_res.selected_count);
    printf(" Execution Time (ms)   | %-22.4f | %-22.4f \n", dp_time, gr_time);
    printf(" Operation Count (ops) | %-22lld | %-22lld \n", dp_ops, gr_ops);
    printf(" Memory Estimate       | %-16lu bytes | %-16lu bytes \n",
           (unsigned long)memory_estimate_knapsack(city->num_stations, city->budget),
           (unsigned long)(city->num_stations * sizeof(Station)));
    printf(" Optimality Status     | Guaranteed OPTIMAL     | Heuristic (May fail)   \n");
    printf("=========================================================================\n");

    printf("\nSelected Stations by DP (Optimal):\n");
    for (int i = 0; i < dp_res.selected_count; i++) {
        int idx = dp_res.selected_indices[i];
        printf(" -> Station [%d] at Node [%d] (%s) - Cost: %d, Benefit: %d\n",
               city->stations[idx].id, city->stations[idx].node_id,
               city->nodes[city->stations[idx].node_id].name,
               city->stations[idx].install_cost, city->stations[idx].benefit_score);
    }

    if (city->num_stations <= 8 && city->budget <= 25) {
        knapsack_print_table(city->stations, city->num_stations, city->budget);
    }

    wait_for_enter();
}

/**
 * Menu Option 3: Trip Planning (Dijkstra Shortest Path + EV Feasibility).
 */
static void menu_trip_planning(CityNetwork *city) {
    printf("\n--- TRIP PLANNING & ROUTING (DIJKSTRA) ---\n");
    if (city->num_nodes < 2) {
        printf("Please load a dataset first.\n");
        wait_for_enter();
        return;
    }

    printf("Available Junctions (0 to %d):\n", city->num_nodes - 1);
    for (int i = 0; i < city->num_nodes; i++) {
        printf(" [%d] %-15s ", i, city->nodes[i].name);
        if ((i + 1) % 4 == 0) printf("\n");
    }
    printf("\n");

    int src = read_int_safe("Enter Origin Junction ID: ", 0, city->num_nodes - 1);
    int dst = read_int_safe("Enter Destination Junction ID: ", 0, city->num_nodes - 1);

    /* Run Dijkstra Array */
    ops_reset();
    clock_t t0 = timer_start();
    DijkstraResult arr_res = dijkstra_array(city, src, dst);
    double arr_time = timer_stop_ms(t0);
    long long arr_ops = ops_get();

    /* Run Dijkstra Heap */
    ops_reset();
    t0 = timer_start();
    DijkstraResult hp_res = dijkstra_heap(city, src, dst);
    double hp_time = timer_stop_ms(t0);
    long long hp_ops = ops_get();

    printf("\n--- ROUTE COMPUTATION RESULTS ---\n");
    dijkstra_print_path(city, &arr_res);
    if (hp_res.is_reachable && hp_res.dist[dst] == arr_res.dist[dst]) {
        printf(" [Verification] Heap and Array implementations match: %d km.\n", hp_res.dist[dst]);
    }

    printf("\nAlgorithm Performance Comparison:\n");
    printf(" 1. Dijkstra Array O(V^2):       Time = %.4f ms | Operations = %lld\n",
           arr_time, arr_ops);
    printf(" 2. Dijkstra Min-Heap O(E log V): Time = %.4f ms | Operations = %lld\n",
           hp_time, hp_ops);

    if (arr_res.is_reachable && city->num_vehicles > 0) {
        printf("\nSelect Vehicle for Range Feasibility Check:\n");
        for (int i = 0; i < city->num_vehicles; i++) {
            printf(" %d. %s (Range: %.0f km, Current Battery: %.0f%%)\n",
                   i + 1, city->vehicles[i].model_name,
                   city->vehicles[i].range_km, city->vehicles[i].current_charge_pct);
        }
        int v_choice = read_int_safe("Select vehicle [1-N]: ", 1, city->num_vehicles) - 1;
        Vehicle *vh = &city->vehicles[v_choice];

        double effective_range = vh->range_km * (vh->current_charge_pct / 100.0);
        double dist_km = (double)arr_res.dist[dst];

        printf("\n--- EV BATTERY FEASIBILITY REPORT ---\n");
        printf(" Vehicle: %s | Effective Usable Range: %.1f km\n",
               vh->model_name, effective_range);
        printf(" Total Trip Distance: %.1f km\n", dist_km);
        if (dist_km <= effective_range) {
            double remaining_pct = ((effective_range - dist_km) / vh->range_km) * 100.0;
            printf(" STATUS: [FEASIBLE] Vehicle can complete the trip directly!\n");
            printf(" Estimated Remaining Battery at Destination: %.1f%%\n", remaining_pct);
        } else {
            printf(" STATUS: [RANGE EXCEEDED] Vehicle cannot reach directly without recharge!\n");
            printf(" Deficit: %.1f km beyond available battery charge.\n", dist_km - effective_range);
        }
    }

    wait_for_enter();
}

/**
 * Menu Option 4: Sorting and Searching Tools.
 */
static void menu_sorting_searching(CityNetwork *city) {
    printf("\n--- SORTING & SEARCHING WORKBENCH ---\n");
    printf("1. Sort Stations by Install Cost (Merge Sort vs Quick Sort)\n");
    printf("2. Sort Stations by Benefit Score (Merge Sort vs Quick Sort)\n");
    printf("3. Sort Stations by Benefit/Cost Ratio\n");
    printf("4. Binary Search for Station by ID (Iterative vs Recursive)\n");
    printf("0. Return\n");

    int choice = read_int_safe("Select choice [0-4]: ", 0, 4);
    if (choice == 0) return;

    if (city->num_stations == 0) {
        printf("Please load a dataset with candidate stations first.\n");
        wait_for_enter();
        return;
    }

    if (choice >= 1 && choice <= 3) {
        StationSortCriterion crit = (choice == 1) ? SORT_BY_COST :
                                   (choice == 2) ? SORT_BY_BENEFIT : SORT_BY_RATIO;
        Station m_arr[MAX_STATIONS];
        Station q_arr[MAX_STATIONS];
        for (int i = 0; i < city->num_stations; i++) {
            m_arr[i] = city->stations[i];
            q_arr[i] = city->stations[i];
        }

        ops_reset();
        clock_t t0 = timer_start();
        merge_sort_stations(m_arr, 0, city->num_stations - 1, crit);
        double m_time = timer_stop_ms(t0);
        long long m_ops = ops_get();

        ops_reset();
        t0 = timer_start();
        quick_sort_stations(q_arr, 0, city->num_stations - 1, crit);
        double q_time = timer_stop_ms(t0);
        long long q_ops = ops_get();

        printf("\nSorted Stations Preview:\n");
        printf(" ID   | Cost | Benefit | Ratio \n");
        printf("------+------+---------+-------\n");
        for (int i = 0; i < city->num_stations; i++) {
            double r = (m_arr[i].install_cost > 0) ?
                ((double)m_arr[i].benefit_score / m_arr[i].install_cost) : 0;
            printf(" %-4d | %-4d | %-7d | %-5.2f\n",
                   m_arr[i].id, m_arr[i].install_cost, m_arr[i].benefit_score, r);
        }

        printf("\nPerformance Comparison:\n");
        printf(" Merge Sort Theta(n log n): Time = %.4f ms | Operations = %lld\n", m_time, m_ops);
        printf(" Quick Sort Avg(n log n)  : Time = %.4f ms | Operations = %lld\n", q_time, q_ops);
    } else if (choice == 4) {
        Station s_arr[MAX_STATIONS];
        for (int i = 0; i < city->num_stations; i++) s_arr[i] = city->stations[i];
        quick_sort_stations(s_arr, 0, city->num_stations - 1, SORT_BY_ID);

        int target = read_int_safe("Enter Station ID to search: ", 1, 99999);

        ops_reset();
        int idx_iter = binary_search_station_by_id(s_arr, city->num_stations, target);
        long long ops_iter = ops_get();

        ops_reset();
        int idx_rec = binary_search_recursive_int((const int*)s_arr, 0, city->num_stations - 1, target);
        (void)idx_rec; /* Handled via station search */

        if (idx_iter != -1) {
            printf("\n[Found] Station ID %d located at index %d:\n", target, idx_iter);
            printf(" Node: %d, Cost: %d, Power: %d kW, Ports: %d, Benefit: %d\n",
                   s_arr[idx_iter].node_id, s_arr[idx_iter].install_cost,
                   s_arr[idx_iter].power_kw, s_arr[idx_iter].ports,
                   s_arr[idx_iter].benefit_score);
        } else {
            printf("\n[Not Found] Station ID %d does not exist in dataset.\n", target);
        }
        printf(" Binary Search Comparisons/Operations: %lld ops\n", ops_iter);
    }

    wait_for_enter();
}

/**
 * Main application entry point and event loop.
 */
int main(void) {
    CityNetwork city;
    city_add_default_sample(&city);

    while (1) {
        printf("\n=========================================================\n");
        printf("     EV CHARGING STATION & TRIP PLANNING SYSTEM (C17)    \n");
        printf("           CSEG3053 Design and Analysis of Algorithms    \n");
        printf("=========================================================\n");
        printf(" Current Dataset: %d Nodes | %d Stations | Budget: %d\n",
               city.num_nodes, city.num_stations, city.budget);
        printf("---------------------------------------------------------\n");
        printf(" 1. Load / Generate Dataset\n");
        printf(" 2. Station Placement (Knapsack: DP vs Greedy Ratio)\n");
        printf(" 3. Trip Planning & EV Feasibility (Dijkstra Array vs Heap)\n");
        printf(" 4. Sorting & Searching Workbench (Merge, Quick, Binary Search)\n");
        printf(" 5. Display City Summary & Connectivity\n");
        printf(" 0. Exit System\n");
        printf("=========================================================\n");

        int choice = read_int_safe("Enter selection [0-5]: ", 0, 5);
        if (choice == 0) {
            printf("\nThank you for using EV Planning System. Exiting cleanly.\n");
            break;
        }

        switch (choice) {
            case 1:
                menu_load_dataset(&city);
                break;
            case 2:
                menu_station_placement(&city);
                break;
            case 3:
                menu_trip_planning(&city);
                break;
            case 4:
                menu_sorting_searching(&city);
                break;
            case 5:
                city_print_summary(&city);
                city_print_stations(&city);
                city_print_roads(&city);
                wait_for_enter();
                break;
            default:
                break;
        }
    }

    return 0;
}
