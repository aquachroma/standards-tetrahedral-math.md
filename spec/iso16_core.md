# ISO-16 Core Specification
## Normative Definitions for Spatiotemporal Parity and Tetrahedral Coordination
**Status:** Draft Standard (v0.2)  
**Scope:** This document defines the core normative requirements for ISO-16 compliant systems, including the lattice model, deterministic numeric format, symmetry rules, error thresholds, and TRUE/FALSE determination.

---

# 1. Terminology (Normative)

## 1.1 SHALL / MUST
Indicates a requirement for conformance.

## 1.2 SHOULD
Indicates a recommended practice.

## 1.3 MAY
Indicates an optional feature.

## 1.4 Implementation
Any hardware, firmware, or software system claiming ISO-16 compliance.

## 1.5 PhaseState
A 16-element ordered set of phases representing a tetrahedral spatiotemporal cell. Each phase SHALL contain a 3-dimensional coordinate (x,y,z) encoded per §2.

## 1.6 Plugin
A deterministic module that produces (at minimum) a warp vector contribution and a bounded error term, per §4.

## 1.7 True Delivery
A state in which the system satisfies both geometric symmetry and error-bound requirements, per §8.

---

# 2. Deterministic Numeric Format (Normative)

## 2.1 Q16.16 Format
All ISO-16 compliant implementations SHALL represent:
- coordinates
- warp vectors
- error terms
- epsilon thresholds  
using signed Q16.16 fixed-point format.

## 2.2 Canonical Epsilon Constant
The canonical epsilon SHALL be:
- epsilon = 2^-16
- Q16.16 encoding: 0x0000_0001

## 2.3 Numerical Determinism
Arithmetic used for normative calculations SHALL be deterministic across platforms.  
Floating-point representations SHALL NOT be used for normative calculations.

---

# 3. Tetrahedral Lattice (Normative)

## 3.1 Phase Ordering
A PhaseState SHALL contain exactly 16 phases, indexed 0 through 15, in a stable, canonical order.

## 3.2 Coordinate Structure
Each phase SHALL contain:
- x : Q16.16
- y : Q16.16
- z : Q16.16

## 3.3 Canonical Distance Metric
Where a distance metric is required, implementations SHALL use the Euclidean norm:

d = sqrt((x1-x2)^2 + (y1-y2)^2 + (z1-z2)^2)

If sqrt is not implemented (e.g., minimal FPGA), an implementation MAY use a monotone surrogate metric (e.g., squared norm) PROVIDED that:
1) the surrogate is used consistently throughout the implementation, and
2) all normative conformance vectors for that implementation are evaluated using the same metric.

## 3.4 Symmetry Requirement (Baseline)
A PhaseState SHALL be considered symmetric if and only if, for all i in [0,14]:

|phase[i] - phase[i+1]| <= epsilon

Where |·| is computed either per-axis or as a vector norm. The chosen method SHALL be consistent across the implementation and SHALL be disclosed in the conformance report.

---

# 4. Plugin Interface (Normative)

## 4.1 Required Fields
Each plugin SHALL provide:
- id (string)
- domain (enum: Refraction, FrameDrag, Jitter, Custom)
- warp_vector (x,y,z in Q16.16)
- error (Q16.16)
- status (enum: OK, INSUFFICIENT_DATA, OUT_OF_RANGE, TIMEOUT, SENSOR_MISMATCH)

## 4.2 Determinism
For a given PhaseState and identical plugin inputs, plugin outputs SHALL be deterministic.

## 4.3 Parallel Execution
All plugins SHALL be evaluated independently and MAY be evaluated in parallel.

## 4.4 Error Accumulation
Total error SHALL be computed as:  
error_total = sum(error_plugin)

Plugins with status != OK SHALL cause the implementation to set error_ok = FALSE and SHALL inhibit execution (see §8), unless explicitly permitted by a conformance vector.

---

# 5. Warp Application (Normative)

## 5.1 Warp Vector Summation
Implementations SHALL compute:  
warp_total = warp_a + warp_b + warp_g + ...

## 5.2 Phase Update
Warped phases SHALL be computed as:  
phase' = phase + warp_total

## 5.3 No Partial Application
Warp vectors SHALL be applied atomically to all 16 phases. Partial application (some phases updated, others not) SHALL NOT be permitted for a normative TRUE outcome.

---

# 6. Symmetry Evaluation (Normative)

## 6.1 Symmetry Condition
A warped PhaseState SHALL be considered symmetric if, for all adjacent phase pairs:

|phase'[i] - phase'[i+1]| <= epsilon

## 6.2 Symmetry Failure
If any delta exceeds epsilon, the implementation SHALL set:  
symmetry_ok = FALSE

---

# 7. Error Evaluation (Normative)

## 7.1 Error Threshold
An implementation SHALL set:  
error_ok = TRUE  
if and only if:  
error_total <= epsilon

## 7.2 Error Failure
If total error exceeds epsilon, the implementation SHALL set:  
error_ok = FALSE

---

# 8. TRUE/FALSE Determination (Normative)

## 8.1 TRUE Condition
A system SHALL output TRUE only when:  
- symmetry_ok == TRUE  
AND  
- error_ok == TRUE

## 8.2 FALSE Condition
A system SHALL output FALSE when:
- symmetry fails, OR
- error exceeds epsilon, OR
- any required plugin status != OK (unless explicitly permitted by a conformance vector).

## 8.3 No Intermediate States
TRUE/FALSE SHALL be mutually exclusive and exhaustive.

## 8.4 Actuation Inhibition Rule
If TRUE is not achieved, the system SHALL inhibit physical actuation.

---

# 9. Deterministic State Machine (Normative)

Implementations SHALL follow the state sequence:
1. LOAD_PHASE_STATE
2. EVAL_PLUGINS
3. ACCUMULATE_WARP
4. APPLY_WARP
5. CHECK_SYMMETRY
6. CHECK_ERROR
7. DECIDE_TRUE_FALSE

No state may be skipped or reordered for a normative TRUE outcome.

---

# 10. Interoperability Requirements (Normative)

## 10.1 Cross-Platform Consistency
Two compliant implementations SHALL produce identical TRUE/FALSE outputs for identical inputs and identical plugin datasets.

## 10.2 Conformance Vectors
Implementations SHALL pass all normative test vectors defined in `iso16_test_vectors.md`.

## 10.3 Seal Integration
Implementations SHALL expose all required fields for Tetra-Seal hashing as defined in `iso16_seal.md`.

---

# 11. Security Considerations (Normative)

- Implementations SHALL NOT modify plugin outputs after evaluation.
- Implementations SHALL provide tamper-evident sealing per `iso16_seal.md`.
- Implementations SHALL ensure deterministic arithmetic per §2.
- Implementations SHOULD isolate legacy UI/logging layers from normative PhaseState evaluation.

---

# 12. Compliance (Normative)

A system is ISO-16 compliant if and only if:
- all SHALL/MUST requirements in this document are met,
- all normative plugin requirements are met,
- all conformance vectors pass,
- all audit requirements pass, and
- the Tetra-Seal is correctly generated and verifiable.
---

END OF DOCUMENT
