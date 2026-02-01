# ISO-16 Specification Directory
## Normative Standards for Spatiotemporal Parity, Plugins, Audit, Conformance, and Sealing
**Status:** Draft Standard  
**Audience:** Implementers, auditors, national laboratories, certification bodies

This directory contains the normative ISO-16 standard, divided into modular documents.
Each file in this folder defines requirements that implementations MUST satisfy to claim ISO-16 compliance.

All documents in this directory are normative unless explicitly marked otherwise.

---

## 1. Purpose of This Directory

The spec/ folder provides:
- a complete, modular, and reviewable definition of the ISO-16 standard
- clear separation between normative requirements and informative guidance
- a stable foundation for certification, audit, and cross-platform interoperability

This structure ensures that vendors, auditors, and national laboratories can independently verify correctness without ambiguity.

---

## 2. Normative Documents

The following documents define the complete ISO-16 normative standard:

### iso16_core.md
Defines the core of the standard, including:
- PhaseState structure
- Q16.16 deterministic numeric format
- symmetry and error rules
- TRUE/FALSE determination
- deterministic state machine
- interoperability requirements

### iso16_plugins.md
Defines the plugin interface, including:
- required fields
- determinism rules
- domain definitions
- warp and error semantics
- plugin versioning
- seal integration

### iso16_audit.md
Defines the audit layer, including:
- audit record structure
- reconstruction rules
- symmetry and error recalculation
- TRUE/FALSE verification
- seal validation
- reproducibility requirements

### iso16_test_vectors.md
Defines the normative conformance vectors that all implementations MUST pass.
These vectors serve as the “weights and measures” for ISO-16 truth.

### iso16_seal.md
Defines the canonical serialization and hashing rules for the Tetra-Seal, including:
- field ordering
- encoding rules
- domain separation
- SHA3-256 requirements
- seal generation and verification

### iso16_conformance.md
Defines how implementations demonstrate compliance, including:
- required artifacts
- conformance process
- cross-platform consistency rules
- determinism requirements
- forbidden behavior
- compliance levels

---

## 3. Document Relationships

The ISO-16 standard is modular but interdependent:

- iso16_core.md defines the truth model.
- iso16_plugins.md defines how environmental corrections are produced.
- iso16_audit.md defines how truth is verified.
- iso16_test_vectors.md defines how truth is validated across implementations.
- iso16_seal.md defines how truth is sealed.
- iso16_conformance.md defines how truth is certified.

All six documents MUST be satisfied for full ISO-16 compliance.

---

## 4. Versioning and Change Control

### 4.1 Versioning
Each normative document declares its version at the top.
Breaking changes increment the MAJOR version.

### 4.2 Stability
Once a version is ratified:
- canonical field sets SHALL NOT change
- conformance vectors SHALL NOT change
- seal serialization SHALL NOT change

### 4.3 Change Proposals
Changes SHOULD be proposed via:
- pull request
- issue with rationale
- cross-document consistency check

Changes MUST NOT break backward compatibility unless explicitly versioned.

---

## 5. Normative vs Informative Material

Only the documents in this directory are normative.

Informative materials (architecture diagrams, tutorials, examples) live outside this folder, typically in:
docs/
tools/
diagrams/

These materials MAY assist implementers but are NOT part of the standard.

---

## 6. Compliance Statement

A system MAY claim:
ISO-16 Compliant (Level 3)

only if:
- all normative documents in this directory are satisfied
- all conformance vectors pass
- all seals validate
- all auditor recalculations match
- cross-platform consistency is demonstrated

Any weaker claim MUST specify the level defined in iso16_conformance.md.

---

## 7. Intended Audience

This directory is intended for:
- hardware and software implementers
- plugin developers
- national laboratories (e.g., PNNL)
- certification bodies
- auditors and regulators
- researchers working on spatiotemporal integrity

---

## 8. Canonical Reference

This directory constitutes the canonical definition of the ISO-16 standard.
All implementations, audits, and certifications SHALL reference these documents.

---
END OF DOCUMENT
