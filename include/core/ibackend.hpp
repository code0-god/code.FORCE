#pragma once
#include "core/types.hpp"
#include <string>

namespace ace
{
    class IBackend
    {
    public:
        virtual ~IBackend() = default;
        virtual const char *name() const noexcept = 0;

        // 자원 관리 API
        [[nodiscard]] virtual void *allocate(size_t nbytes) = 0;
        virtual void deallocate(void *ptr, void *ctx) noexcept = 0;

        // 선택적 커널 실행 API
        virtual bool gemv(const GemvArgs &args, std::string &used_kernel_name) = 0;
    };
}