# ISO-16 Audit Specification
## Normative Requirements for Veracity, Reconstruction, and Judicial Certification
**Status:** Draft Standard  
**Scope:** This document defines the audit requirements for ISO-16 compliant systems, including reconstruction rules, parity checks, temporal verification, variable integrity, and the Tetra-Seal.

---

## 1. Terminology (Normative)

### 1.1 SHALL / MUST
Indicates a requirement for conformance.

### 1.2 SHOULD
Indicates a recommended practice.

### 1.3 MAY
Indicates an optional feature.

### 1.4 Audit Record
A structured set of fields produced by an implementation for verification.

### 1.5 Auditor
Any independent system, laboratory, or regulator performing verification.

### 1.6 Tetra-Seal
A tamper-evident cryptographic hash over canonical audit fields.

---

## 2. Purpose of the Audit Layer (Normative)

The ISO-16 audit layer SHALL provide:

1. Reconstruction of the spatiotemporal state  
2. Verification of plugin contributions  
3. Validation of symmetry and error conditions  
4. Certification of TRUE/FALSE outcomes  
5. Tamper-evident sealing of all relevant fields  

The audit layer SHALL NOT modify or reinterpret the implementation’s internal logic.

---

## 3. Audit Record Structure (Normative)

An ISO-16 compliant implementation SHALL produce an audit record containing:

- phase_state_initial (16×3 Q16.16)  
- plugin_outputs (warp vectors + error terms)  
- warp_total (3×Q16.16)  
- error_total (Q16.16)  
- phase_state_warped (16×3 Q16.16)  
- symmetry_ok (boolean)  
- error_ok (boolean)  
- true_delivery (boolean)  
- seal_inputs (canonical serialization)  
- tetra_seal (hash)  

All fields listed above SHALL be present and complete.

---

## 4. Temporal Sync Audit (Normative)

### 4.1 Deterministic Ordering
Audit records SHALL reflect the exact order of operations defined in the ISO-16 state machine.

### 4.2 No Retroactive Modification
Implementations SHALL NOT modify plugin outputs, warp totals, error totals, symmetry results, or TRUE/FALSE decisions after they have been produced.

### 4.3 Clock Independence
Auditors SHALL verify that the implementation does not rely on external or wall-clock timing for correctness.

---

## 5. Geometric Parity Audit (Normative)

### 5.1 Symmetry Recalculation
Auditors SHALL recompute |phase'[i] - phase'[i+1]| for all 15 adjacent phase pairs.

### 5.2 Symmetry Threshold
Auditors SHALL confirm symmetry_ok == TRUE iff all deltas <= epsilon.

### 5.3 Failure Condition
If any delta exceeds epsilon, auditors SHALL mark symmetry_ok = FALSE regardless of implementation output.

---

## 6. Variable Integrity Audit (Normative)

### 6.1 Error Recalculation
Auditors SHALL recompute error_total = sum(error_plugin).

### 6.2 Error Threshold
Auditors SHALL confirm error_ok == TRUE iff error_total <= epsilon.

### 6.3 Plugin Integrity
Auditors SHALL verify that plugin outputs match recorded inputs, no plugin output was altered post-evaluation, and plugin IDs and domains are valid.

### 6.4 Forbidden Behavior
Implementations SHALL NOT inject additional error terms, suppress plugin error terms, reorder plugin outputs, or modify plugin warp vectors.

---

## 7. TRUE/FALSE Verification (Normative)

### 7.1 Auditor Recalculation
Auditors SHALL recompute true_delivery = (symmetry_ok AND error_ok).

### 7.2 Mandatory Consistency
Auditors SHALL confirm implementation.true_delivery == auditor.true_delivery.

### 7.3 Failure Condition
If the values differ, the implementation SHALL be marked non-compliant.

---

## 8. Tetra-Seal Requirements (Normative)

### 8.1 Canonical Serialization
Implementations SHALL serialize audit fields in the exact order defined in iso16_seal.md.

### 8.2 Hash Function
Implementations SHALL compute the Tetra-Seal using the canonical hash function defined in iso16_seal.md.

### 8.3 Seal Integrity
Auditors SHALL recompute the seal and confirm tetra_seal == auditor_recomputed_seal.

### 8.4 Failure Condition
If the seal does not match, the audit SHALL fail immediately.

---

## 9. Reproducibility Requirements (Normative)

### 9.1 Deterministic Replay
Given the initial PhaseState, plugin outputs, warp totals, and error totals, an auditor SHALL be able to reproduce symmetry_ok, error_ok, and true_delivery exactly.

### 9.2 Cross-Platform Consistency
Auditors SHALL confirm that results are identical across CPU, FPGA/ASIC, and independent implementations.

---

## 10. Conformance Requirements (Normative)

### 10.1 Required Test Vectors
Implementations SHALL pass all normative vectors defined in iso16_test_vectors.md.

### 10.2 Auditor Independence
Auditors SHALL be able to verify compliance without access to proprietary plugin internals, implementation-specific optimizations, or hardware-specific timing.

### 10.3 Minimal Disclosure
Implementations SHALL expose only the fields required for audit and sealing.

---

## 11. Security Considerations (Normative)

- Audit records SHALL be immutable once sealed.  
- Implementations SHALL prevent replay attacks by including unique identifiers in the seal inputs.  
- Implementations SHALL NOT allow external systems to override TRUE/FALSE outputs.  
- All arithmetic SHALL be deterministic and reproducible.

---

## 12. Compliance

A system is ISO-16 audit-compliant if and only if:
- all SHALL requirements in this document are met  
- all conformance vectors pass  
- all seals validate  
- all recalculated values match implementation outputs  
- no forbidden behavior is detected
---

END OF DOCUMENT
