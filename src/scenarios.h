/**
 * @file scenarios.h
 * @brief Three mandatory evaluation scenarios comparing multi-algorithmic paradigms.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#ifndef SCENARIOS_H
#define SCENARIOS_H

#include "model.h"
#include "analysis.h"
#include "sorting_searching.h"
#include "divide_conquer.h"
#include "greedy.h"
#include "dp.h"
#include "backtracking.h"
#include "flow_matching.h"
#include "string_match.h"

/* Scenario 1: Normal Operation (20 nodes, 12 candidate sites, budget fits ~half) */
void run_scenario_1_normal(void);

/* Scenario 2: Constraint Shock (Key station goes offline, budget slashed by 30%) */
void run_scenario_2_shock(void);

/* Scenario 3: Large-Data Stress (100 nodes, 1000+ operations/requests, scalability comparison) */
void run_scenario_3_stress(void);

#endif /* SCENARIOS_H */
