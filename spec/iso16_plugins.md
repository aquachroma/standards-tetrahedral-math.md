# ISO-16 Plugin Specification
## Normative Requirements for Environmental Correction Modules
**Status:** Draft Standard  
**Scope:** This document defines the normative plugin interface, required fields, determinism rules, domain definitions, and integration requirements for all ISO-16 compliant plugins.

---

## 1. Terminology (Normative)

### 1.1 SHALL / MUST
Indicates a requirement for conformance.

### 1.2 SHOULD
Indicates a recommended practice.

### 1.3 MAY
Indicates an optional feature.

### 1.4 Plugin
A deterministic module that accepts a PhaseState and produces a warp vector and error term.

### 1.5 Domain
The physical or computational phenomenon the plugin models (e.g., Refraction, FrameDrag, Jitter).

### 1.6 Warp Vector
A 3-dimensional correction vector applied to all phases.

### 1.7 Error Term
A Q16.16 scalar representing plugin-specific uncertainty or distortion.

---

## 2. Plugin Purpose (Normative)

Plugins SHALL provide environmental corrections to the ISO-16 lattice by:
1. Evaluating the PhaseState
2. Producing a warp vector
3. Producing an error term
4. Declaring their domain and identity

Plugins SHALL NOT:
- modify other plugins’ outputs
- modify the PhaseState directly
- access internal state of the True Delivery Loop

---

## 3. Required Plugin Fields (Normative)

Each plugin SHALL expose the following fields:

### 3.1 id
A unique, stable string identifier.

### 3.2 domain
An enumerated value:
- Refraction
- FrameDrag
- Jitter
- Custom

### 3.3 warp_vector
A 3-element vector of Q16.16 values:
warp_vector = { warp_x, warp_y, warp_z }

### 3.4 error
A Q16.16 scalar representing plugin-specific error.

### 3.5 version
A semantic version string (e.g., 1.0.0).

### 3.6 seal_inputs
A canonical serialization of plugin outputs for Tetra-Seal hashing.

---

## 4. Determinism Requirements (Normative)

### 4.1 Deterministic Output
Given identical PhaseState input, a plugin SHALL produce identical warp vector, error term, and seal inputs.

### 4.2 No Hidden State
Plugins SHALL NOT rely on:
- internal mutable state
- external clocks
- random number generators
- nondeterministic hardware features

### 4.3 Pure Function Requirement
Plugins SHALL behave as pure functions:
output = f(phase_state)

---

## 5. Execution Model (Normative)

### 5.1 Parallel Evaluation
All plugins SHALL be evaluated independently and MAY be evaluated in parallel.

### 5.2 No Inter-Plugin Communication
Plugins SHALL NOT:
- read other plugins’ outputs
- modify other plugins’ outputs
- depend on plugin ordering

### 5.3 Input Format
Plugins SHALL accept the PhaseState exactly as defined in iso16_core.md.

---

## 6. Warp Vector Rules (Normative)

### 6.1 Vector Format
Warp vectors SHALL be 3-element Q16.16 vectors.

### 6.2 Magnitude
Plugins SHALL ensure warp magnitudes are physically meaningful for their domain.

### 6.3 No Partial Warp
Plugins SHALL NOT apply warp to individual phases; warp is global.

### 6.4 Summation
Warp vectors SHALL be summed by the True Delivery Loop:
warp_total = Σ warp_plugin

---

## 7. Error Term Rules (Normative)

### 7.1 Error Format
Error terms SHALL be Q16.16 scalars.

### 7.2 Error Meaning
Error represents plugin-specific uncertainty or distortion.

### 7.3 Error Accumulation
Total error SHALL be computed as:
error_total = Σ error_plugin

### 7.4 Forbidden Behavior
Plugins SHALL NOT:
- output negative error
- output NaN or non-finite values
- suppress or override other plugins’ error terms

---

## 8. Domain Definitions (Normative)

### 8.1 Refraction
Models medium-dependent bending of trajectories.

### 8.2 FrameDrag
Models rotational or orbital frame-drag effects.

### 8.3 Jitter
Models mechanical or vibrational noise.

### 8.4 Custom
Represents any additional domain not covered above.
Custom plugins SHALL document their physical basis.

---

## 9. Plugin Identification and Versioning (Normative)

### 9.1 Stable Identity
Plugin id values SHALL be stable across versions.

### 9.2 Semantic Versioning
Plugins SHALL use semantic versioning:
MAJOR.MINOR.PATCH

### 9.3 Backward Compatibility
Breaking changes SHALL increment the MAJOR version.

---

## 10. Seal Integration (Normative)

### 10.1 Required Fields
Plugins SHALL expose id, domain, warp_vector, error, and version as part of the canonical seal input.

### 10.2 Serialization
Plugins SHALL serialize fields exactly as defined in iso16_seal.md.

### 10.3 Deterministic Ordering
Field ordering SHALL match the canonical order.

---

## 11. Audit Requirements (Normative)

### 11.1 Auditor Recalculation
Auditors SHALL be able to recompute warp vector and error term from the PhaseState and plugin definition.

### 11.2 Plugin Integrity
Auditors SHALL verify plugin outputs match recorded values, plugin identity and version are valid, and seal inputs are correct.

### 11.3 Forbidden Behavior
Plugins SHALL NOT modify PhaseState, modify other plugins’ outputs, or depend on nondeterministic behavior.

---

## 12. Compliance

A plugin is ISO-16 compliant if and only if:
- all SHALL requirements in this document are met
- outputs are deterministic
- seal inputs are correct
- plugin passes all conformance vectors
- plugin behavior is auditable and reproducible

---
END OF DOCUMENT
