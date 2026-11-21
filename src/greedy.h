/**
 * @file greedy.h
 * @brief Complete Greedy algorithm suite for EV Scheduling, Infrastructure & Compression.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#ifndef GREEDY_H
#define GREEDY_H

#include "model.h"
#include "analysis.h"
#include "dp.h"

/* -------------------------------------------------------------
 * 1. CHARGING SCHEDULE STRUCTS (ACTIVITY SELECTION & INTERVALS)
 * ------------------------------------------------------------- */
typedef struct {
    int id;
    int vehicle_id;
    int start;
    int finish;
} ChargingSession;

typedef struct {
    int count;
    int session_ids[MAX_TRIPS];
} ActivitySelectionResult;

typedef struct {
    int total_chargers_needed;
    int session_assigned_charger[MAX_TRIPS];
} IntervalPartitionResult;

ActivitySelectionResult activity_selection(ChargingSession sessions[], int n);
IntervalPartitionResult interval_partitioning(ChargingSession sessions[], int n);

/* -------------------------------------------------------------
 * 2. POWER ALLOCATION (FRACTIONAL KNAPSACK)
 * ------------------------------------------------------------- */
typedef struct {
    int vehicle_id;
    double kw_demanded;
    double priority_weight;
    double kw_allocated;
} PowerRequest;

typedef struct {
    double total_priority_served;
    double total_kw_allocated;
} FractionalKnapsackResult;

FractionalKnapsackResult fractional_knapsack_power(PowerRequest reqs[], int n, double total_grid_kw);

/* -------------------------------------------------------------
 * 3. INFRASTRUCTURE BACKBONE: PRIM & KRUSKAL MST
 * ------------------------------------------------------------- */
typedef struct {
    int u;
    int v;
    int weight;
} GraphEdge;

typedef struct {
    int total_weight;
    int edge_count;
    GraphEdge edges[MAX_NODES];
} MSTResult;

MSTResult prim_mst(const CityNetwork *city);
MSTResult kruskal_mst(const CityNetwork *city);

/* -------------------------------------------------------------
 * 4. LOG COMPRESSION: HUFFMAN CODING
 * ------------------------------------------------------------- */
#define HUFFMAN_MAX_CHARS 256

typedef struct {
    char ch;
    int freq;
    char code[32];
} HuffmanCodeEntry;

typedef struct {
    int distinct_chars;
    HuffmanCodeEntry table[HUFFMAN_MAX_CHARS];
    int original_bits;
    int compressed_bits;
    double compression_ratio;
} HuffmanResult;

HuffmanResult huffman_compress(const char *text, char *encoded_output, size_t out_size);
void huffman_decompress(const char *encoded, const HuffmanResult *huff, char *decoded_output);

/* -------------------------------------------------------------
 * 5. ROUTING & KNAPSACK (PHASE I FOUNDATIONS)
 * ------------------------------------------------------------- */
typedef struct {
    int source;
    int dest;
    int dist[MAX_NODES];
    int parent[MAX_NODES];
    int path[MAX_NODES];
    int path_length;
    int is_reachable;
} DijkstraResult;

KnapsackResult knapsack_greedy_ratio(const Station stations[], int count, int budget);
DijkstraResult dijkstra_array(const CityNetwork *city, int src, int dest);
DijkstraResult dijkstra_heap(const CityNetwork *city, int src, int dest);
void dijkstra_print_path(const CityNetwork *city, const DijkstraResult *res);
int is_path_ev_feasible(const CityNetwork *city,
                        const DijkstraResult *route,
                        double vehicle_range,
                        double *out_battery_used);

#endif /* GREEDY_H */
