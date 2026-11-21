# Algorithmic Complexity Report (Phase I)

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. Summary Complexity Matrix

| Algorithm | Problem Mapped | Paradigm | Best Time | Average Time | Worst Time | Auxiliary Space |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **0/1 Knapsack DP** | Station Placement | Dynamic Programming | $\Omega(m \cdot B)$ | $\Theta(m \cdot B)$ | $O(m \cdot B)$ | $\Theta(m \cdot B)$ table |
| **Greedy Ratio Knapsack**| Heuristic Placement | Greedy Heuristic | $\Omega(m \log m)$ | $\Theta(m \log m)$ | $O(m \log m)$ | $O(m)$ scratch |
| **Dijkstra (Array)** | Shortest Feasible Route | Greedy | $\Omega(V^2)$ | $\Theta(V^2)$ | $O(V^2)$ | $\Theta(V)$ arrays |
| **Dijkstra (Min-Heap)**| Highway Sparse Routing | Greedy | $\Omega(V \log V)$ | $\Theta((V+E)\log V)$| $O((V+E)\log V)$| $\Theta(V)$ heap |
| **Merge Sort** | Station Ranking by Cost | Divide & Conquer | $\Omega(n \log n)$ | $\Theta(n \log n)$ | $O(n \log n)$ | $\Theta(n)$ temp array|
| **Quick Sort (Lomuto)** | Sorting by Benefit/ID | Divide & Conquer | $\Omega(n \log n)$ | $\Theta(n \log n)$ | $O(n^2)$ (sorted) | $O(\log n)$ call stack |
| **Binary Search (Iterative)**| Station ID Lookup | Prune & Search | $\Omega(1)$ | $\Theta(\log n)$ | $O(\log n)$ | $\Theta(1)$ |
| **Binary Search (Recursive)**| Station ID Lookup | Prune & Search | $\Omega(1)$ | $\Theta(\log n)$ | $O(\log n)$ | $O(\log n)$ call stack |

*Notation:*  
$m$ = Number of candidate stations ($m \le 50$)  
$B$ = Capital installation budget ($B \le 1000$)  
$V$ = Number of road network junctions ($V \le 100$)  
$E$ = Number of physical road segments ($E \le V^2$)  
$n$ = Number of elements in array to sort/search  

---

## 2. In-Depth Asymptotic Analysis & Proofs

### 2.1 0/1 Knapsack Dynamic Programming
- **Time Complexity:**
  - The algorithm fills a 2D table of dimension $(m + 1) \times (B + 1)$.
  - Each cell `dp[i][w]` is computed via exactly one comparison and an addition: $O(1)$ constant operations.
  - Total time is $\Theta(m \cdot B)$.
  - *Note:* This is **pseudo-polynomial**, not polynomial, because it is polynomial in the magnitude of the integer $B$, but exponential in the length of the binary representation of $B$ ($\log B$ bits).
- **Space Complexity:**
  - A matrix of $(m + 1) \times (B + 1)$ integers is allocated. For $m=50, B=1000$, auxiliary memory is $51 \times 1001 \times 4 \text{ bytes} \approx 204 \text{ KB}$, well within embedded and offline microcontroller/desktop limits.
- **Invariants & Edge Cases:**
  - **Edge Case 1:** $B = 0$ or $m = 0 \implies$ returns 0 benefit cleanly.
  - **Edge Case 2:** Candidate item with $c_i > B \implies$ cannot be selected; `dp[i][w] = dp[i-1][w]`.

### 2.2 Dijkstra's Algorithm: Array vs Min-Heap
- **Array-based Dijkstra:**
  - Finding the minimum tentative distance across all unvisited vertices takes $\Theta(V)$ time using linear scan.
  - Done $V$ times: $V \times \Theta(V) = \Theta(V^2)$.
  - Relaxation of neighbors takes $\sum_{v} \text{deg}(v) = 2E = O(V^2)$.
  - Total Time: $\Theta(V^2)$.
  - Optimal when graph is dense ($E \approx V^2$).
- **Min-Heap Dijkstra:**
  - Extract-min takes $O(\log V)$, done at most $V$ times $\implies O(V \log V)$.
  - Edge relaxations and heap insertions take $O(E \log V)$.
  - Total Time: $O((V + E) \log V)$.
  - Optimal when graph is sparse ($E \ll V^2$).

### 2.3 Quick Sort Adversarial Behavior
- When using Lomuto partitioning with the last element as pivot:
  - If the input array is already sorted or reverse sorted:
    - Partition splits the array into sizes $0$ and $n - 1$.
    - Recurrence: $T(n) = T(n - 1) + T(0) + \Theta(n) = T(n - 1) + \Theta(n)$.
    - Expanding: $T(n) = c \sum_{i=1}^n i = \Theta(n^2)$.
- Contrast with Merge Sort, which splits strictly into $n/2$ and $n/2$ regardless of input order, guaranteeing $\Theta(n \log n)$ performance even on adversarial inputs.
