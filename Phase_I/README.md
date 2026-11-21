# EV Charging Station Placement & Trip Planning System

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project Number:** 33  
**Implementation Standard:** Pure Offline Standard C17 / C11 (GNU GCC)  
**Author:** 2nd / 3rd Year B.Tech Computer Science & Engineering Student  

---

## 1. Project Overview
The **EV Charging Station Placement & Trip Planning System** is an offline computational decision-support tool for smart-city infrastructure engineers and EV fleet operators. It solves two complementary optimization problems:
1. **Station Placement Problem (0/1 Knapsack):** Allocates a finite municipal capital budget across candidate junctions to maximize daily public charging benefit.
2. **Energy-Constrained Trip Guidance (Dijkstra Shortest Path):** Calculates shortest-distance travel routes and evaluates battery state-of-charge feasibility against vehicle range.

The project is implemented in beginner-friendly, clean, transparent C code with **zero external dependencies**, allowing line-by-line viva explanation, whiteboard tracing, and live modification during evaluation.

---

## 2. Directory Structure
```
EV_Project/
  ├── Makefile                     # Build automation (all, run, test, bench, clean)
  ├── build.bat                    # Windows 1-click build batch script
  ├── README.md                    # Offline build, execution, and project manual
  ├── Phase_I/                     # Frozen snapshot of Phase I submission
  ├── Phase_II/                    # Reserved for Phase II final submission
  ├── src/
  │   ├── main.c                   # Interactive menu CLI with defensive input
  │   ├── model.h / model.c        # Domain entities, validation, text file parser
  │   ├── analysis.h / analysis.c  # Op counters, timers, memory estimates, CSV log
  │   ├── sorting_searching.h/.c   # Merge sort, quick sort, binary search (iter & rec)
  │   ├── greedy.h / greedy.c      # Greedy knapsack ratio heuristic, Dijkstra array & heap
  │   └── dp.h / dp.c              # 0/1 Knapsack DP with table printing & reconstruction
  ├── data/
  │   ├── small.txt                # 8-node test network with 6 candidate stations
  │   ├── medium.txt               # 30-node urban grid with 15 candidate stations
  │   ├── large.txt                # 100-node city network with 40 candidate stations
  │   ├── adversarial.txt          # Knapsack counterexample dataset (DP beats Greedy)
  │   └── events_log.txt           # Real-world operational logs for text matching
  ├── tests/
  │   └── test_runner.c            # Automated test harness (unit, edge, adversarial)
  └── docs/
      ├── problem_statement.md     # Requirements, mathematical model, and flow
      ├── pseudocode.md            # Pseudocode, selection rationale, invariants
      ├── complexity_report.md     # Asymptotic tables (O, Omega, Theta), space bounds
      ├── viva_notes.md            # Hand traces, examiner Q&A, live modification guide
      └── test_case_table.md       # Traceability matrix of expected vs actual results
```

---

## 3. Offline Compilation & Execution Instructions

This project requires **no internet connection** and uses only standard C libraries (`stdio.h, stdlib.h, string.h, time.h, limits.h, math.h`).

### Windows (Command Prompt / PowerShell)
1. **Compile with GCC:**
   ```cmd
   build.bat
   ```
   *Or manually:*
   ```cmd
   gcc -Wall -Wextra -std=c11 -O2 -o ev_planner.exe src\main.c src\model.c src\analysis.c src\sorting_searching.c src\greedy.c src\dp.c -lm
   gcc -Wall -Wextra -std=c11 -O2 -o test_runner.exe tests\test_runner.c src\model.c src\analysis.c src\sorting_searching.c src\greedy.c src\dp.c -lm
   ```

2. **Run Interactive Menu Application:**
   ```cmd
   ev_planner.exe
   ```

3. **Run Automated Test Suite:**
   ```cmd
   test_runner.exe
   ```

### Linux / macOS / MinGW Make
- Build both targets: `make all`
- Run application: `make run`
- Run test suite: `make test`
- Clean artifacts: `make clean`

---

## 4. Phase I Accomplishments Checklist
- [x] Full domain model: Nodes, Roads, Stations, Vehicles, Trips, Budget.
- [x] Robust file parser with defensive comments/section skipping.
- [x] Defensive CLI input handler (`read_int_safe`) immune to string inputs and invalid ranges.
- [x] Hand-written Merge Sort and Quick Sort with operation counting.
- [x] Hand-written Binary Search (Iterative and Recursive) with oracle equivalence test.
- [x] 0/1 Knapsack Dynamic Programming with table printing and item reconstruction.
- [x] Greedy Knapsack Heuristic with adversarial counterexample proof ($50 > 33$).
- [x] Dijkstra Shortest Path (Array-based $\Theta(V^2)$ and Min-Heap $O((V+E)\log V)$).
- [x] EV Battery Feasibility verification based on vehicle range and battery state of charge.
- [x] Automated test suite: **37 / 37 PASS (100% green)**.
- [x] Full documentation: Problem Statement, Pseudocode, Complexity Report, Viva Notes, Test Table.
