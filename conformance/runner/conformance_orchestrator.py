#!/usr/bin/env python3
"""
ISO-16 Conformance Orchestrator
-------------------------------
Orchestrates the execution of all conformance vectors against the
Reference Runner and validates the resulting Tetra-Seals.

Outputs:
  • Per-vector PASS/FAIL status
  • Aggregate JSON report (conformance_report.json)
"""

import argparse
import json
import pathlib
import sys
from datetime import datetime
from iso16_reference_runner import run_vector


def run_suite(strict: bool = False) -> int:
    base_dir = pathlib.Path(__file__).resolve().parent.parent
    vectors_dir = base_dir / "vectors"
    results_dir = base_dir / "conformance_results"
    results_dir.mkdir(exist_ok=True)

    vector_files = sorted(vectors_dir.glob("V*.json"))
    if not vector_files:
        print(f"[-] No vectors found in {vectors_dir}")
        return 1

    start_ts = datetime.now()
    print(f"[*] ISO-16 Conformance Suite Started: {start_ts}")
    print(f"[*] Found {len(vector_files)} vectors. Processing...\n")

    report = {
        "timestamp": start_ts.isoformat(),
        "summary": {"pass": 0, "fail": 0, "total": len(vector_files)},
        "details": []
    }

    print(f"{'Vector ID':<15} | {'Status':<10} | {'Seal Verification':<20} | {'Time (s)':>8}")
    print("-" * 80)

    for v_path in vector_files:
        with v_path.open() as f:
            vector_data = json.load(f)

        vector_id = vector_data.get("id", v_path.stem)
        expected_seal = vector_data.get("expected_seal", "").strip().lower()

        # Basic seal sanity check
        if len(expected_seal) != 64:
            print(f"{vector_id:<15} | {'FAIL':<10} | {'Invalid expected seal length':<20} | {'-':>8}")
            report["summary"]["fail"] += 1
            report["details"].append({
                "vector_id": vector_id,
                "status": "FAIL",
                "reason": "invalid_expected_seal_length",
                "expected": expected_seal,
                "actual": None,
                "elapsed_seconds": None
            })
            if strict:
                break
            continue

        # 1. Execute the Reference Runner
        t0 = datetime.now()
        run_vector(v_path, results_dir)
        elapsed = (datetime.now() - t0).total_seconds()

        # 2. Load actual result
        result_path = results_dir / f"{vector_id}_result.json"
        if not result_path.exists():
            print(f"{vector_id:<15} | {'FAIL':<10} | {'Missing result file':<20} | {elapsed:8.3f}")
            report["summary"]["fail"] += 1
            report["details"].append({
                "vector_id": vector_id,
                "status": "FAIL",
                "reason": "missing_result_file",
                "expected": expected_seal,
                "actual": None,
                "elapsed_seconds": elapsed
            })
            if strict:
                break
            continue

        with result_path.open() as f:
            actual_data = json.load(f)

        actual_seal = actual_data.get("seal_out", "").strip().lower()
        is_pass = (expected_seal == actual_seal)
        status = "PASS" if is_pass else "FAIL"

        if is_pass:
            report["summary"]["pass"] += 1
        else:
            report["summary"]["fail"] += 1

        # Short preview of the seal for the console
        preview = actual_seal[:16] + "..." if actual_seal else "N/A"

        print(f"{vector_id:<15} | {status:<10} | {preview:<20} | {elapsed:8.3f}")

        detail = {
            "vector_id": vector_id,
            "status": status,
            "expected": expected_seal,
            "actual": actual_seal,
            "elapsed_seconds": elapsed
        }
        if not is_pass:
            detail["reason"] = "seal_mismatch"
        report["details"].append(detail)

        if strict and not is_pass:
            break

    # 3. Final Report
    report_path = results_dir / "conformance_report.json"
    with report_path.open("w") as f:
        json.dump(report, f, indent=2)

    print("-" * 80)
    print(f"[*] Results: {report['summary']['pass']} Passed, {report['summary']['fail']} Failed.")
    print(f"[*] Full report saved to: {report_path}")
    return 0 if report["summary"]["fail"] == 0 else 1


def main():
    parser = argparse.ArgumentParser(description="ISO-16 Conformance Orchestrator")
    parser.add_argument(
        "--strict",
        action="store_true",
        help="Stop on first failure instead of running all vectors."
    )
    args = parser.parse_args()

    exit_code = run_suite(strict=args.strict)
    sys.exit(exit_code)


if __name__ == "__main__":
    main()
