# ISO-16 Seal Specification
## Normative Requirements for Canonical Serialization and Judicial Hashing
**Status:** Draft Standard  
**Scope:** This document defines the canonical serialization format, field ordering, hash function, and verification rules for the ISO-16 Tetra-Seal. The Tetra-Seal provides tamper-evident integrity for all audit-relevant fields.

---

## 1. Terminology (Normative)

### 1.1 SHALL / MUST
Indicates a requirement for conformance.

### 1.2 SHOULD
Indicates a recommended practice.

### 1.3 MAY
Indicates an optional feature.

### 1.4 Seal Inputs
The ordered, canonical byte sequence used to compute the Tetra-Seal.

### 1.5 Tetra-Seal
A cryptographic hash over the canonical seal inputs.

### 1.6 Auditor
Any independent system verifying the seal.

---

## 2. Purpose of the Tetra-Seal (Normative)

The Tetra-Seal SHALL provide:
1. Tamper-evident integrity for all audit-relevant fields
2. Cross-implementation reproducibility
3. A stable, canonical representation of truth
4. A verifiable chain of custody for TRUE/FALSE determinations

The seal SHALL NOT be used to alter or reinterpret implementation outputs.

---

## 3. Canonical Field Set (Normative)

The following fields SHALL be included in the seal inputs, in the exact order listed:

1. phase_state_initial
2. plugin_outputs
3. warp_total
4. error_total
5. phase_state_warped
6. symmetry_ok
7. error_ok
8. true_delivery
9. implementation_id
10. timestamp
11. nonce

All fields above SHALL be present.
No additional fields SHALL be included in the canonical seal input.

---

## 4. Canonical Serialization Rules (Normative)

### 4.1 Encoding Format
Seal inputs SHALL be serialized using the following rules:
- All numeric values SHALL be encoded as big-endian signed Q16.16 integers.
- Boolean values SHALL be encoded as a single byte:
  - 0x00 for FALSE
  - 0x01 for TRUE
- Strings SHALL be UTF-8 encoded without null terminators.
- Arrays SHALL be serialized in index order without separators.
- No whitespace, indentation, or formatting characters SHALL be included.

### 4.2 PhaseState Serialization
Each phase SHALL be serialized as:
[x][y][z]
Each coordinate is a 32-bit Q16.16 big-endian integer.

The full PhaseState serialization SHALL be:
phase[0].x phase[0].y phase[0].z
phase[1].x phase[1].y phase[1].z
...
phase[15].x phase[15].y phase[15].z

### 4.3 Plugin Output Serialization
For each plugin, in lexicographic order of plugin id:
[id_length][id_bytes]
[domain_code]
[warp_x][warp_y][warp_z]
[error]
[version_length][version_bytes]

Domain codes:
- Refraction = 0x01
- FrameDrag  = 0x02
- Jitter     = 0x03
- Custom     = 0xFF

### 4.4 Warp and Error Serialization
Warp vectors SHALL be serialized as three Q16.16 values.
Error terms SHALL be serialized as a single Q16.16 value.

### 4.5 Timestamp
Timestamps SHALL be encoded as a 64-bit unsigned integer representing microseconds since Unix epoch.

### 4.6 Nonce
The nonce SHALL be a 128-bit random value generated per audit record.

---

## 5. Canonical Hash Function (Normative)

### 5.1 Hash Algorithm
The Tetra-Seal SHALL be computed using:
SHA3-256

### 5.2 Output Format
The seal SHALL be represented as:
- 32 bytes (raw binary), or
- 64-character lowercase hexadecimal string

Both representations SHALL be accepted by auditors.

### 5.3 Domain Separation
The following ASCII prefix SHALL be prepended to the seal inputs:
ISO16-SEAL-V1:

This prefix SHALL be included in the hash computation.

---

## 6. Seal Generation Procedure (Normative)

Implementations SHALL compute the seal as follows:
1. Serialize all fields in canonical order.
2. Prepend the domain separation prefix.
3. Compute SHA3-256 over the resulting byte sequence.
4. Store the resulting 32-byte value as tetra_seal.

No additional transformations SHALL be applied.

---

## 7. Seal Verification Procedure (Normative)

Auditors SHALL:
1. Reconstruct the canonical seal input from the audit record.
2. Prepend the domain separation prefix.
3. Compute SHA3-256 over the byte sequence.
4. Compare the result to the provided tetra_seal.

### 7.1 Pass Condition
The seal SHALL be considered valid if:
auditor_recomputed_seal == tetra_seal

### 7.2 Failure Condition
If the values differ, the audit SHALL fail immediately.

---

## 8. Forbidden Behavior (Normative)

Implementations SHALL NOT:
- omit required fields
- reorder fields
- modify fields after seal generation
- include non-canonical fields in the seal input
- use non-deterministic serialization
- use a hash function other than SHA3-256
- generate seals without a nonce

Any such behavior SHALL constitute non-compliance.

---

## 9. Compliance

A system is ISO-16 seal-compliant if and only if:
- all canonical fields are present
- serialization rules are followed exactly
- SHA3-256 is used
- domain separation is applied
- auditor recomputation matches the implementation seal
- no forbidden behavior occurs

---
END OF DOCUMENT
