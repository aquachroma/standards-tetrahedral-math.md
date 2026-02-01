#pragma once
#include "iso16_lattice.h"
#include "iso16_plugin.h"
#include "iso16_face_audit.h"
#include <vector>

namespace iso16 {

struct TrueDeliveryResult {
    bool executed = false;
    bool true_state = false;
    double error_metric = 0.0;
    std::string failure_reason;
};

// Informative reference: poll plugins, compose corrections, check truth, execute/inhibit.
inline TrueDeliveryResult true_delivery_loop(
    PhaseState& inout_state,
    std::vector<IPlugin*>& plugins,
    double epsilon = ISO16_EPSILON
) {
    TrueDeliveryResult r{};

    // 1) Poll/resolve plugins
    for (auto* p : plugins) {
        if (!p) continue;
        auto st = p->resolve();
        if (st != PluginStatus::OK) {
            r.executed = false;
            r.true_state = false;
            r.failure_reason = "PLUGIN_NON_OK";
            return r;
        }
    }

    // 2) Apply a minimal placeholder "warp": sum a tiny correction derived from plugin output size.
    // Replace with real face-wise prewarp vectors in a full implementation.
    double corr = 0.0;
    for (auto* p : plugins) {
        auto m = p->manifest();
        corr += static_cast<double>(m.outputs.size()) * (epsilon / 10.0);
    }
    for (auto& f : inout_state.faces) {
        f.v = f.v - corr; // move toward 0
    }

    // 3) Parity check
    r.error_metric = error_metric_Linf(inout_state);
    r.true_state = r.error_metric < epsilon;

    // 4) Execute/Inhibit
    if (r.true_state) {
        r.executed = true; // placeholder for actuation call
    } else {
        r.executed = false;
        r.failure_reason = "PARITY_FAIL";
    }
    return r;
}

} // namespace iso16
