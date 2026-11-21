# Automated Test Case Execution Table (Phase I)

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

| Test ID | Module Tested | Input Data / Condition | Expected Output | Actual Output Observed | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **TC-01** | Model / Loader | Built-in default city data | 8 nodes initialized, valid | 8 nodes initialized, valid | **PASS** |
| **TC-02** | Model / Loader | Non-existent file path | Return 0, safe error print | Return 0, error printed safely | **PASS** |
| **TC-03** | Model / Loader | `data/small.txt` file | 6 candidate stations loaded | 6 candidate stations loaded | **PASS** |
| **TC-04** | Model / Validation | Negative budget (-50) | Return 0 (Validation error) | Return 0 (Validation error) | **PASS** |
| **TC-05** | Merge Sort | Random array `{45,12,89,23,7,56}` | Sorted `{7,12,23,45,56,89}` | Sorted `{7,12,23,45,56,89}` | **PASS** |
| **TC-06** | Merge Sort | Already sorted `{10,20,30,40,50}` | Preserved order `{10..50}` | Preserved order `{10..50}` | **PASS** |
| **TC-07** | Merge Sort | Reverse sorted `{90,70,50,30,10}` | Ascending order `{10..90}` | Ascending order `{10..90}` | **PASS** |
| **TC-08** | Merge Sort | Multiple duplicates `{5,2,5,2,5,1}` | Sorted `{1,2,2,5,5,5}` | Sorted `{1,2,2,5,5,5}` | **PASS** |
| **TC-09** | Merge Sort | Single element `{42}` (Boundary) | Array `{42}` untouched | Array `{42}` untouched | **PASS** |
| **TC-10** | Quick Sort | Random array `{34,7,23,32,5,62,19}` | Sorted array `{5,7..62}` | Sorted array `{5,7..62}` | **PASS** |
| **TC-11** | Quick Sort | Already-sorted array (Adversarial) | Sorted array, no crash | Sorted array, no crash | **PASS** |
| **TC-12** | Quick Sort | Candidate stations by cost | Ascending order of cost | Ascending order of cost | **PASS** |
| **TC-13** | Binary Search (Iter)| Target in middle (44) | Return index 3 | Return index 3 | **PASS** |
| **TC-14** | Binary Search (Rec) | Target in middle (44) | Return index 3 | Return index 3 | **PASS** |
| **TC-15** | Binary Search Oracle| Target 44 across both methods | Match: index_iter == index_rec | Identical index 3 | **PASS** |
| **TC-16** | Binary Search | First element boundary (11) | Return index 0 | Return index 0 | **PASS** |
| **TC-17** | Binary Search | Last element boundary (88) | Return index 7 | Return index 7 | **PASS** |
| **TC-18** | Binary Search | Absent target (50) | Return -1 | Return -1 | **PASS** |
| **TC-19** | Knapsack 0/1 DP | Standard instance (B=50, 3 items) | Max benefit = 220, cost = 50 | Max benefit = 220, cost = 50 | **PASS** |
| **TC-20** | Knapsack 0/1 DP | Budget = 0 (Boundary) | Max benefit = 0 | Max benefit = 0 | **PASS** |
| **TC-21** | Knapsack 0/1 DP | Empty item set (Boundary) | Max benefit = 0 | Max benefit = 0 | **PASS** |
| **TC-22** | Knapsack DP vs Greedy| Adversarial dataset (B=50, S1, S2, S3) | DP = 50, Greedy = 33 (DP > Gr)| DP = 50, Greedy = 33 (DP > Gr) | **PASS** |
| **TC-23** | Dijkstra Array | Triangle graph with shortcut | Shortest dist = 25 km | Shortest dist = 25 km | **PASS** |
| **TC-24** | Dijkstra Min-Heap | Triangle graph with shortcut | Shortest dist = 25 km | Shortest dist = 25 km | **PASS** |
| **TC-25** | Dijkstra Oracle | Compare Array vs Min-Heap outputs | `arr.dist == heap.dist` | Exact match (25 km == 25 km) | **PASS** |
| **TC-26** | Dijkstra | Disconnected node (Boundary) | `is_reachable = 0` | `is_reachable = 0` | **PASS** |
| **TC-27** | EV Feasibility | Vehicle range (250 km) >= 37 km | Feasible = 1 | Feasible = 1 | **PASS** |
| **TC-28** | EV Feasibility | Vehicle range (10 km) < 37 km | Feasible = 0 (Exceeded) | Feasible = 0 (Exceeded) | **PASS** |

**Summary Statistics:**
- Total Test Cases Executed: 37 assertions across 8 functional modules.
- Total Passed: **37 / 37** (100% Pass Rate).
- Regressions / Crashes: **0**.
