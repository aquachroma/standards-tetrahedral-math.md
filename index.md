# **Directory Index (Informative)**  
## ISO‑16 Reference Implementation Repository

This directory index provides an informative overview of the structure and contents of the ISO‑16 reference implementation repository.  
It is intended to assist implementers, auditors, and reviewers in locating the materials relevant to their work.  
The normative requirements of ISO‑16 are defined exclusively in the specification documents located in `spec/`.

---

## **A.1 Top‑Level Structure**

```
/
├── spec/
│   ├── iso16_core.md
│   ├── iso16_plugins.md
│   ├── iso16_seal.md
│   ├── iso16_conformance.md
│   └── iso16_audit.md
│
├── src/
│   ├── iso16_lattice.h
│   ├── iso16_true_delivery.h
│   ├── iso16_plugin.h
│   ├── *.cpp
│   │
│   ├── plugins/
│   │   ├── plugin_alpha.*
│   │   ├── plugin_beta.*
│   │   ├── plugin_gamma.*
│   │   ├── plugin_delta.*
│   │   └── plugin_epsilon.*
│   │
│   └── examples/
│       ├── hello_iso16.cpp
│       ├── example_run_vector.cpp
│       └── *.cpp
│
├── hdl/
│   ├── iso16_true_delivery.v
│   ├── iso16_waveform_logger.v
│   ├── plugin_*.v
│   └── tb_iso16_waveform.v
│
├── conformance/
│   ├── vectors/
│   ├── expected/
│   └── metadata/
│
├── waveforms/
│   ├── *.png
│   └── *.vcd
│
├── tools/
│   ├── validation/
│   ├── waveform/
│   └── utilities/
│
└── build/
    └── (generated artifacts)

```

Each directory is described in the following clauses.

---

## **A.2 `spec/` — Normative Specification Documents**

This directory contains the **normative text** of ISO‑16.  
These documents define the requirements that all conforming implementations must satisfy.

Contents include:

- **iso16_core.md** — Core architecture, state machine, lattice, and execution model  
- **iso16_plugins.md** — Plugin interface, stability rules, determinism requirements  
- **iso16_seal.md** — Canonical serialization, seal boundary, SHA3‑256 requirements  
- **iso16_conformance.md** — Conformance criteria, required outputs, validation rules  
- **iso16_audit.md** — Audit guidance, verification procedures, reproducibility requirements  

These documents supersede all source code and examples.

---

## **A.3 `src/` — C++ Reference Implementation (Informative)**

This directory contains the **informative software reference implementation** of ISO‑16.

Contents include:

- canonical lattice representation  
- plugin interface definitions  
- True Delivery Loop implementation  
- canonical serialization and seal generation  
- utility functions and supporting types  

This implementation is intended to illustrate the normative behavior defined in `spec/`.

---

## **A.4 `src/plugins/` — Reference Plugin Suite (Informative)**

This directory contains the **informative reference plugins** used for testing, examples, and waveform generation.

Plugins include:

- **plugin_alpha** — linear warp  
- **plugin_beta** — rotational warp  
- **plugin_gamma** — jitter/noise  
- **plugin_delta** — drift/bias  
- **plugin_epsilon** — saturation/clipping  

These plugins are deterministic and stable during COLLECT, but they do not define normative plugin behavior.

---

## **A.5 `src/examples/` — Example Programs (Informative)**

This directory contains small, self‑contained programs demonstrating:

- plugin instantiation  
- execution of the True Delivery Loop  
- inspection of seals  
- lattice manipulation  
- comparison of outputs  

These examples are intended for onboarding and vendor integration.

---

## **A.6 `hdl/` — Verilog Reference Model (Informative)**

This directory contains the **informative hardware reference model**, including:

- `iso16_true_delivery.v` — cycle‑accurate state machine  
- `iso16_waveform_logger.v` — canonical waveform capture wrapper  
- plugin stubs and test harnesses  
- `tb_iso16_waveform.v` — canonical waveform testbench  

The HDL is designed to match the normative specification and conformance vectors.

---

## **A.7 `conformance/` — Conformance Vectors (Authoritative Test Artifacts)**

This directory contains the **canonical input vectors and expected outputs** used to verify conformance.

Contents include:

- input phase states  
- plugin outputs  
- expected accumulator values  
- expected seals  
- expected waveform timing  

These materials are authoritative for determining conformance.

---

## **A.8 `waveforms/` — Golden Waveform Templates (Authoritative Test Artifacts)**

This directory contains the **golden waveform traces** generated from the reference HDL.

Contents include:

- PNG waveform diagrams  
- VCD waveform dumps  
- timing annotations  

These waveforms illustrate the expected behavior of each state in the True Delivery Loop.

---

## **A.9 `tools/` — Auxiliary Tools (Informative)**

This directory contains optional utilities for:

- generating waveforms  
- validating seals  
- comparing outputs  
- running batch conformance tests  

These tools are informative and do not define conformance.

---

## **A.10 `build/` — Generated Artifacts (Non‑Versioned)**

This directory is created during compilation or simulation and may contain:

- compiled binaries  
- waveform dumps  
- intermediate files  

It is not part of the standard and should not be version‑controlled.

---