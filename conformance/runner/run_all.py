#!/usr/bin/env python3
import json
from pathlib import Path

from reference_runner import run as run_single
from utils.schema_validate import validate_json

ROOT = Path(__file__).resolve().parents[1]
VECTORS_DIR = ROOT / "conformance" / "vectors"
EXPECTED_DIR = ROOT / "conformance" / "expected"
SEAL_DIR = ROOT / "conformance" / "seal_vectors"
SCHEMA_VECTOR = ROOT / "conformance" / "schema" / "vector_schema.json"
SCHEMA_EXPECTED = ROOT / "conformance" / "schema" / "expected_schema.json"

def _iter_pairs():
    for vec_path in sorted(VECTORS_DIR.glob("V*.json")):
        vid = vec_path.stem
        exp_path = EXPECTED_DIR / f"{vid}_expected.json"
        if exp_path.exists():
            yield vid, vec_path, exp_path

def main():
    failures = []

    print("=== ISO‑16 Conformance: Core Vectors ===")
    for vid, vpath, epath in _iter_pairs():
        try:
            run_single(str(vpath), str(epath))
        except Exception as e:
            failures.append((vid, str(e)))

    print("\n=== ISO‑16 Conformance: Seal Vectors ===")
    seal_input = SEAL_DIR / "SEALTEST_input.json"
    seal_expected = SEAL_DIR / "SEALTEST_expected.json"

    if seal_input.exists() and seal_expected.exists():
        with seal_input.open() as f:
            vec = json.load(f)
        with seal_expected.open() as f:
            exp = json.load(f)

        # Just validate schemas + seal equality here if you like,
        # or call a dedicated seal test helper.
        # (You can wire this to your Python seal test script.)

        print("SEALTEST: (hook your seal check here)")

    if failures:
        print("\n❌ Overall: FAIL")
        for vid, msg in failures:
            print(f"  {vid}: {msg}")
        raise SystemExit(1)

    print("\n✅ Overall: PASS")

if __name__ == "__main__":
    main()
