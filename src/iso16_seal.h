#pragma once
#include "iso16_lattice.h"
#include "iso16_plugin.h"
#include <string>
#include <vector>

namespace iso16 {

// Informative: In a full implementation, this must strictly follow ISO16-SEAL payload rules.
std::string compute_seal_sha256_hex(
    const std::string& spec_version,
    const PhaseState& state,
    const std::vector<PluginManifest>& plugins_sorted_by_id,
    uint32_t resolution = ISO16_RESOLUTION,
    double epsilon = ISO16_EPSILON
);

} // namespace iso16
