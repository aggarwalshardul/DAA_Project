# Algorithmic Correctness Notes & Invariant Proofs

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. Dijkstra's Algorithm Correctness & Invariant

### 1.1 Invariant Statement
At the start of each iteration of the outer loop:
- For every vertex $u \in \text{Visited}$, $\text{dist}[u]$ is the exact shortest-path weight from source $s$ to $u$: $\text{dist}[u] = \delta(s, u)$.
- For every vertex $v \notin \text{Visited}$, $\text{dist}[v]$ is the length of the shortest path from $s$ to $v$ that passes **only** through vertices in $\text{Visited}$.

### 1.2 Maintenance (Inductive Step)
When the algorithm selects an unvisited vertex $u$ with minimum tentative distance $\text{dist}[u]$:
- Suppose there exists an alternative, strictly shorter path $P$ from $s$ to $u$.
- Since $s \in \text{Visited}$ and $u \notin \text{Visited}$, the path $P$ must leave $\text{Visited}$ at some edge $(x, y)$ where $x \in \text{Visited}$ and $y \notin \text{Visited}$.
- By the non-negativity of edge weights ($w(e) \ge 0$):
  $$\text{length}(P) \ge \text{dist}[x] + w(x, y) \ge \text{dist}[y]$$
- But since the algorithm chose $u$ with minimum tentative distance among unvisited vertices, $\text{dist}[u] \le \text{dist}[y]$.
- Therefore:
  $$\text{length}(P) \ge \text{dist}[y] \ge \text{dist}[u]$$
- This contradicts the assumption that $\text{length}(P) < \text{dist}[u]$. Hence, $\text{dist}[u] = \delta(s, u)$.

---

## 2. 0/1 Knapsack Dynamic Programming: Optimal Substructure

### 2.1 Theorem Statement
Let $S_k \subseteq \{1, \dots, k\}$ be an optimal subset of stations for budget $w$.
1. If item $k \notin S_k$, then $S_k$ is an optimal subset of $\{1, \dots, k-1\}$ with budget $w$.
2. If item $k \in S_k$, then $S_k \setminus \{k\}$ is an optimal subset of $\{1, \dots, k-1\}$ with budget $w - c_k$.

### 2.2 Proof (Cut-and-Paste)
Suppose item $k \in S_k$, but $S' = S_k \setminus \{k\}$ is NOT optimal for budget $w - c_k$ among items $\{1, \dots, k-1\}$.
- Then there exists another valid subset $S^*$ of $\{1, \dots, k-1\}$ with cost $\le w - c_k$ such that $\text{benefit}(S^*) > \text{benefit}(S')$.
- Then the set $S^* \cup \{k\}$ has cost $\le (w - c_k) + c_k = w$.
- Its total benefit would be:
  $$\text{benefit}(S^* \cup \{k\}) = \text{benefit}(S^*) + b_k > \text{benefit}(S') + b_k = \text{benefit}(S_k)$$
- This contradicts the optimality of $S_k$ for budget $w$. Thus, the subproblem must be optimal.

---

## 3. Minimum Spanning Tree: Kruskal & Prim Cut Property

### 3.1 The Cut Property
Let $G = (V, E)$ be a connected, undirected graph with real edge weights. Let $(S, V \setminus S)$ be any cut of $G$ (a partition of $V$).
If an edge $e = (u, v)$ is a light edge crossing the cut $(S, V \setminus S)$ (an edge with minimum weight among all edges with one endpoint in $S$ and one in $V \setminus S$), then $e$ belongs to some Minimum Spanning Tree of $G$.

### 3.2 Correctness of Prim's Algorithm
- **Invariant:** At each step, the set of edges chosen forms a subtree $T$ of some MST of $G$.
- **Step:** Prim's algorithm maintains $S = \text{vertices in } T$. It greedily adds the minimum-weight edge connecting $S$ to $V \setminus S$. By the Cut Property, this light edge preserves the invariant.

### 3.3 Correctness of Kruskal's Algorithm
- Kruskal's algorithm inspects edges in non-decreasing order of weight.
- When inspecting edge $(u, v)$, if $u$ and $v$ belong to different connected components, adding $(u, v)$ crosses the cut between $u$'s component and the rest of the graph, and is the lightest available edge. By the Cut Property, it is safe to include.

---

## 4. Ford-Fulkerson Max-Flow Min-Cut Theorem

### 4.1 Theorem Statement
In any flow network $G = (V, E)$ with source $s$ and sink $t$, the following conditions are equivalent:
1. $f$ is a maximum flow in $G$.
2. The residual network $G_f$ contains no augmenting paths from $s$ to $t$.
3. The value of flow $|f|$ equals the capacity of some cut $(S, T)$ in $G$: $|f| = c(S, T)$.

### 4.2 Termination & Correctness
When the DFS in `ford_fulkerson_max_flow` finds no path from $s$ to $t$ in $G_f$:
- Let $S$ be the set of vertices reachable from $s$ in $G_f$, and $T = V \setminus S$.
- Clearly $s \in S$ and $t \in T$ (otherwise $t$ would be reachable, contradicting termination).
- For any edge $(u, v)$ with $u \in S, v \in T$, we must have $c_f(u, v) = 0$ (otherwise $v$ would be reachable in $G_f$).
- This means $f(u, v) = c(u, v)$ (the edge is fully saturated).
- By the conservation of flow, the net flow across $(S, T)$ equals $|f|$. Since $|f| = c(S, T)$, no larger flow can exist.

---

## 5. Maximum Bipartite Matching: Augmenting Paths

### 5.1 Berge's Lemma
A matching $M$ in a graph $G$ is maximum if and only if $G$ contains no $M$-augmenting paths.

### 5.2 Correctness of DFS Augmentation
- An augmenting path is an alternating path that starts and ends at unassigned/unmatched vertices.
- When an augmenting path $P$ is found, swapping matched and unmatched edges along $P$ increases the total size of the matching by exactly 1:
  $$|M \oplus P| = |M| + 1$$
- Since our DFS checks every unassigned vehicle and stops only when no further augmenting paths exist, Berge's Lemma guarantees that the resulting matching is maximum.
