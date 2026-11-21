/**
 * @file greedy.c
 * @brief Implementations of Greedy Knapsack and Dijkstra's Shortest Path.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "greedy.h"

/* -------------------------------------------------------------
 * GREEDY KNAPSACK (RATIO-BASED HEURISTIC FOR 0/1 PLACEMENT)
 * ------------------------------------------------------------- */

/**
 * Greedy 0/1 Knapsack heuristic: sorts candidate stations by benefit/cost ratio
 * and greedily picks items that fit within the remaining budget.
 * Note: For 0/1 knapsack, this is a heuristic and can be suboptimal.
 * Time: O(n log n) for sorting + O(n) for selection. Space: O(n).
 */
KnapsackResult knapsack_greedy_ratio(const Station stations[], int count, int budget) {
    KnapsackResult res;
    res.max_benefit = 0;
    res.total_cost = 0;
    res.selected_count = 0;

    if (count <= 0 || budget <= 0) return res;

    /* Make local copy of candidate items to sort */
    Station *sorted = (Station *)malloc((size_t)count * sizeof(Station));
    int *orig_idx = (int *)malloc((size_t)count * sizeof(int));
    for (int i = 0; i < count; i++) {
        sorted[i] = stations[i];
        orig_idx[i] = i;
    }

    /* Simple selection sort by ratio descending */
    for (int i = 0; i < count - 1; i++) {
        int max_i = i;
        double rMax = (sorted[i].install_cost > 0) ?
            ((double)sorted[i].benefit_score / sorted[i].install_cost) : 0;
        for (int j = i + 1; j < count; j++) {
            ops_increment(1);
            double rJ = (sorted[j].install_cost > 0) ?
                ((double)sorted[j].benefit_score / sorted[j].install_cost) : 0;
            if (rJ > rMax) {
                rMax = rJ;
                max_i = j;
            }
        }
        if (max_i != i) {
            Station tmpS = sorted[i]; sorted[i] = sorted[max_i]; sorted[max_i] = tmpS;
            int tmpI = orig_idx[i]; orig_idx[i] = orig_idx[max_i]; orig_idx[max_i] = tmpI;
            ops_increment(6);
        }
    }

    /* Greedily take items that fit in remaining budget */
    int rem_budget = budget;
    for (int i = 0; i < count; i++) {
        ops_increment(1);
        if (sorted[i].install_cost <= rem_budget) {
            res.selected_indices[res.selected_count++] = orig_idx[i];
            res.total_cost += sorted[i].install_cost;
            res.max_benefit += sorted[i].benefit_score;
            rem_budget -= sorted[i].install_cost;
        }
    }

    free(sorted);
    free(orig_idx);
    return res;
}

/* -------------------------------------------------------------
 * DIJKSTRA ARRAY-BASED O(V^2)
 * ------------------------------------------------------------- */

/**
 * Finds the unvisited vertex with minimum distance value.
 * Time: O(V), Space: O(1).
 */
