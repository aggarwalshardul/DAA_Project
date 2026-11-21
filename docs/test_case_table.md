# Comprehensive Test Case Execution Matrix (All Syllabus Modules)

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project:** EV Charging Station Placement & Trip Planning System (Project 33)

| Test ID | Module Tested | Input Data / Condition | Expected Output | Actual Output Observed | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **TC-01** | Model / Initialization | Default built-in city network | 8 nodes initialized | 8 nodes initialized | **PASS** |
| **TC-02** | Model / Validation | Default sample graph | Returns 1 (Valid) | Returns 1 (Valid) | **PASS** |
| **TC-03** | Model / File Parser | `data/small.txt` file | Returns 1, 6 candidate sites | Returns 1, 6 sites loaded | **PASS** |
| **TC-04** | Model / Boundary | Negative budget (-1) | Returns 0 (Rejected safely) | Returns 0 (Rejected safely) | **PASS** |
| **TC-05** | Merge Sort | Random array `{45,12,89,23,7,56}` | Ascending sorted array | Ascending sorted array | **PASS** |
| **TC-06** | Quick Sort | Reverse array `{5,4,3,2,1,0}` | Ascending sorted array | Ascending sorted array | **PASS** |
| **TC-07** | Binary Search (Iterative)| Target 30 in `{10,20,30,40,50,60}` | Returns index 2 | Returns index 2 | **PASS** |
| **TC-08** | Binary Search (Recursive)| Target 30 in `{10,20,30,40,50,60}` | Returns index 2 | Returns index 2 | **PASS** |
| **TC-09** | Binary Search (Boundary)| Absent key 99 | Returns -1 | Returns -1 | **PASS** |
| **TC-10** | Maximum Subarray (D&C) | Array `{-2,1,-3,4,-1,2,1,-5}` | Max sum = 6 (`{4,-1,2,1}`) | Max sum = 6 (`{4,-1,2,1}`) | **PASS** |
| **TC-11** | Max Subarray Oracle | Compare D&C vs Brute Force | Identical maximum sum (6) | Identical sum (6 == 6) | **PASS** |
| **TC-12** | Fast Exponentiation | Base 2.0, Exponent 10 | Value 1024.0 | Value 1024.0 | **PASS** |
| **TC-13** | Fast Power Oracle | Compare Fast Power vs Naive | Identical outputs (1024.0) | Identical outputs (1024.0) | **PASS** |
| **TC-14** | Celebrity / Hub Discovery| 4 nodes with hub at node 2 | Returns 2 (Linear time) | Returns 2 | **PASS** |
| **TC-15** | Celebrity Hub Oracle | Compare Linear vs Brute Force | Identical hub ID (2) | Identical hub ID (2) | **PASS** |
| **TC-16** | Strassen Matrix Mult | 2x2 Matrix Multiplication | Equal to Classical product | Exact match with Classical | **PASS** |
| **TC-17** | Activity Selection | 3 overlapping charging sessions | Max non-overlapping count = 1 | Selected 1 session | **PASS** |
| **TC-18** | Fractional Knapsack | Grid capacity 75 kW, 2 requests | 75 kW fully allocated | 75 kW allocated | **PASS** |
| **TC-19** | MST Oracle (Prim vs Kruskal)| 8-node road network | `prim.weight == kruskal.weight`| Identical MST weight | **PASS** |
| **TC-20** | Huffman Compression | Text `"ABRACADABRA"` | Decoded text == Original | Perfect reconstruction | **PASS** |
| **TC-21** | 0/1 Knapsack DP | Adversarial counterexample | Optimal benefit = 50 | Optimal benefit = 50 | **PASS** |
| **TC-22** | Knapsack DP vs Greedy | Adversarial counterexample | DP benefit (50) > Greedy (33) | DP (50) > Greedy (33) | **PASS** |
| **TC-23** | Matrix Chain Order | 4 matrices dimensions | Min scalar operations > 0 | Optimal parens computed | **PASS** |
| **TC-24** | Longest Common Subseq | Planned route vs GPS trajectory | LCS length >= 2 | Shared sequence identified| **PASS** |
| **TC-25** | TSP Bitmask DP | 4 inspection stops | Min tour cost = 80 km | Min tour cost = 80 km | **PASS** |
| **TC-26** | N-Queens Problem | Board size 8x8 | Exactly 92 valid solutions | Exactly 92 solutions | **PASS** |
| **TC-27** | Sum of Subsets | Costs `{15,20,30,35}`, budget 50 | At least 1 exact match | Exact subsets identified | **PASS** |
| **TC-28** | Knapsack Backtracking | 3 sites, budget 50 | Optimal benefit = 220 | Benefit = 220 | **PASS** |
| **TC-29** | Branch & Bound (FIFO) | 3 sites, budget 50 | Optimal benefit = 220 | Benefit = 220 | **PASS** |
| **TC-30** | Branch & Bound (Least-Cost)| 3 sites, budget 50 | Optimal benefit = 220 | Benefit = 220 | **PASS** |
| **TC-31** | Ford-Fulkerson Max Flow | Grid capacity network | Max flow = 23 kW | Max flow = 23 kW | **PASS** |
| **TC-32** | Minimum Cut Identification | Saturated bottleneck lines | Cut edges count > 0 | Bottleneck lines output | **PASS** |
| **TC-33** | Bipartite Matching | 2 vehicles, 2 compatible bays | Match count = 2 | Both matched to distinct bays| **PASS** |
| **TC-34** | Chinese Remainder Theorem | Remainders `{2,3,2}`, moduli `{3,5,7}`| Total counter = 23 | Total counter = 23 | **PASS** |
| **TC-35** | Naive String Matching | Pattern `"AABA"` in text | 3 match occurrences found | 3 occurrences found | **PASS** |
| **TC-36** | Rabin-Karp String Match | Pattern `"AABA"` in text | 3 match occurrences found | 3 occurrences found | **PASS** |
| **TC-37** | Finite Automaton Match | Pattern `"AABA"` in text | 3 match occurrences found | 3 occurrences found | **PASS** |
| **TC-38** | KMP String Matching | Pattern `"AABA"` in text | 3 match occurrences found | 3 occurrences found | **PASS** |
| **TC-39** | String Matcher Oracle | Compare KMP vs Naive positions | Identical match index array | Exact position agreement | **PASS** |

**Summary:** 39 assertions executed, **39 / 39 PASSED (100% Green)**, 0 failures.
