# **Waveform Documentation Overview**
### ISO‑16 True Delivery Loop — Timing & Visualization Materials  
*(Informative)*

This directory contains the **visual and cycle‑accurate documentation** for the ISO‑16 True Delivery Loop. These materials provide auditors, hardware teams, and implementers with a consistent way to interpret timing diagrams, verify state transitions, and confirm deterministic behavior across platforms.

Waveforms serve as the **observable proof** that an implementation matches the normative state machine defined in the ISO‑16 specification.

---

## **1. Purpose of This Directory**

The files in this directory:

- define how ISO‑16 waveforms must be annotated  
- describe the True Delivery Loop state machine  
- specify plugin timing rules  
- document symmetry and error checks  
- define the seal boundary timing  
- provide reference waveform templates and captures for conformance vectors  

Together, they form the **visual audit layer** of ISO‑16.

---

## **2. Directory Structure**

```
waveforms/
  annotation_guide.md
  error_check.md
  index.md
  plugin_timing.md
  README.md
  seal_timing.md
  symmetry_check.md
  symmetry_check.md
  true_delivery_loop.md
  
  V0001_waveform_template.md
  V0002_waveform_template.md
  V0003_waveform_template.md
  V0004_waveform_template.md
  V0005_waveform_template.md

  V0001_waveform.png
  V0002_waveform.png
  V0003_waveform.png
  V0004_waveform.png
  V0005_waveform.png
  
  vendor_waveform_submission_checklist.md
  waveform_annotation_guide.md
```

This structure ensures that auditors and vendors can quickly locate both the **normative timing rules** and the **Golden Waveform references**.

---

## **3. Contents**

### **Annotation & Conventions**
- **`waveform_annotation_guide.md`**  
  Canonical rules for signal names, colors, markers, and formatting used in all waveform captures.

### **Core Timing Documents**
- **`true_delivery_loop.md`**  
  Cycle‑accurate description of the True Delivery Loop state machine.
- **`plugin_timing.md`**  
  Timing rules for plugin warp/error sampling.
- **`symmetry_check.md`**  
  Expected behavior of the symmetry evaluation.
- **`error_check.md`**  
  Expected behavior of the error threshold evaluation.
- **`seal_timing.md`**  
  Timing of canonical serialization and seal generation.

### **Golden Waveform Templates**
These describe the **expected waveform shapes** for the five conformance vectors:

- **`V0001_waveform_template.md`** — Perfect symmetry, zero warp, zero error (TRUE)  
- **`V0002_waveform_template.md`** — Symmetric lattice, error == epsilon (TRUE)  
- **`V0003_waveform_template.md`** — Symmetric lattice, error > epsilon (FALSE)  
- **`V0004_waveform_template.md`** — Asymmetric lattice, zero error (FALSE)  
- **`V0005_waveform_template.md`** — Symmetry restored, zero error (TRUE)  

These templates guide vendors before PNG captures are produced.

### **Reference Waveforms**
- **`V0001_waveform.png`**  
- **`V0002_waveform.png`**  
- **`V0003_waveform.png`**  
- **`V0004_waveform.png`**  
- **`V0005_waveform.png`**  

These PNG files represent the **canonical Golden Waveforms** for the five conformance vectors.

> **Provenance Note:**  
> All PNG waveform captures in this directory are generated from the ISO‑16 reference HDL implementation using the canonical testbench and `iso16_waveform_logger.v`.

---

## **4. How to Use These Documents**

### **For Auditors**
Use these files to verify that an implementation’s waveforms:

- follow the canonical annotation rules  
- match the expected state transitions  
- show correct plugin sampling behavior  
- produce the correct symmetry/error outcomes  
- align with the seal boundary timing  
- match the Golden Waveforms for V0001–V0005  

### **For Hardware Teams**
Use these documents to:

- align HDL implementations with the reference state machine  
- confirm accumulator and comparator behavior  
- validate plugin interface timing  
- generate waveforms that match the Golden Waveforms  

### **For Vendors**
Use this directory alongside the conformance suite to:

- debug mismatches  
- validate plugin implementations  
- ensure deterministic behavior across toolchains  
- cross‑check results against `conformance/expected/`  

---

## **5. Cross‑References**

- **Conformance Suite:**  
  Expected outputs and canonical seals are defined in:  
  `conformance/expected/`

- **Test Vectors:**  
  Input vectors are defined in:  
  `conformance/vectors/`

- **Schemas:**  
  JSON schemas for vectors and expected outputs are in:  
  `conformance/schema/`

---

## **6. Golden Waveforms**

The PNG files in this directory represent the **canonical waveform outputs** for the five conformance vectors.  
All implementations MUST match these waveforms in:

- state progression  
- accumulator updates  
- check signal transitions  
- seal boundary timing  

These captures serve as the **visual oracle** for ISO‑16.

---

## **7. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.3 | 2026‑01‑31 | Added V0004 and V0005 Golden Waveform templates and PNG references |
| 0.2 | 2026‑01‑31 | Added directory structure, template links, provenance note, and conformance cross‑reference |
| 0.1 | 2026‑01‑31 | Initial release of waveform documentation overview |