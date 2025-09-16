# **code.FORCE**
**Full-stack Optimization and Reconfigurable Co-design Environment**  
*"Experimenting with GEMV acceleration — from CPU baselines and Roofline models to hardware co-design."*

---

## **Overview**

`code.FORCE` is a **practice-oriented project** for exploring **GEMV (General Matrix-Vector Multiplication)** acceleration through comprehensive **SW/HW co-design**. It is designed both as a **learning companion for computer architecture courses** and as a **foundation for accelerator co-design experiments** involving optimization techniques and custom hardware.

The project focuses on:
- Building a reproducible **software/hardware stack** for performance analysis based on the Roofline model.  
- Evaluating the impact of modern acceleration techniques like **quantization** (INT8) and **structured sparsity** (2:4).
- Understanding design trade-offs between **CPU baselines, tiled kernels, and systolic-array models**  
- Producing benchmarks and correctness tests for **portfolio and research use**

---

## **Layered Architecture**
The project is designed around a "Contract"-based architecture to ensure clean separation of concerns between layers.

- **Core (`include/core`)**  
  - `types.hpp`: The central **contract** defining system-wide data types (`DType`), compute policies (`MathOp`), and sparsity patterns (`Sparsity`). This is the immutable agreement shared by all layers.
  - `backend.hpp`: Defines the minimal `IBackend` interface. It has **no dependency on the Tensor implementation**.

- **Tensor (`include/tensor`)**  
  - A user-friendly **handle** for managing data and its metadata (shape, stride). It safely manages memory allocated by a `Backend` via the RAII pattern.

- **Backend (`src/backend/*`)**  
  - A pure **resource provider**. Responsible for memory operations (`allocate`/`deallocate`) and data movement (`copy`/`fill`).
  - Optionally provides an accelerated `gemv` kernel through the `IBackend` interface.

- **Algorithm (`src/alg`)**  
  - The ***GEMV dispatcher*** and reference software kernels.
  - It delegates the operation to the `Backend` if an accelerated kernel is available; otherwise, it **falls back to a software kernel** (e.g., `naive`, `tiled`, `systolic_sw`).

- **App (`src/app`)**  
  - The **CLI benchmark harness** that parses flags and produces standardized logs for analysis.

- **Test (`test`)**  
  - The **verification suite**, composed of Unit, Golden, and Performance tests.

---

## **Project Structure**
The project is organized by layers, with a clear distinction between public interfaces (`include`) and implementations (`src`).


```
code.GAP/
├─ scripts/
│  └─ build.sh
│
├─ include/
│  ├─ core/
│  │  ├─ types.hpp            # Core contracts (DType, MathOp, GemvArgs)
│  │  ├─ backend.hpp          # IBackend abstract interface
│  │  └─ backend_factory.hpp  # Declares the backend factory functions
│  └─ tensor/
│     ├─ tensor.hpp           # Tensor<T> handle declaration
│     └─ tensor.tpp           # Tensor<T> template implementation
│
├─ src/
│  ├─ core/
│  │  └─ backend_factory.cpp  # Implements the backend registry and factory
│  ├─ backend/
│  │  ├─ cpu_scalar/          # Portable scalar baseline
│  │  │  ├─ cpu_scalar_backend.hpp
│  │  │  └─ cpu_scalar_backend.cpp
│  │  ├─ cpu_neon/            # Arm NEON optimized backend
│  │  ├─ cpu_avx/             # x86 AVX optimized backend
│  │  ├─ sim/                 # Systolic array simulator backend
│  │  └─ hw_fpga/             # Hooks for the FPGA backend
│  ├─ alg/
│  │  ├─ gemv_dispatch.cpp    # Dispatches to backend or SW fallbacks
│  │  ├─ gemv_naive.cpp       # Naive software kernel
│  │  ├─ gemv_tiled.cpp       # Tiled/blocked software kernel
│  │  └─ gemv_systolic_sw.cpp # Systolic array software model
│  └─ app/
│     └─ main.cpp           # CLI benchmark runner
│
└─ test/
   ├─ unit_tensor.cpp      # Unit tests for Tensor class
   ├─ golden_gemv.cpp      # Correctness tests against a reference
   └─ perf_gemv.cpp        # Performance benchmarks
```

---

## **Build & Run**
```bash
# Build the project (default: cpu.scalar backend)
bash scripts/build.sh

# Example 1: Run a FP32 benchmark using the tiled software kernel
./build/gap_app --backend=cpu.scalar --kernel=sw.tiled \
    --dtype=f32 --math=Fp32 --M=4096 --N=4096

# Example 2: Run an INT8 benchmark with 2:4 structured sparsity
./build/gap_app --backend=cpu.scalar --kernel=sw.tiled \
    --dtype=i8 --math=Int8xInt8_To_Int32 --sparsity=block2_4 \
    --M=4096 --N=4096 --runs=30 --warmup=5
```

Run all tests:

```bash
ctest --test-dir build -V
```

---

## **Roadmap**
The project is structured in four sequential phases, progressing from fundamental software concepts to hardware implementation.

- **Phase 1. Software Kernels (Practice)**
  - Implement baseline GEMV kernels in pure software to establish correctness and a performance reference.
  - Kernels: Naive (row-major), Tiled/Blocking for cache locality.

- **Phase 2. CPU Optimization**
  - Develop CPU-specific backends using SIMD intrinsics to optimize the software kernels.
  - Targets: Arm NEON (cpu.neon), x86 AVX (cpu.avx).

- **Phase 3. Accelerator Modeling**
  - Build a software model of a systolic array to understand its dataflow and performance characteristics without designing hardware.
  - Model: A cycle-approximate, output-stationary systolic array simulator (`systolic_sw`).

- **Phase 4. FPGA Co-Design (Hardware)**
  - Design, implement, and verify a small but complete hardware GEMV accelerator on an FPGA.
  - **Microarchitecture: INT8, 8x8 Output-Stationary Systolic Array**
    - **PE (Processing Element)**: A MAC unit performing an `int8 * int8 + int32 -> int32` operation.
    - **Dataflow**: The `x` vector is broadcast to PE columns, `A` matrix rows are streamed in, and `y` results are accumulated locally within PE rows.
    - **On-Chip Memory**: BRAM is used for buffering tiles of `A` and `x`.
  - **Host Interface**:
    - **Control**: AXI-Lite for register-based control (M, N, base addresses, start/done signals).
    - **Data Transfer**: Initial bring-up uses AXI-Lite for simplicity, with a final goal of integrating an AXI-DMA for efficient tile transfers.
  - **Verification**: The host measures the cycle count from `start` to `done` and validates correctness by comparing the results against a software reference with a set tolerance (`~1e-3`).

---

## **Learning Outcomes**
- Apply **computer architecture concepts** (pipelines, SIMD, locality, memory hierarchy) in practice.
- Compare **CPU baselines vs. accelerator models** to understand design trade-offs using quantitative analysis.
- Gain **hands-on experience** with modern optimization techniques like quantization and structured sparsity.
- Build a **portfolio-ready framework** for accelerator co-design research and development

---

## **License**
TBD (to be decided based on collaboration or course policy)
