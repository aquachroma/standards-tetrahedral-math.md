# ISO‑16 Reference Implementation  
## True Delivery Loop  
### Informative Reference Materials

This repository provides the *informative* reference implementation for ISO‑16,
the True Delivery Loop. The written specification located in `spec/` constitutes
the *normative* definition of the standard. All source code, examples, hardware
descriptions, and conformance artifacts in this repository are provided solely
to support understanding, verification, and independent implementation of the
standard.

---

## 1 Scope

ISO‑16 defines a deterministic procedure for evaluating phase‑based state
transitions using a fixed lattice, a plugin‑contributed warp and error model,
and a canonical sealing process. The True Delivery Loop ensures that compliant
implementations produce identical results when provided with identical inputs.

This repository contains:

- the informative C++ reference implementation  
- the informative Verilog HDL reference model  
- the canonical plugin suite  
- conformance vectors and expected outputs  
- golden waveform templates  
- examples and auxiliary materials  

The contents of this repository do *not* supersede the normative text of the
ISO‑16 specification.

---

## 2 Normative References

The following documents are normative for ISO‑16:

- `spec/iso16_core.md` — Core Architecture  
- `spec/iso16_plugins.md` — Plugin Interface and Requirements  
- `spec/iso16_seal.md` — Canonical Serialization and Seal Boundary  
- `spec/iso16_conformance.md` — Conformance Requirements and Vectors  
- `spec/iso16_audit.md` — Audit and Verification Guidance  

Where discrepancies arise, these documents take precedence over all source code
and examples contained in this repository.

---

## 3 Terms and Definitions

For the purposes of ISO‑16, the following terms apply:

- **True Delivery Loop**: The deterministic seven‑state evaluation pipeline.  
- **Plugin**: A deterministic module contributing warp and error values.  
- **Phase State**: The canonical representation of the system’s lattice state.  
- **Seal**: The SHA3‑256 digest of the canonical serialization of results.  
- **Golden Waveform**: The authoritative timing trace for the reference HDL.  

Additional definitions are provided in `spec/iso16_core.md`.

---

## 4 Overview of Repository Structure

```
spec/                 Normative specification documents
src/                  Informative C++ reference implementation
src/plugins/          Reference plugin suite (informative)
src/examples/         Example programs (informative)
hdl/                  Informative Verilog reference model
conformance/          Canonical input vectors and expected outputs
waveforms/            Golden waveform templates
```

Each directory contains a `README.md` describing its contents and intended use.

---

## 5 Reference Implementations (Informative)

### 5.1 C++ Reference Implementation

The C++ implementation in `src/` provides:

- the canonical lattice representation  
- the plugin interface  
- the True Delivery Loop  
- canonical serialization and seal generation  

It is intended as a readable reference for software implementers.

### 5.2 HDL Reference Model

The Verilog HDL in `hdl/` provides:

- a cycle‑accurate state machine  
- warp and error accumulation  
- lattice warp application  
- symmetry and error checks  
- seal boundary timing  

It is intended for hardware implementers and for generating golden waveforms.

---

## 6 Conformance Materials

The `conformance/` directory contains:

- canonical input vectors  
- expected accumulator values  
- expected seals  
- expected waveform behavior  

These materials are authoritative for determining conformance.

---

## 7 Golden Waveforms

The `waveforms/` directory contains PNG and VCD waveform traces generated from
the reference HDL using the canonical testbench. These traces illustrate the
expected timing behavior of each state in the True Delivery Loop.

Golden waveforms are authoritative for timing‑related conformance.

---

## 8 Examples

The `src/examples/` directory contains small, self‑contained programs
demonstrating:

- plugin instantiation  
- execution of the True Delivery Loop  
- inspection of seals and intermediate values  

These examples are informative and do not define conformance.

---

## 9 Implementation Notes

The reference implementations are designed to be:

- deterministic  
- portable  
- auditable  
- minimal  

They do not include optimizations, vendor‑specific extensions, or performance
enhancements. Implementers may optimize their own systems provided that all
normative requirements of ISO‑16 are met.

---

## 10 Disclaimer

This repository is provided for informational purposes only. The normative
requirements of ISO‑16 are defined exclusively in the specification documents
located in `spec/`. Implementers must consult the normative text when claiming
conformance.

```

---
