# **Seal Timing — Canonical Serialization & SHA3‑256 Boundary**  
### ISO‑16 Seal Generation Timing Rules  
*(Informative)*

This document defines the **timing behavior**, **signal transitions**, and **waveform expectations** for the ISO‑16 seal generation process. The seal boundary marks the moment when the True Delivery Loop transitions from physical evaluation to cryptographic notarization.

All waveform captures MUST follow the conventions defined in the *Waveform Annotation Guide*.

---

## **1. Purpose**

The seal timing defines:

- when canonical serialization begins  
- when the SHA3‑256 engine is invoked  
- how long the seal boundary lasts  
- when the final 256‑bit Tetra‑Seal becomes valid  

The seal boundary is the **auditable transition** between:

- physical truth evaluation (symmetry + error checks)  
- cryptographic truth notarization (SHA3‑256)  

The output of this process is:

```
seal_start
seal_ready
seal[255:0]
```

---

## **2. Inputs to Seal Generation**

Seal generation consumes:

- `warp_sum`  
- `error_sum`  
- `phase_warped`  
- `symmetry_ok`  
- `error_ok`  
- `true_delivery`  
- `vector_id`  
- plugin metadata (domain, version, status)  

These values MUST remain stable for the entire seal boundary.

---

## **3. Timing Requirements**

### **3.1 SEAL State Entry**

On the rising edge entering the `SEAL` state:

- `seal_start` MUST assert for exactly one cycle  
- canonical serialization MUST begin immediately  
- SHA3‑256 MUST begin absorbing bytes on the same cycle  

### **3.2 Stability Requirements**

During the entire seal boundary:

- all inputs to serialization MUST remain stable  
- no additional computation may modify the inputs  
- no plugin outputs may change  
- no accumulator values may change  

### **3.3 SEAL State Exit**

`seal_ready` MUST assert when:

- SHA3‑256 has absorbed all canonical bytes  
- the digest is fully computed  
- the 256‑bit seal is stable  

`seal_ready` MUST remain asserted through `DONE`.

---

## **4. Seal Boundary Definition**

The seal boundary is defined as:

```
seal_start == 1   →   seal_ready == 1
```

This interval represents the **cryptographic notarization window**.

### **4.1 Minimum Duration**

ISO‑16 does not mandate a specific number of cycles for SHA3‑256, but:

- the duration MUST be deterministic  
- the duration MUST be consistent across runs  
- the duration MUST be visible in waveforms  

### **4.2 Forbidden Behavior**

- `seal_ready` MUST NOT assert in the same cycle as `seal_start`  
- `seal_ready` MUST NOT assert before symmetry/error checks complete  
- `seal` MUST NOT change after `seal_ready` asserts  

---

## **5. Expected Waveform Behavior**

### **5.1 Seal Start**

During the first cycle of the `SEAL` state:

- `seal_start = 1`  
- `seal_ready = 0`  
- SHA3‑256 begins absorbing canonical bytes  

### **5.2 Seal Boundary**

During the intermediate cycles:

- `seal_start = 0`  
- `seal_ready = 0`  
- SHA3‑256 continues hashing  

### **5.3 Seal Ready**

When hashing completes:

- `seal_ready = 1`  
- `seal[255:0]` becomes valid  
- `true_delivery` MUST remain stable  

---

## **6. ASCII Timing Diagram (Informative)**

```
clk:        ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
           ─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─

state:     ...   CHECK     SEAL                DONE
                   |---------|-------------------|

symmetry_ok:        1
error_ok:           1
true_delivery:       1

seal_start:                    |───|
seal_ready:                        |────────|
seal[255:0]:                      <== stable ==>
```

---

## **7. Conformance Requirements**

An implementation is conformant if:

1. `seal_start` asserts for exactly one cycle  
2. Canonical serialization begins on the same cycle as `seal_start`  
3. All inputs remain stable during the seal boundary  
4. `seal_ready` asserts only after hashing completes  
5. `seal` remains stable after `seal_ready`  
6. Waveforms match the Golden Waveforms for V0001–V0005  

---

## **8. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial release |