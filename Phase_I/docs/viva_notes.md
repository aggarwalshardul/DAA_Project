# Viva Preparation Notes & Examiner Q&A Guide (Phase I)

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. 0/1 Knapsack Dynamic Programming (Station Placement)

### Idea in 3 Lines
1. We want to pick a subset of candidate charging station sites that gives maximum total public benefit without exceeding our capital budget $B$.
2. Because each station can either be installed or not (0 or 1), fractional installation is impossible.
3. We build a 2D table where subproblem `dp[i][w]` stores the best benefit possible using a subset of the first $i$ stations with a budget limit of $w$.

### Hand Trace on a Tiny Example
- **Budget $B = 5$**
- **Candidate Sites:**
  - $S_1$: Cost = 2, Benefit = 3
  - $S_2$: Cost = 3, Benefit = 4
  - $S_3$: Cost = 4, Benefit = 5
- **DP Table Construction:**
  - $i=0$: `[0, 0, 0, 0, 0, 0]`
  - $i=1$ ($c=2, v=3$):
    - $w=0,1$: `dp[0][w] = 0`
    - $w=2..5$: `max(dp[0][w], dp[0][w-2]+3) = 3` $\implies$ `[0, 0, 3, 3, 3, 3]`
  - $i=2$ ($c=3, v=4$):
    - $w=0,1$: `0`
    - $w=2$: `3`
    - $w=3$: `max(3, 0+4) = 4`
    - $w=4$: `max(3, 0+4) = 4`
    - $w=5$: `max(3, dp[1][5-3]+4 = 3+4) = 7` $\implies$ `[0, 0, 3, 4, 4, 7]`
  - $i=3$ ($c=4, v=5$):
    - $w=5$: `max(7, dp[2][5-4]+5 = 0+5=5) = 7`
- **Result:** Max benefit = `7` (Stations $S_1 + S_2$, total cost $2+3=5 \le 5$).

### Complexity
- **Time:** $\Theta(m \cdot B)$ (fills table of $(m+1) \times (B+1)$ cells).
- **Space:** $\Theta(m \cdot B)$ auxiliary memory.

### 3 Common Viva Questions & Answers
- **Q1: Why is 0/1 knapsack solved with DP rather than greedy?**
  *Answer:* Because the 0/1 knapsack problem does not satisfy the greedy choice property. Taking an item with the highest benefit/cost ratio can consume capacity that prevents picking two smaller items whose combined value exceeds the greedy item.
- **Q2: Why is the running time $\Theta(m \cdot B)$ called pseudo-polynomial?**
  *Answer:* Because the input size of an integer $B$ is proportional to the number of bits needed to represent it, $\log_2 B$. Thus the running time is $O(m \cdot 2^k)$ where $k$ is the number of bits, which is exponential in terms of input length.
- **Q3: How do you know which specific stations were chosen?**
  *Answer:* By backtracking through the table from `dp[m][B]`. If `dp[i][w] != dp[i-1][w]`, item $i$ was included, so we record its ID and subtract its cost from $w$ (`w = w - cost[i]`), continuing up to $i=1$.

### Modification Example for Viva
- **Examiner asks:** *"What if each station can be installed up to twice (e.g. Phase 1 and Phase 2 expansion)?"*
  *Modification:* This becomes the **Bounded Knapsack Problem**. We can either transform each station into two separate items ($S_{i,1}$ and $S_{i,2}$) or change the recurrence loop to test $k \in \{0, 1, 2\}$ copies:
  `dp[i][w] = max { dp[i-1][w - k * cost[i]] + k * benefit[i] }` for $0 \le k \le 2$ such that $k \cdot \text{cost}[i] \le w$.

---

## 2. Greedy Knapsack Heuristic & Counterexample

### Idea in 3 Lines
1. Sort candidate stations in descending order of benefit-to-cost ratio ($b_i / c_i$).
2. Iteratively select stations that fit within the remaining budget.
3. Fast $O(m \log m)$ heuristic, but does not guarantee the optimal combination for discrete 0/1 choices.

### Hand Trace on Counterexample
- **Budget $B = 50$**
  - Site A: Cost 30, Benefit 33 $\implies$ Ratio = $1.10$
  - Site B: Cost 25, Benefit 25 $\implies$ Ratio = $1.00$
  - Site C: Cost 25, Benefit 25 $\implies$ Ratio = $1.00$
- **Greedy Execution:** Picks Site A (ratio 1.10). Remaining budget = $50 - 30 = 20$. Neither Site B (cost 25) nor Site C (cost 25) can fit! Greedy Total Benefit = **33**.
- **Optimal DP Execution:** Picks Site B + Site C (total cost $25+25 = 50 \le 50$). Total Benefit = **50**.
- **Conclusion:** DP beats Greedy ($50 > 33$), proving greedy choice failure!

