#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace iso16 {

enum class PluginStatus {
    OK = 0,
    INSUFFICIENT_DATA,
    OUT_OF_RANGE,
    TIMEOUT,
    SENSOR_MISMATCH
};

struct PluginManifest {
    std::string plugin_id;
    std::string name;
    std::string version;
    std::string domain;

    // Minimal typed-ish representation (informative).
    std::unordered_map<std::string, std::string> inputs;
    std::unordered_map<std::string, std::string> outputs;

    std::string timestamp_basis_json; // opaque blob for now
    std::string digest_sha256_hex;
    PluginStatus status = PluginStatus::OK;
};

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual PluginManifest manifest() const = 0;

    // Apply plugin corrections to an internal buffer; left abstract for v0.1.
    virtual PluginStatus resolve() = 0;
};

} // namespace iso16
