# ISO-16 Conformance Test Vectors
## Normative Requirements for Cross-Implementation Consistency
**Status:** Draft Standard  
**Scope:** This document defines the normative conformance vectors that all ISO-16 compliant implementations SHALL pass. These vectors are the “weights and measures” for ISO-16 truth, symmetry, and error behavior.

---

## 1. Terminology (Normative)

### 1.1 SHALL / MUST
Indicates a requirement for conformance.

### 1.2 SHOULD
Indicates a recommended practice.

### 1.3 MAY
Indicates an optional feature.

### 1.4 Conformance Vector
A fully specified input/output case that an implementation MUST reproduce exactly.

### 1.5 Expected Result
The canonical output for a given conformance vector.

---

## 2. Conformance Requirements (Normative)

1. An implementation SHALL execute all normative conformance vectors defined in this document.
2. For each vector, an implementation SHALL produce outputs that match the expected results exactly.
3. Any deviation from expected results SHALL constitute non-compliance.
4. Conformance vectors SHALL be treated as normative, not illustrative.

---

## 3. Data Model for Vectors (Normative)

Each conformance vector SHALL be representable as a structured record with the following fields:

- id (string)
- description (string)
- phase_state_initial (16×3 Q16.16)
- plugin_inputs (per-plugin configuration, if applicable)
- plugin_outputs (warp vectors + error terms)
- warp_total_expected (3×Q16.16)
- error_total_expected (Q16.16)
- phase_state_warped_expected (16×3 Q16.16)
- symmetry_ok_expected (boolean)
- error_ok_expected (boolean)
- true_delivery_expected (boolean)

Implementations MAY store these vectors as JSON, CBOR, or any equivalent structured format, but the semantics SHALL match this model.

---

## 4. Vector V0001 — Perfect Symmetry, Zero Error (TRUE)

### 4.1 Identifier
id = "V0001"

### 4.2 Description
Perfectly symmetric lattice, no plugin error, expected TRUE.

### 4.3 Initial PhaseState
All 16 phases SHALL be identical at (0,0,0).

### 4.4 Plugin Outputs
All plugin warp vectors and errors SHALL be zero.

### 4.5 Expected Aggregates
warp_total_expected = (0,0,0)
error_total_expected = 0x0000_0000

### 4.6 Expected Symmetry and Error
symmetry_ok_expected = TRUE
error_ok_expected = TRUE

### 4.7 Expected TRUE/FALSE
true_delivery_expected = TRUE

---

## 5. Vector V0002 — Error == Epsilon (TRUE)

id = "V0002"

Symmetric lattice, total error exactly epsilon.

error_total_expected = 0x0000_0001
true_delivery_expected = TRUE

---

## 6. Vector V0003 — Error > Epsilon (FALSE)

id = "V0003"

error_total_expected = 0x0000_0002
true_delivery_expected = FALSE

---

## 7. Vector V0004 — Symmetry Broken, Zero Error (FALSE)

id = "V0004"

Asymmetric PhaseState at index 5.
true_delivery_expected = FALSE

---

## 8. Vector V0005 — Symmetry Restored, Zero Error (TRUE)

id = "V0005"

Symmetry restored.
true_delivery_expected = TRUE

---

## 9. Implementation Notes (Informative)

- Implementations MAY include additional non-normative vectors.
- Only vectors defined in this document are normative.

---

## 10. Compliance

An implementation is ISO-16 conformance-compliant if and only if:
- all vectors V0001–V0005 are executed
- all expected outputs match exactly
- all requirements in iso16_core.md, iso16_audit.md, and iso16_plugins.md are satisfied

---
END OF DOCUMENT
