# Complexity Classes and Reducibility Analysis

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. Complexity Classification of Project Problems

In computational complexity theory, decision problems (problems with a YES/NO answer) are classified into complexity classes according to the resources required to solve or verify them.

| Problem Name | Formulation in EV Domain | Formal Complexity Class | Best Known Exact Algorithm |
| :--- | :--- | :--- | :--- |
| **Shortest Path Decision** | Is there a route from $u$ to $v$ with distance $\le K$? | **P** (Polynomial Time) | Dijkstra $O(E + V \log V)$, Bellman-Ford $O(V E)$ |
| **Minimum Spanning Tree** | Can all stations be cabled with total cost $\le W$? | **P** (Polynomial Time) | Kruskal $O(E \log E)$, Prim $O(V^2)$ |
| **Max Flow Decision** | Can power grid transmit $\ge F$ kW from substation to hub? | **P** (Polynomial Time) | Edmonds-Karp $O(V E^2)$, Ford-Fulkerson |
| **Bipartite Matching** | Can $\ge M$ vehicles be paired with distinct charging bays? | **P** (Polynomial Time) | Hopcroft-Karp $O(E \sqrt{V})$, Ford-Fulkerson |
| **0/1 Knapsack Decision** | Can stations be chosen with cost $\le B$ and benefit $\ge K$?| **NP-Complete** | Dynamic Programming $\Theta(n \cdot B)$ (Pseudo-polynomial)|
| **Station Placement (Opt)** | Maximize benefit subject to budget $B$ | **NP-Hard** | DP, Branch and Bound, Backtracking |
| **TSP Decision** | Is there an inspection tour visiting $N$ stations with cost $\le D$?| **NP-Complete** | Held-Karp Bitmask DP $\Theta(n^2 2^n)$ |
| **TSP Optimization** | Find shortest closed tour visiting all $N$ stations | **NP-Hard** | Dynamic Programming, Branch and Bound |
| **Subset-Sum Decision** | Is there a subset of candidate station costs summing to $B$?| **NP-Complete** | Backtracking, Dynamic Programming $\Theta(n \cdot B)$ |

---

## 2. Polynomial-Time Many-One Reduction: Subset-Sum $\le_P$ 0/1 Knapsack Decision

To establish that the decision version of the EV Station Placement problem is **NP-Complete**, we demonstrate a polynomial-time reduction from the known NP-Complete problem **SUBSET-SUM**.

### 2.1 Problem Definitions
1. **SUBSET-SUM Instance $\langle S, T \rangle$:**
   - Input: A finite set of positive integers $S = \{s_1, s_2, \dots, s_n\}$ and a target integer $T \in \mathbb{Z}^+$.
   - Question: Does there exist a subset $S' \subseteq S$ such that $\sum_{s_i \in S'} s_i = T$?

2. **0/1 KNAPSACK-DECISION Instance $\langle \mathcal{I}, B, K \rangle$:**
   - Input: A set of candidate stations $\mathcal{I} = \{1, 2, \dots, n\}$, each with installation cost $c_i \in \mathbb{Z}^+$ and benefit score $b_i \in \mathbb{Z}^+$, a capital budget $B \in \mathbb{Z}^+$, and a target benefit threshold $K \in \mathbb{Z}^+$.
   - Question: Does there exist a subset $\mathcal{I}' \subseteq \mathcal{I}$ such that:
     $$\sum_{i \in \mathcal{I}'} c_i \le B \quad \text{and} \quad \sum_{i \in \mathcal{I}'} b_i \ge K \text{ ?}$$

---

### 2.2 Reduction Construction ($f: \text{SUBSET-SUM} \to \text{KNAPSACK-DECISION}$)
Given an arbitrary instance of SUBSET-SUM $\langle S, T \rangle$:
1. For each element $s_i \in S$, construct a candidate charging station $i$ with:
   - Installation cost: $c_i = s_i$
   - Benefit score: $b_i = s_i$
2. Set the Knapsack capital budget: $B = T$.
3. Set the target benefit threshold: $K = T$.

