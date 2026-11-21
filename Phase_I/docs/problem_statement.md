# Problem Statement: EV Charging Station Placement & Trip Planning System

**Course:** CSEG3053 – Design and Analysis of Algorithms  
**Project No:** 33  
**Implementation Standard:** Pure Offline Standard C17  
**Academic Target:** B.Tech Computer Science & Engineering (2nd / 3rd Year)

---

## 1. Domain Context & Target User
As urban transportation shifts rapidly towards electromobility, city municipal corporations, smart-city infrastructure planners, and commercial EV fleet operators face a dual optimization challenge:
1. **Infrastructure Capital Allocation:** Where should high-power DC fast-charging stations be placed within an urban road network under a strict municipal capital budget to maximize public benefit (expected charging sessions and trips served)?
2. **Energy-Constrained Trip Guidance:** For any dispatch trip request, what is the shortest route connecting origin and destination such that the vehicle's remaining battery charge is not depleted en route?

The **Target User** is a City Transportation Engineer or Fleet Logistics Manager who must evaluate capital investment plans offline and generate deterministic, verifiable routing plans without relying on cloud services or external black-box APIs.

---

## 2. Real-World Need
- **Capital Scarcity:** Installing fast-charging hubs involves heavy civil and electrical transformer costs ($20k–$80k per station). Budget limits mean only a subset of candidate junctions can receive stations.
- **Range Anxiety & Infeasibility:** Unlike fossil-fuel vehicles that refuel in 3 minutes anywhere, EVs have finite battery capacity (30 kWh to 100 kWh) and energy consumption proportional to distance and terrain. An optimal topological shortest path is useless if the vehicle runs out of charge midway.
- **Algorithmic Trade-offs:** Fast heuristics (greedy methods) run in $O(n \log n)$ time but make locally optimal decisions that can leave valuable budget unspent or pick suboptimal station subsets. Dynamic Programming guarantees mathematical optimality in pseudo-polynomial time $O(n \cdot W)$. The system must allow planners to compare both paradigms quantitatively.

---

## 3. Mathematical Formulation & Entities

### 3.1 Entities and Schema
1. **Road Network Graph $G = (V, E)$:**
   - Vertices $V = \{v_0, v_1, \dots, v_{n-1}\}$: City junctions / towns ($|V| \le 100$).
   - Edges $E \subseteq V \times V$: Undirected roads with non-negative edge weight $d(u, v) \in \mathbb{R}^+$ representing distance in kilometers.
2. **Candidate Stations $S = \{s_1, s_2, \dots, s_m\}$ ($m \le 50$):**
   - Each site $s_i$ has a junction location $\text{node}(s_i) \in V$, installation capital cost $c_i \in \mathbb{Z}^+$, capacity $P_i$ (kW), available charging ports $k_i$, and an estimated daily benefit score $b_i \in \mathbb{Z}^+$ (projected charging demand).
3. **Budget $B \in \mathbb{Z}^+$:**
   - Upper bound on total capital expenditure: $\sum_{i \in \text{Selected}} c_i \le B$.
4. **Fleet Vehicles $\mathcal{V}$:**
   - Battery capacity $C_{\text{bat}}$ (kWh), maximum range $R_{\max}$ (km), state of charge $\text{SOC} \in [0, 100]\%$, and specific consumption $\eta$ (kWh/km).
5. **Trip Requests $\mathcal{T}$:**
   - Source $u \in V$, Destination $v \in V$, departure and deadline timestamps.

### 3.2 Optimization Objectives
1. **Station Placement (0/1 Knapsack Decision & Optimization):**
   $$\max \sum_{i=1}^m b_i \cdot x_i \quad \text{subject to} \quad \sum_{i=1}^m c_i \cdot x_i \le B, \quad x_i \in \{0, 1\}$$
2. **Shortest Feasible Path (Dijkstra with Battery Constraint):**
   $$\min_{\text{Path } P = (u=v_0, v_1, \dots, v_k=v)} \sum_{j=0}^{k-1} d(v_j, v_{j+1}) \quad \text{subject to} \quad \sum_{j=0}^{k-1} d(v_j, v_{j+1}) \le R_{\text{usable}}$$
   where $R_{\text{usable}} = R_{\max} \times (\text{SOC} / 100)$.

---

## 4. System Workflow Diagram

```
+-------------------------------------------------------------+
|                      USER CLI INTERFACE                     |
+-------------------------------------------------------------+
                               |
         +---------------------+---------------------+
         |                                           |
         v                                           v
+-------------------------------+       +-------------------------------+
|  1. STATION PLACEMENT ENGINE  |       |    2. TRIP ROUTING ENGINE     |
|   (Capital Allocation Lab)    |       |  (Energy-Constrained Routing) |
+-------------------------------+       +-------------------------------+
         |                                           |
    +----+----+                                 +----+----+
    |         |                                 |         |
    v         v                                 v         v
+-------+ +---------+                       +-------+ +---------+
|  DP   | | Greedy  |                       | Array | | MinHeap |
| Table | | Ratio   |                       | O(V²) | | O(ElogV)|
+-------+ +---------+                       +-------+ +---------+
    |         |                                 |         |
    +----+----+                                 +----+----+
         |                                           |
         v                                           v
+-------------------------------+       +-------------------------------+
| Metrics: Benefit, Budget Used,|       | Metrics: Path, Distance (km), |
| Operations, Runtime, Quality  |       | Battery Feasibility, Deficit  |
+-------------------------------+       +-------------------------------+
```

---

## 5. Input / Output Specifications
- **Input Formats:**
  - Plain text configuration files (`data/small.txt`, `data/medium.txt`, `data/large.txt`, `data/adversarial.txt`) adhering to explicit section markers: `[META]`, `[NODES]`, `[ROADS]`, `[STATIONS]`, `[VEHICLES]`, `[TRIPS]`.
  - Manual interactive CLI input with strict buffer flushing and range bounds checking.
- **Output Formats:**
  - Standard formatted terminal tables with alignment and ASCII borders.
  - CSV performance logs (`benchmark.csv`) for empirical graph generation.

---

## 6. Success Criteria
1. **Robustness:** Zero crashes, memory leaks, or unhandled exceptions under any invalid, negative, or out-of-range user inputs.
2. **Correctness:** 100% test pass rate across unit, boundary, stress, and adversarial cases.
3. **Educational Transparency:** Code written in straightforward C without complex pointer indirection or obfuscated macros so the student can explain every line in a viva.
