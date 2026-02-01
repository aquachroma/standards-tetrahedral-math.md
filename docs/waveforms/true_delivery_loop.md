# **True Delivery Loop — Cycle‑Accurate Behavior**
### ISO‑16 State Machine Timing Specification  
*(Informative)*

This document describes the **cycle‑accurate behavior** of the ISO‑16 True Delivery Loop. It defines the canonical state machine, the expected signal transitions, and the timing relationships required for conformance. All waveform captures in this directory follow the conventions defined in the *Waveform Annotation Guide*.

---

# **1. Overview**

The True Delivery Loop is a deterministic, synchronous state machine responsible for:

- collecting plugin warp/error outputs  
- accumulating Q16.16 values  
- applying the warp to the phase lattice  
- performing symmetry and error checks  
- initiating the Tetra‑Seal boundary  

The loop executes once per conformance vector and MUST produce identical results across:

- software implementations  
- FPGA soft cores  
- ASIC fixed‑point units  
- simulation environments  

---

# **2. State Machine Definition**

The True Delivery Loop consists of **seven states**, executed in a fixed order:

| State | Description |
|-------|-------------|
| `IDLE` | Reset/initialization boundary |
| `COLLECT` | Sample plugin outputs |
| `ACCUMULATE` | Update warp/error sums |
| `APPLY` | Apply warp to phase lattice |
| `CHECK` | Perform symmetry + error checks |
| `SEAL` | Begin canonical serialization + SHA3‑256 |
| `DONE` | Final stable state |

The state machine advances **one state per rising clock edge** unless otherwise noted.

---

# **3. Cycle‑Accurate Timing**

The following table defines the required behavior of each state.

### **3.1 State Timing Table**

| State | Cycle Behavior | Required Stable Signals |
|-------|----------------|--------------------------|
| **IDLE** | Reset alignment; accumulators = 0 | `warp_sum`, `error_sum`, `phase_warped` |
| **COLLECT** | Sample plugin outputs | `plugin_valid[id]`, `plugin_warp[id]`, `plugin_error[id]` |
| **ACCUMULATE** | Add plugin warp/error to accumulators | `warp_sum`, `error_sum` |
| **APPLY** | Apply warp_sum to phase_in → phase_warped | `phase_warped[x][y][z]` |
| **CHECK** | Evaluate symmetry + error conditions | `symmetry_ok`, `error_ok`, `true_delivery` |
| **SEAL** | Assert `seal_start`; begin SHA3‑256 | `seal_start`, `seal_ready` |
| **DONE** | Hold final values stable | All outputs stable |

---

# **4. Expected Signal Behavior**

### **4.1 IDLE**
- `warp_sum = 0`  
- `error_sum = 0`  
- `phase_warped = phase_in`  
- `true_delivery = 0`  
- No plugin signals sampled  

### **4.2 COLLECT**
- All plugin outputs MUST be valid on this cycle  
- `plugin_valid[id]` MUST be `1`  
- Warp/error values MUST remain stable for the entire cycle  

### **4.3 ACCUMULATE**
- `warp_sum += plugin_warp[id]`  
- `error_sum += plugin_error[id]`  
- Accumulators update on the rising edge  

### **4.4 APPLY**
- `phase_warped[x][y][z] = phase_in[x][y][z] + warp_sum[x/y/z]`  
- Q16.16 wraparound MUST match the reference implementation  

### **4.5 CHECK**
- `symmetry_ok` computed from `phase_warped`  
- `error_ok` computed from `error_sum`  
- `true_delivery = symmetry_ok & error_ok`  

### **4.6 SEAL**
- `seal_start` asserted for one cycle  
- Canonical serialization begins immediately  
- `seal_ready` asserted when SHA3‑256 completes  

### **4.7 DONE**
- All outputs MUST remain stable  
- No further state transitions occur  

---

# **5. ASCII Timing Diagram (Informative)**

```
clk:        ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
           ─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─

state:     IDLE   COLLECT   ACCUM   APPLY   CHECK   SEAL    DONE
           |-------|---------|--------|--------|-------|--------|

plugin_valid:      1         1        0        0       0        0

warp_sum:   0 → (0x00010000, 0x00020000, 0x00030000)

error_sum:  0 → 1

symmetry_ok:                          1
error_ok:                             1
true_delivery:                        1

seal_start:                                    |────|
seal_ready:                                            |────|
seal[255:0]: d8e4f0c4e9b6a5b8...
```

---

# **6. Conformance Requirements**

An implementation is conformant if:

1. **State transitions occur exactly as defined**  
2. **Plugin outputs are sampled only in COLLECT**  
3. **Accumulators update only in ACCUMULATE**  
4. **Phase warp is applied only in APPLY**  
5. **Checks occur only in CHECK**  
6. **Seal boundary begins in SEAL**  
7. **All outputs remain stable in DONE**  
8. **Waveforms match the Golden Waveforms for V0001–V0003**  

---

# **7. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial release |

