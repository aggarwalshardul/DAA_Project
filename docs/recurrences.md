# Recurrence Relations Formulation and Solutions

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

---

## 1. Overview of Recurrence Relations in the Project

The project implements several recursive and divide-and-conquer modules. This document presents formal mathematical formulations, solving methods (Iteration, Substitution, Recursion Trees, and Master Theorem), and documents cases where the Master Theorem does **not** apply.

---

## 2. Detailed Recurrence Formulations & Solutions

### 2.1 Merge Sort (Station Ranking by Cost / Benefit)
- **Recurrence Formulation:**
  The array of $n$ elements is divided into two equal halves of size $n/2$. Each half is recursively sorted, and the two sorted subarrays are merged in linear time:
  $$T(n) = \begin{cases} \Theta(1) & \text{if } n = 1 \\ 2T(n/2) + \Theta(n) & \text{if } n > 1 \end{cases}$$

- **Solution via Master Theorem:**
  - Standard form: $T(n) = a T(n/b) + f(n)$ with $a = 2, b = 2, f(n) = \Theta(n)$.
  - $n^{\log_b a} = n^{\log_2 2} = n^1 = n$.
  - Comparing $f(n) = \Theta(n)$ with $n^{\log_b a} = \Theta(n) \implies f(n) = \Theta(n^{\log_b a})$.
  - **Master Theorem Case 2 applies:**
    $$T(n) = \Theta(n^{\log_b a} \log n) = \Theta(n \log n)$$

- **Verification via Recursion Tree:**
  - Level $i$ has $2^i$ subproblems, each of size $n / 2^i$.
  - Work at level $i$: $2^i \times c(n / 2^i) = c \cdot n$.
  - Total depth: $\log_2 n$.
  - Total Work: $\sum_{i=0}^{\log_2 n} c \cdot n = c \cdot n (\log_2 n + 1) = \Theta(n \log n)$.

---

### 2.2 Quick Sort (Best vs Worst Case)
- **Best / Balanced Case Recurrence:**
  Pivot divides array exactly into two halves:
  $$T(n) = 2T(n/2) + \Theta(n) \implies \Theta(n \log n) \quad \text{(Master Theorem Case 2)}$$

- **Worst Case (Adversarial / Already-Sorted with Last Element as Pivot):**
  Lomuto partition places pivot at the end, yielding partitions of sizes $0$ and $n - 1$:
  $$T(n) = T(n - 1) + T(0) + \Theta(n) = T(n - 1) + c \cdot n$$

- **Solution via Iteration / Unrolling:**
  $$T(n) = T(n - 1) + c \cdot n$$
  $$T(n) = T(n - 2) + c(n - 1) + c \cdot n$$
  $$T(n) = T(1) + c \sum_{i=2}^n i = c \left( \frac{n(n + 1)}{2} - 1 \right) = \Theta(n^2)$$

- **Why Master Theorem Does NOT Apply Here:**
  The subproblem size decreases by a constant subtractive amount ($n - 1$), rather than a multiplicative factor ($n / b$). The Master Theorem requires $T(n) = a T(n/b) + f(n)$ with $b > 1$, so it is inapplicable.

---

### 2.3 Binary Search (Iterative & Recursive Station Lookup)
- **Recurrence Formulation:**
  $$T(n) = T(n/2) + \Theta(1)$$
- **Solution via Master Theorem:**
  - $a = 1, b = 2, f(n) = \Theta(1) = \Theta(n^0)$.
  - $n^{\log_b a} = n^{\log_2 1} = n^0 = 1$.
  - Since $f(n) = \Theta(n^{\log_b a})$, **Master Theorem Case 2 applies:**
    $$T(n) = \Theta(n^0 \log n) = \Theta(\log n)$$

---

### 2.4 Maximum Subarray (Station Net Profit Period)
- **Recurrence Formulation:**
  Dividing into left, right halves and finding the crossing subarray:
  $$T(n) = 2T(n/2) + \Theta(n)$$
- **Solution via Master Theorem:**
  - Identical to Merge Sort: $a=2, b=2, f(n)=\Theta(n) \implies \Theta(n \log n)$.

---

### 2.5 Strassen Matrix Multiplication
- **Recurrence Formulation:**
  Multiplies two $n \times n$ matrices using 7 sub-matrix multiplications of size $(n/2) \times (n/2)$ plus 18 additions:
  $$T(n) = 7T(n/2) + \Theta(n^2)$$
- **Solution via Master Theorem:**
  - $a = 7, b = 2, f(n) = \Theta(n^2)$.
  - $n^{\log_b a} = n^{\log_2 7} \approx n^{2.807}$.
  - Compare $f(n) = \Theta(n^2)$ with $n^{2.807}$:
    $$n^2 = O(n^{2.807 - \epsilon}) \quad \text{for } \epsilon \approx 0.807 > 0$$
  - **Master Theorem Case 1 applies:**
    $$T(n) = \Theta(n^{\log_2 7}) \approx \Theta(n^{2.807})$$
  - *Comparison with Classical:* Classical multiplication is $\Theta(n^3)$. Strassen saves multiplications ($7$ instead of $8$), yielding a strictly superior asymptotic complexity for large $n$.

---

### 2.6 Fast Exponentiation (Binary Powering)
- **Recurrence Formulation:**
  $$T(n) = T(n/2) + \Theta(1)$$
- **Solution via Master Theorem:**
  - $a = 1, b = 2, f(n) = \Theta(1) \implies \Theta(\log n)$.
  - Reduces battery degradation computing from $n$ steps to $\log_2 n$ steps.

---

### 2.7 Celebrity / Hub Station Discovery
- **Recurrence Formulation:**
  Each query `reach[candidate][i]` eliminates at least one candidate station:
  $$T(n) = T(n - 1) + \Theta(1)$$
- **Solution via Iteration:**
  $$T(n) = T(n - 1) + c = T(n - 2) + 2c = \dots = T(1) + c(n - 1) = \Theta(n)$$
- Verification pass also takes $\Theta(n)$ time $\implies$ Total time $\Theta(n)$.
- *Note:* Master Theorem does **not** apply because the problem shrinks additively by 1 ($n - 1$).

---

## 3. Boundary Cases: Where the Master Theorem Does NOT Apply

The Master Theorem ($T(n) = a T(n/b) + f(n)$) fails in the following well-defined structural cases:

1. **Additive Reductions ($b$ is not a division factor):**
   - Example: $T(n) = T(n - 1) + n$ (Quick sort worst-case).
   - Reason: Subproblem size is $n - 1$, not $n / b$. Solved via iteration or substitution.
2. **Non-Polynomial Gap between $f(n)$ and $n^{\log_b a}$:**
   - Example: $T(n) = 2T(n/2) + n / \log n$.
   - $n^{\log_2 2} = n$, but $f(n) = n / \log n$ is asymptotically smaller than $n$ by a factor of $\log n$, not by a polynomial factor $n^\epsilon$.
   - Reason: Case 1 requires $f(n) = O(n^{\log_b a - \epsilon})$ for some constant $\epsilon > 0$. Since $\log n$ grows slower than any $n^\epsilon$, Master Theorem cannot be used.
3. **Variable Coefficients:**
   - Example: $T(n) = n T(n/2) + n!$.
   - Reason: $a$ is a function of $n$, not a constant integer.
