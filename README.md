# **code.GAP**

**GEMV Accelerator Project**  
*"Experimenting with GEMV acceleration — from CPU baselines to hardware co-design."*

---

## **Overview**
`code.GAP` is a **practice-oriented project** for exploring **GEMV (General Matrix-Vector Multiplication)** acceleration.  
It is designed both as a **learning companion for computer architecture courses** (MIPS, Arm NEON, RISC-V concepts) and as a **foundation for accelerator co-design experiments**.

The project focuses on:
- Building a reproducible **software/hardware stack** for GEMV  
- Practicing optimization in **RISC environments** (Arm A1, extensible to RISC-V)  
- Understanding trade-offs between **CPU baselines, tiled kernels, and systolic-array models**  
- Producing benchmarks and correctness tests for **portfolio and research use**

---

## **Layered Architecture**
- **Core (`include/gap/core`, `src/core`)**  
  - Minimal public interfaces (error types, backend registry)

- **Tensor (`include/gap/tensor`)**  
  - Tensor2D handle with stride/view support, RAII memory management

- **Backend (`include/gap/backend`, `src/backend/*`)**  
  - Resource provider (allocate, deallocate, fill, copy)  
  - Optional optimized GEMV kernels (e.g., NEON, AVX, RTL hooks)  
  - *Multiple backends exist side by side (cpu.scalar, cpu.neon, cpu.avx, sim, hw.fpga)*

- **Algorithm (`src/alg`)**  
  - GEMV entry point (`gemv_dispatch`)  
  - Includes **software reference kernels** (naive, blocking, tiling, systolic-SW)  
  - Falls back to SW kernels if the selected backend does not provide GEMV

- **App (`src/app`)**  
  - CLI benchmark harness (parses flags, selects backends/kernels, measures performance)

- **Test (`test`)**  
  - Unit tests, golden correctness tests, performance runs

---

## **Project Structure**
```makefile
code.GAP/
├─ scripts/
│   └─ build.sh                 # Build script (backend toggles)
│
├─ include/gap/
│   ├─ core/                    # Core APIs (errors, registry)
│   ├─ backend/                 # Backend interface (IBackend)
│   └─ tensor/                  # Tensor handle (metadata, stride/view)
│
├─ src/
│   ├─ core/                    # Registry implementation
│   ├─ backend/                 # Multiple backend implementations
│   │   ├─ cpu_scalar/          # Portable scalar baseline
│   │   ├─ cpu_neon/            # Arm NEON optimized (A1)
│   │   ├─ cpu_avx/             # x86 AVX optimized (optional, test only)
│   │   ├─ sim/                 # Systolic simulator backend (optional)
│   │   └─ hw_fpga/             # FPGA/RTL backend hooks (optional)
│   │
│   ├─ alg/                     # GEMV dispatcher + SW reference kernels
│   │   ├─ gemv.hpp / gemv.cpp  # Dispatch logic (SW/HW/Auto)
│   │   ├─ gemv_naive.cpp
│   │   ├─ gemv_blocking.cpp
│   │   ├─ gemv_tiling.cpp
│   │   └─ gemv_systolic_sw.cpp
│   │
│   └─ app/
│       └─ bench.cpp            # CLI benchmark runner
│
└─ test/
    ├─ unit_tensor.cpp          # Tensor unit tests
    ├─ unit_backend.cpp         # Backend primitive tests
    ├─ golden_gemv.cpp          # GEMV correctness validation
    └─ perf_gemv.cpp            # GEMV performance benchmark
```

---

## **Build & Run**
```bash
# Default build (scalar backend only)
bash scripts/build.sh

# Run benchmark (example: 512x512 GEMV)
./build/gap_app --backend=cpu.scalar --kernel=sw.naive --M=512 --N=512

# Switch backend (Arm NEON, if compiled)
./build/gap_app --backend=cpu.neon --kernel=hw --M=2048 --N=2048
```

Run all tests:

```bash
ctest --test-dir build -V
```

---

## **Roadmap**
- **Phase 0–1 (Practice)**
  - Naive / blocking / tiling GEMV in pure software

- **Phase 2 (Optimization)**
  - CPU SIMD backends
  - Arm A1: NEON (`cpu.neon`)
  - x86: AVX (`cpu.avx`, optional)

- **Phase 3 (Modeling)**
  - Software systolic-array simulator (output-/row-/weight-stationary)

- **Phase 4 (Hardware)**
  - FPGA/RTL systolic GEMV engine, integrated via `backend.hw_fpga`

---

## **Learning Outcomes**
- Apply **computer architecture concepts** (pipelines, SIMD, locality) in practice
- Compare **CPU baselines vs. accelerator models** to understand design trade-offs
- Gain **hands-on RISC optimization experience** (Arm NEON, extensible to RISC-V)
- Build a **portfolio-ready framework** for accelerator co-design research

---

## **License**
TBD (to be decided based on collaboration or course policy)
