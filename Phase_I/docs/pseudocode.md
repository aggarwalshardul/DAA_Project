# Pseudocode, Data Structures & Algorithm Selection Guide (Phase I)

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. Primary Algorithm 1: 0/1 Knapsack Dynamic Programming

### 1.1 Algorithmic Selection Rationale
Station placement is an exact 0/1 decision: an infrastructure site is either built ($x_i = 1$) or not built ($x_i = 0$). Fractional installation is impossible. Since this problem is weakly NP-complete, dynamic programming provides guaranteed theoretical optimality in pseudo-polynomial time $O(m \cdot B)$, which is extremely fast for practical municipal budgets ($B \le 1000$).

### 1.2 Pseudocode
```
Algorithm 01_KNAPSACK_DP(stations, m, B):
    Input:  stations[1..m] with cost[i] and benefit[i]; Budget B
    Output: max_benefit, list of selected station indices

    Let dp[0..m][0..B] be a 2D integer table

    // Base cases: zero items or zero budget capacity
    For w = 0 to B do:
        dp[0][w] = 0
    For i = 0 to m do:
        dp[i][0] = 0

    // Bottom-up subproblem computation
    For i = 1 to m do:
        For w = 1 to B do:
            If stations[i].cost <= w then:
                incl = dp[i - 1][w - stations[i].cost] + stations[i].benefit
                excl = dp[i - 1][w]
                dp[i][w] = max(incl, excl)
            Else:
                dp[i][w] = dp[i - 1][w]

    max_benefit = dp[m][B]

    // Backtracking reconstruction of selected stations
    selected = []
    curr_w = B
    For i = m down to 1 do:
        If dp[i][curr_w] != dp[i - 1][curr_w] then:
            Append stations[i] to selected
            curr_w = curr_w - stations[i].cost

    Return max_benefit, selected
```

### 1.3 Correctness & Loop Invariant
- **Optimal Substructure:** An optimal solution to a subproblem of budget $w$ considering items $1..i$ either excludes item $i$ (meaning the optimal solution is that of $1..i-1$ with budget $w$) or includes item $i$ (meaning the remaining budget $w - c_i$ must be filled optimally by items $1..i-1$).
- **Loop Invariant:** At the end of outer iteration $i$, for every $w \in [0, B]$, `dp[i][w]` stores the exact maximum benefit achievable using any subset of items $\{1, \dots, i\}$ whose cumulative cost does not exceed $w$.

---

## 2. Primary Algorithm 2: Dijkstra's Shortest Path Algorithm

### 2.1 Algorithmic Selection Rationale
City road lengths $d(u, v) \ge 0$ are non-negative physical distances. Dijkstra's algorithm uses a greedy paradigm to establish shortest paths from an origin to all network junctions.
- For small/dense road networks ($V \le 100$), the **Array-based implementation** ($O(V^2)$) is straightforward, cache-friendly, and has low constant factor overhead.
- For large/sparse highway networks, the **Min-Heap implementation** ($O((V + E) \log V)$) scales efficiently.

### 2.2 Pseudocode (Array-Based)
```
Algorithm DIJKSTRA_ARRAY(G, source, dest):
    Input:  Graph G=(V, E) with adjacency matrix adj[u][v], source node, dest node
    Output: dist[0..V-1], parent[0..V-1], reconstructed path

    For each v in V do:
        dist[v] = INFINITY
        visited[v] = FALSE
        parent[v] = -1

    dist[source] = 0

    Repeat V - 1 times:
        // Greedy choice: select unvisited vertex with minimum tentative distance
        u = argmin { dist[v] : v in V and not visited[v] }
        If u == -1 or dist[u] == INFINITY then break
        visited[u] = TRUE

        // Relaxation step for each neighbor v
        For each neighbor v in V do:
            If not visited[v] and adj[u][v] < INFINITY then:
                If dist[u] + adj[u][v] < dist[v] then:
                    dist[v] = dist[u] + adj[u][v]
                    parent[v] = u

    // Reconstruct path by following parent pointers from dest back to source
    path = RECONSTRUCT_PATH(parent, dest)
    Return dist, path
```

### 2.3 Correctness & Loop Invariant
- **Greedy Choice Property:** When vertex $u$ with minimum tentative distance is marked `visited`, its distance `dist[u]` is already guaranteed to be the true shortest path distance from `source`. Because edge weights are non-negative, any alternative path to $u$ would have to traverse some other unvisited vertex $x$, but `dist[x] >= dist[u]`, making any detour strictly longer or equal.

---

## 3. Sorting & Searching Algorithms

### 3.1 Merge Sort (Divide & Conquer)
- **Recurrence:** $T(n) = 2T(n/2) + \Theta(n)$
- **Master Theorem:** $a=2, b=2, d=1 \implies \Theta(n \log n)$ in all cases (best, average, worst).
- **Pseudocode:**
```
Algorithm MERGE_SORT(arr, left, right):
    If left < right then:
        mid = left + (right - left) / 2
        MERGE_SORT(arr, left, mid)
        MERGE_SORT(arr, mid + 1, right)
        MERGE(arr, left, mid, right)
```

### 3.2 Quick Sort (Divide & Conquer / Lomuto Partition)
- **Recurrence:**
  - Best/Average Case: $T(n) = 2T(n/2) + \Theta(n) \implies \Theta(n \log n)$
  - Worst Case (sorted/reversed array with end pivot): $T(n) = T(n-1) + \Theta(n) \implies \Theta(n^2)$
- **Pseudocode:**
```
Algorithm QUICK_SORT(arr, low, high):
    If low < high then:
        pi = PARTITION(arr, low, high)
        QUICK_SORT(arr, low, pi - 1)
        QUICK_SORT(arr, pi + 1, high)

Algorithm PARTITION(arr, low, high):
    pivot = arr[high]
    i = low - 1
    For j = low to high - 1 do:
        If arr[j] <= pivot then:
            i = i + 1
            SWAP(arr[i], arr[j])
    SWAP(arr[i + 1], arr[high])
    Return i + 1
```

### 3.3 Binary Search (Iterative & Recursive)
- **Recurrence for Recursive:** $T(n) = T(n/2) + \Theta(1) \implies \Theta(\log n)$
- **Invariant:** If target exists in `arr[0..N-1]`, it must be situated within the index range `[low, high]`.
