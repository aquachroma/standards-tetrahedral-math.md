#!/usr/bin/env python3
"""
ISO‑16 Reference Runner (Informative)
-------------------------------------

This reference implementation demonstrates how to:

1. Load a conformance vector
2. Validate it against vector_schema.json
3. Execute the ISO‑16 deterministic state machine:
      LOAD_PHASE_STATE
      EVAL_PLUGINS
      ACCUMULATE_WARP
      APPLY_WARP
      CHECK_SYMMETRY
      CHECK_ERROR
      DECIDE_TRUE_FALSE
4. Generate a canonical audit record
5. Compute the Tetra‑Seal
6. Compare results to expected outputs

This file is *informative* and does not replace the normative rules
defined in the ISO‑16 specification.
"""

import json
import hashlib
from pathlib import Path

from utils.q16 import q16_add, q16_sub, q16_abs, q16_leq
from utils.schema_validate import validate_json
from utils.seal import canonical_serialize_and_hash

# Canonical epsilon (Q16.16)
EPSILON = 1  # 0x00000001


# ------------------------------------------------------------
# Load and validate vector + expected output
# ------------------------------------------------------------
def load_vector(path):
    with open(path, "r") as f:
        data = json.load(f)
    validate_json(data, "conformance/schema/vector_schema.json")
    return data


def load_expected(path):
    with open(path, "r") as f:
        data = json.load(f)
    validate_json(data, "conformance/schema/expected_schema.json")
    return data


# ------------------------------------------------------------
# ISO‑16 State Machine Steps
# ------------------------------------------------------------

def eval_plugins(plugins):
    """Return (warp_total, error_total) as Q16.16 integers."""
    warp_total = [0, 0, 0]
    error_total = 0

    for p in plugins.values():
        # Accumulate warp
        warp_total = [
            q16_add(warp_total[0], p["warp_vector"][0]),
            q16_add(warp_total[1], p["warp_vector"][1]),
            q16_add(warp_total[2], p["warp_vector"][2]),
        ]

        # Accumulate error
        error_total = q16_add(error_total, p["error"])

        # Plugin status check
        if p["status"] != "OK":
            # Per iso16_core.md §8.2
            return warp_total, error_total, False

    return warp_total, error_total, True


def apply_warp(phase_state, warp):
    """Apply warp vector to all 16 phases."""
    warped = []
    for (x, y, z) in phase_state:
        warped.append([
            q16_add(x, warp[0]),
            q16_add(y, warp[1]),
            q16_add(z, warp[2])
        ])
    return warped


def check_symmetry(phase_state):
    """Check |phase[i] - phase[i+1]| <= epsilon for all i."""
    for i in range(15):
        p1 = phase_state[i]
        p2 = phase_state[i + 1]

        dx = q16_abs(q16_sub(p1[0], p2[0]))
        dy = q16_abs(q16_sub(p1[1], p2[1]))
        dz = q16_abs(q16_sub(p1[2], p2[2]))

        # Vector norm surrogate: max component
        if not (q16_leq(dx, EPSILON) and q16_leq(dy, EPSILON) and q16_leq(dz, EPSILON)):
            return False
    return True


def check_error(error_total):
    """Check error_total <= epsilon."""
    return q16_leq(error_total, EPSILON)


# ------------------------------------------------------------
# Execute full ISO‑16 True Delivery Loop
# ------------------------------------------------------------
def execute_iso16(vector):
    phase_state_initial = vector["initial_phase_state"]
    plugins = vector["plugins"]

    # Step 1–3: Evaluate plugins and accumulate warp/error
    warp_total, error_total, plugin_status_ok = eval_plugins(plugins)

    # Step 4: Apply warp
    phase_state_warped = apply_warp(phase_state_initial, warp_total)

    # Step 5: Symmetry
    symmetry_ok = check_symmetry(phase_state_warped)

    # Step 6: Error
    error_ok = plugin_status_ok and check_error(error_total)

    # Step 7: TRUE/FALSE
    true_delivery = symmetry_ok and error_ok

    return {
        "warp_total": warp_total,
        "error_total": error_total,
        "phase_state_warped": phase_state_warped,
        "symmetry_ok": symmetry_ok,
        "error_ok": error_ok,
        "true_delivery": true_delivery
    }


# ------------------------------------------------------------
# Compare against expected outputs
# ------------------------------------------------------------
def compare_results(actual, expected):
    mismatches = []

    if actual["warp_total"] != expected["warp_total"]:
        mismatches.append("warp_total")

    if actual["error_total"] != expected["error_total"]:
        mismatches.append("error_total")

    if actual["phase_state_warped"] != expected["phase_state_warped"]:
        mismatches.append("phase_state_warped")

    if actual["symmetry_ok"] != expected["symmetry_ok"]:
        mismatches.append("symmetry_ok")

    if actual["error_ok"] != expected["error_ok"]:
        mismatches.append("error_ok")

    if actual["true_delivery"] != expected["true_delivery"]:
        mismatches.append("true_delivery")

    return mismatches


# ------------------------------------------------------------
# Main entry point
# ------------------------------------------------------------
def run(vector_path, expected_path):
    vector = load_vector(vector_path)
    expected = load_expected(expected_path)

    actual = execute_iso16(vector)

    # Compute seal
    tetra_seal = canonical_serialize_and_hash(
        vector,
        actual
    )

    # Compare seal
    seal_match = (tetra_seal == expected["tetra_seal"])

    mismatches = compare_results(actual, expected)

    print(f"\n=== ISO‑16 Conformance Result: {vector['vector_id']} ===")
    if mismatches or not seal_match:
        print("❌ FAIL")
        if mismatches:
            print("Mismatched fields:", mismatches)
        if not seal_match:
            print("Seal mismatch")
    else:
        print("✅ PASS — All fields and seal match canonical expected output")


if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: reference_runner.py <vector.json> <expected.json>")
        sys.exit(1)

    run(sys.argv[1], sys.argv[2])
    
# Example logic for waveform annotation export
def annotate_waveform_event(signal_name, value):
    color_map = {
        "warp": "Orange",
        "check": "Purple",
        "seal": "Red"
    }
    # Log formatted for GTKWave or ModelSim
    print(f"TIME: {current_time} | SIGNAL: {signal_name} | HEX: {hex(value)} | COLOR: {color_map.get(signal_name)}")
