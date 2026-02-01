# **ISO‑16 Waveform Documentation Index**
### True Delivery Loop — Timing, Checks, and Golden Waveforms  
*(Informative)*

This directory contains the complete set of **timing, visualization, and annotation materials** for the ISO‑16 True Delivery Loop. These documents define how implementations must present and interpret waveforms for audit, verification, and conformance testing.

Waveforms serve as the **observable evidence** that an implementation matches the canonical ISO‑16 state machine and evaluation rules.

---

## **1. Annotation & Conventions**

### **`waveform_annotation_guide.md`**  
Defines the canonical rules for:

- signal naming  
- color conventions  
- cycle markers  
- annotation symbols  
- screenshot formatting  

All waveform captures MUST follow these conventions.

---

## **2. Core Timing Specifications**

### **`true_delivery_loop.md`**  
Cycle‑accurate definition of the True Delivery Loop state machine, including:

- state transitions  
- accumulator behavior  
- check timing  
- seal boundary entry and exit  

### **`plugin_timing.md`**  
Defines the sampling window and stability requirements for plugin warp/error outputs.

### **`symmetry_check.md`**  
Describes the symmetry evaluation performed during the `CHECK` state.

### **`error_check.md`**  
Defines the error threshold evaluation (`error_sum ≤ epsilon`) and expected waveform behavior.

### **`seal_timing.md`**  
Specifies the timing of canonical serialization, SHA3‑256 hashing, and seal readiness.

---

## **3. Golden Waveform Templates**

These templates describe the **expected waveform shapes** for the first three conformance vectors.  
They serve as the **visual oracle** for hardware teams and auditors.

- **`V0001_waveform_template.md`** — Perfect symmetry, zero warp, zero error (TRUE)  
- **`V0002_waveform_template.md`** — Symmetric lattice, error == epsilon (TRUE)  
- **`V0003_waveform_template.md`** — Symmetric lattice, error > epsilon (FALSE)  

PNG waveform captures may be added alongside these templates.

---

## **4. How to Use This Directory**

### **For Auditors**
Use these documents to verify:

- correct state progression  
- correct plugin sampling  
- correct accumulator updates  
- correct symmetry/error outcomes  
- correct seal boundary timing  
- alignment with Golden Waveforms  

### **For Hardware Teams**
Use this directory to:

- implement the True Delivery Loop faithfully  
- debug mismatches against Golden Waveforms  
- ensure deterministic behavior across toolchains  

### **For Vendors**
Use these materials with the conformance suite to:

- validate plugin implementations  
- confirm seal correctness  
- ensure cross‑platform reproducibility  

---

## **5. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial index for waveform documentation |