#pragma once
#include "iso16_lattice.h"
#include <cmath>

namespace iso16 {

// Informative reference error metric.
// NOTE: Spec requires properties; exact metric may vary by implementation.
inline double error_metric_Linf(const PhaseState& s) {
    double m = 0.0;
    for (const auto& f : s.faces) {
        m = std::max(m, std::abs(f.v));
    }
    return m;
}

// Informative "truth" helper consistent with spec defaults.
inline bool is_true(const PhaseState& s, double epsilon = ISO16_EPSILON) {
    return error_metric_Linf(s) < epsilon;
}

} // namespace iso16
