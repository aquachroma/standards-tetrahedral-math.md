#ifndef ISO16_SEAL_HPP
#define ISO16_SEAL_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cstring>

// Requires a SHA3‑256 implementation.
// You may plug in any deterministic SHA3‑256 function with signature:
//
//     std::vector<uint8_t> sha3_256(const uint8_t* data, size_t len);
//
// This header assumes such a function exists.

namespace iso16 {

using q16 = int32_t;

// ------------------------------------------------------------
// Encoding helpers
// ------------------------------------------------------------

inline void append_be32(std::vector<uint8_t>& out, q16 value) {
    // Big‑endian 32‑bit signed integer
    out.push_back((value >> 24) & 0xFF);
    out.push_back((value >> 16) & 0xFF);
    out.push_back((value >>  8) & 0xFF);
    out.push_back((value >>  0) & 0xFF);
}

inline void append_bool(std::vector<uint8_t>& out, bool b) {
    out.push_back(b ? 0x01 : 0x00);
}

inline void append_string(std::vector<uint8_t>& out, const std::string& s) {
    // UTF‑8, no terminator
    out.insert(out.end(), s.begin(), s.end());
}

inline void append_length_prefixed_string(std::vector<uint8_t>& out,
                                          const std::string& s) {
    if (s.size() > 255) {
        throw std::runtime_error("String too long for canonical encoding");
    }
    out.push_back(static_cast<uint8_t>(s.size()));
    out.insert(out.end(), s.begin(), s.end());
}

inline void append_be64(std::vector<uint8_t>& out, uint64_t v) {
    for (int i = 7; i >= 0; --i) {
        out.push_back((v >> (8 * i)) & 0xFF);
    }
}

// ------------------------------------------------------------
// Canonical serialization
// ------------------------------------------------------------
//
// The caller must provide:
//   - initial_phase_state: vector<array<q16,3>> size 16
//   - plugins: vector<Plugin> (already sorted lexicographically by id)
//   - warp_total: array<q16,3>
//   - error_total: q16
//   - phase_state_warped: vector<array<q16,3>> size 16
//   - symmetry_ok, error_ok, true_delivery
//   - implementation_id
//   - timestamp (uint64)
//   - nonce (16 bytes)
//
// This function returns the canonical byte sequence BEFORE hashing.
//

struct Plugin {
    std::string id;
    std::string domain;   // "Refraction", "FrameDrag", "Jitter", "Custom"
    q16 warp_x;
    q16 warp_y;
    q16 warp_z;
    q16 error;
    std::string version;
};

inline uint8_t domain_code(const std::string& d) {
    if (d == "Refraction") return 0x01;
    if (d == "FrameDrag")  return 0x02;
    if (d == "Jitter")     return 0x03;
    return 0xFF; // Custom
}

inline std::vector<uint8_t> canonical_serialize(
    const std::vector<std::array<q16,3>>& initial_phase_state,
    const std::vector<Plugin>& plugins_sorted,
    const std::array<q16,3>& warp_total,
    q16 error_total,
    const std::vector<std::array<q16,3>>& phase_state_warped,
    bool symmetry_ok,
    bool error_ok,
    bool true_delivery,
    const std::string& implementation_id,
    uint64_t timestamp,
    const std::array<uint8_t,16>& nonce
) {
    std::vector<uint8_t> out;
    out.reserve(4096); // avoid reallocations

    // --------------------------------------------------------
    // 1. phase_state_initial (16×3 Q16.16)
    // --------------------------------------------------------
    for (const auto& p : initial_phase_state) {
        append_be32(out, p[0]);
        append_be32(out, p[1]);
        append_be32(out, p[2]);
    }

    // --------------------------------------------------------
    // 2. plugin_outputs (already sorted lexicographically)
    // --------------------------------------------------------
    for (const auto& p : plugins_sorted) {
        append_length_prefixed_string(out, p.id);
        out.push_back(domain_code(p.domain));

        append_be32(out, p.warp_x);
        append_be32(out, p.warp_y);
        append_be32(out, p.warp_z);

        append_be32(out, p.error);

        append_length_prefixed_string(out, p.version);
    }

    // --------------------------------------------------------
    // 3. warp_total (3×Q16.16)
    // --------------------------------------------------------
    append_be32(out, warp_total[0]);
    append_be32(out, warp_total[1]);
    append_be32(out, warp_total[2]);

    // --------------------------------------------------------
    // 4. error_total (Q16.16)
    // --------------------------------------------------------
    append_be32(out, error_total);

    // --------------------------------------------------------
    // 5. phase_state_warped (16×3 Q16.16)
    // --------------------------------------------------------
    for (const auto& p : phase_state_warped) {
        append_be32(out, p[0]);
        append_be32(out, p[1]);
        append_be32(out, p[2]);
    }

    // --------------------------------------------------------
    // 6. symmetry_ok
    // --------------------------------------------------------
    append_bool(out, symmetry_ok);

    // --------------------------------------------------------
    // 7. error_ok
    // --------------------------------------------------------
    append_bool(out, error_ok);

    // --------------------------------------------------------
    // 8. true_delivery
    // --------------------------------------------------------
    append_bool(out, true_delivery);

    // --------------------------------------------------------
    // 9. implementation_id
    // --------------------------------------------------------
    append_string(out, implementation_id);

    // --------------------------------------------------------
    // 10. timestamp (uint64 big‑endian)
    // --------------------------------------------------------
    append_be64(out, timestamp);

    // --------------------------------------------------------
    // 11. nonce (16 bytes)
    // --------------------------------------------------------
    out.insert(out.end(), nonce.begin(), nonce.end());

    return out;
}

// ------------------------------------------------------------
// Seal hashing
// ------------------------------------------------------------
//
// The caller must provide a SHA3‑256 implementation.
// This function returns a lowercase hex string.
//

inline std::string canonical_serialize_and_hash(
    const std::vector<std::array<q16,3>>& initial_phase_state,
    const std::vector<Plugin>& plugins_sorted,
    const std::array<q16,3>& warp_total,
    q16 error_total,
    const std::vector<std::array<q16,3>>& phase_state_warped,
    bool symmetry_ok,
    bool error_ok,
    bool true_delivery,
    const std::string& implementation_id,
    uint64_t timestamp,
    const std::array<uint8_t,16>& nonce,
    std::vector<uint8_t> (*sha3_256)(const uint8_t*, size_t)
) {
    static const std::string prefix = "ISO16-SEAL-V1:";

    std::vector<uint8_t> body = canonical_serialize(
        initial_phase_state,
        plugins_sorted,
        warp_total,
        error_total,
        phase_state_warped,
        symmetry_ok,
        error_ok,
        true_delivery,
        implementation_id,
        timestamp,
        nonce
    );

    std::vector<uint8_t> prefixed;
    prefixed.reserve(prefix.size() + body.size());
    prefixed.insert(prefixed.end(), prefix.begin(), prefix.end());
    prefixed.insert(prefixed.end(), body.begin(), body.end());

    std::vector<uint8_t> digest = sha3_256(prefixed.data(), prefixed.size());

    // Convert to lowercase hex
    static const char* hex = "0123456789abcdef";
    std::string hexout;
    hexout.reserve(digest.size() * 2);
    for (uint8_t b : digest) {
        hexout.push_back(hex[(b >> 4) & 0xF]);
        hexout.push_back(hex[b & 0xF]);
    }
    return hexout;
}

} // namespace iso16

#endif // ISO16_SEAL_HPP
