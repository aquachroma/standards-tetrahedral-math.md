# **Symmetry Check — Timing & Evaluation Rules**  
### ISO‑16 Phase Lattice Symmetry Evaluation  
*(Informative)*

This document defines the **expected behavior**, **timing**, and **waveform characteristics** of the ISO‑16 symmetry check. The symmetry check is one of the two gating conditions (along with the error threshold check) that determine whether a vector produces a **TRUE DELIVERY** result.

All waveform captures MUST follow the conventions defined in the *Waveform Annotation Guide*.

---

## **1. Purpose**

The symmetry check evaluates whether the **warped phase lattice** remains within the allowable symmetry bounds defined by ISO‑16. It ensures that the accumulated warp does not introduce directional bias or structural distortion.

The output of this evaluation is the boolean signal:

```
symmetry_ok
```

This signal is consumed during the `CHECK` state of the True Delivery Loop.

---

## **2. Inputs to the Symmetry Check**

The symmetry check operates on:

- the **warped phase lattice**  
  ```
  phase_warped[x][y][z]
  ```
- the **canonical symmetry rule**, which compares each cell to its symmetric counterpart

For the 4×4 lattice used in ISO‑16:

- symmetry is evaluated across the **center of the lattice**  
- each cell must match its symmetric partner within the allowed tolerance (typically exact match for integer Q16.16 values)

---

## **3. Timing Requirements**

### **3.1 Evaluation Window**

The symmetry check is performed **only** during the `CHECK` state.

### **3.2 Stability Requirements**

During the entire `CHECK` cycle:

- `phase_warped` MUST remain stable  
- `warp_sum` MUST remain stable  
- `symmetry_ok` MUST be computed combinationally and latched on the rising edge  

### **3.3 Forbidden Behavior**

- Symmetry MUST NOT be evaluated in `COLLECT`, `ACCUMULATE`, `APPLY`, `SEAL`, or `DONE`  
- `symmetry_ok` MUST NOT change after the `CHECK` cycle  
- Implementations MUST NOT apply additional filtering, smoothing, or averaging  

---

## **4. Symmetry Rule Definition**

For each lattice index `i`, ISO‑16 defines a symmetric partner `j`.

The symmetry rule is:

```
symmetry_ok = true
for all (i, j) in symmetric_pairs:
    if phase_warped[i] != phase_warped[j]:
        symmetry_ok = false
```

For the 4×4 lattice, the symmetric pairs are:

```
0 ↔ 15
1 ↔ 14
2 ↔ 13
3 ↔ 12
4 ↔ 11
5 ↔ 10
6 ↔ 9
7 ↔ 8
```

Each cell is a 3‑component Q16.16 vector.

---

## **5. Expected Waveform Behavior**

### **5.1 Symmetric Case**

If all symmetric pairs match:

- `symmetry_ok` transitions **0 → 1** during the `CHECK` cycle  
- The signal remains `1` through `SEAL` and `DONE`  

### **5.2 Asymmetric Case**

If any symmetric pair differs:

- `symmetry_ok` remains `0`  
- `true_delivery` MUST be `0` regardless of error conditions  

---

## **6. ASCII Timing Diagram (Informative)**

```
clk:        ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
           ─┘ └─┘ └─┘ └─┘ └─┘ └─

state:     ...   APPLY     CHECK     SEAL     DONE
                   |---------|---------|--------|

phase_warped:      <====== stable ======>

symmetry_ok:                 0 → 1      (symmetric case)
true_delivery:               0 → 1
```

---

## **7. Conformance Requirements**

An implementation is conformant if:

1. Symmetry is evaluated **only** during the `CHECK` state  
2. `phase_warped` remains stable during the evaluation  
3. `symmetry_ok` is computed exactly as defined  
4. No additional tolerances or heuristics are applied  
5. `symmetry_ok` remains stable after the `CHECK` cycle  
6. Waveforms match the Golden Waveforms for V0001–V0005  

---

## **8. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial release |