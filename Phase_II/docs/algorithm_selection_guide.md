# Algorithm Selection Guide & Engineering Trade-Offs

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. Station Placement: Paradigm Selection Guide

When deciding how to allocate an EV capital installation budget among candidate station sites:

```
                  [STATION PLACEMENT DECISION]
                               |
                Is exact optimality required?
                               |
             +-----------------+-----------------+
             |                                   |
           [YES]                                [NO]
             |                                   |
    Is Budget B small/moderate?             Use GREEDY BY RATIO
    (B <= 5,000 integer units)              - Time: O(m log m)
             |                              - Memory: O(1)
       +-----+-----+                        - Fast, but sub-optimal
       |           |                          (e.g., 33 vs 50 on counterexample)
     [YES]        [NO]
       |           |
    Use DP       Are candidate sites m small?
  KNAPSACK       (m <= 25 items)
  - O(m * B)       |
  - O(m * B) mem +-----+-----+
                 |           |
               [YES]        [NO]
                 |           |
             Use B&B     Use APPROXIMATION
           LEAST-COST    (FPTAS / Heuristic)
           - Explores fewest
             nodes via bounds
```

### Detailed Trade-Off Matrix
| Approach | Time Complexity | Space Complexity | Optimality Guarantee | Best Operating Condition |
| :--- | :--- | :--- | :--- | :--- |
| **0/1 Knapsack DP** | $\Theta(m \cdot B)$ | $\Theta(m \cdot B)$ | **100% Guaranteed** | Moderate budget $B \le 2000$, discrete costs. |
| **Branch & Bound (LC)**| $O(2^m)$ worst, pruned | $O(\text{Queue})$ | **100% Guaranteed** | Large budget $B$, but small candidate count $m \le 25$. |
| **Greedy by Ratio** | $\Theta(m \log m)$ | $\Theta(1)$ | **Heuristic (No)** | Real-time response, memory-constrained microcontrollers. |
| **Backtracking (DFS)**| $O(2^m)$ | $O(m)$ stack | **100% Guaranteed** | Small benchmark testing, memory-critical exact search. |

---

## 2. Road Routing: Shortest Path Selection Guide

```
                      [EV ROUTING QUERY]
                               |
                   Are there negative weights?
                               |
             +-----------------+-----------------+
             |                                   |
           [YES]                                [NO]
             |                                   |
      BELLMAN-FORD                       Is query single-source
     - O(V * E) time                     or all-pairs?
     - Detects negative                  |
       energy cycles             +-------+-------+
                                 |               |
                          [SINGLE-SOURCE]   [ALL-PAIRS]
                                 |               |
                         Is graph dense?   FLOYD-WARSHALL
                         (E ~ V²)          - Theta(V³) time
                                 |         - Theta(V²) mem
                           +-----+-----+   - Best for repeated
                           |           |     fleet matrix lookups
                         [YES]        [NO]
                           |           |
                       DIJKSTRA    DIJKSTRA
                        ARRAY      MIN-HEAP
                       - O(V²)     - O((V+E) log V)
                       - Cache-    - Best for sparse
                         friendly    highway networks
```

---

## 3. Infrastructure Backbone: MST Selection (Prim vs Kruskal)

| Criterion | Prim's Algorithm | Kruskal's Algorithm | Recommended Selection |
| :--- | :--- | :--- | :--- |
| **Network Density** | $\Theta(V^2)$ with matrix | $O(E \log E)$ with edge list | Prim for dense city cores; Kruskal for sparse inter-city links. |
| **Data Representation**| Adjacency Matrix | Edge Array / List | If network is already in matrix form, Prim avoids edge extraction. |
| **Implementation Complexity**| Moderate (array scan) | Requires Disjoint Set (Union-Find)| Prim is easier to trace in live viva. |

---

## 4. Log Inspection: String Matching Selection Guide

| Algorithm | Preprocessing Time | Matching Time | Space | Practical Recommendation |
| :--- | :--- | :--- | :--- | :--- |
| **Naive Search** | None ($0$) | $O((n - m + 1) \cdot m)$ | $O(1)$ | Use only for tiny patterns ($m \le 3$) and short texts. |
| **Rabin-Karp** | $\Theta(m)$ | Avg $O(n + m)$, Worst $O(n \cdot m)$| $O(1)$ | Best when searching for multiple event patterns simultaneously. |
| **Finite Automaton**| $O(m^3 \cdot |\Sigma|)$ | $\Theta(n)$ transitions | $O(m \cdot |\Sigma|)$ | Best when streaming high-frequency sensor logs continuously. |
| **Knuth-Morris-Pratt (KMP)**| $\Theta(m)$ (LPS array) | $\Theta(n)$ | $\Theta(m)$ | **Best general-purpose offline log matcher**: linear, no backtracking. |