static int min_distance_vertex(const int dist[], const int visited[], int num_nodes) {
    int min_val = INF_DIST;
    int min_index = -1;

    for (int v = 0; v < num_nodes; v++) {
        ops_increment(1);
        if (!visited[v] && dist[v] <= min_val) {
            min_val = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

/**
 * Reconstructs path from source to dest using the parent array.
 * Time: O(V), Space: O(1).
 */
static void reconstruct_path(DijkstraResult *res, int dest) {
    int temp_path[MAX_NODES];
    int count = 0;
    int curr = dest;

    while (curr != -1 && count < MAX_NODES) {
        temp_path[count++] = curr;
        curr = res->parent[curr];
    }

    res->path_length = count;
    for (int i = 0; i < count; i++) {
        res->path[i] = temp_path[count - 1 - i];
    }
}

/**
 * Dijkstra Shortest Path using an array.
 * Computes shortest path from src to dest.
 * Time Complexity: Theta(V^2) where V is the number of junctions.
 * Space Complexity: O(V) auxiliary memory for dist, visited, parent.
 */
DijkstraResult dijkstra_array(const CityNetwork *city, int src, int dest) {
    DijkstraResult res;
    res.source = src;
    res.dest = dest;
    res.path_length = 0;
    res.is_reachable = 0;

    int visited[MAX_NODES] = {0};

    for (int i = 0; i < city->num_nodes; i++) {
        res.dist[i] = INF_DIST;
        res.parent[i] = -1;
    }
    res.dist[src] = 0;

    for (int step = 0; step < city->num_nodes - 1; step++) {
        int u = min_distance_vertex(res.dist, visited, city->num_nodes);
        if (u == -1 || res.dist[u] == INF_DIST) break;
        visited[u] = 1;

        for (int v = 0; v < city->num_nodes; v++) {
            ops_increment(1);
            int weight = city->adj_matrix[u][v];
            if (!visited[v] && weight > 0 && weight < INF_DIST) {
                if (res.dist[u] + weight < res.dist[v]) {
                    res.dist[v] = res.dist[u] + weight;
                    res.parent[v] = u;
                }
            }
        }
    }

    if (dest >= 0 && dest < city->num_nodes && res.dist[dest] < INF_DIST) {
        res.is_reachable = 1;
        reconstruct_path(&res, dest);
    }
    return res;
}

/* -------------------------------------------------------------
 * MIN-HEAP DIJKSTRA O((V + E) log V)
 * ------------------------------------------------------------- */

typedef struct {
    int v;
    int dist;
} HeapNode;

typedef struct {
    int size;
    HeapNode data[MAX_NODES * 4];
} MinHeap;

static void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode t = *a; *a = *b; *b = t;
    ops_increment(3);
}

static void heap_push(MinHeap *h, int v, int dist) {
    int i = h->size++;
    h->data[i].v = v;
    h->data[i].dist = dist;
    while (i > 0) {
        int parent = (i - 1) / 2;
        ops_increment(1);
        if (h->data[i].dist < h->data[parent].dist) {
            heap_swap(&h->data[i], &h->data[parent]);
            i = parent;
        } else break;
    }
}

static HeapNode heap_pop(MinHeap *h) {
    HeapNode min_node = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (2 * i + 1 < h->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = left;
        if (right < h->size && h->data[right].dist < h->data[left].dist) {
            smallest = right;
        }
        ops_increment(1);
        if (h->data[smallest].dist < h->data[i].dist) {
            heap_swap(&h->data[i], &h->data[smallest]);
            i = smallest;
        } else break;
    }
    return min_node;
}

/**
 * Dijkstra Shortest Path using a binary Min-Heap.
 * Time Complexity: O((V + E) log V). Space: O(V).
 */
DijkstraResult dijkstra_heap(const CityNetwork *city, int src, int dest) {
    DijkstraResult res;
    res.source = src;
    res.dest = dest;
    res.path_length = 0;
    res.is_reachable = 0;

    for (int i = 0; i < city->num_nodes; i++) {
        res.dist[i] = INF_DIST;
        res.parent[i] = -1;
    }
    res.dist[src] = 0;

    MinHeap heap;
    heap.size = 0;
    heap_push(&heap, src, 0);

    while (heap.size > 0) {
        HeapNode top = heap_pop(&heap);
        int u = top.v;
        if (top.dist > res.dist[u]) continue;

        for (int v = 0; v < city->num_nodes; v++) {
            ops_increment(1);
            int weight = city->adj_matrix[u][v];
            if (weight > 0 && weight < INF_DIST) {
                if (res.dist[u] + weight < res.dist[v]) {
                    res.dist[v] = res.dist[u] + weight;
                    res.parent[v] = u;
                    heap_push(&heap, v, res.dist[v]);
                }
            }
        }
    }

    if (dest >= 0 && dest < city->num_nodes && res.dist[dest] < INF_DIST) {
        res.is_reachable = 1;
        reconstruct_path(&res, dest);
    }
    return res;
}

/**
 * Prints the reconstructed shortest path.
 * Time: O(P) where P is path length. Space: O(1).
 */
void dijkstra_print_path(const CityNetwork *city, const DijkstraResult *res) {
    if (!res->is_reachable) {
        printf(" Destination [%d] is UNREACHABLE from Source [%d].\n",
               res->dest, res->source);
        return;
    }
    printf(" Path: ");
    for (int i = 0; i < res->path_length; i++) {
        int u = res->path[i];
        printf("[%d]%s", u, city->nodes[u].name);
        if (i < res->path_length - 1) printf(" -> ");
    }
    printf("\n Total Distance: %d km\n", res->dist[res->dest]);
}

/**
 * Verifies whether the planned route is feasible for the EV's battery range.
 * Time: O(P), Space: O(1).
 */
int is_path_ev_feasible(const CityNetwork *city,
                        const DijkstraResult *route,
                        double vehicle_range,
                        double *out_battery_used) {
    if (!route || !route->is_reachable) {
        return 0;
    }
    int total_dist = route->dist[route->dest];
    if (out_battery_used) {
        *out_battery_used = (double)total_dist;
    }

    /* In basic Phase I model: feasible if total trip <= vehicle range */
    (void)city;
    return (total_dist <= vehicle_range) ? 1 : 0;
}
