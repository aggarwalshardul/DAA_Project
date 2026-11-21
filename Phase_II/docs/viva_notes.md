# Master Viva Preparation Guide & Comprehensive Examiner Notes

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. Per-Algorithm Viva Breakdown (Idea, Trace, Complexity, Q&A, Modification)

### Module 1: 0/1 Knapsack Dynamic Programming (Station Placement)
- **Idea in 3 lines:** We select candidate charging station sites to maximize total public charging benefit without exceeding installation budget $B$. Because a site is either fully installed or not, fractional installation is prohibited. A 2D table `dp[i][w]` is built bottom-up to find the global optimum.
- **Trace on Tiny Example:** Items: $S_1(c=2, b=3), S_2(c=3, b=4), S_3(c=4, b=5)$; Budget $B=5$.
  `dp[1] = [0, 0, 3, 3, 3, 3]`; `dp[2] = [0, 0, 3, 4, 4, 7]`; `dp[3][5] = max(7, dp[2][1]+5) = 7`. Selected: $S_1 + S_2$ (Cost 5, Benefit 7).
- **Complexity:** Time $\Theta(m \cdot B)$, Space $\Theta(m \cdot B)$.
- **3 Viva Questions:**
  1. *Q:* Why is $\Theta(m \cdot B)$ called pseudo-polynomial?  
     *A:* Because $B$ is an integer value whose size is $k = \log_2 B$ bits; thus time is $O(m \cdot 2^k)$, exponential in the length of $B$.
  2. *Q:* How do you reconstruct the items?  
     *A:* Backtrack from `dp[m][B]`. If `dp[i][w] != dp[i-1][w]`, item $i$ was picked; subtract $c_i$ from $w$ and continue.
  3. *Q:* What if $B$ is very large (e.g. 10 million)?  
     *A:* The DP table would exceed RAM; use Branch & Bound, Meet-in-the-Middle ($O(2^{n/2})$), or FPTAS approximation instead.
- **Modification Question:** *How to enforce that Station 1 and Station 2 cannot both be chosen (mutual exclusion)?*  
  *Modification:* Before table construction, evaluate two separate knapsacks: one without Station 1, one without Station 2, and take the maximum benefit.

---

### Module 2: Dijkstra Shortest Path (Array vs Min-Heap)
- **Idea in 3 lines:** Computes shortest non-negative driving distance from an origin junction to all city nodes. Greedily marks the closest unvisited node as visited and relaxes its outgoing edges. Route is reconstructed using parent pointers and verified against the EV's usable range.
- **Trace on Tiny Example:** $V=\{0, 1, 2\}$; edges $(0,1)=10, (1,2)=15, (0,2)=35$. Start=0, Dest=2.
  Relax $(0,1)\to 10$, $(0,2)\to 35$. Visit 1 (`dist=10`); relax $(1,2)\to 10+15=25 < 35$. Route: $0 \to 1 \to 2$ (25 km).
- **Complexity:** Array: Time $\Theta(V^2)$, Space $\Theta(V)$. Min-Heap: Time $O((V+E)\log V)$, Space $\Theta(V)$.
- **3 Viva Questions:**
  1. *Q:* Why does Dijkstra fail on negative edge weights?  
     *A:* Dijkstra assumes once a node is visited, its distance is finalized; a negative edge later could offer a shorter path, violating the greedy choice property.
  2. *Q:* When is the Array version faster than the Min-Heap version?  
     *A:* In dense networks where $E \approx V^2$, Array is $O(V^2)$ while Heap is $O(V^2 \log V)$. Array also has lower cache miss overhead.
  3. *Q:* How is EV battery feasibility validated?  
     *A:* Compare total route distance against vehicle usable range ($R_{\max} \times \text{SOC}/100$).
- **Modification Question:** *What if roads have elevation changes that consume extra energy uphill?*  
  *Modification:* Replace physical distance with effective energy cost $E(u, v) = \text{distance} + k \cdot \max(0, \text{elevation}(v) - \text{elevation}(u))$. Since energy remains non-negative, Dijkstra runs unchanged.

---