**Reduction Complexity:**  
Constructing $n$ candidate items with $c_i = b_i = s_i$ takes $O(n)$ time and $O(n)$ space. Hence, the transformation runs in linear polynomial time.

---

### 2.3 Proof of Equivalence (Correctness)

We prove that $\langle S, T \rangle$ is a YES-instance for SUBSET-SUM $\iff \langle \mathcal{I}, B, K \rangle$ is a YES-instance for KNAPSACK-DECISION.

#### Direction 1 ($\implies$):
Assume $\langle S, T \rangle$ is a YES-instance for SUBSET-SUM.
- Then there exists a subset $S' \subseteq S$ such that $\sum_{s_i \in S'} s_i = T$.
- Select the corresponding stations $\mathcal{I}' = \{i : s_i \in S'\}$.
- Total installation cost:
  $$\sum_{i \in \mathcal{I}'} c_i = \sum_{s_i \in S'} s_i = T = B \le B$$
- Total benefit score:
  $$\sum_{i \in \mathcal{I}'} b_i = \sum_{s_i \in S'} s_i = T = K \ge K$$
- Thus, $\mathcal{I}'$ satisfies both cost and benefit constraints $\implies$ KNAPSACK-DECISION answers **YES**.

#### Direction 2 ($\impliedby$):
Assume $\langle \mathcal{I}, B, K \rangle$ is a YES-instance for KNAPSACK-DECISION.
- Then there exists a subset of stations $\mathcal{I}' \subseteq \mathcal{I}$ such that:
  $$\sum_{i \in \mathcal{I}'} c_i \le B = T \quad \text{and} \quad \sum_{i \in \mathcal{I}'} b_i \ge K = T$$
- By our reduction mapping, $c_i = b_i = s_i$ for all $i$. Therefore:
  $$\sum_{i \in \mathcal{I}'} c_i = \sum_{i \in \mathcal{I}'} s_i = \sum_{i \in \mathcal{I}'} b_i$$
- This implies:
  $$T \le \sum_{i \in \mathcal{I}'} b_i = \sum_{i \in \mathcal{I}'} s_i = \sum_{i \in \mathcal{I}'} c_i \le T$$
- The only way a value can be both $\ge T$ and $\le T$ is if it equals $T$ exactly:
  $$\sum_{i \in \mathcal{I}'} s_i = T$$
- Thus, the corresponding set $S' = \{s_i : i \in \mathcal{I}'\}$ sums exactly to $T \implies$ SUBSET-SUM answers **YES**.

---

### 2.4 Membership in NP
To complete the proof of NP-completeness, we verify that 0/1 Knapsack Decision $\in \text{NP}$:
- **Certificate:** A subset of candidate station indices $\mathcal{I}' \subseteq \{1, \dots, n\}$.
- **Verifier:** Compute $C = \sum_{i \in \mathcal{I}'} c_i$ and $B_{\text{tot}} = \sum_{i \in \mathcal{I}'} b_i$ in $O(n)$ time. Check if $C \le B$ and $B_{\text{tot}} \ge K$.
- Since polynomial verification is possible, 0/1 Knapsack Decision $\in \text{NP}$.

**Conclusion:** Because 0/1 Knapsack Decision $\in \text{NP}$ and SUBSET-SUM $\le_P$ 0/1 Knapsack Decision, the decision version of the EV station placement problem is **NP-Complete**.

---

## 3. Why the Station Placement Problem is Hard in Practice

Although Dynamic Programming solves 0/1 Knapsack in $\Theta(n \cdot B)$ time, this is **pseudo-polynomial**:
1. When capital budgets are measured in exact currency units (e.g., $\$5,000,000$), $B$ is represented by $k = \log_2 B \approx 23$ bits. The DP table requires $n \cdot 2^k$ entries, rendering the memory and time exponential in terms of input length.
2. In real-world urban networks, station placement involves additional constraints such as maximum grid transformer capacity, spatial distance thresholds between neighboring stations, and multi-vehicle queuing, elevating the placement problem to the **Capacitated Facility Location Problem** or **Set Covering Problem**, which are strongly NP-hard.