---

## 3. Dijkstra's Shortest Path Algorithm

### Idea in 3 Lines
1. Finds the minimum total driving distance from a start junction to all other junctions on a non-negative road network.
2. Maintains tentative shortest distances, greedily marks the closest unvisited junction as visited, and relaxes all outgoing edges.
3. Path is reconstructed using parent pointers, then checked against the vehicle's remaining battery range.

### Hand Trace on a Tiny Example
- **Graph:** $V = \{0, 1, 2\}$, Edges: $(0, 1)=10$, $(1, 2)=15$, $(0, 2)=35$. Source = 0, Dest = 2.
- **Initial:** `dist = [0, INF, INF]`, `visited = [F, F, F]`, `parent = [-1, -1, -1]`.
- **Step 1:** Min unvisited is node 0 (`dist=0`). Visit 0.
  - Relax $(0, 1)$: `dist[1] = min(INF, 0 + 10) = 10`, `parent[1] = 0`.
  - Relax $(0, 2)$: `dist[2] = min(INF, 0 + 35) = 35`, `parent[2] = 0`.
- **Step 2:** Min unvisited is node 1 (`dist=10`). Visit 1.
  - Relax $(1, 2)$: `dist[2] = min(35, 10 + 15) = 25 < 35` $\implies$ `dist[2] = 25`, `parent[2] = 1`.
- **Step 3:** Min unvisited is node 2 (`dist=25`). Dest reached!
- **Path Reconstruction:** From dest 2: `2 <- parent[2]=1 <- parent[1]=0` $\implies$ Route: `0 -> 1 -> 2`, Distance = **25 km** (better than direct 35 km).

### Complexity
- **Array-based:** Time $\Theta(V^2)$, Space $\Theta(V)$.
- **Min-Heap based:** Time $O((V + E) \log V)$, Space $\Theta(V)$.

### 3 Common Viva Questions & Answers
- **Q1: Can Dijkstra handle negative road lengths?**
  *Answer:* No. Dijkstra assumes that once a vertex is marked visited, its distance is final because adding further non-negative edges can only increase path length. Negative edge weights violate this greedy choice property; Bellman-Ford must be used instead.
- **Q2: When should we use the Array version vs the Min-Heap version?**
  *Answer:* In dense networks where $E \approx V^2$, the Array version runs in $O(V^2)$ while the Min-Heap runs in $O(V^2 \log V)$; the Array version is faster and uses less memory. In sparse networks where $E \ll V^2$, the Min-Heap version runs in $O(E \log V)$, which is significantly faster than $O(V^2)$.
- **Q3: How do you verify EV battery range feasibility?**
  *Answer:* We compute the total route distance $D = \text{dist}[\text{dest}]$. We compare $D$ against the vehicle's effective usable range $R_{\text{usable}} = R_{\max} \times (\text{SOC} / 100)$. If $D \le R_{\text{usable}}$, the trip is feasible; otherwise, an intermediate recharge stop is required.

### Modification Example for Viva
- **Examiner asks:** *"What if road tolls also need to be paid in addition to distance?"*
  *Modification:* We formulate a multi-objective cost function $w(u, v) = \alpha \cdot \text{distance}(u, v) + \beta \cdot \text{toll}(u, v)$ where $\alpha, \beta \ge 0$ are weighting factors. Because the composite weights remain non-negative, Dijkstra's algorithm still applies without modification to the core loop.

---

## 4. Sorting & Searching Modules

### Merge Sort vs Quick Sort Viva Comparison
| Feature | Merge Sort | Quick Sort (Lomuto) |
| :--- | :--- | :--- |
| **Strategy** | Divide at fixed midpoint | Divide at pivot position |
| **Work location** | Work done in COMBINE step (merge) | Work done in DIVIDE step (partition) |
| **Worst-case Time**| $\Theta(n \log n)$ guaranteed | $\Theta(n^2)$ on already-sorted input |
| **Auxiliary Space**| $\Theta(n)$ temporary buffer | $O(\log n)$ recursion stack (in-place) |
| **Stability** | Stable | Unstable (with Lomuto swap) |

### Binary Search (Iterative vs Recursive)
- **Recurrence for Recursive:** $T(n) = T(n/2) + O(1) \implies \Theta(\log n)$ by Master Theorem Case 2 ($a=1, b=2, d=0 \implies n^{\log_2 1} = n^0 = 1 = n^d$).
- **Space Difference:** Iterative uses $O(1)$ constant memory; recursive uses $O(\log n)$ stack frames on the call stack.
