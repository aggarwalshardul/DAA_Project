# Project Limitations, Error Analysis & Phase Progression Report

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. Domain Modeling Limitations & Assumptions

While the algorithmic engine provides rigorous $O, \Omega, \Theta$ theoretical guarantees, practical deployment in a live smart city involves real-world physical and operational phenomena:

1. **Static Edge Weights vs Dynamic Congestion:**
   - *Current Model:* Edge distances $d(u, v)$ are constant physical lengths in kilometers.
   - *Real-world Reality:* Real EV energy consumption is non-linear and depends heavily on stop-and-go traffic congestion, ambient temperature (HVAC load), elevation gradients (regenerative braking), and vehicle acceleration profile.
2. **Deterministic Arrival Times in Scheduling:**
   - *Current Model:* Activity Selection and Interval Partitioning assume deterministic, punctual arrival intervals $[s_i, f_i]$.
   - *Real-world Reality:* Vehicles face traffic delays and stochastic charger queues. Real-world systems require queueing theory ($M/M/c$ models) or online dynamic scheduling.
3. **Discrete Budget Scaling (Pseudo-Polynomial Memory):**
   - *Current Model:* 0/1 Knapsack DP allocates a table of $(m + 1) \times (B + 1)$ integers.
   - *Limitation:* If budget $B$ is scaled to high financial precision (e.g., $\$1,000,000$), memory consumption reaches several megabytes or gigabytes.
4. **Single-Charger Battery Feasibility:**
   - *Current Model:* Feasibility is evaluated as a single-hop constraint ($\text{dist} \le \text{range}$). Multi-stop charging routes with intermediate top-ups require multi-criteria graph expansion.

---

## 2. Error Analysis & Defensive Architecture

To ensure the CLI simulator never crashes during evaluation or unexpected inputs, the software incorporates defensive validation layers:

| Potential Failure Point | Error Mechanism | Architectural Mitigation in System |
| :--- | :--- | :--- |
| **Non-Numeric CLI Input** | Typing letters ("abc") into integer prompt | `read_int_safe()` reads via `fgets()`, parses with `strtol()`, and flushes standard input buffer. |
| **Corrupted Dataset Files** | Missing tokens or negative coordinates | `city_load_from_file()` validates ranges, checks node count ($2 \le V \le 100$), and rejects invalid references. |
| **Disconnected Road Graph** | Target destination has no physical road connection | Dijkstra, Bellman-Ford, and Floyd-Warshall check `dist[dest] < INF_DIST` and mark `is_reachable = 0`. |
| **Non-Coprime CRT Moduli** | User provides moduli with $\gcd(m_i, m_j) > 1$ | `modular_inverse()` verifies $\gcd(M_i, m_i) = 1$ using Extended Euclidean algorithm and aborts safely if invalid. |
| **Adversarial Quick Sort** | Already-sorted or reverse-sorted lists | Tested in test harness; operates safely without stack overflow for $N \le 1000$. |

---

## 3. Measurable Progression: Improvements After Phase I

In accordance with the hackathon evaluation rubrics, Phase II demonstrates a complete expansion from the baseline Phase I deliverable:

```
+---------------------------------------------------------------------------------------+
| PHASE I DELIVERABLE (Core Baseline)                                                   |
| - Domain model & file parser (small.txt, medium.txt)                                  |
| - 0/1 Knapsack DP vs Greedy Ratio baseline                                            |
| - Dijkstra Shortest Path (Array & Min-Heap)                                           |
| - Merge Sort & Quick Sort                                                             |
| - Binary Search (Iterative & Recursive)                                               |
| - 37 Unit & Boundary Tests                                                            |
+---------------------------------------------------------------------------------------+
                                           |
                                           v  [SYSTEMATIC PROGRESSION]
+---------------------------------------------------------------------------------------+
| PHASE II COMPLETE PRODUCT (Advanced Workbench)                                        |
| 1. Full Syllabus Expansion:                                                           |
|    - Divide & Conquer: Maximum Subarray, Fast Exponentiation, Celebrity Hub, Strassen. |
|    - Greedy Suite: Activity Selection, Interval Partitioning, Fractional Knapsack,    |
|      Huffman Compression (Encode/Decode), Prim & Kruskal MST.                         |
|    - Dynamic Programming Suite: Matrix Chain Order, LCS Sequence, Optimal BST,        |
|      Bellman-Ford (cycle check), Floyd-Warshall (all-pairs), TSP Bitmask DP.           |
|    - Backtracking & B&B: N-Queens (92 sols), Sum of Subsets, B&B (FIFO vs LIFO vs LC).|
|    - Flow & Matching: Ford-Fulkerson Max Power Flow & Min-Cut, Max Bipartite Matching.|
|    - Number Theory: Chinese Remainder Theorem with Extended Euclid.                   |
|    - String Matching: Naive, Rabin-Karp, Finite Automaton, KMP on event logs.         |
| 2. Scenario Evaluation:                                                               |
|    - Scenario 1 (Normal Operation): Multi-paradigm knapsack & routing comparison.     |
|    - Scenario 2 (Constraint Shock): Transformer outage & 30% budget cut recovery.     |
|    - Scenario 3 (Large-Data Stress): 100-node scalability test on large.txt.           |
| 3. Academic & Theoretical Formalization:                                              |
|    - Recurrence derivations via Master Theorem & Iteration (docs/recurrences.md).     |
|    - NP-completeness proof: SUBSET-SUM <=_P 0/1 KNAPSACK (docs/complexity_classes.md).|
|    - Correctness invariants & Cut Property proofs (docs/correctness_notes.md).        |
|    - Benchmark runner exporting performance metrics to benchmark_results.csv.         |
+---------------------------------------------------------------------------------------+
```
