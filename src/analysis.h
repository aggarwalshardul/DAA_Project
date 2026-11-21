/**
 * @file analysis.h
 * @brief Instrumentation, operation counters, timing, and memory estimation.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 */

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Global operation counter for empirical analysis */
extern long long g_ops;

/* Operation counter functions */
void ops_reset(void);
void ops_increment(long long delta);
long long ops_get(void);

/* Timing helpers */
clock_t timer_start(void);
double timer_stop_ms(clock_t start_time);

/* Memory estimation helpers */
size_t memory_estimate_knapsack(int num_items, int capacity);
size_t memory_estimate_graph(int num_nodes);

/* CSV export helper */
void benchmark_log_csv(const char *filename,
                       const char *algorithm,
                       int input_size,
                       const char *case_type,
                       double time_ms,
                       long long operations,
                       size_t memory_bytes,
                       double solution_quality);

#endif /* ANALYSIS_H */
