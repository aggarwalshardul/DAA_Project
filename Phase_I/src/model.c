/**
 * @file model.c
 * @brief Implementation of domain model initialization, file I/O, and validation.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Clean, readable C code for B.Tech student viva defense.
 */

#include "model.h"

/**
 * Initializes a CityNetwork structure with empty/default values.
 * Complexity: Time O(V^2) where V = MAX_NODES, Space O(1) auxiliary.
 */
void city_init(CityNetwork *city) {
    if (!city) return;
    city->num_nodes = 0;
    city->num_stations = 0;
    city->num_vehicles = 0;
    city->num_trips = 0;
    city->budget = 0;

    for (int i = 0; i < MAX_NODES; i++) {
        city->nodes[i].id = i;
        city->nodes[i].name[0] = '\0';
        for (int j = 0; j < MAX_NODES; j++) {
            if (i == j) {
                city->adj_matrix[i][j] = 0;
            } else {
                city->adj_matrix[i][j] = INF_DIST;
            }
        }
    }
}

/**
 * Populates a default 8-node sample city for instant testing without files.
 * Complexity: Time O(1), Space O(1).
 */
void city_add_default_sample(CityNetwork *city) {
    city_init(city);
    city->num_nodes = 8;
    city->budget = 100;

    const char *names[8] = {
        "Downtown", "Uptown", "TechPark", "Airport",
        "Suburbs", "Harbor", "MetroStation", "Mall"
    };
    for (int i = 0; i < 8; i++) {
        city->nodes[i].id = i;
        strncpy(city->nodes[i].name, names[i], MAX_NAME_LEN - 1);
        city->nodes[i].name[MAX_NAME_LEN - 1] = '\0';
    }

    /* Roads (undirected edges with distance in km) */
    int roads[10][3] = {
        {0, 1, 10}, {0, 2, 15}, {1, 3, 20}, {1, 4, 25},
        {2, 3, 30}, {2, 5, 35}, {3, 6, 12}, {4, 6, 18},
        {5, 7, 22}, {6, 7, 14}
    };
    for (int i = 0; i < 10; i++) {
        int u = roads[i][0];
        int v = roads[i][1];
        int d = roads[i][2];
        city->adj_matrix[u][v] = d;
        city->adj_matrix[v][u] = d;
    }

    /* Candidate station sites: id, node, cost, power, ports, benefit, active */
    city->num_stations = 5;
    Station s_init[5] = {
        {101, 0, 30, 120, 4, 85, 0},
        {102, 2, 25, 90,  3, 60, 0},
        {103, 3, 40, 150, 6, 95, 0},
        {104, 5, 20, 60,  2, 45, 0},
        {105, 6, 35, 120, 4, 75, 0}
    };
    for (int i = 0; i < 5; i++) {
        city->stations[i] = s_init[i];
    }

    /* Vehicles */
    city->num_vehicles = 3;
    Vehicle v_init[3] = {
        {1, "Sedan-EV", 50.0, 250.0, 80.0, 0.20},
        {2, "City-Mini", 30.0, 150.0, 90.0, 0.18},
        {3, "Cargo-Van", 75.0, 320.0, 60.0, 0.25}
    };
    for (int i = 0; i < 3; i++) {
        city->vehicles[i] = v_init[i];
    }

    /* Trips */
    city->num_trips = 4;
    Trip t_init[4] = {
        {1, 0, 3, 800,  930,  1},
        {2, 4, 7, 900,  1030, 2},
        {3, 2, 6, 1100, 1245, 1},
        {4, 5, 1, 1400, 1600, 3}
    };
    for (int i = 0; i < 4; i++) {
        city->trips[i] = t_init[i];
    }
}

/**
 * Validates that the city network satisfies domain invariants.
 * Returns 1 if valid, 0 if invalid with error print.
 * Complexity: Time O(N + S + T), Space O(1).
 */
int city_validate(const CityNetwork *city) {
    if (!city) return 0;
    if (city->num_nodes < 2 || city->num_nodes > MAX_NODES) {
        printf("[Validation Error] Node count %d invalid (must be 2..%d)\n",
               city->num_nodes, MAX_NODES);
        return 0;
    }
    if (city->budget < 0) {
        printf("[Validation Error] Budget cannot be negative (%d)\n", city->budget);
        return 0;
    }
    for (int i = 0; i < city->num_stations; i++) {
        int nid = city->stations[i].node_id;
        if (nid < 0 || nid >= city->num_nodes) {
            printf("[Validation Error] Station %d references invalid node %d\n",
                   city->stations[i].id, nid);
            return 0;
        }
    }
    for (int i = 0; i < city->num_trips; i++) {
        int u = city->trips[i].source_node;
        int v = city->trips[i].dest_node;
        if (u < 0 || u >= city->num_nodes || v < 0 || v >= city->num_nodes) {
            printf("[Validation Error] Trip %d has invalid endpoints (%d -> %d)\n",
                   city->trips[i].id, u, v);
            return 0;
        }
    }
    return 1;
}

