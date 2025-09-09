#pragma once
#include "tensor.hpp"
#include <typeinfo>
#include <stdexcept>
#include <memory>   // std::unique_ptr
#include <cstdlib>  // std::free

#ifdef __GNUG__
  #include <cxxabi.h> // abi::__cxa_demangle
#endif

namespace gap
{
    template <typename T>
    Tensor<T>::Tensor(const std::string &name, int rows, int cols)
        : name_{name}
    {
        ne_[0] = rows;
        ne_[1] = cols;
        nb_[1] = sizeof(T);       // col stride: 1 element
        nb_[0] = ne_[1] * nb_[1]; // row stride: cols * sizeof(T)
        // TODO: data                data_는 필요할 때 new/delete로 관리
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
    Tensor<T>::~Tensor() {
        
    }
}