### Module 3: Minimum Spanning Tree (Prim vs Kruskal)
- **Idea in 3 lines:** Connects all network stations with minimum total electrical cabling cost without forming cycles. Prim grows a single tree vertex-by-vertex; Kruskal sorts all edges and merges components using Disjoint Set Union-Find.
- **Trace on Tiny Example:** Triangle graph $(0,1)=10, (1,2)=15, (0,2)=25$.
  Kruskal sorts: $10, 15, 25$. Pick $(0,1)=10$ (roots 0,1 merged). Pick $(1,2)=15$ (roots 0,2 merged). $(0,2)$ forms cycle, discarded. MST weight = 25.
- **Complexity:** Prim: $\Theta(V^2)$ with matrix. Kruskal: $\Theta(E \log E)$ sorting $+ O(E \alpha(V))$ Union-Find.
- **3 Viva Questions:**
  1. *Q:* What is the Cut Property in MST?  
     *A:* For any cut of the graph, the minimum-weight edge crossing the cut must belong to some MST.
  2. *Q:* What optimizations make Kruskal's Union-Find near linear?  
     *A:* Path compression in `find()` and union-by-rank, making operations run in $O(\alpha(V))$ (inverse Ackermann function).
  3. *Q:* Can an MST have cycles?  
     *A:* By definition, a spanning tree on $V$ vertices has exactly $V - 1$ edges and zero cycles.
- **Modification Question:** *What if a specific high-voltage cable between two substations MUST be included?*  
  *Modification:* In Kruskal's, initialize the Union-Find structure by pre-unioning that mandatory edge before sorting and processing the remaining edges.

---

