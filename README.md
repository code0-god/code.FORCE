# **code.GAP**  
**GEMV Accelerator Project**  
*"Experimenting with GEMV acceleration &mdash; from CPU baselines to hardware co-design."*

---

## **Overview**
`code.GAP` is a study-oriented project focused on **GEMV (General Matrix-Vector Multiplication)**.  
It explores different implementations&mdash;from **naive CPU baselines** to **tiled optimizations** and **systolic-array hardware simulations**&mdash;to learn architectural trade-offs in matrix-vector computation.  

The project is designed to serve as both a **hands-on practice environment** for computer architecture concepts and a foundation for future **accelerator co-design experiments**.

---

## **Current Features**
- **Tensor2D class** with stride/view support  
- **Software kernels**
  - Naive GEMV
  - Tiled GEMV (BM/BN/BK parameters)  
- **Hardware kernel (optional)**
  - Systolic array simulator (output-stationary model)  
- **Measurement utilities**
  - Cycle/time measurement using `chrono` (and optional RDTSC)  
  - Correctness check with reference results  

---

## **Project Structure**
```makefile
code.GAP/
├─ build.sh                  # Build script (SW default, HW optional)
├─ Makefile                  # SW/HW toggle (MATX_HAS_SW / MATX_HAS_HW)
├─ README.md
├─ include/
│  └─ matx/
│     ├─ params.hpp          # Global parameters (BM/BN/BK, S/R/Kc)
│     ├─ tensor.hpp          # 2D Tensor class
│     └─ kernels/
│        ├─ sw.hpp           # SW kernels (naive, tiled)
│        └─ hw_systolic.hpp  # HW systolic simulator (optional)
├─ apps/
│  └─ bench_matmul.cpp       # Main runner: run kernels, measure cycles/time
└─ tests/
└─ correctness/
├─ test_small.cpp      # Small case correctness tests
└─ test_random.cpp     # Random input validation
```

---

## **Build & Run**

```bash
# Default: SW kernels only (CPU fallback)
./build.sh

# With HW simulator (systolic model)
HW=1 ./build.sh
```

Example runs:

```bash
# Run tiled GEMV on CPU
./build/bench_matmul --kernel sw.tiled --M 512 --N 512 --K 512

# Run systolic simulator
./build/bench_matmul --kernel hw.systolic --M 256 --N 256 --K 256 --S 16 --R 16 --Kc 32
```

---

## **Future Extensions**

- **Software kernels**
  - Blocked / Register tiling
  - Cache-oblivious recursion
  - OpenMP multithreading
  - SIMD (AVX, NEON)
- **Hardware models**

  - Variants of systolic arrays (output-stationary, weight-stationary, row-stationary)
  - RTL integration (Verilog/Bluespec → Verilator co-simulation)
- **Analysis tools**

  - Cycle count / CPI calculation
  - PE utilization metrics
  - Memory traffic measurement
- **Tests & Benchmarks**

  - Correctness: small/random testcases
  - Performance: BM/BN/BK and S/R/Kc sweeps

---

## **Learning Outcomes**

- Apply **computer architecture concepts** (pipelines, parallelism, locality) in code
- Compare **CPU baseline vs. accelerator models** to understand design trade-offs
- Prepare for **co-design practice**, from high-level simulation to RTL integration

---

## **License**
TBD (to be decided based on collaboration or course policy)
