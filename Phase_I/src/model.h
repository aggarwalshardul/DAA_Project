/**
 * @file model.h
 * @brief Domain entities and data structures for the EV Planning System.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation using fixed-size arrays and beginner-friendly structs.
 */

#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_STATIONS 50
#define MAX_VEHICLES 50
#define MAX_TRIPS 100
#define MAX_NAME_LEN 32
#define INF_DIST 999999

/**
 * @brief Represents a road network junction / city point.
 */
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
} Node;

/**
 * @brief Represents a candidate charging station installation site.
 */
typedef struct {
    int id;               /* Unique station ID */
    int node_id;          /* Junction where station is located */
    int install_cost;     /* Cost in thousands of currency units */
    int power_kw;         /* Charging capacity in kW */
    int ports;            /* Number of charging ports */
    int benefit_score;    /* Expected daily EV trips served */
    int is_active;        /* 1 = active / selected, 0 = inactive */
} Station;

/**
 * @brief Represents an electric vehicle.
 */
typedef struct {
    int id;
    char model_name[MAX_NAME_LEN];
    double battery_capacity_kwh;
    double range_km;               /* Maximum driving range on full charge */
    double current_charge_pct;     /* Current battery % (0 to 100) */
    double consumption_kwh_per_km; /* Energy consumption per km */
} Vehicle;

/**
 * @brief Represents a scheduled EV trip request.
 */
typedef struct {
    int id;
    int source_node;
    int dest_node;
    int start_time;       /* Start hour/minute (e.g. 0800) */
    int end_time;         /* Required arrival time */
    int vehicle_id;
} Trip;

/**
 * @brief Complete city network containing road topology, candidate stations,
 * vehicles, and trips.
 */
typedef struct {
    int num_nodes;
    Node nodes[MAX_NODES];
    int adj_matrix[MAX_NODES][MAX_NODES]; /* Distance in km, INF_DIST if no road */
    
    int num_stations;
    Station stations[MAX_STATIONS];
    
    int num_vehicles;
    Vehicle vehicles[MAX_VEHICLES];
    
    int num_trips;
    Trip trips[MAX_TRIPS];
    
    int budget;           /* Total available budget for station placement */
} CityNetwork;

/* Function prototypes */
void city_init(CityNetwork *city);
void city_add_default_sample(CityNetwork *city);
int city_load_from_file(CityNetwork *city, const char *filepath);
int city_save_to_file(const CityNetwork *city, const char *filepath);
int city_validate(const CityNetwork *city);
void city_print_summary(const CityNetwork *city);
void city_print_stations(const CityNetwork *city);
void city_print_roads(const CityNetwork *city);

#endif /* MODEL_H */
