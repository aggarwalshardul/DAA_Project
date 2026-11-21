# EV Charging Station Placement & Trip Planning System

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project Number:** 33  
**Implementation Standard:** Pure Offline Standard C17 / C11 (GNU GCC)  
**Academic Target:** 2nd / 3rd Year B.Tech Computer Science & Engineering  

---

## 1. Quick Start Guide (How to Run)

The project is completely self-contained, fully offline, and requires only standard C headers (`stdio.h, stdlib.h, string.h, time.h, limits.h, math.h`).

### Step 1: Open Terminal / Command Prompt
Navigate to your project directory:
```cmd
cd C:\Users\aggar\.gemini\antigravity\scratch\EV_Project
```

### Step 2: Build the Binaries (One-Click)
Run the provided batch script (compiles with `gcc -Wall -Wextra -std=c11 -O2` with zero warnings):
```cmd
build.bat
```
*Or using GNU Make:*
```cmd
make
```

### Step 3: Run the Interactive CLI Application
Launch the menu-driven decision simulator:
```cmd
ev_planner.exe
```
*Or via Make:*
```cmd
make run
```

### Step 4: Run the Automated Test Suite (Self-Validation)
Run all 39 automated unit, boundary, stress, and adversarial tests:
```cmd
test_runner.exe
```
*Or via Make:*
```cmd
make test
```

---

## 2. Interactive Menu Navigation (All 16 Options)

When you run `ev_planner.exe`, the interactive console presents the full syllabus workbench:

```
=========================================================
       EV CHARGING PLANNER - FULL SYLLABUS CLI (C17)     
          CSEG3053 Design and Analysis of Algorithms     
=========================================================
 1. Load / Generate Dataset
 2. Station Placement (Knapsack: Greedy / DP / BT / B&B)
 3. Trip Planning (Dijkstra / Bellman-Ford / Floyd / TSP)
 4. Infrastructure Network (Prim / Kruskal MST)
 5. Charging Schedule (Activity / Interval / Fractional)
 6. Capacity & Assignment (Ford-Fulkerson / Bipartite)
 7. Sorting & Searching Tools
 8. Divide & Conquer Tools (MaxSubarray/Power/Celebrity/Strassen)
 9. DP Lab (LCS / Matrix Chain / Optimal BST)
 10. Backtracking Lab (N-Queens / Subset Sum)
 11. Event Logs (Huffman Compression + 4 String Matchers)
 12. Number Theory (Chinese Remainder Theorem)
 13. Run 3 Required Demo Scenarios
 14. Benchmark Runner (Export CSV)
 15. Run Test Suite (Self-Validation)
 16. Complexity Classes & Reduction Demo
 0. Exit System
=========================================================
```

### Recommended Viva Demonstration Sequence
1. **Option 1 $\to$ 1:** Load the 8-node built-in default city network.
2. **Option 2:** Run Station Placement to compare **Greedy vs DP vs Backtracking vs Branch & Bound (FIFO vs Least-Cost)**.
3. **Option 3 $\to$ 1:** Compute the shortest route using Dijkstra and check vehicle battery range feasibility.
4. **Option 13 $\to$ 4:** Run all three practical scenarios (**Normal Operation**, **Constraint Shock** where a hub fails and budget cuts 30%, and **Large-Data Stress** on 100 nodes).
5. **Option 16:** Demonstrate the formal polynomial-time reduction: **Subset-Sum $\le_P$ 0/1 Knapsack Decision**.

---

## 3. Complete Project File Inventory

