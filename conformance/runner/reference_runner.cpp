// ISO‑16 Reference Runner (Informative)
// -------------------------------------
// C++ reference implementation mirroring runner/reference_runner.py.
// This is INFORMATIVE, not normative. Normative behavior is defined
// in the ISO‑16 spec documents under spec/.
//
// Responsibilities:
// 1. Load a conformance vector (JSON)
// 2. Execute the ISO‑16 state machine:
//      LOAD_PHASE_STATE
//      EVAL_PLUGINS
//      ACCUMULATE_WARP
//      APPLY_WARP
//      CHECK_SYMMETRY
//      CHECK_ERROR
//      DECIDE_TRUE_FALSE
// 3. Compare against expected outputs (JSON)
// 4. (Optionally) integrate with a seal implementation
//
// Dependencies: nlohmann/json (header-only JSON library)
//   https://github.com/nlohmann/json
//
// Build example:
//   g++ -std=c++17 -O2 -o iso16_runner reference_runner.cpp
//   ./iso16_runner ../conformance/vectors/V0001.json ../conformance/expected/V0001_expected.json

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <cmath>
#include <stdexcept>

#include "json.hpp"  // nlohmann::json

using json = nlohmann::json;

// Q16.16 is represented as int32_t
using q16 = int32_t;

// Canonical epsilon (Q16.16)
static constexpr q16 EPSILON = 1; // 0x00000001

// ---------------------- Q16.16 helpers ----------------------

inline q16 q16_add(q16 a, q16 b) {
    return static_cast<q16>(a + b);
}

inline q16 q16_sub(q16 a, q16 b) {
    return static_cast<q16>(a - b);
}

inline q16 q16_abs(q16 a) {
    return (a < 0) ? static_cast<q16>(-a) : a;
}

inline bool q16_leq(q16 a, q16 b) {
    return a <= b;
}

// ---------------------- Types ----------------------

struct Phase {
    q16 x;
    q16 y;
    q16 z;
};

struct Plugin {
    std::string id;
    std::string domain;
    q16 warp_x;
    q16 warp_y;
    q16 warp_z;
    q16 error;
    std::string version;
    std::string status;
};

struct VectorInput {
    std::string vector_id;
    std::string description;
    std::vector<Phase> initial_phase_state;
    std::vector<Plugin> plugins;
};

struct ExpectedOutput {
    std::string vector_id;
    std::vector<q16> warp_total;          // size 3
    q16 error_total;
    std::vector<Phase> phase_state_warped; // size 16
    bool symmetry_ok;
    bool error_ok;
    bool true_delivery;
    std::string tetra_seal;              // hex string
};

struct ActualOutput {
    std::vector<q16> warp_total;          // size 3
    q16 error_total;
    std::vector<Phase> phase_state_warped;
    bool symmetry_ok;
    bool error_ok;
    bool true_delivery;
};

// ---------------------- JSON helpers ----------------------

json load_json_file(const std::string &path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    json j;
    f >> j;
    return j;
}

VectorInput parse_vector(const json &j) {
    VectorInput v;
    v.vector_id = j.at("vector_id").get<std::string>();
    v.description = j.value("description", "");

    const auto &phases = j.at("initial_phase_state");
    if (!phases.is_array() || phases.size() != 16) {
        throw std::runtime_error("initial_phase_state must have 16 phases");
    }

    v.initial_phase_state.reserve(16);
    for (const auto &p : phases) {
        if (!p.is_array() || p.size() != 3) {
            throw std::runtime_error("Each phase must be an array of 3 Q16.16 integers");
        }
        Phase ph{
            p[0].get<q16>(),
            p[1].get<q16>(),
            p[2].get<q16>()
        };
        v.initial_phase_state.push_back(ph);
    }

    const auto &plugins_obj = j.at("plugins");
    if (!plugins_obj.is_object()) {
        throw std::runtime_error("plugins must be an object");
    }

    for (auto it = plugins_obj.begin(); it != plugins_obj.end(); ++it) {
        const auto &pj = it.value();
        Plugin pl;
        pl.id = pj.at("id").get<std::string>();
        pl.domain = pj.at("domain").get<std::string>();
        const auto &wv = pj.at("warp_vector");
        if (!wv.is_array() || wv.size() != 3) {
            throw std::runtime_error("warp_vector must be array[3]");
        }
        pl.warp_x = wv[0].get<q16>();
        pl.warp_y = wv[1].get<q16>();
        pl.warp_z = wv[2].get<q16>();
        pl.error = pj.at("error").get<q16>();
        pl.version = pj.at("version").get<std::string>();
        pl.status = pj.at("status").get<std::string>();
        v.plugins.push_back(pl);
    }

    return v;
}

ExpectedOutput parse_expected(const json &j) {
    ExpectedOutput e;
    e.vector_id = j.at("vector_id").get<std::string>();

    const auto &wt = j.at("warp_total");
    if (!wt.is_array() || wt.size() != 3) {
        throw std::runtime_error("warp_total must be array[3]");
    }
    e.warp_total = { wt[0].get<q16>(), wt[1].get<q16>(), wt[2].get<q16>() };

    e.error_total = j.at("error_total").get<q16>();

    const auto &phases = j.at("phase_state_warped");
    if (!phases.is_array() || phases.size() != 16) {
        throw std::runtime_error("phase_state_warped must have 16 phases");
    }
    e.phase_state_warped.reserve(16);
    for (const auto &p : phases) {
        if (!p.is_array() || p.size() != 3) {
            throw std::runtime_error("Each warped phase must be array[3]");
        }
        Phase ph{
            p[0].get<q16>(),
            p[1].get<q16>(),
            p[2].get<q16>()
        };
        e.phase_state_warped.push_back(ph);
    }

    e.symmetry_ok = j.at("symmetry_ok").get<bool>();
    e.error_ok = j.at("error_ok").get<bool>();
    e.true_delivery = j.at("true_delivery").get<bool>();
    e.tetra_seal = j.at("tetra_seal").get<std::string>();

    return e;
}

