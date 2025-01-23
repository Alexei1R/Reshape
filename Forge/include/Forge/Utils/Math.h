// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef MATH_H
#define MATH_H

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_CTOR_INIT

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <type_traits>

namespace forge {
namespace math {

// Type aliases for common vector types
template <typename T>
using vec2 = glm::vec<2, T>;

template <typename T>
using vec3 = glm::vec<3, T>;

template <typename T>
using vec4 = glm::vec<4, T>;

// Common type definitions
using vec2i = vec2<int32_t>;
using vec2f = vec2<float>;
using vec2d = vec2<double>;
using vec2u = vec2<uint32_t>;

using vec3i = vec3<int32_t>;
using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec3u = vec3<uint32_t>;

using vec4i = vec4<int32_t>;
using vec4f = vec4<float>;
using vec4d = vec4<double>;
using vec4u = vec4<uint32_t>;

// Matrix types with template alias for flexibility
template <typename T>
using mat3 = glm::mat<3, 3, T>;

template <typename T>
using mat4 = glm::mat<4, 4, T>;

using mat3f = mat3<float>;
using mat3d = mat3<double>;
using mat4f = mat4<float>;
using mat4d = mat4<double>;

// Quaternion types
template <typename T>
using quat = glm::tquat<T, glm::defaultp>; // Fixed: Using correct GLM quaternion type

using quatf = quat<float>;
using quatd = quat<double>;

// Constants
template <typename T>
inline constexpr T PI = T(3.14159265358979323846);

template <typename T>
inline constexpr T EPSILON = std::is_same_v<T, float> ? T(1e-6) : T(1e-12);

// Transformation functions
template <typename T>
[[nodiscard]] constexpr mat4<T> translate(const mat4<T>& matrix, const vec3<T>& translation) noexcept {
    return glm::translate(matrix, translation);
}

template <typename T>
[[nodiscard]] constexpr mat4<T> rotate(const mat4<T>& matrix, T angle, const vec3<T>& axis) noexcept {
    return glm::rotate(matrix, angle, glm::normalize(axis));
}

template <typename T>
[[nodiscard]] constexpr mat4<T> scale(const mat4<T>& matrix, const vec3<T>& scale) noexcept {
    return glm::scale(matrix, scale);
}

// Camera functions
template <typename T>
[[nodiscard]] mat4<T> perspective(T fov, T aspect, T near, T far) noexcept {
    assert(near > T(0) && far > near && "Invalid near/far planes");
    return glm::perspective(fov, aspect, near, far);
}

template <typename T>
[[nodiscard]] mat4<T> lookAt(const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up) noexcept {
    return glm::lookAt(eye, center, glm::normalize(up));
}

template <typename T>
[[nodiscard]] mat4<T> ortho(T left, T right, T bottom, T top, T near, T far) noexcept {
    assert(near < far && "Invalid near/far planes");
    return glm::ortho(left, right, bottom, top, near, far);
}

// Matrix operations
template <typename T>
[[nodiscard]] constexpr mat4<T> identity() noexcept {
    return mat4<T>(T(1));
}

// Angle conversions
template <typename T>
[[nodiscard]] constexpr T radians(T degrees) noexcept {
    return glm::radians(degrees);
}

template <typename T>
[[nodiscard]] constexpr T degrees(T radians) noexcept {
    return glm::degrees(radians);
}

// Vector operations
template <typename T>
[[nodiscard]] constexpr vec3<T> normalize(const vec3<T>& v) noexcept {
    return glm::normalize(v);
}

template <typename T>
[[nodiscard]] constexpr T dot(const vec3<T>& a, const vec3<T>& b) noexcept {
    return glm::dot(a, b);
}

template <typename T>
[[nodiscard]] constexpr vec3<T> cross(const vec3<T>& a, const vec3<T>& b) noexcept {
    return glm::cross(a, b);
}

// Quaternion operations
template <typename T>
[[nodiscard]] quat<T> quaternion(const vec3<T>& axis, T angle) noexcept {
    return glm::angleAxis(angle, glm::normalize(axis));
}

template <typename T>
[[nodiscard]] quat<T> quaternion(const mat4<T>& matrix) noexcept {
    return glm::quat_cast(matrix);
}

template <typename T>
[[nodiscard]] mat4<T> toMat4(const quat<T>& q) noexcept {
    return glm::mat4_cast(q);
}

template <typename T>
[[nodiscard]] constexpr quat<T> normalize(const quat<T>& q) noexcept {
    return glm::normalize(q);
}

template <typename T>
[[nodiscard]] quat<T> slerp(const quat<T>& a, const quat<T>& b, T t) noexcept {
    return glm::slerp(a, b, std::clamp(t, T(0), T(1)));
}

// Utility functions
template <typename T>
[[nodiscard]] constexpr bool approxEqual(T a, T b, T epsilon = EPSILON<T>) noexcept {
    return std::abs(a - b) <= epsilon;
}

template <typename T>
[[nodiscard]] constexpr vec3<T> lerp(const vec3<T>& a, const vec3<T>& b, T t) noexcept {
    return a + (b - a) * std::clamp(t, T(0), T(1));
}

// Transform composition
template <typename T>
[[nodiscard]] constexpr mat4<T> combine(const mat4<T>& a, const mat4<T>& b) noexcept {
    return a * b;
}

} // namespace math
} // namespace forge

#endif
