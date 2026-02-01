# **Plugin Timing Specification**
### ISO‑16 Plugin Warp/Error Sampling Rules  
*(Informative)*

This document defines the **cycle‑accurate timing requirements** for plugin interfaces in the ISO‑16 True Delivery Loop. All plugin waveforms MUST follow these rules to ensure deterministic behavior across software, FPGA, and ASIC implementations.

---

# **1. Purpose**

Plugins contribute two values to the True Delivery Loop:

- a **warp vector** (`plugin_warp[id][x/y/z]`)  
- an **error contribution** (`plugin_error[id]`)  

These values must be sampled **exactly once**, at a well‑defined point in the state machine, to guarantee cross‑platform determinism.

---

# **2. Canonical Timing Model**

Plugins are sampled **only** during the `COLLECT` state.

### **2.1 Required Behavior**

| Signal | Requirement |
|--------|-------------|
| `plugin_valid[id]` | MUST be `1` for the entire COLLECT cycle |
| `plugin_warp[id]` | MUST remain stable for the entire COLLECT cycle |
| `plugin_error[id]` | MUST remain stable for the entire COLLECT cycle |
| `plugin_domain[id]` | MUST be constant for the entire vector |

### **2.2 Forbidden Behavior**

- Plugins MUST NOT change outputs during `ACCUMULATE`, `APPLY`, `CHECK`, `SEAL`, or `DONE`.  
- Plugins MUST NOT depend on internal state from previous vectors.  
- Plugins MUST NOT assert `plugin_valid[id]` outside COLLECT.

---

# **3. Cycle‑Accurate Timing**

### **3.1 COLLECT (Sampling Window)**

On the rising edge entering `COLLECT`:

- The True Delivery Loop **latches** all plugin outputs.  
- All plugin signals MUST be valid and stable.  
- No further changes to plugin outputs are permitted.

### **3.2 ACCUMULATE (Use Window)**

On the rising edge entering `ACCUMULATE`:

- `warp_sum += plugin_warp[id]`  
- `error_sum += plugin_error[id]`  

Plugins are **not** sampled here; their values are already latched.

### **3.3 APPLY, CHECK, SEAL, DONE**

Plugins are ignored in these states.

---

# **4. Multi‑Plugin Timing**

If multiple plugins are present:

- All plugins are sampled **in the same COLLECT cycle**  
- Accumulation order is **lexicographically sorted by plugin ID**  
- This ordering MUST match the canonical serialization order  

This ensures deterministic behavior across:

- Python reference runner  
- C++ reference runner  
- FPGA/ASIC implementations  

---

# **5. ASCII Timing Diagram (Informative)**

```
clk:        ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
           ─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─

state:     IDLE   COLLECT   ACCUM   APPLY   CHECK   SEAL   DONE
           |-------|---------|--------|--------|-------|-------|

plugin_valid[id]:
           0       1         0        0        0       0       0

plugin_warp[id]:   <==== stable ====>|
plugin_error[id]:  <==== stable ====>|

warp_sum:          (sampled)   updated →
error_sum:         (sampled)   updated →
```

---

# **6. Conformance Requirements**

An implementation is conformant if:

1. Plugin outputs are **stable** for the entire COLLECT cycle  
2. Plugin outputs are **sampled exactly once**  
3. Accumulators update **only** in ACCUMULATE  
4. Plugins do not influence behavior outside COLLECT  
5. Multi‑plugin accumulation follows **canonical ordering**  

---

# **7. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial release |