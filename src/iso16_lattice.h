#pragma once
#include <array>
#include <cstdint>
#include <string>

namespace iso16 {

static constexpr uint32_t ISO16_FACES = 16;
static constexpr uint32_t ISO16_RESOLUTION = 65536;
static constexpr double ISO16_EPSILON = 1.0 / static_cast<double>(ISO16_RESOLUTION);

struct FaceValue {
    // Minimal scalar placeholder. Implementations may replace with vector/quaternion etc.
    double v = 0.0;
};

struct PhaseState {
    std::array<FaceValue, ISO16_FACES> faces{};

    std::string to_string() const; // informative debugging
};

struct LatticeCell {
    // Placeholder for tetra vertices etc. Keep it minimal for v0.1.
    // In a full impl, include vertex coordinates and calibration params.
    uint32_t id = 0;
    PhaseState state;
};

} // namespace iso16
