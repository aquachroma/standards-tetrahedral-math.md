# **V0005 Golden Waveform Template**  
### Symmetry restored, zero warp, zero error — TRUE case  
*(Informative)*

This document defines the expected waveform characteristics for **Conformance Vector V0005**, which verifies that an implementation correctly returns to a **TRUE DELIVERY** result after symmetry has been restored. This vector is the positive counterpart to V0004 and ensures that the symmetry comparator, accumulator, and seal boundary behave deterministically after a recovery scenario.

All waveform captures MUST follow the conventions defined in the *Waveform Annotation Guide*.

---

## **1. Expected Behavior Summary**

| Signal | Expected Value |
|--------|----------------|
| `warp_sum` | `(0, 0, 0)` |
| `error_sum` | `0` |
| `symmetry_ok` | `1` |
| `error_ok` | `1` |
| `true_delivery` | `1` |
| `seal_start` | 1‑cycle pulse at SEAL entry |
| `seal_ready` | asserts when hashing completes |

---

## **2. Expected State Progression**

The True Delivery Loop MUST follow the canonical sequence:

```
IDLE → COLLECT → ACCUMULATE → APPLY → CHECK → SEAL → DONE
```

This sequence MUST be visible in the waveform capture.

---

## **3. Expected Signal Transitions**

### **3.1 Plugin and Accumulator Behavior**

- All plugin `warp_vector` values are `(0,0,0)`  
- All plugin `error` values are `0`  
- `warp_sum` remains `(0,0,0)` throughout  
- `error_sum` remains `0` throughout  

### **3.2 Symmetry Evaluation**

- The lattice is fully symmetric  
- `symmetry_ok` MUST transition **0 → 1** during the `CHECK` cycle  
- No implementation may apply smoothing or tolerance  

### **3.3 Error Evaluation**

- `error_ok` transitions **0 → 1** during `CHECK`  
- This is expected because `error_sum = 0 ≤ epsilon`  

### **3.4 True Delivery Result**

- `true_delivery` MUST transition **0 → 1** during `CHECK`  
- This confirms that both gating conditions are satisfied  

### **3.5 Seal Boundary**

- `seal_start` pulses high for exactly one cycle at SEAL entry  
- `seal_ready` asserts when SHA3‑256 completes  
- The seal encodes a **TRUE** result  

---

## **4. ASCII Waveform Scaffold (Informative)**

```
clk:        ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
           ─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─

state:     IDLE   COLLECT   ACCUM   APPLY   CHECK   SEAL    DONE

warp_sum:  0       0         0       0       0       0       0
error_sum: 0       0         0       0       0       0       0

symmetry_ok:                        0 → 1
error_ok:                           0 → 1
true_delivery:                      0 → 1

seal_start:                                   |──|
seal_ready:                                       |────────|
```

This scaffold defines the **shape** of the waveform; the PNG capture must visually match this behavior.

---

## **5. Conformance Requirements**

An implementation is conformant if:

1. The waveform matches the expected state progression  
2. `symmetry_ok` transitions `0 → 1` during CHECK  
3. `error_ok` transitions `0 → 1` during CHECK  
4. `true_delivery` transitions `0 → 1`  
5. Seal boundary signals (`seal_start`, `seal_ready`) follow canonical timing  
6. The PNG waveform capture matches this template  

---

## **6. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial release of V0005 Golden Waveform template |