```
EV_Project/
  ├── Makefile                       # Automated build script (all, run, test, bench, clean)
  ├── build.bat                      # Windows 1-click build batch script
  ├── README.md                      # This manual and execution guide
  ├── Phase_I/                       # Frozen deliverable snapshot of Phase I
  ├── Phase_II/                      # Final complete deliverable snapshot of Phase II
  │
  ├── src/
  │   ├── main.c                     # 16-option interactive menu CLI with defensive input
  │   ├── model.h / model.c          # Nodes, Roads, Stations, Vehicles, Trips, file parser
  │   ├── analysis.h / analysis.c    # Global op counter (g_ops), timer, memory, CSV export
  │   ├── sorting_searching.h / .c   # Merge sort, Quick sort, Binary search (iter & rec)
  │   ├── divide_conquer.h / .c      # Max subarray, Fast power, Celebrity hub, Strassen
  │   ├── greedy.h / greedy.c        # Activity selection, Interval partition, Fractional knapsack,
  │   │                              # Huffman coding, Dijkstra (Array & Heap), Prim, Kruskal
  │   ├── dp.h / dp.c                # 0/1 Knapsack DP, MCM, LCS, Optimal BST,
  │   │                              # Bellman-Ford, Floyd-Warshall, TSP Bitmask DP
  │   ├── backtracking.h / .c        # N-Queens, Sum of Subsets, Knapsack BT, B&B (FIFO/LIFO/LC)
  │   ├── flow_matching.h / .c       # Ford-Fulkerson Max Flow & Min-Cut, Max Bipartite Matching
  │   ├── number_theory.h / .c       # Chinese Remainder Theorem with Extended Euclid
  │   ├── string_match.h / .c        # Naive, Rabin-Karp, Finite Automaton, KMP
  │   └── scenarios.h / scenarios.c  # 3 Practical evaluation scenarios (Normal, Shock, Stress)
  │
  ├── data/
  │   ├── small.txt                  # 8-node test network with 6 candidate stations
  │   ├── medium.txt                 # 30-node urban grid with 15 candidate stations
  │   ├── large.txt                  # 100-node city network with 40 candidate stations
  │   ├── adversarial.txt            # Counterexample dataset where Greedy fails (33 vs 50 DP)
  │   └── events_log.txt             # Realistic station charging logs for pattern matching
  │
  ├── tests/
  │   └── test_runner.c              # 39 automated unit, boundary, stress, and oracle tests
  │
  └── docs/
      ├── problem_statement.md       # Target user, mathematical model, and system flow
      ├── pseudocode.md              # Complete handwritten pseudocode & invariants
      ├── recurrences.md             # Formal recurrence derivations via Master Theorem & Iteration
      ├── complexity_report.md       # Full asymptotic matrix (O, Omega, Theta, auxiliary space)
      ├── correctness_notes.md       # Mathematical correctness proofs & cut properties
      ├── complexity_classes_and_reduction.md # P, NP, NPC classifications & Subset-Sum reduction
      ├── algorithm_selection_guide.md # Practical decision trees and engineering trade-offs
      ├── limitations.md             # Model limitations, error analysis, Phase progression
      ├── viva_notes.md              # 3-line pitches, hand traces, 3 Q&As per module, top 15 Q&As
      └── test_case_table.md         # Full traceability matrix of 39 test assertions
```

---

## 4. Empirical Benchmark Data Sample

| Algorithm | Input Size ($N$) | Dataset Type | Execution Time (ms) | Operation Count (`g_ops`) | Memory Used | Result Quality |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **0/1 Knapsack DP** | 12 sites, B=180 | Normal City | 0.0410 ms | 2,364 ops | ~9.2 KB | **100% Optimal** |
| **Knapsack Greedy** | 12 sites, B=180 | Normal City | 0.0080 ms | 86 ops | ~0.5 KB | 92.4% (Suboptimal)|
| **B&B Least-Cost** | 12 sites, B=180 | Normal City | 0.0250 ms | 148 ops | ~1.2 KB | **100% Optimal** |
| **Dijkstra Array** | 100 nodes | Dense/Grid | 0.0820 ms | 9,900 ops | 1.2 KB | **Optimal Path** |
| **Dijkstra Heap** | 100 nodes | Sparse | 0.0450 ms | 1,840 ops | 2.4 KB | **Optimal Path** |
| **Merge Sort** | 1000 items | Random | 0.3120 ms | 18,940 ops | 4.0 KB | Fully Sorted |
| **Quick Sort** | 1000 items | Random | 0.1980 ms | 11,250 ops | 0 KB (In-place) | Fully Sorted |
| **KMP Search** | 500 chars | Log Text | 0.0150 ms | 512 ops | ~0.1 KB | All Matches Found|
| **Naive Search** | 500 chars | Log Text | 0.0620 ms | 1,840 ops | 0 KB | All Matches Found|
