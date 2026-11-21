# Presentation: EV Charging Station Placement & Trip Planning System

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project Number:** 33  
**Candidate Name:** Shardul Aggarwal  
**SAP ID:** 590022064  
**Program:** B.Tech CSE (AI/ML)  
**Institution:** School of Computer Science, UPES Dehradun  

---

## Slide 1: Title & Candidate Information

### Project Title
# EV Charging Station Placement & Trip Planning System
### Course: CSEG3053 – Design and Analysis of Algorithms (Project 33)

- **Student Name:** Shardul Aggarwal
- **SAP ID:** 590022064
- **Program / Branch:** B.Tech Computer Science & Engineering (Artificial Intelligence & Machine Learning)
- **Institution:** School of Computer Science, UPES Dehradun
- **Implementation Standard:** Pure Standard C17 (GNU GCC), 100% Offline, Zero External Libraries

---

## Slide 2: Problem Statement & Industrial Motivation

### Dual Optimization Challenge
1. **Capital Infrastructure Allocation (Station Placement):**
   - Municipal capital budgets are strictly limited ($B \in \mathbb{Z}^+$).
   - Installing DC fast chargers costs $20k–$80k per station.
   - *Goal:* Select an optimal subset of candidate junctions to maximize daily public charging benefit without exceeding budget (Weakly NP-Complete 0/1 Knapsack problem).

2. **Energy-Constrained Fleet Routing:**
   - Electric Vehicles have finite battery capacities (30–100 kWh).
   - Shortest physical paths are useless if the vehicle runs out of battery midway.
   - *Goal:* Compute shortest paths and verify battery state-of-charge feasibility ($D \le R_{\max} \times \text{SOC}\%$) to prevent vehicle stranding.

### Core Software Engineering Constraints
- Strictly beginner-friendly standard C17 using fixed-size arrays (`#define MAX_NODES 100`).
- Every assessed algorithm written by hand from first principles (no `qsort`, `bsearch`, or libraries).
- Fully instrumented with execution timers (`clock()`), memory estimates, and an operation counter (`long long g_ops`).

---

## Slide 3: System Architecture & Complete DAA Workbench

### Domain Entities
- **Road Network $G = (V, E)$:** Up to 100 junctions, stored as an adjacency matrix `adj_matrix[100][100]`.
- **Candidate Stations:** ID, junction node, installation cost, power (kW), ports, benefit score.
- **Vehicles & Trips:** Battery capacity (kWh), range (km), current battery %, consumption (kWh/km).

### 8 Comprehensive Syllabus Areas Mapped & Implemented
1. **Divide & Conquer:** Merge Sort, Quick Sort, Binary Search (iterative & recursive), Maximum Subarray, Strassen Matrix Multiplication, Fast Exponentiation, Celebrity Hub Discovery.
2. **Greedy Methods:** Activity Selection, Interval Partitioning, Fractional Knapsack, Huffman Coding, Dijkstra (Array & Heap), Prim & Kruskal MST.
3. **Dynamic Programming:** 0/1 Knapsack DP, Matrix Chain Multiplication, LCS, Optimal BST, Bellman-Ford, Floyd-Warshall, TSP Bitmask DP.
4. **Backtracking & Branch and Bound:** N-Queens ($8\times 8 \to 92$ solutions), Sum of Subsets, Knapsack Backtracking, B&B (FIFO, LIFO, Least-Cost).
5. **Flow & Matching:** Ford-Fulkerson Max Flow & Min-Cut, Maximum Bipartite Matching.
6. **Number Theory & Strings:** Chinese Remainder Theorem with Extended Euclid, Naive, Rabin-Karp, Finite Automaton, KMP.

---

## Slide 4: Station Placement: DP vs Greedy vs Branch & Bound

### Dynamic Programming Formulation
- **Recurrence:**
  $$\text{dp}[i][w] = \max(\text{dp}[i-1][w], \text{dp}[i-1][w - c_i] + b_i) \quad \text{for } c_i \le w$$
- Table construction: $\Theta(m \cdot B)$ pseudo-polynomial time.
- Backtracking item reconstruction identifies exactly which stations are selected.

### Greedy Ratio Failure Counterexample (Proving Greedy Suboptimality)
- **Budget $B = 50$ units.**
  - Site A: Cost 30, Benefit 33 $\implies$ Ratio = $1.10$
  - Site B: Cost 25, Benefit 25 $\implies$ Ratio = $1.00$
  - Site C: Cost 25, Benefit 25 $\implies$ Ratio = $1.00$
- **Greedy Result:** Takes Site A (Benefit = **33**, stops because remaining budget is 20).
- **Optimal DP Result:** Takes Sites B + C (Cost = 50, Benefit = **50**).
- **Conclusion:** $50 > 33$, proving greedy choice failure!

### Branch & Bound Pruning Comparison
- **FIFO (Queue):** Explores 312 nodes.
- **Least-Cost (Best-First with Upper Bounds):** Explores only **148 nodes** (52% pruned) to guarantee the identical optimal benefit!

---

## Slide 5: Fleet Routing Engine & Battery Feasibility

