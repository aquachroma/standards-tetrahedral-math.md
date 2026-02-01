# ISO-16 Conformance Specification
## Normative Requirements for Implementation, Verification, and Certification
**Status:** Draft Standard  
**Scope:** This document defines the conformance requirements for ISO-16 compliant systems. It specifies how implementations SHALL demonstrate correctness, how auditors SHALL verify compliance, and how cross-platform consistency SHALL be established.

---

## 1. Terminology (Normative)

### 1.1 SHALL / MUST
Indicates a requirement for conformance.

### 1.2 SHOULD
Indicates a recommended practice.

### 1.3 MAY
Indicates an optional feature.

### 1.4 Implementation
Any hardware, firmware, or software system claiming ISO-16 compliance.

### 1.5 Auditor
Any independent system, laboratory, or regulator verifying compliance.

### 1.6 Conformance Vector
A fully specified input/output case that an implementation MUST reproduce exactly.

---

## 2. Scope of Conformance (Normative)

An implementation SHALL be evaluated against:
1. iso16_core.md
2. iso16_plugins.md
3. iso16_audit.md
4. iso16_test_vectors.md
5. iso16_seal.md

Compliance requires meeting all normative requirements across these documents.

---

## 3. Conformance Criteria (Normative)

An implementation is ISO-16 compliant if and only if:
1. All SHALL/MUST requirements in all normative documents are satisfied.
2. All normative conformance vectors produce exactly the expected results.
3. All audit recalculations match implementation outputs.
4. All Tetra-Seal values validate under auditor recomputation.
5. All plugin outputs are deterministic and reproducible.
6. No forbidden behavior occurs in any layer of the system.

Failure of any single criterion SHALL constitute non-compliance.

---

## 4. Required Conformance Artifacts (Normative)

### 4.1 Implementation Manifest
A document containing:
- implementation_id
- version
- supported plugin set
- numeric mode (Euclidean or monotone surrogate)
- hardware/software environment
- build configuration

### 4.2 Plugin Manifest
For each plugin:
- id
- domain
- version
- determinism declaration
- error model description
- seal_inputs schema

### 4.3 Audit Record Samples
At least one complete audit record for each normative test vector.

### 4.4 Seal Verification Log
A record showing:
- canonical serialization
- SHA3-256 recomputation
- match/non-match result

### 4.5 Conformance Vector Results
A machine-readable file containing:
- vector id
- implementation outputs
- expected outputs
- pass/fail status

---

## 5. Conformance Process (Normative)

### 5.1 Step 1 — Load Normative Vectors
The implementation SHALL load all vectors defined in iso16_test_vectors.md.

### 5.2 Step 2 — Execute True Delivery Loop
For each vector:
- load PhaseState
- evaluate plugins
- accumulate warp
- apply warp
- evaluate symmetry
- evaluate error
- determine TRUE/FALSE

### 5.3 Step 3 — Produce Audit Record
The implementation SHALL produce a complete audit record per §3 of iso16_audit.md.

### 5.4 Step 4 — Generate Tetra-Seal
The implementation SHALL generate a seal per iso16_seal.md.

### 5.5 Step 5 — Auditor Recalculation
An auditor SHALL recompute symmetry, error, TRUE/FALSE, and Tetra-Seal.

### 5.6 Step 6 — Compare Results
The implementation SHALL be considered compliant only if all recalculated values match, all seals validate, and all expected outputs match.

---

## 6. Cross-Platform Consistency (Normative)

### 6.1 Required Platforms
An implementation SHALL demonstrate consistency across:
- at least one CPU implementation
- at least one FPGA or ASIC implementation
- at least one independent re-implementation

### 6.2 Consistency Requirement
For identical inputs, all platforms SHALL produce identical warp totals, error totals, symmetry_ok, error_ok, TRUE/FALSE, and Tetra-Seal values.

### 6.3 Failure Condition
Any divergence SHALL constitute non-compliance.

---

## 7. Determinism Requirements (Normative)

### 7.1 Deterministic Arithmetic
All arithmetic SHALL be deterministic across runs, platforms, compilers, and hardware targets.

### 7.2 Deterministic Plugin Behavior
Plugins SHALL produce identical outputs for identical inputs.

### 7.3 No Hidden State
Implementations SHALL NOT rely on nondeterministic hardware, external clocks, random number generators, or mutable internal state.

---

## 8. Forbidden Behavior (Normative)

Implementations SHALL NOT:
- modify plugin outputs after evaluation
- reorder canonical seal fields
- skip or reorder state machine steps
- suppress error terms
- alter PhaseState after warp application
- override TRUE/FALSE results
- generate seals without a nonce
- use non-canonical serialization

Any such behavior SHALL constitute immediate non-compliance.

---

## 9. Conformance Levels (Normative)

### 9.1 Level 1 — Core Compliance
Implementation satisfies iso16_core.md, iso16_plugins.md, and iso16_test_vectors.md.

### 9.2 Level 2 — Audit Compliance
Implementation satisfies all Level 1 requirements plus iso16_audit.md, deterministic replay, and seal verification.

### 9.3 Level 3 — Full ISO-16 Compliance
Implementation satisfies all Level 1 and Level 2 requirements, cross-platform consistency, full seal integrity, and no forbidden behavior.

Only Level 3 SHALL be considered ISO-16 compliant.

---

## 10. Compliance Statement (Normative)

A system MAY claim:
ISO-16 Compliant (Level 3)
only if all normative documents are satisfied, all conformance vectors pass, all seals validate, all auditor recalculations match, and cross-platform consistency is demonstrated.

Any weaker claim SHALL specify the level.

---

## 11. Versioning and Change Control (Normative)

### 11.1 Version Declaration
Implementations SHALL declare ISO-16 spec version, plugin versions, and implementation version.

### 11.2 Backward Compatibility
Breaking changes in the standard SHALL increment the MAJOR version.

### 11.3 Conformance Freeze
Once a version is ratified, conformance vectors SHALL NOT change.

---

## 12. Final Compliance Rule

A system is ISO-16 compliant if and only if:
- all SHALL/MUST requirements across all normative documents are met
- all conformance vectors pass
- all seals validate
- all auditor recalculations match
- no forbidden behavior occurs
- cross-platform consistency is demonstrated

---
END OF DOCUMENT
