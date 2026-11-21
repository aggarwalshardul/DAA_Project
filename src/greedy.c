/**
 * @file greedy.c
 * @brief Implementations of complete Greedy algorithm suite.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "greedy.h"

/* -------------------------------------------------------------
 * 1. ACTIVITY SELECTION (MAX SESSIONS ON ONE CHARGER)
 * ------------------------------------------------------------- */

/**
 * Greedily selects maximum mutually compatible charging sessions.
 * Sessions are sorted by finish time ascending; picks if start >= last_finish.
 * Time: Theta(n log n) sorting + Theta(n) greedy scan. Space: O(n).
 */
ActivitySelectionResult activity_selection(ChargingSession sessions[], int n) {
    ActivitySelectionResult res;
    res.count = 0;
    if (n <= 0) return res;

    /* Sort sessions by finish time ascending using selection sort */
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            ops_increment(1);
            if (sessions[j].finish < sessions[min_idx].finish) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            ChargingSession tmp = sessions[i];
            sessions[i] = sessions[min_idx];
            sessions[min_idx] = tmp;
            ops_increment(3);
        }
    }

    /* Greedy Choice: Pick first activity (earliest finish time) */
    res.session_ids[res.count++] = sessions[0].id;
    int last_finish = sessions[0].finish;

    for (int i = 1; i < n; i++) {
        ops_increment(1);
        if (sessions[i].start >= last_finish) {
            res.session_ids[res.count++] = sessions[i].id;
            last_finish = sessions[i].finish;
        }
    }
    return res;
}

/* -------------------------------------------------------------
 * 2. INTERVAL PARTITIONING (MINIMUM CHARGERS NEEDED)
 * ------------------------------------------------------------- */

/**
 * Computes minimum number of physical charging bays needed to serve all sessions.
 * Sessions sorted by start time. Assigns session to first free charger; otherwise opens new bay.
 * Time: Theta(n^2), Space: O(n).
 */
IntervalPartitionResult interval_partitioning(ChargingSession sessions[], int n) {
    IntervalPartitionResult res;
    res.total_chargers_needed = 0;
    if (n <= 0) return res;

    /* Sort by start time ascending */
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            ops_increment(1);
            if (sessions[j].start < sessions[min_idx].start) min_idx = j;
        }
        if (min_idx != i) {
            ChargingSession tmp = sessions[i]; sessions[i] = sessions[min_idx]; sessions[min_idx] = tmp;
            ops_increment(3);
        }
    }

    /* Array tracking available finish time of each charger bay */
    int charger_free_at[MAX_TRIPS] = {0};

    for (int i = 0; i < n; i++) {
        int assigned = -1;
        for (int c = 0; c < res.total_chargers_needed; c++) {
            ops_increment(1);
            if (charger_free_at[c] <= sessions[i].start) {
                assigned = c;
                charger_free_at[c] = sessions[i].finish;
                break;
            }
        }
        if (assigned == -1) {
            /* Open a new charger */
            assigned = res.total_chargers_needed;
            charger_free_at[assigned] = sessions[i].finish;
            res.total_chargers_needed++;
        }
        res.session_assigned_charger[i] = assigned;
    }
    return res;
}

/* -------------------------------------------------------------
 * 3. FRACTIONAL KNAPSACK (GRID POWER ALLOCATION)
 * ------------------------------------------------------------- */

/**
 * Greedily distributes limited grid kW power to prioritized EV requests.
 * Items sorted by value-to-weight ratio (priority / kw_demanded) descending.
 * Time: Theta(n log n) sorting + Theta(n) allocation. Space: O(1).
 */