### Shortest Path Routing Algorithms
- **Dijkstra Array ($\Theta(V^2)$):** Optimal for dense urban road networks ($E \approx V^2$), minimal cache misses.
- **Dijkstra Min-Heap ($O((V+E)\log V)$):** Array-backed binary heap for sparse highway networks ($E \ll V^2$).
- **Bellman-Ford ($O(V \cdot E)$):** Handles negative costs and detects negative energy cycles.
- **Floyd-Warshall ($\Theta(V^3)$):** Precomputes all-pairs shortest paths for instant fleet matrix dispatch.

### Live EV Battery Feasibility Checker
$$\text{Usable Range} = R_{\max} \times \left(\frac{\text{SOC}\%}{100}\right)$$
$$\text{Feasible} \iff \text{Route Distance} \le \text{Usable Range}$$
- Evaluates whether the EV can reach its destination directly or flags an energy deficit requiring an intermediate recharge.

### Multi-Stop Inspection Tour
- **Held-Karp TSP Bitmask DP ($\Theta(n^2 2^n)$):** Computes minimum-cost closed tours for $n \le 10$ stops without exploring $n!$ brute-force permutations.

---

## Slide 6: Capacity Allocation, Matching & Operational Intelligence

### Grid Capacity (Ford-Fulkerson Max Flow & Min-Cut)
- Models electrical power transmission from Substation Source to Charging Hub Sink.
- Computes maximum delivered power (kW) and outputs **Minimum-Cut bottleneck transmission lines**.
- Verified by the Max-Flow Min-Cut Theorem ($|f| = c(S, T)$).

### Fleet Bay Allocation (Maximum Bipartite Matching)
- Pairs arriving fleet vehicles with compatible charging bays using augmenting paths ($O(V \cdot E)$).
- Governed by Berge's Lemma: stops when no augmenting path exists.

### Distributed Counters (Chinese Remainder Theorem)
- Reconstructs total operational energy counters from remainder readings across partitioned sub-meters counting modulo pairwise coprime integers ($m_1, m_2, m_3$).
- Solved via the Extended Euclidean Algorithm in $\Theta(k \log M)$ time.

### Log Compression & Pattern Matching
- **Huffman Coding:** Compresses operational logs with variable-length prefix-free codes.
- **Knuth-Morris-Pratt (KMP):** Scans logs in linear $\Theta(n + m)$ time using the LPS array without backtracking text.

---

## Slide 7: Empirical Performance & 3 Practical Scenarios

### Empirical Benchmark Summary Table
| Algorithm | Input Size ($N$) | Dataset Type | Time (ms) | Operations (`g_ops`) | Memory | Optimality / Status |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **0/1 Knapsack DP** | 12 sites, $B=180$ | Normal City | 0.0410 ms | 2,364 | 9.2 KB | **100% Optimal** |
| **Knapsack Greedy** | 12 sites, $B=180$ | Normal City | 0.0080 ms | 86 | 0.5 KB | 92.4% (Suboptimal) |
| **B&B Least-Cost** | 12 sites, $B=180$ | Normal City | 0.0250 ms | 148 | 1.2 KB | **100% Optimal** |
| **Dijkstra Array** | 100 nodes | Dense Grid | 0.0820 ms | 9,900 | 0.8 KB | **Optimal Route** |
| **Dijkstra Heap** | 100 nodes | Sparse Net | 0.0450 ms | 1,840 | 1.6 KB | **Optimal Route** |
| **KMP Search** | 500 chars | Log Text | 0.0150 ms | 512 | 0.1 KB | Linear Scan |
| **Naive Search** | 500 chars | Log Text | 0.0620 ms | 1,840 | 0 KB | Quadratic Worst-Case |

### 3 Evaluated Scenarios
1. **Scenario 1 (Normal Operation):** 20 junctions, 12 sites, capital budget fits ~half. Side-by-side comparison of Knapsack paradigms and routing algorithms.
2. **Scenario 2 (Constraint Shock):** Station 101 at Node 0 suffers transformer outage + budget cut by 30%. Identifies stranded trips and reallocates fewer bays.
3. **Scenario 3 (Large-Data Stress):** Scales to 100 nodes and 1,000 items, demonstrating $O(n \log n)$ vs $O(n^2)$ scaling behavior.

---

## Slide 8: Complexity Reduction, Verification & Conclusion

### Formal Polynomial Reduction: SUBSET-SUM $\le_P$ 0/1 KNAPSACK DECISION
1. Given Subset-Sum instance $\langle S = \{s_1, \dots, s_n\}, T \rangle$.
2. Construct candidate stations with cost $c_i = s_i$ and benefit $b_i = s_i$. Set budget $B = T$ and threshold $K = T$.
3. Because cost equals benefit, a subset with cost $\le T$ achieves benefit $\ge T \iff$ the subset sums to $T$ exactly.
4. **Proves that the EV Station Placement Decision Problem is NP-Complete!**

### Project Verification Summary
- **Compilation:** `gcc -Wall -Wextra -std=c11 -O2` with **zero warnings**.
- **Automated Tests:** **39 / 39 tests PASSED (100% Green)**.
- **CLI Usability:** Complete 16-option menu with defensive input validation.
- **Repository Snapshots:** Frozen releases saved in `Phase_I/` and `Phase_II/`.

### Candidate
**Shardul Aggarwal** • SAP ID: 590022064 • B.Tech CSE (AI/ML)  
*Thank you! Open for questions.*
