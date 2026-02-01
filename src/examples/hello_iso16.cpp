#include <iostream>
#include <iomanip>
#include <vector>

#include "../iso16_lattice.h"
#include "../iso16_true_delivery.h"
#include "../iso16_plugin.h"
#include "../plugins/plugin_alpha.h"
#include "../plugins/plugin_beta.h"
#include "../plugins/plugin_gamma.h"
#include "../plugins/plugin_delta.h"
#include "../plugins/plugin_epsilon.h"

using namespace iso16;

int main() {
    // ---------------------------------------------------------------------
    // Construct initial phase state (slightly warped)
    // ---------------------------------------------------------------------
    PhaseState s;
    for (auto& f : s.faces) {
        f.v = 0.00002;   // small distortion
    }

    // ---------------------------------------------------------------------
    // Instantiate reference plugins
    // ---------------------------------------------------------------------
    PluginAlpha   alpha("P-ALPHA",   "Refraction");
    PluginBeta    beta("P-BETA",    "Frame Drag");
    PluginGamma   gamma("P-GAMMA",  "Jitter");
    PluginDelta   delta("P-DELTA",  "Drift");
    PluginEpsilon epsilon("P-EPS",  "Saturation");

    std::vector<IPlugin*> plugins{
        &alpha, &beta, &gamma, &delta, &epsilon
    };

    // ---------------------------------------------------------------------
    // Run the canonical True Delivery Loop
    // ---------------------------------------------------------------------
    auto result = true_delivery_loop(s, plugins);

    // ---------------------------------------------------------------------
    // Print results
    // ---------------------------------------------------------------------
    std::cout << "executed      = " << result.executed << "\n";
    std::cout << "true_delivery = " << result.true_state << "\n";
    std::cout << "error_metric  = " << result.error_metric << "\n";
    std::cout << "reason        = " << result.failure_reason << "\n";

    // Print seal in hex
    std::cout << "seal          = ";
    for (auto b : result.seal_bytes) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << "\n";

    return result.true_state ? 0 : 1;
}
