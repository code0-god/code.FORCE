#pragma once
#include "tensor.hpp"
#include <typeinfo>
#include <utility> // std::move
#include <memory>  // std::unique_ptr
#include <cstdlib> // std::free

#ifdef __GNUG__
#include <cxxabi.h> // abi::__cxa_demangle
#endif

namespace gap
{
    template <typename T>
    Tensor<T>::Tensor(const std::string &name, size_t rows, size_t cols)
        : name_{name}
    {
        ne_[0] = rows;
        ne_[1] = cols;

        for (int d = 2; d < GAP_DIMS; ++d)
            ne_[d] = 1; // higher dims default = 1
        nb_[GAP_DIMS - 1] = static_cast<size_t>(sizeof(T));
        for (int d = GAP_DIMS - 2; d >= 0; --d)
            nb_[d] = ne_[d + 1] * nb_[d + 1];

        // data_는 attach()를 통해 backend에서 제공.
        // REQUIRE: attached pointer must satisfy at least alignof(T).
    }

    template <typename T>
    std::string Tensor<T>::typeName()
    {
#ifdef __GNUG__
        int status = 0;
        std::unique_ptr<char, void (*)(void *)> demangled(
            abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status),
            std::free);
        return status == 0 && demangled ? std::string(demangled.get()) : typeid(T).name();
#else
        return typeid(T).name();
#endif
    }

    template <typename T>
    void Tensor<T>::attach(void *ptr, Deleter del, void *ctx) noexcept
    {
        if (ptr == data_ && del == deleter_ && ctx == deleter_ctx_)
            return;

#ifndef NDEBUG
        if (ptr)
        {
            auto p = reinterpret_cast<std::uintptr_t>(ptr);
            assert((p % alignof(T)) == 0 && "attach(): alignment violation");
        }
#endif
        dispose(); // clear previous
        data_ = ptr;
        deleter_ = del;
        deleter_ctx_ = ctx;
        owning_ = (ptr != nullptr && deleter_ != nullptr);
    }

    template <typename T>
    void *Tensor<T>::release() noexcept
    {
        void *out = data_;
        data_ = nullptr;
        deleter_ = nullptr;
        deleter_ctx_ = nullptr;
        owning_ = false;
        return out;
    }

    // 이동 생성자 & 이동 대입 연산자 오버라이딩
    // other: 기존 객체
    template <typename T>
    Tensor<T>::Tensor(Tensor &&other) noexcept
        : name_{std::move(other.name_)},
          data_{std::exchange(other.data_, nullptr)},
          deleter_{std::exchange(other.deleter_, nullptr)},
          deleter_ctx_{std::exchange(other.deleter_ctx_, nullptr)},
          owning_{std::exchange(other.owning_, false)}
    {
        for (int i = 0; i < GAP_DIMS; ++i)
        {
            ne_[i] = std::exchange(other.ne_[i], 0);
            nb_[i] = std::exchange(other.nb_[i], 0);
        }
    }

    template <typename T>
    Tensor<T> &Tensor<T>::operator=(Tensor &&other) noexcept
    {
        if (this != &other)
        {
            dispose();
            name_ = std::move(other.name_);
            data_ = std::exchange(other.data_, nullptr);
            deleter_ = std::exchange(other.deleter_, nullptr);
            deleter_ctx_ = std::exchange(other.deleter_ctx_, nullptr);
            owning_ = std::exchange(other.owning_, false);

            for (int i = 0; i < GAP_DIMS; ++i)
            {
                ne_[i] = std::exchange(other.ne_[i], 0);
                nb_[i] = std::exchange(other.nb_[i], 0);
            }
        }
        return *this;
    }

    template <typename T>
    void Tensor<T>::dispose() noexcept
    {
        if (owning_ && data_ && deleter_)
            deleter_(data_, deleter_ctx_);

        data_ = nullptr;
        deleter_ = nullptr;
        deleter_ctx_ = nullptr;
        owning_ = false;
    }
}