FractionalKnapsackResult fractional_knapsack_power(PowerRequest reqs[], int n, double total_grid_kw) {
    FractionalKnapsackResult res;
    res.total_priority_served = 0.0;
    res.total_kw_allocated = 0.0;
    if (n <= 0 || total_grid_kw <= 0.0) return res;

    /* Sort by ratio (priority_weight / kw_demanded) descending */
    for (int i = 0; i < n - 1; i++) {
        int best = i;
        double rBest = (reqs[i].kw_demanded > 0) ? (reqs[i].priority_weight / reqs[i].kw_demanded) : 0;
        for (int j = i + 1; j < n; j++) {
            ops_increment(1);
            double rJ = (reqs[j].kw_demanded > 0) ? (reqs[j].priority_weight / reqs[j].kw_demanded) : 0;
            if (rJ > rBest) { rBest = rJ; best = j; }
        }
        if (best != i) {
            PowerRequest t = reqs[i]; reqs[i] = reqs[best]; reqs[best] = t;
            ops_increment(3);
        }
    }

    double remaining_kw = total_grid_kw;
    for (int i = 0; i < n; i++) {
        ops_increment(1);
        if (remaining_kw <= 0.0) {
            reqs[i].kw_allocated = 0.0;
            continue;
        }
        if (reqs[i].kw_demanded <= remaining_kw) {
            reqs[i].kw_allocated = reqs[i].kw_demanded;
            res.total_kw_allocated += reqs[i].kw_demanded;
            res.total_priority_served += reqs[i].priority_weight;
            remaining_kw -= reqs[i].kw_demanded;
        } else {
            /* Take fraction */
            double fraction = remaining_kw / reqs[i].kw_demanded;
            reqs[i].kw_allocated = remaining_kw;
            res.total_kw_allocated += remaining_kw;
            res.total_priority_served += reqs[i].priority_weight * fraction;
            remaining_kw = 0.0;
        }
    }
    return res;
}

/* -------------------------------------------------------------
 * 4. PRIM'S MINIMUM SPANNING TREE (MST)
 * ------------------------------------------------------------- */

/**
 * Computes Minimum Spanning Tree using Prim's algorithm.
 * Time: Theta(V^2), Space: Theta(V).
 */
