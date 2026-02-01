# **ISO‑16 Architecture Overview**
### Core System Model, Execution Pipeline, and Audit Boundary  
*(Normative)*

ISO‑16 defines a deterministic, auditable computation model for evaluating phase‑based plugin outputs, applying warp transformations, performing symmetry and error checks, and producing a cryptographically notarized result known as the **Tetra‑Seal**.  

This document provides a high‑level architectural description of the ISO‑16 system, including its components, execution flow, plugin interface, and audit boundary.

---

# **1. Architectural Goals**

ISO‑16 is designed to:

- provide a **deterministic execution pipeline**  
- ensure **vendor‑neutral plugin integration**  
- enforce **strict timing and stability rules**  
- produce **auditable, reproducible results**  
- generate a **cryptographic seal** that binds inputs, outputs, and evaluation state  

The architecture balances **physical interpretability**, **formal auditability**, and **hardware implementability**.

---

# **2. System Components**

ISO‑16 consists of the following major components:

### **2.1 Plugin Layer**
Plugins provide:

- a 3‑component warp vector  
- a scalar error contribution  
- metadata (domain, version, status)

Plugins MUST adhere to the canonical sampling window defined in `plugin_timing.md`.

### **2.2 Accumulator Layer**
The accumulator computes:

- `warp_sum[x/y/z]` — sum of all plugin warp vectors  
- `error_sum` — sum of all plugin error values  

Both values MUST remain stable during the CHECK and SEAL states.

### **2.3 Phase Lattice**
A 4×4 lattice of Q16.16 vectors representing the warped phase field:

```
phase_warped[x][y][z]
```

Warp is applied during the APPLY state.

### **2.4 Symmetry Comparator**
Evaluates whether the lattice satisfies the canonical symmetry rule:

```
phase_warped[i] == phase_warped[j]
```

for all symmetric index pairs.

### **2.5 Error Comparator**
Evaluates whether:

```
error_sum ≤ epsilon
```

where `epsilon = 1` in the reference suite.

### **2.6 True Delivery Gate**
Combines the two checks:

```
true_delivery = symmetry_ok AND error_ok
```

### **2.7 Seal Engine**
Performs:

- canonical serialization  
- SHA3‑256 hashing  
- seal boundary timing (`seal_start` → `seal_ready`)  

The resulting 256‑bit digest is the **Tetra‑Seal**.

---

# **3. Execution Pipeline**

ISO‑16 executes a fixed, deterministic state machine:

```
IDLE
  → COLLECT
  → ACCUMULATE
  → APPLY
  → CHECK
  → SEAL
  → DONE
```

Each state is described below.

---

## **3.1 IDLE**
System awaits a new vector.  
All accumulators and comparators are reset.

---

## **3.2 COLLECT**
Plugins assert `plugin_valid[id]` and provide:

- warp vector  
- error value  
- metadata  

Inputs MUST remain stable for the entire COLLECT window.

---

## **3.3 ACCUMULATE**
The system computes:

- `warp_sum = Σ plugin_warp`  
- `error_sum = Σ plugin_error`  

No normalization or filtering is permitted.

---

## **3.4 APPLY**
The warp vector is applied to the canonical lattice:

```
phase_warped = phase_canonical + warp_sum
```

The lattice MUST remain stable after APPLY.

---

## **3.5 CHECK**
Three evaluations occur:

1. **Symmetry Check**  
2. **Error Threshold Check**  
3. **True Delivery Gate**

All checks MUST be combinational and latched on the rising edge.

---

## **3.6 SEAL**
The seal boundary begins:

- `seal_start` asserts for exactly one cycle  
- canonical serialization begins immediately  
- SHA3‑256 absorbs all bytes  
- `seal_ready` asserts when hashing completes  

The seal MUST remain stable after `seal_ready`.

---

## **3.7 DONE**
Outputs are finalized:

- `true_delivery`  
- `seal[255:0]`  
- metadata  

System returns to IDLE.

---

# **4. Audit Boundary**

ISO‑16 defines a strict audit boundary that ensures reproducibility and verifiability.

### **4.1 Inputs Inside the Boundary**
- plugin warp vectors  
- plugin error values  
- plugin metadata  
- canonical lattice  
- epsilon  
- vector ID  

### **4.2 Outputs Inside the Boundary**
- `true_delivery`  
- `seal[255:0]`  
- waveform traces  
- accumulator values  
- comparator results  

### **4.3 Outputs Outside the Boundary**
- vendor‑specific logs  
- simulator artifacts  
- performance metrics  

Only the **canonical outputs** are used for conformance.

---

# **5. Plugin Interface Specification**

Plugins MUST implement:

```
plugin_valid[id]
plugin_warp[id][x/y/z]
plugin_error[id]
plugin_metadata[id]
```

Plugins MUST NOT:

- modify lattice values  
- alter accumulator behavior  
- influence seal generation  
- introduce nondeterminism  

The plugin interface is fully defined in `plugin_timing.md`.

---

# **6. Determinism Requirements**

ISO‑16 requires:

- deterministic state progression  
- deterministic accumulator behavior  
- deterministic comparator results  
- deterministic seal boundary duration  
- deterministic SHA3‑256 output  

Any nondeterministic behavior invalidates conformance.

---

# **7. Waveform Requirements**

Every implementation MUST produce waveforms that match:

- the canonical state machine  
- accumulator transitions  
- symmetry/error check behavior  
- seal boundary timing  

Golden Waveform Templates for V0001–V0005 are provided in:

```
docs/waveforms/
```

---

# **8. Cryptographic Seal**

The Tetra‑Seal is computed as:

```
seal = SHA3-256( canonical_serialization(...) )
```

Serialization includes:

- vector ID  
- plugin metadata  
- warp_sum  
- error_sum  
- symmetry_ok  
- error_ok  
- true_delivery  
- lattice values  
- timing markers  

The seal binds the entire evaluation into a single, verifiable digest.

---

# **9. Conformance**

An implementation is conformant if:

- all state transitions match the canonical sequence  
- all timing rules are followed  
- all checks behave as defined  
- all waveforms match the Golden Templates  
- all seals match the expected outputs  
- all plugin interactions follow the interface spec  

---

# **10. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial architecture overview |