/**
 * Parses and loads a CityNetwork from a structured text file.
 * Returns 1 on success, 0 on failure.
 * Complexity: Time O(V^2 + S + T), Space O(1).
 */
int city_load_from_file(CityNetwork *city, const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        printf("[Error] Cannot open file: %s\n", filepath);
        return 0;
    }

    city_init(city);
    char line[256];
    char section[64] = "";

    while (fgets(line, sizeof(line), fp)) {
        /* Skip comments and empty lines */
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        if (line[0] == '[') {
            sscanf(line, "[%63[^]]", section);
            continue;
        }

        if (strcmp(section, "META") == 0) {
            sscanf(line, "nodes=%d budget=%d", &city->num_nodes, &city->budget);
        } else if (strcmp(section, "NODES") == 0) {
            int nid;
            char nname[MAX_NAME_LEN];
            if (sscanf(line, "%d %31s", &nid, nname) == 2 && nid >= 0 && nid < MAX_NODES) {
                city->nodes[nid].id = nid;
                strncpy(city->nodes[nid].name, nname, MAX_NAME_LEN - 1);
                city->nodes[nid].name[MAX_NAME_LEN - 1] = '\0';
            }
        } else if (strcmp(section, "ROADS") == 0) {
            int u, v, d;
            if (sscanf(line, "%d %d %d", &u, &v, &d) == 3) {
                if (u >= 0 && u < MAX_NODES && v >= 0 && v < MAX_NODES && d >= 0) {
                    city->adj_matrix[u][v] = d;
                    city->adj_matrix[v][u] = d;
                }
            }
        } else if (strcmp(section, "STATIONS") == 0) {
            if (city->num_stations < MAX_STATIONS) {
                Station *s = &city->stations[city->num_stations];
                if (sscanf(line, "%d %d %d %d %d %d",
                           &s->id, &s->node_id, &s->install_cost,
                           &s->power_kw, &s->ports, &s->benefit_score) == 6) {
                    s->is_active = 0;
                    city->num_stations++;
                }
            }
        } else if (strcmp(section, "VEHICLES") == 0) {
            if (city->num_vehicles < MAX_VEHICLES) {
                Vehicle *vh = &city->vehicles[city->num_vehicles];
                if (sscanf(line, "%d %31s %lf %lf %lf %lf",
                           &vh->id, vh->model_name, &vh->battery_capacity_kwh,
                           &vh->range_km, &vh->current_charge_pct,
                           &vh->consumption_kwh_per_km) == 6) {
                    city->num_vehicles++;
                }
            }
        } else if (strcmp(section, "TRIPS") == 0) {
            if (city->num_trips < MAX_TRIPS) {
                Trip *tr = &city->trips[city->num_trips];
                if (sscanf(line, "%d %d %d %d %d %d",
                           &tr->id, &tr->source_node, &tr->dest_node,
                           &tr->start_time, &tr->end_time, &tr->vehicle_id) == 6) {
                    city->num_trips++;
                }
            }
        }
    }

    fclose(fp);
    return city_validate(city);
}

/**
 * Saves current CityNetwork to a file.
 * Returns 1 on success, 0 on failure.
 * Complexity: Time O(V^2 + S + T), Space O(1).
 */
