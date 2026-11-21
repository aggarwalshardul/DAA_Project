/**
 * @file analysis.c
 * @brief Implementation of operation counting, timers, and CSV performance logging.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 */

#include "analysis.h"

/* Definition of the global operation counter */
long long g_ops = 0;

/**
 * Resets the global operation counter to zero.
 * Complexity: Time O(1), Space O(1).
 */
void ops_reset(void) {
    g_ops = 0;
}

/**
 * Increments the global operation counter by delta.
 * Complexity: Time O(1), Space O(1).
 */
void ops_increment(long long delta) {
    g_ops += delta;
}

/**
 * Returns current count of operations recorded.
 * Complexity: Time O(1), Space O(1).
 */
long long ops_get(void) {
    return g_ops;
}

/**
 * Captures current CPU clock tick.
 * Complexity: Time O(1), Space O(1).
 */
clock_t timer_start(void) {
    return clock();
}

/**
 * Calculates elapsed milliseconds since start_time.
 * Complexity: Time O(1), Space O(1).
 */
double timer_stop_ms(clock_t start_time) {
    clock_t end_time = clock();
    double elapsed_sec = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    return elapsed_sec * 1000.0;
}

/**
 * Estimates theoretical auxiliary memory consumed by 0/1 knapsack DP table.
 * Table dimensions: (num_items + 1) * (capacity + 1) integers.
 * Complexity: Time O(1), Space O(1).
 */
size_t memory_estimate_knapsack(int num_items, int capacity) {
    return (size_t)(num_items + 1) * (size_t)(capacity + 1) * sizeof(int);
}

/**
 * Estimates memory consumed by an N x N graph adjacency matrix.
 * Complexity: Time O(1), Space O(1).
 */
size_t memory_estimate_graph(int num_nodes) {
    return (size_t)num_nodes * (size_t)num_nodes * sizeof(int);
}

/**
 * Appends a benchmark execution row to a CSV file.
 * Creates file with header if it does not yet exist.
 * Complexity: Time O(1), Space O(1).
 */
void benchmark_log_csv(const char *filename,
                       const char *algorithm,
                       int input_size,
                       const char *case_type,
                       double time_ms,
                       long long operations,
                       size_t memory_bytes,
                       double solution_quality) {
    if (!filename) return;

    FILE *check_fp = fopen(filename, "r");
    int write_header = 0;
    if (!check_fp) {
        write_header = 1;
    } else {
        fclose(check_fp);
    }

    FILE *fp = fopen(filename, "a");
    if (!fp) {
        printf("[Warning] Could not open %s for writing benchmark log.\n", filename);
        return;
    }

    if (write_header) {
        fprintf(fp, "algorithm,size,case,time_ms,operations,memory_bytes,result_quality\n");
    }

    fprintf(fp, "%s,%d,%s,%.4f,%lld,%lu,%.2f\n",
            algorithm, input_size, case_type, time_ms,
            operations, (unsigned long)memory_bytes, solution_quality);

    fclose(fp);
}
