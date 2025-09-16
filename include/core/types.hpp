#pragma once
#include <cstddef>
#include <string>

namespace gap {
    // Data Type, Operations, Sparsity pattern contract
    enum class DType { F32, F16, BF16, I8, I32 };
    enum class Ops { FP32, TF32, FP16, BF16, ACC };
    enum class Sparsity { DENSE, BLOCK2_4 };
    
    // 연산 정책을 담는 구조체
    struct OperationConfig {
        Ops math_op;
        Sparsity sparsity;
        std::string kernel; // e.g., "sw.tiled", "backend"
    };

    // 타입 정보가 제거된(type-erased) GEMV 연산 인자
    struct GemvArgs { /* ... A, x, y 포인터 및 shape, stride, 정책 등 ... */ };
}