// ---------------------- ISO‑16 logic ----------------------

std::tuple<std::vector<q16>, q16, bool> eval_plugins(const std::vector<Plugin> &plugins) {
    std::vector<q16> warp_total(3, 0);
    q16 error_total = 0;
    bool plugin_status_ok = true;

    for (const auto &p : plugins) {
        warp_total[0] = q16_add(warp_total[0], p.warp_x);
        warp_total[1] = q16_add(warp_total[1], p.warp_y);
        warp_total[2] = q16_add(warp_total[2], p.warp_z);

        error_total = q16_add(error_total, p.error);

        if (p.status != "OK") {
            plugin_status_ok = false;
        }
    }

    return {warp_total, error_total, plugin_status_ok};
}

std::vector<Phase> apply_warp(const std::vector<Phase> &phases, const std::vector<q16> &warp) {
    std::vector<Phase> warped;
    warped.reserve(phases.size());
    for (const auto &p : phases) {
        Phase w{
            q16_add(p.x, warp[0]),
            q16_add(p.y, warp[1]),
            q16_add(p.z, warp[2])
        };
        warped.push_back(w);
    }
    return warped;
}

bool check_symmetry(const std::vector<Phase> &phases) {
    if (phases.size() != 16) return false;
    for (size_t i = 0; i < 15; ++i) {
        const auto &p1 = phases[i];
        const auto &p2 = phases[i + 1];

        q16 dx = q16_abs(q16_sub(p1.x, p2.x));
        q16 dy = q16_abs(q16_sub(p1.y, p2.y));
        q16 dz = q16_abs(q16_sub(p1.z, p2.z));

        if (!(q16_leq(dx, EPSILON) && q16_leq(dy, EPSILON) && q16_leq(dz, EPSILON))) {
            return false;
        }
    }
    return true;
}

bool check_error(q16 error_total) {
    return q16_leq(error_total, EPSILON);
}

ActualOutput execute_iso16(const VectorInput &v) {
    auto [warp_total, error_total, plugin_status_ok] = eval_plugins(v.plugins);
    auto warped = apply_warp(v.initial_phase_state, warp_total);
    bool symmetry_ok = check_symmetry(warped);
    bool error_ok = plugin_status_ok && check_error(error_total);
    bool true_delivery = symmetry_ok && error_ok;

    ActualOutput out;
    out.warp_total = warp_total;
    out.error_total = error_total;
    out.phase_state_warped = warped;
    out.symmetry_ok = symmetry_ok;
    out.error_ok = error_ok;
    out.true_delivery = true_delivery;
    return out;
}

// ---------------------- Comparison ----------------------

bool phases_equal(const std::vector<Phase> &a, const std::vector<Phase> &b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].x != b[i].x || a[i].y != b[i].y || a[i].z != b[i].z) {
            return false;
        }
    }
    return true;
}

void compare_results(const ActualOutput &actual, const ExpectedOutput &expected,
                     std::vector<std::string> &mismatches) {
    if (actual.warp_total.size() != 3 ||
        actual.warp_total[0] != expected.warp_total[0] ||
        actual.warp_total[1] != expected.warp_total[1] ||
        actual.warp_total[2] != expected.warp_total[2]) {
        mismatches.push_back("warp_total");
    }

    if (actual.error_total != expected.error_total) {
        mismatches.push_back("error_total");
    }

    if (!phases_equal(actual.phase_state_warped, expected.phase_state_warped)) {
        mismatches.push_back("phase_state_warped");
    }

    if (actual.symmetry_ok != expected.symmetry_ok) {
        mismatches.push_back("symmetry_ok");
    }

    if (actual.error_ok != expected.error_ok) {
        mismatches.push_back("error_ok");
    }

    if (actual.true_delivery != expected.true_delivery) {
        mismatches.push_back("true_delivery");
    }
}

// ---------------------- Main ----------------------

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: reference_runner <vector.json> <expected.json>\n";
        return 1;
    }

    try {
        std::string vector_path = argv[1];
        std::string expected_path = argv[2];

        json j_vec = load_json_file(vector_path);
        json j_exp = load_json_file(expected_path);

        VectorInput vec = parse_vector(j_vec);
        ExpectedOutput exp = parse_expected(j_exp);

        ActualOutput act = execute_iso16(vec);

        std::vector<std::string> mismatches;
        compare_results(act, exp, mismatches);

        std::cout << "\n=== ISO‑16 Conformance Result: " << vec.vector_id << " ===\n";
        if (!mismatches.empty()) {
            std::cout << "❌ FAIL\n";
            std::cout << "Mismatched fields: ";
            for (size_t i = 0; i < mismatches.size(); ++i) {
                std::cout << mismatches[i];
                if (i + 1 < mismatches.size()) std::cout << ", ";
            }
            std::cout << "\n";
        } else {
            std::cout << "✅ PASS — All fields match canonical expected output\n";
        }

    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