int city_save_to_file(const CityNetwork *city, const char *filepath) {
    if (!city) return 0;
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        printf("[Error] Cannot write to file: %s\n", filepath);
        return 0;
    }

    fprintf(fp, "# EV Planning System Dataset\n");
    fprintf(fp, "[META]\nnodes=%d budget=%d\n\n", city->num_nodes, city->budget);

    fprintf(fp, "[NODES]\n");
    for (int i = 0; i < city->num_nodes; i++) {
        fprintf(fp, "%d %s\n", city->nodes[i].id,
                city->nodes[i].name[0] ? city->nodes[i].name : "Junction");
    }

    fprintf(fp, "\n[ROADS]\n");
    for (int i = 0; i < city->num_nodes; i++) {
        for (int j = i + 1; j < city->num_nodes; j++) {
            if (city->adj_matrix[i][j] > 0 && city->adj_matrix[i][j] < INF_DIST) {
                fprintf(fp, "%d %d %d\n", i, j, city->adj_matrix[i][j]);
            }
        }
    }

    fprintf(fp, "\n[STATIONS]\n");
    for (int i = 0; i < city->num_stations; i++) {
        const Station *s = &city->stations[i];
        fprintf(fp, "%d %d %d %d %d %d\n",
                s->id, s->node_id, s->install_cost, s->power_kw, s->ports, s->benefit_score);
    }

    fprintf(fp, "\n[VEHICLES]\n");
    for (int i = 0; i < city->num_vehicles; i++) {
        const Vehicle *vh = &city->vehicles[i];
        fprintf(fp, "%d %s %.1f %.1f %.1f %.2f\n",
                vh->id, vh->model_name, vh->battery_capacity_kwh,
                vh->range_km, vh->current_charge_pct, vh->consumption_kwh_per_km);
    }

    fprintf(fp, "\n[TRIPS]\n");
    for (int i = 0; i < city->num_trips; i++) {
        const Trip *tr = &city->trips[i];
        fprintf(fp, "%d %d %d %d %d %d\n",
                tr->id, tr->source_node, tr->dest_node,
                tr->start_time, tr->end_time, tr->vehicle_id);
    }

    fclose(fp);
    return 1;
}

/**
 * Prints a high-level summary of the city network.
 * Complexity: Time O(V^2), Space O(1).
 */
void city_print_summary(const CityNetwork *city) {
    if (!city) return;
    int road_count = 0;
    for (int i = 0; i < city->num_nodes; i++) {
        for (int j = i + 1; j < city->num_nodes; j++) {
            if (city->adj_matrix[i][j] > 0 && city->adj_matrix[i][j] < INF_DIST) {
                road_count++;
            }
        }
    }

    printf("\n=========================================\n");
    printf("        CITY NETWORK SUMMARY             \n");
    printf("=========================================\n");
    printf(" Junction Nodes     : %d\n", city->num_nodes);
    printf(" Road Segments      : %d\n", road_count);
    printf(" Candidate Stations : %d\n", city->num_stations);
    printf(" Fleet Vehicles     : %d\n", city->num_vehicles);
    printf(" Planned Trips      : %d\n", city->num_trips);
    printf(" Installation Budget: %d units\n", city->budget);
    printf("=========================================\n");
}

/**
 * Prints all candidate station sites in a clean table.
 * Complexity: Time O(S), Space O(1).
 */
void city_print_stations(const CityNetwork *city) {
    if (!city || city->num_stations == 0) {
        printf("No candidate stations available.\n");
        return;
    }
    printf("\n--------------------------------------------------------------------------\n");
    printf(" ID   | Node | Junction Name      | Cost | Power(kW) | Ports | Benefit | Ratio \n");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < city->num_stations; i++) {
        const Station *s = &city->stations[i];
        const char *name = (s->node_id >= 0 && s->node_id < city->num_nodes)
                           ? city->nodes[s->node_id].name : "Unknown";
        double ratio = (s->install_cost > 0) ? ((double)s->benefit_score / s->install_cost) : 0.0;
        printf(" %-4d | %-4d | %-18s | %-4d | %-9d | %-5d | %-7d | %-5.2f \n",
               s->id, s->node_id, name, s->install_cost, s->power_kw, s->ports, s->benefit_score, ratio);
    }
    printf("--------------------------------------------------------------------------\n");
}

/**
 * Prints road network adjacency connections.
 * Complexity: Time O(V^2), Space O(1).
 */
void city_print_roads(const CityNetwork *city) {
    if (!city) return;
    printf("\n-------------------------------------------------\n");
    printf(" From Node -> To Node       | Distance (km)      \n");
    printf("-------------------------------------------------\n");
    int count = 0;
    for (int i = 0; i < city->num_nodes; i++) {
        for (int j = i + 1; j < city->num_nodes; j++) {
            if (city->adj_matrix[i][j] > 0 && city->adj_matrix[i][j] < INF_DIST) {
                printf(" [%d] %-12s <-> [%d] %-12s : %d km\n",
                       i, city->nodes[i].name, j, city->nodes[j].name, city->adj_matrix[i][j]);
                count++;
            }
        }
    }
    if (count == 0) {
        printf(" No road connections exist.\n");
    }
    printf("-------------------------------------------------\n");
}
