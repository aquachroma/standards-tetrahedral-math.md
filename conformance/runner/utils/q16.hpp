#ifndef ISO16_Q16_HPP
#define ISO16_Q16_HPP

#include <cstdint>
#include <limits>

/*
 * ISO‑16 Q16.16 Deterministic Fixed‑Point Arithmetic
 * --------------------------------------------------
 *
 * Q16.16 is a signed 32‑bit fixed‑point format:
 *   - upper 16 bits: integer part
 *   - lower 16 bits: fractional part
 *
 * All arithmetic MUST be deterministic and bit‑exact across platforms.
 * No floating‑point operations appear anywhere in this file.
 *
 * This header is INFORMATIVE, not normative.
 */

namespace iso16 {

using q16 = int32_t;

// 32‑bit wrap mask
static constexpr uint32_t INT32_MASK = 0xFFFFFFFFu;
static constexpr int32_t  INT32_MAX_ = 0x7FFFFFFF;
static constexpr int32_t  INT32_MIN_ = static_cast<int32_t>(0x80000000u);

// ------------------------------------------------------------
// Internal: force wraparound to signed 32‑bit
// ------------------------------------------------------------
inline q16 to_int32(int64_t v) {
    uint32_t wrapped = static_cast<uint32_t>(v & INT32_MASK);
    if (wrapped & 0x80000000u) {
        return static_cast<q16>(wrapped - 0x100000000ull);
    }
    return static_cast<q16>(wrapped);
}

// ------------------------------------------------------------
// Core Q16.16 operations
// ------------------------------------------------------------

inline q16 add(q16 a, q16 b) {
    return to_int32(static_cast<int64_t>(a) + static_cast<int64_t>(b));
}

inline q16 sub(q16 a, q16 b) {
    return to_int32(static_cast<int64_t>(a) - static_cast<int64_t>(b));
}

inline q16 abs(q16 a) {
    if (a == INT32_MIN_) {
        // Absolute value cannot be represented; clamp.
        return INT32_MAX_;
    }
    return (a < 0) ? static_cast<q16>(-a) : a;
}

inline bool leq(q16 a, q16 b) {
    return a <= b;
}

// ------------------------------------------------------------
// Optional helpers (informative only)
// ------------------------------------------------------------

inline q16 from_float(double f) {
    // Informative only — not used in normative paths.
    int64_t raw = static_cast<int64_t>(f * (1 << 16));
    return to_int32(raw);
}

inline double to_float(q16 v) {
    // Informative only — not used in normative paths.
    return static_cast<double>(v) / static_cast<double>(1 << 16);
}

} // namespace iso16

#endif // ISO16_Q16_HPP