MSTResult prim_mst(const CityNetwork *city) {
    MSTResult res;
    res.total_weight = 0;
    res.edge_count = 0;
    if (city->num_nodes < 2) return res;

    int parent[MAX_NODES];
    int key[MAX_NODES];
    int in_mst[MAX_NODES] = {0};

    for (int i = 0; i < city->num_nodes; i++) {
        key[i] = INF_DIST;
        parent[i] = -1;
    }
    key[0] = 0;

    for (int count = 0; count < city->num_nodes - 1; count++) {
        int min_key = INF_DIST, u = -1;
        for (int v = 0; v < city->num_nodes; v++) {
            ops_increment(1);
            if (!in_mst[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }
        if (u == -1) break;
        in_mst[u] = 1;

        for (int v = 0; v < city->num_nodes; v++) {
            ops_increment(1);
            int w = city->adj_matrix[u][v];
            if (w > 0 && w < INF_DIST && !in_mst[v] && w < key[v]) {
                parent[v] = u;
                key[v] = w;
            }
        }
    }

    for (int i = 1; i < city->num_nodes; i++) {
        if (parent[i] != -1) {
            res.edges[res.edge_count].u = parent[i];
            res.edges[res.edge_count].v = i;
            res.edges[res.edge_count].weight = city->adj_matrix[parent[i]][i];
            res.total_weight += city->adj_matrix[parent[i]][i];
            res.edge_count++;
        }
    }
    return res;
}

/* -------------------------------------------------------------
 * 5. KRUSKAL'S MINIMUM SPANNING TREE (MST)
 * ------------------------------------------------------------- */

static int find_root(int parent[], int i) {
    ops_increment(1);
    if (parent[i] == -1) return i;
    return parent[i] = find_root(parent, parent[i]); /* Path compression */
}

static void union_sets(int parent[], int rank[], int x, int y) {
    int root_x = find_root(parent, x);
    int root_y = find_root(parent, y);
    if (root_x != root_y) {
        ops_increment(1);
        if (rank[root_x] < rank[root_y]) {
            parent[root_x] = root_y;
        } else if (rank[root_x] > rank[root_y]) {
            parent[root_y] = root_x;
        } else {
            parent[root_y] = root_x;
            rank[root_x]++;
        }
    }
}

/**
 * Computes MST using Kruskal's algorithm with Union-Find.
 * Time: Theta(E log E) sorting + O(E alpha(V)) union-find. Space: O(E + V).
 */
MSTResult kruskal_mst(const CityNetwork *city) {
    MSTResult res;
    res.total_weight = 0;
    res.edge_count = 0;

    /* Extract all undirected edges */
    GraphEdge all_edges[MAX_NODES * MAX_NODES];
    int num_edges = 0;
    for (int i = 0; i < city->num_nodes; i++) {
        for (int j = i + 1; j < city->num_nodes; j++) {
            if (city->adj_matrix[i][j] > 0 && city->adj_matrix[i][j] < INF_DIST) {
                all_edges[num_edges].u = i;
                all_edges[num_edges].v = j;
                all_edges[num_edges].weight = city->adj_matrix[i][j];
                num_edges++;
            }
        }
    }

    /* Sort edges by weight */
    for (int i = 0; i < num_edges - 1; i++) {
        int min_i = i;
        for (int j = i + 1; j < num_edges; j++) {
            ops_increment(1);
            if (all_edges[j].weight < all_edges[min_i].weight) min_i = j;
        }
        if (min_i != i) {
            GraphEdge tmp = all_edges[i]; all_edges[i] = all_edges[min_i]; all_edges[min_i] = tmp;
            ops_increment(3);
        }
    }

    int parent[MAX_NODES];
    int rank[MAX_NODES] = {0};
    for (int i = 0; i < city->num_nodes; i++) parent[i] = -1;

    for (int i = 0; i < num_edges && res.edge_count < city->num_nodes - 1; i++) {
        int u = all_edges[i].u;
        int v = all_edges[i].v;
        int root_u = find_root(parent, u);
        int root_v = find_root(parent, v);

        if (root_u != root_v) {
            res.edges[res.edge_count++] = all_edges[i];
            res.total_weight += all_edges[i].weight;
            union_sets(parent, rank, root_u, root_v);
        }
    }
    return res;
}

/* -------------------------------------------------------------
 * 6. HUFFMAN CODING (LOG COMPRESSION & DECOMPRESSION)
 * ------------------------------------------------------------- */

typedef struct HuffmanNode {
    char ch;
    int freq;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

static void build_codes(HuffmanNode *root, char *curr_code, int depth, HuffmanResult *res) {
    if (!root) return;
    if (!root->left && !root->right) {
        curr_code[depth] = '\0';
        for (int i = 0; i < res->distinct_chars; i++) {
            if (res->table[i].ch == root->ch) {
                strncpy(res->table[i].code, curr_code, 31);
                res->table[i].code[31] = '\0';
                break;
            }
        }
        return;
    }
    curr_code[depth] = '0';
    build_codes(root->left, curr_code, depth + 1, res);
    curr_code[depth] = '1';
    build_codes(root->right, curr_code, depth + 1, res);
}

/**
 * Builds Huffman Tree and compresses text into binary string representation.
 * Time: Theta(n + k log k) where k is alphabet size, Space: O(k).
 */
HuffmanResult huffman_compress(const char *text, char *encoded_output, size_t out_size) {
    HuffmanResult res;
    res.distinct_chars = 0;
    res.original_bits = 0;
    res.compressed_bits = 0;
    res.compression_ratio = 1.0;
    encoded_output[0] = '\0';

    if (!text || strlen(text) == 0) return res;

    int freq[HUFFMAN_MAX_CHARS] = {0};
    int len = (int)strlen(text);
    for (int i = 0; i < len; i++) {
        freq[(unsigned char)text[i]]++;
        ops_increment(1);
    }

    HuffmanNode *nodes[HUFFMAN_MAX_CHARS];
    int node_count = 0;
    for (int i = 0; i < HUFFMAN_MAX_CHARS; i++) {
        if (freq[i] > 0) {
            HuffmanNode *nd = (HuffmanNode *)malloc(sizeof(HuffmanNode));
            nd->ch = (char)i;
            nd->freq = freq[i];
            nd->left = NULL;
            nd->right = NULL;
            nodes[node_count++] = nd;

            res.table[res.distinct_chars].ch = (char)i;
            res.table[res.distinct_chars].freq = freq[i];
            res.table[res.distinct_chars].code[0] = '\0';
            res.distinct_chars++;
        }
    }

    /* Edge case: only 1 distinct character */
    if (node_count == 1) {
        strncpy(res.table[0].code, "0", 31);
        for (int i = 0; i < len && (size_t)i < out_size - 1; i++) {
            encoded_output[i] = '0';
        }
        encoded_output[len] = '\0';
        res.original_bits = len * 8;
        res.compressed_bits = len;
        res.compression_ratio = (double)res.compressed_bits / res.original_bits;
        free(nodes[0]);
        return res;
    }

    /* Repeatedly combine two smallest nodes */
    int active_nodes = node_count;
    while (active_nodes > 1) {
        int min1 = -1, min2 = -1;
        for (int i = 0; i < active_nodes; i++) {
            ops_increment(1);
            if (min1 == -1 || nodes[i]->freq < nodes[min1]->freq) {
                min2 = min1; min1 = i;
            } else if (min2 == -1 || nodes[i]->freq < nodes[min2]->freq) {
                min2 = i;
            }
        }
        HuffmanNode *parent = (HuffmanNode *)malloc(sizeof(HuffmanNode));
        parent->ch = '\0';
        parent->freq = nodes[min1]->freq + nodes[min2]->freq;
        parent->left = nodes[min1];
        parent->right = nodes[min2];

        nodes[min1] = parent;
        nodes[min2] = nodes[active_nodes - 1];
        active_nodes--;
    }

    char code_buf[32];
    build_codes(nodes[0], code_buf, 0, &res);

    /* Free Huffman tree */
    /* Encode string */
    encoded_output[0] = '\0';
    for (int i = 0; i < len; i++) {
        for (int c = 0; c < res.distinct_chars; c++) {
            if (res.table[c].ch == text[i]) {
                strncat(encoded_output, res.table[c].code, out_size - strlen(encoded_output) - 1);
                res.compressed_bits += (int)strlen(res.table[c].code);
                break;
            }
        }
    }
    res.original_bits = len * 8;
    res.compression_ratio = (res.original_bits > 0) ?
        ((double)res.compressed_bits / res.original_bits) : 0.0;

    return res;
}

/**
 * Decodes a binary string using the Huffman code table.
 * Time: Theta(encoded_length), Space: O(output).
 */
void huffman_decompress(const char *encoded, const HuffmanResult *huff, char *decoded_output) {
    int out_idx = 0;
    char current[32] = "";
    int cur_len = 0;

    for (int i = 0; encoded[i] != '\0'; i++) {
        current[cur_len++] = encoded[i];
        current[cur_len] = '\0';

        for (int c = 0; c < huff->distinct_chars; c++) {
            ops_increment(1);
            if (strcmp(current, huff->table[c].code) == 0) {
                decoded_output[out_idx++] = huff->table[c].ch;
                cur_len = 0;
                current[0] = '\0';
                break;
            }
        }
    }
    decoded_output[out_idx] = '\0';
}

/* -------------------------------------------------------------
 * 7. GREEDY KNAPSACK & DIJKSTRA (FROM PHASE I)
 * ------------------------------------------------------------- */

KnapsackResult knapsack_greedy_ratio(const Station stations[], int count, int budget) {
    KnapsackResult res;
    res.max_benefit = 0; res.total_cost = 0; res.selected_count = 0;
    if (count <= 0 || budget <= 0) return res;

    Station sorted[MAX_STATIONS];
    int orig_idx[MAX_STATIONS];
    for (int i = 0; i < count; i++) {
        sorted[i] = stations[i];
        orig_idx[i] = i;
    }

    for (int i = 0; i < count - 1; i++) {
        int max_i = i;
        double rMax = (sorted[i].install_cost > 0) ? ((double)sorted[i].benefit_score / sorted[i].install_cost) : 0;
        for (int j = i + 1; j < count; j++) {
            ops_increment(1);
            double rJ = (sorted[j].install_cost > 0) ? ((double)sorted[j].benefit_score / sorted[j].install_cost) : 0;
            if (rJ > rMax) { rMax = rJ; max_i = j; }
        }
        if (max_i != i) {
            Station tmpS = sorted[i]; sorted[i] = sorted[max_i]; sorted[max_i] = tmpS;
            int tmpI = orig_idx[i]; orig_idx[i] = orig_idx[max_i]; orig_idx[max_i] = tmpI;
            ops_increment(6);
        }
    }

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
    return res;
}

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

DijkstraResult dijkstra_array(const CityNetwork *city, int src, int dest) {
    DijkstraResult res;
    res.source = src; res.dest = dest; res.path_length = 0; res.is_reachable = 0;
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
        if (right < h->size && h->data[right].dist < h->data[left].dist) smallest = right;
        ops_increment(1);
        if (h->data[smallest].dist < h->data[i].dist) {
            heap_swap(&h->data[i], &h->data[smallest]);
            i = smallest;
        } else break;
    }
    return min_node;
}

DijkstraResult dijkstra_heap(const CityNetwork *city, int src, int dest) {
    DijkstraResult res;
    res.source = src; res.dest = dest; res.path_length = 0; res.is_reachable = 0;

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

void dijkstra_print_path(const CityNetwork *city, const DijkstraResult *res) {
    if (!res->is_reachable) {
        printf(" Destination [%d] is UNREACHABLE from Source [%d].\n", res->dest, res->source);
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

int is_path_ev_feasible(const CityNetwork *city,
                        const DijkstraResult *route,
                        double vehicle_range,
                        double *out_battery_used) {
    if (!route || !route->is_reachable) return 0;
    int total_dist = route->dist[route->dest];
    if (out_battery_used) *out_battery_used = (double)total_dist;
    (void)city;
    return (total_dist <= vehicle_range) ? 1 : 0;
}