### Module 4: Maximum Subarray Problem (Divide & Conquer)
- **Idea in 3 lines:** Finds the contiguous sequence of days where a charging station generated the highest net operational profit. Recursively divides the timeline in half, finding the best in the left half, right half, or crossing the midpoint.
- **Trace on Tiny Example:** `[-2, 5, -1, 7, -3]`. Midpoint divides `[-2, 5]` and `[-1, 7, -3]`. Crossing subarray expands left from mid (`5`) and right from mid (`-1 + 7 = 6`), total crossing sum = $5 + 6 = 11$. Subarray `{5, -1, 7}` gives max sum 11.
- **Complexity:** Time $\Theta(n \log n)$, Space $O(\log n)$ call stack. (Kadane's algorithm is $O(n)$).
- **3 Viva Questions:**
  1. *Q:* Why does the crossing subarray take $O(n)$ time?  
     *A:* Because it must include the midpoint, it only needs a 1D linear scan left and right from the center.
  2. *Q:* Solve the recurrence $T(n) = 2T(n/2) + \Theta(n)$.  
     *A:* By Master Theorem Case 2 ($a=2, b=2, n^{\log_2 2} = n = f(n)$), $T(n) = \Theta(n \log n)$.
  3. *Q:* What happens if all numbers are negative?  
     *A:* The algorithm returns the single least-negative element.

---

### Module 5: Fast Exponentiation vs Naive Power
- **Idea in 3 lines:** Computes $x^n$ (e.g. multi-year battery capacity degradation $(1-r)^n$) in logarithmic steps by repeated squaring: if $n$ is even, $x^n = (x^{n/2})^2$; if odd, $x \cdot (x^{(n-1)/2})^2$.
- **Complexity:** Fast Power: $\Theta(\log n)$ time, $O(1)$ space. Naive: $\Theta(n)$ time.
- **Viva Q:** *How is this used in cryptography and modular arithmetic?*  
  *A:* Modular exponentiation $(a^b \bmod m)$ enables RSA and Diffie-Hellman by keeping intermediate values small using modulo at each squaring step.

---

### Module 6: Celebrity / Hub Station Discovery
- **Idea in 3 lines:** A Hub Station is a critical junction that every other station can directly reach, but it can directly reach none. Solved in $\Theta(n)$ time by elimination: if $A$ can reach $B$, $A$ cannot be the hub; if $A$ cannot reach $B$, $B$ cannot be the hub.
- **Complexity:** Time $\Theta(n)$, Space $O(1)$ auxiliary. (Brute force is $\Theta(n^2)$).

---

### Module 7: Strassen Matrix Multiplication
- **Idea in 3 lines:** Multiplies two $n \times n$ matrices (e.g., 2-hop road reachability) using 7 recursive sub-matrix multiplications instead of 8, lowering the exponent from 3 to $\approx 2.807$.
- **Complexity:** Time $\Theta(n^{\log_2 7}) \approx \Theta(n^{2.807})$, Space $O(n^2)$.
- **Viva Q:** *Why don't we use Strassen for small matrices ($n \le 4$)?*  
  *A:* Strassen has large constant factor overhead due to 18 matrix additions and allocation; classical $O(n^3)$ is faster for small $n$ (crossover threshold).

---

### Module 8: Activity Selection & Interval Partitioning
- **Idea in 3 lines:** Activity Selection greedily chooses the maximum number of non-overlapping charging sessions on one charger by sorting by earliest finish time. Interval Partitioning sorts by start time and finds the minimum number of physical charger bays needed for all sessions.
- **Complexity:** Activity Selection: $\Theta(n \log n)$ time. Interval Partitioning: $O(n^2)$ or $O(n \log n)$ with min-heap.
- **Viva Q:** *Why sort by finish time rather than duration or start time?*  
  *A:* Earliest finish time frees the resource as early as possible, maximizing remaining time for future compatible activities (Greedy Choice Property).

---

### Module 9: Fractional Knapsack (Grid Power Allocation)
- **Idea in 3 lines:** Allocates limited grid power (kW) among vehicles by sorting by priority-per-kW ratio ($p_i / w_i$). Takes 100% of high-priority requests until capacity is nearly full, then takes a fractional slice of the next request.
- **Complexity:** Time $\Theta(n \log n)$ sorting $+ \Theta(n)$ allocation. Space $O(1)$.
- **Viva Q:** *Why does greedy give 100% optimal for fractional knapsack, but fail for 0/1 knapsack?*  
  *A:* In fractional knapsack, capacity can be completely filled without leaving unused slack space.

---

### Module 10: Huffman Coding (Log Compression)
- **Idea in 3 lines:** Compresses event log text by assigning variable-length prefix-free binary codes based on character frequency: frequent characters get short codes (e.g. `'E'` $\to$ `01`), rare characters get long codes.
- **Complexity:** Time $\Theta(n + k \log k)$, Space $O(k)$ where $k$ is alphabet size.
- **Viva Q:** *What is the prefix-free property and why is it essential?*  
  *A:* No code is a prefix of another code. This ensures unambiguous decoding without needing separator symbols.

---

### Module 11: Matrix Chain Multiplication (MCM)
- **Idea in 3 lines:** Finds the optimal parenthesization of matrix products $A_1 \times A_2 \times \dots \times A_n$ to minimize total scalar multiplications.
- **Recurrence:** $m[i][j] = \min_{i \le k < j} \{ m[i][k] + m[k+1][j] + p_{i-1} p_k p_j \}$.
- **Complexity:** Time $\Theta(n^3)$, Space $\Theta(n^2)$.

---

### Module 12: Longest Common Subsequence (LCS)
- **Idea in 3 lines:** Compares two EV routes (e.g., scheduled dispatch vs actual GPS trajectory) to find the longest sequence of junctions visited in the same relative order without requiring consecutive adjacency.
- **Complexity:** Time $\Theta(m \cdot n)$, Space $\Theta(m \cdot n)$.

---

### Module 13: Optimal Binary Search Tree (OBST)
- **Idea in 3 lines:** Arranges candidate station keys into a binary search tree to minimize expected lookup time given non-uniform access frequencies ($p_i$) and unsuccessful search frequencies ($q_i$).
- **Complexity:** Time $\Theta(n^3)$, Space $\Theta(n^2)$.

---

### Module 14: Bellman-Ford & Floyd-Warshall
- **Idea in 3 lines:** Bellman-Ford computes single-source shortest paths by relaxing all edges $V-1$ times and detects negative cycles ($O(V \cdot E)$). Floyd-Warshall computes all-pairs shortest paths in $\Theta(V^3)$ by considering each node $k$ as an intermediate vertex.
- **Viva Q:** *How does Bellman-Ford detect negative cycles?*  
  *A:* If after $V-1$ iterations any edge can still be relaxed, a negative-weight cycle exists because a simple shortest path cannot have more than $V-1$ edges.

---

### Module 15: Travelling Salesperson Problem (TSP Bitmask DP)
- **Idea in 3 lines:** Finds the minimum-cost closed inspection tour visiting $N$ stations ($N \le 10$). Uses Held-Karp dynamic programming with bitmask states: `dp[mask][i]` represents the shortest path visiting all vertices in `mask` ending at vertex `i`.
- **Complexity:** Time $\Theta(n^2 2^n)$, Space $\Theta(n 2^n)$. (Compared to brute force $O(n!)$).

---

### Module 16: N-Queens & Sum of Subsets (Backtracking)
- **Idea in 3 lines:** N-Queens places $N$ non-attacking queens on an $N \times N$ board using depth-first search with conflict pruning. Sum of Subsets finds exact subsets of station costs summing to budget $B$, pruning branches when the current sum exceeds $B$ or remaining sum is insufficient.
- **Complexity:** N-Queens: $O(N!)$ worst case. Sum of Subsets: $O(2^n)$ worst case.

---

### Module 17: Branch & Bound (FIFO vs LIFO vs Least-Cost)
- **Idea in 3 lines:** Solves 0/1 Knapsack by exploring a state space tree. Computes a fractional upper bound at each node; if a node's bound is $\le$ the best solution found so far, the entire subtree is pruned.
- **Comparison:**
  - **FIFO (Queue):** Breadth-first exploration; finds bounds level by level.
  - **LIFO (Stack):** Depth-first exploration; reaches leaves quickly to establish an initial lower bound.
  - **Least-Cost (Best-First):** Always expands the active node with the highest upper bound; explores the **fewest** nodes.

---

### Module 18: Ford-Fulkerson Max Flow & Min-Cut
- **Idea in 3 lines:** Determines the maximum power (kW) that can flow from an electrical substation to an EV charging hub through a transmission grid. Repeatedly finds augmenting paths in the residual graph using DFS until no augmenting path exists.
- **Min-Cut:** The bottleneck transmission lines are those from the reachable set $S$ to unreachable set $T$. Max flow value = Min cut capacity.
- **Complexity:** Time $O(E \cdot |f_{\max}|)$, Space $O(V^2)$.

---

### Module 19: Maximum Bipartite Matching
- **Idea in 3 lines:** Assigns EV fleet vehicles to compatible charging bays such that no two vehicles share a bay and the total number of simultaneous charges is maximized. Solved using augmenting paths (DFS).
- **Complexity:** Time $O(V \cdot E)$, Space $O(V)$.

---

### Module 20: Chinese Remainder Theorem (CRT)
- **Idea in 3 lines:** Reconstructs a total distributed energy counter $x$ from remainder readings across partitioned sub-meters counting modulo pairwise-coprime integers $m_1, m_2, \dots, m_k$. Uses the Extended Euclidean algorithm to compute modular inverses.
- **Complexity:** Time $\Theta(k \log M)$, Space $O(1)$.

---

### Module 21: String Matching (Naive, Rabin-Karp, DFA, KMP)
- **Comparison Table:**
  | Algorithm | Preprocessing | Matching Time | Worst-Case Text Example |
  | :--- | :--- | :--- | :--- |
  | **Naive** | $0$ | $O((n-m+1)m)$ | `aaaa...a` with pattern `aaab` |
  | **Rabin-Karp** | $O(m)$ | Avg $O(n+m)$, Worst $O(nm)$ | Many hash collisions |
  | **Finite Automaton**| $O(m^3 |\Sigma|)$| $\Theta(n)$ | None (strictly linear scan) |
  | **KMP** | $\Theta(m)$ | $\Theta(n)$ | None (linear via LPS array) |

---

## 2. Top 15 Master Viva Questions & Model Answers

1. **Q: What is the core difference between Divide & Conquer and Dynamic Programming?**  
   *Answer:* Divide & Conquer solves subproblems independently and combines them (e.g., Merge Sort); if subproblems overlap, it re-computes them redundantly. Dynamic Programming is used when subproblems overlap; it computes each subproblem once, memoizes or tabulates it, and reuses the stored result.

2. **Q: Why does the 0/1 Knapsack problem have a pseudo-polynomial time algorithm, and does that mean P = NP?**  
   *Answer:* No, P does not equal NP. The DP algorithm runs in $\Theta(n \cdot B)$ time, where $B$ is the numeric value of the budget. However, the input size of $B$ in binary is $k = \lceil \log_2 B \rceil$ bits. Thus the time complexity is $\Theta(n \cdot 2^k)$, which is exponential in the input size.

3. **Q: In Quick Sort, how can you prevent the $O(n^2)$ worst-case behavior on already-sorted input?**  
   *Answer:* Instead of using the last element as pivot (Lomuto), we can use Randomized Quick Sort (picking a random pivot) or Median-of-Three partitioning (median of first, middle, last), which yields expected $\Theta(n \log n)$ time on all inputs.

4. **Q: Explain the Cut Property in Minimum Spanning Trees.**  
   *Answer:* For any partition of graph vertices into two non-empty sets $S$ and $V \setminus S$, the lightest (minimum weight) edge crossing between $S$ and $V \setminus S$ is guaranteed to belong to some Minimum Spanning Tree of the graph.

5. **Q: How does Dijkstra's algorithm guarantee that visiting a vertex finalizes its shortest path?**  
   *Answer:* By the greedy choice property and non-negative edge weights. Because edge weights are $\ge 0$, any alternative path from the source to the current closest unvisited vertex $u$ would have to pass through another unvisited vertex $v$. Since $\text{dist}[v] \ge \text{dist}[u]$, adding non-negative edges cannot produce a shorter path.

6. **Q: How does Bellman-Ford detect negative weight cycles?**  
   *Answer:* Any simple shortest path has at most $V - 1$ edges. Bellman-Ford relaxes all edges $V - 1$ times. If on the $V$-th pass any distance can still be relaxed ($\text{dist}[u] + w < \text{dist}[v]$), a negative cycle must exist.

7. **Q: What is the Max-Flow Min-Cut Theorem?**  
   *Answer:* The maximum amount of flow passing from source $s$ to sink $t$ in a capacitated network equals the minimum total capacity of edges in a cut $(S, T)$ separating $s$ and $t$.

8. **Q: Why is KMP faster than Naive string matching on adversarial texts?**  
   *Answer:* When a mismatch occurs, Naive shifts the pattern by 1 and re-compares from the start. KMP uses the precomputed $\pi$ (LPS) array to know the longest proper prefix that is also a suffix, sliding the pattern forward without ever backtracking the text index $i$.

9. **Q: What is the principle of optimality in Dynamic Programming?**  
   *Answer:* An optimal solution to a problem contains within it optimal solutions to its subproblems. If subproblems could be solved better, the overall solution could be improved, contradicting optimality.

10. **Q: What is the difference between FIFO, LIFO, and Least-Cost Branch and Bound?**  
    *Answer:* FIFO uses a queue and explores breadth-first. LIFO uses a stack and explores depth-first. Least-Cost (Best-First) uses a priority queue to always expand the node with the most promising bound, exploring the fewest total nodes.

11. **Q: Explain the polynomial reduction from Subset-Sum to 0/1 Knapsack Decision.**  
    *Answer:* Given a set $S$ and target $T$, create a candidate station for each $s_i \in S$ with cost $c_i = s_i$ and benefit $b_i = s_i$. Set budget $B = T$ and threshold $K = T$. Because cost equals benefit, a solution with cost $\le T$ and benefit $\ge T$ exists if and only if a subset sums exactly to $T$.

12. **Q: What is the difference between NP, NP-Complete, and NP-Hard?**  
    *Answer:* NP is the class of decision problems verifiable in polynomial time. NP-Complete problems are in NP and are as hard as any problem in NP (any problem in NP reduces to them in polynomial time). NP-Hard problems are at least as hard as NP-Complete problems, but need not be in NP (e.g. optimization problems).

13. **Q: Why does Huffman coding produce an optimal prefix code?**  
    *Answer:* Huffman coding repeatedly merges the two least-frequent symbols, ensuring that less frequent characters are placed deeper in the tree (longer codes) and frequent characters are placed near the root (shorter codes), minimizing the weighted path length $\sum f_i \cdot l_i$.

14. **Q: In Matrix Chain Multiplication, why can't we simply multiply from left to right?**  
    *Answer:* Matrix multiplication is associative ($(AB)C = A(BC)$), but the number of scalar multiplications depends drastically on the order. For dimensions $10 \times 100 \times 5 \times 50$, $(A_1 A_2) A_3$ takes 7,500 operations, while $A_1 (A_2 A_3)$ takes 75,000 operations (a $10\times$ difference!).

15. **Q: What conditions are required for the Chinese Remainder Theorem to hold?**  
    *Answer:* The moduli $m_1, m_2, \dots, m_k$ must be pairwise coprime (i.e. $\gcd(m_i, m_j) = 1$ for all $i \ne j$).
