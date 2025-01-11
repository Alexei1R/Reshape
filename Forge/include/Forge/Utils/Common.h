// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef COMMON_H
#define COMMON_H

#include "error.h"
#include <expected>
#include <iostream>
#include <memory>

#if defined(__linux__) && (defined(__GNUC__) || defined(__clang__))
#    define DEBUG_BREAK() std::abort()
#elif defined(_MSC_VER)
#    define DEBUG_BREAK() __debugbreak()
#else
#    define DEBUG_BREAK() std::abort()
#endif

// Assertion macro with message
#define FORGE_ASSERT(value, message)                                                                                    \
    if (!(value)) {                                                                                                     \
        std::cerr << "[CRITICAL ERROR]\n FILE " << __FILE__ << "\n LINE :" << __LINE__ << "\t" << message << std::endl; \
        DEBUG_BREAK();                                                                                                  \
    }

//
// NOTE: In Feature may want to create custom allocators
//

template <typename T>
using Unique = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Unique<T> CreateUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Shared<T> CreateShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif
