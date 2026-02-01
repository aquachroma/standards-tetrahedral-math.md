# **ISO‑16 Reference Implementation (`src/`)**
### Informative C++ Model of the True Delivery Loop  
*(Informative)*

This directory contains the **informative, non‑normative reference implementation** of the ISO‑16 True Delivery Loop. It provides a minimal, auditable, vendor‑neutral model of the core ISO‑16 algorithms, including lattice operations, plugin evaluation, accumulator behavior, symmetry/error checks, and canonical seal generation.

The reference implementation exists to support:

- vendors implementing ISO‑16 in hardware or software  
- auditors verifying conformance  
- engineers learning the system  
- tutorials and examples throughout the documentation set  

It is **not** part of the normative specification and MUST NOT be treated as authoritative over the written standard.

---

# **1. Contents**

```
src/
  iso16_lattice.h          # Canonical lattice + Q16.16 helpers
  iso16_face_audit.h       # Symmetry evaluation
  iso16_true_delivery.h    # True Delivery Loop (software model)
  iso16_plugin.h           # Plugin interface + metadata
  iso16_seal.h             # Canonical serialization + SHA3-256

  plugins/                 # Example plugin implementations
  examples/                # Small programs demonstrating end-to-end usage
```

Each file is intentionally small, readable, and designed to mirror the structure of the normative documents in `docs/`.

---

# **2. Purpose of the Reference Implementation**

The reference implementation serves four key roles:

### **2.1 Behavioral Oracle**
It provides a **ground‑truth model** for:

- warp accumulation  
- error accumulation  
- lattice warping  
- symmetry evaluation  
- error threshold evaluation  
- true_delivery gating  
- canonical serialization  
- SHA3‑256 seal generation  

Vendors may compare their outputs against this implementation to debug mismatches.

### **2.2 Teaching Tool**
Engineers can read the reference implementation to understand:

- how the True Delivery Loop operates  
- how plugin data flows through the system  
- how the seal is constructed  
- how the lattice is represented  

It is intentionally written in a clear, direct style.

### **2.3 Conformance Support**
The conformance suite uses this implementation to:

- generate expected outputs  
- validate plugin behavior  
- cross‑check seals  
- produce example vectors  

### **2.4 Example Integration**
The `examples/` directory demonstrates:

- loading plugins  
- running the True Delivery Loop  
- printing results  
- verifying seals  

These examples are used in tutorials such as `docs/tutorials/quickstart.md`.

---

# **3. Non‑Normative Status**

This directory is **informative**.  
The normative sources of truth are:

- the ISO‑16 specification documents in `docs/`  
- the Golden Waveform Templates  
- the conformance vectors and expected outputs  
- the canonical serialization rules  

If a discrepancy is found between this implementation and the written specification, **the written specification prevails**.

---

# **4. Using the Reference Implementation**

### **4.1 For Vendors**
Use this implementation to:

- validate your plugin outputs  
- confirm accumulator behavior  
- verify symmetry/error logic  
- cross‑check seals  
- debug mismatches against the conformance suite  

### **4.2 For Auditors**
Use this implementation to:

- reproduce vendor results  
- inspect intermediate values  
- verify canonical serialization  
- confirm seal correctness  

### **4.3 For Developers**
Use this implementation to:

- learn the True Delivery Loop  
- experiment with custom plugins  
- build new examples or tutorials  

---

# **5. Building and Running Examples**

From the repository root:

```
make examples
```

This compiles the programs in `src/examples/`.

To run a specific example:

```
./build/examples/run_vector V0001.json
```

Examples demonstrate:

- plugin loading  
- warp/error accumulation  
- true_delivery computation  
- seal generation  

---

# **6. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial reference implementation README |