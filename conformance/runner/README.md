# ISO‑16 Reference Runner  
### Informative Reference Implementation for Conformance Execution

The `runner/` directory contains **informative** reference implementations used to execute the ISO‑16 conformance vectors defined in `conformance/vectors/` and compare results against the canonical expected outputs in `conformance/expected/`.

These runners are **not normative**.  
They exist to help implementers, vendors, and auditors understand:

- how to load and validate conformance vectors  
- how to execute the ISO‑16 deterministic state machine  
- how to apply plugin warp vectors and error terms  
- how to compute symmetry and error thresholds  
- how to determine TRUE/FALSE delivery  
- how to generate and verify the Tetra‑Seal  
- how to compare results against canonical expected outputs  

Any implementation claiming ISO‑16 compliance MUST follow the normative rules in the `spec/` directory, not the internal logic of these reference runners.

---

## 1. Directory Structure

```
runner/
  README.md              # This file
  reference_runner.py    # Python reference implementation
  run_all.py             # Full-suite runner: auto-discovers all conformance vectors, validates schemas, recomputes Tetra-Seals, and produces a single CI-grade pass/fail report.
  run_vectors.py         # Selective runner: executes specified vectors, validates schemas, recomputes seals, diffs against expected outputs, and optionally writes audit/diff artifacts.
  reference_runner.cpp   # Optional C++ reference implementation
  utils/                 # Helper modules for Q16.16 arithmetic, canonical serialization, seal generation, and schema validation. Keeps the main runner clean and auditable.
    q16.py               # Implements deterministic Q16.16 arithmetic required by the ISO‑16 core spec. 
    q16.hpp              # Ensures cross‑platform consistency.
    seal.py              # Implements canonical serialization and SHA3‑256 hashing per iso16_seal.md.
    seal.cpp             # Used to recompute the Tetra‑Seal during conformance runs.
    seal.hpp             # Mirrors the behavior of seal.py 
    schema_validate.py   # Validates vectors and expected outputs against vector_schema.json and expected_schema.json. Prevents malformed inputs from entering the conformance pipeline.
```

---

## 2. Purpose of the Reference Runner

The reference runner demonstrates:

1. **Loading** a conformance vector  
2. **Validating** it against `vector_schema.json`  
3. **Executing** the ISO‑16 state machine:  
   - LOAD_PHASE_STATE  
   - EVAL_PLUGINS  
   - ACCUMULATE_WARP  
   - APPLY_WARP  
   - CHECK_SYMMETRY  
   - CHECK_ERROR  
   - DECIDE_TRUE_FALSE  
4. **Generating** a canonical audit record  
5. **Computing** the Tetra‑Seal  
6. **Comparing** results to the canonical expected output  
7. **Producing** a pass/fail conformance report  

This provides a transparent, reproducible baseline for implementers and auditors.

---

## 3. Running the Python Reference Runner

From the repository root:

```
python3 runner/reference_runner.py \
    conformance/vectors/V0001.json \
    conformance/expected/V0001_expected.json
```

The runner will:

- validate both files against their schemas  
- execute the ISO‑16 state machine  
- recompute warp, error, symmetry, and TRUE/FALSE  
- recompute the Tetra‑Seal  
- compare all results to the expected output  
- print a pass/fail summary  

---

## 4. Deterministic Arithmetic (Q16.16)

All arithmetic in the reference runner uses the deterministic Q16.16 helpers in:

```
runner/utils/q16.py
```

These helpers ensure:

- cross‑platform reproducibility  
- bit‑exact behavior  
- no floating‑point drift  

Implementers MAY use their own Q16.16 libraries, but results MUST match exactly.

---

## 5. Seal Generation and Verification

The reference runner uses:

```
runner/utils/seal.py
```

to perform:

- canonical serialization  
- domain separation prefixing  
- SHA3‑256 hashing  
- lowercase hex encoding  

This matches the normative rules in `spec/iso16_seal.md`.

---

## 6. Schema Validation

Before execution, all vectors and expected outputs are validated using:

```
runner/utils/schema_validate.py
```

against:

- `conformance/schema/vector_schema.json`  
- `conformance/schema/expected_schema.json`  

This prevents malformed or incomplete inputs from entering the conformance pipeline.

---

## 7. Intended Audience

The reference runner is designed for:

- vendors implementing ISO‑16  
- auditors verifying compliance  
- national laboratories (e.g., PNNL)  
- researchers validating deterministic behavior  
- hardware teams building FPGA/ASIC implementations  

It is intentionally minimal, readable, and transparent.

---

## 8. Normative Status

The reference runner is **informative**.

The normative requirements are defined exclusively in:

- `spec/iso16_core.md`  
- `spec/iso16_plugins.md`  
- `spec/iso16_audit.md`  
- `spec/iso16_test_vectors.md`  
- `spec/iso16_seal.md`  
- `spec/iso16_conformance.md`  

Implementations MUST follow the normative specification, not the internal logic of this runner.

---

END OF DOCUMENT