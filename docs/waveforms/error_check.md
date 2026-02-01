# **Error Check — Timing & Threshold Evaluation**  
### ISO‑16 Error Accumulation and Epsilon Threshold  
*(Informative)*

This document defines the **expected behavior**, **timing**, and **waveform characteristics** of the ISO‑16 error threshold check. The error check is one of the two gating conditions (along with the symmetry check) that determine whether a vector produces a **TRUE DELIVERY** result.

All waveform captures MUST follow the conventions defined in the *Waveform Annotation Guide*.

---

## **1. Purpose**

The error check ensures that the **total accumulated error** contributed by all plugins remains within the allowable threshold defined by ISO‑16:

```
error_ok = (error_sum ≤ epsilon)
```

This prevents plugin‑level noise, jitter, or instability from producing a valid TRUE DELIVERY result.

The output of this evaluation is the boolean signal:

```
error_ok
```

This signal is consumed during the `CHECK` state of the True Delivery Loop.

---

## **2. Inputs to the Error Check**

The error check operates on:

- the **accumulated error value**  
  ```
  error_sum
  ```
- the **global threshold**  
  ```
  epsilon
  ```

For the ISO‑16 reference suite, epsilon is defined as:

```
epsilon = 1
```

---

## **3. Timing Requirements**

### **3.1 Evaluation Window**

The error check is performed **only** during the `CHECK` state.

### **3.2 Stability Requirements**

During the entire `CHECK` cycle:

- `error_sum` MUST remain stable  
- `epsilon` MUST remain constant  
- `error_ok` MUST be computed combinationally and latched on the rising edge  

### **3.3 Forbidden Behavior**

- Error evaluation MUST NOT occur in `COLLECT`, `ACCUMULATE`, `APPLY`, `SEAL`, or `DONE`  
- `error_ok` MUST NOT change after the `CHECK` cycle  
- Implementations MUST NOT apply smoothing, filtering, or adaptive thresholds  

---

## **4. Error Rule Definition**

The error check is a simple threshold comparison:

```
if error_sum ≤ epsilon:
    error_ok = true
else:
    error_ok = false
```

Where:

- `error_sum` is the sum of all plugin error contributions  
- `epsilon` is a fixed constant defined by the conformance suite  

---

## **5. Expected Waveform Behavior**

### **5.1 Valid Case (error_sum ≤ epsilon)**

- `error_ok` transitions **0 → 1** during the `CHECK` cycle  
- The signal remains `1` through `SEAL` and `DONE`  

### **5.2 Invalid Case (error_sum > epsilon)**

- `error_ok` remains `0`  
- `true_delivery` MUST be `0` regardless of symmetry  

---

## **6. ASCII Timing Diagram (Informative)**

```
clk:        ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
           ─┘ └─┘ └─┘ └─┘ └─┘ └─

state:     ...   APPLY     CHECK     SEAL     DONE
                   |---------|---------|--------|

error_sum:         <====== stable ======>

error_ok:                    0 → 1      (valid case)
true_delivery:               0 → 1
```

---

## **7. Conformance Requirements**

An implementation is conformant if:

1. Error is evaluated **only** during the `CHECK` state  
2. `error_sum` remains stable during evaluation  
3. `error_ok` is computed exactly as defined  
4. No additional tolerances or heuristics are applied  
5. `error_ok` remains stable after the `CHECK` cycle  
6. Waveforms match the Golden Waveforms for V0001–V0005  

---

## **8. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial release |