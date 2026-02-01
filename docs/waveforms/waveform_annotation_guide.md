# **Waveform Annotation Guide**
### ISO‑16 True Delivery Loop — Signal Interpretation & Annotation Conventions  
*(Informative)*

This guide defines the **visual conventions**, **signal naming**, and **annotation rules** used throughout the ISO‑16 waveform documentation.  
Its purpose is to make every timing diagram in the standard **legible, consistent, and auditor‑verifiable**, regardless of the simulation tool or hardware platform.

---

# **1. Purpose of This Guide**

ISO‑16 relies on cycle‑accurate behavior for:

- the **True Delivery Loop**  
- **plugin warp/error accumulation**  
- **symmetry and error checks**  
- **final seal generation**

Waveforms are the clearest way to demonstrate that an implementation matches the normative state machine.  
This guide ensures that all waveform captures:

- use consistent signal names  
- use consistent colors  
- use consistent annotation styles  
- highlight the same key transitions  
- are readable by auditors and hardware teams  

---

# **2. Signal Naming Conventions**

All waveforms in ISO‑16 use the following canonical signal names.  
Implementations may use internal names, but waveform exports MUST relabel to these names for documentation and audit purposes.

### **2.1 Core State Machine Signals**

| Signal | Type | Meaning |
|--------|------|---------|
| `clk` | Clock | Rising‑edge active system clock |
| `rst_n` | Boolean | Active‑low synchronous reset |
| `state` | Enum | Current True Delivery Loop state |
| `cycle` | Integer | Optional cycle counter for clarity |

### **2.2 Phase State Signals**

| Signal | Type | Meaning |
|--------|------|---------|
| `phase_in[x][y][z]` | Q16.16 | Initial phase state (16×3) |
| `phase_warped[x][y][z]` | Q16.16 | Final warped phase state |

### **2.3 Plugin Interface Signals**

| Signal | Type | Meaning |
|--------|------|---------|
| `plugin_valid[id]` | Boolean | Plugin output is valid this cycle |
| `plugin_warp[id][x/y/z]` | Q16.16 | Plugin warp vector |
| `plugin_error[id]` | Q16.16 | Plugin error contribution |
| `plugin_domain[id]` | Enum | Domain code (Refraction, FrameDrag, Jitter, Custom) |

### **2.4 Accumulation Signals**

| Signal | Type | Meaning |
|--------|------|---------|
| `warp_sum[x/y/z]` | Q16.16 | Running sum of plugin warp vectors |
| `error_sum` | Q16.16 | Running sum of plugin error values |

### **2.5 Check Signals**

| Signal | Type | Meaning |
|--------|------|---------|
| `symmetry_ok` | Boolean | Symmetry check result |
| `error_ok` | Boolean | Error threshold check result |
| `true_delivery` | Boolean | Final True Delivery result |

### **2.6 Seal Signals**

| Signal | Type | Meaning |
|--------|------|---------|
| `seal_start` | Boolean | Canonical serialization begins |
| `seal_ready` | Boolean | SHA3‑256 digest valid |
| `seal[255:0]` | Bits | Final Tetra‑Seal |

---

# **3. Color Conventions**

To ensure consistency across tools, ISO‑16 adopts the following color palette:

| Category | Color | Meaning |
|----------|--------|---------|
| Clock / Reset | Gray | Infrastructure signals |
| State Machine | Blue | State transitions |
| Plugin Inputs | Green | Warp/error inputs |
| Accumulators | Orange | Running sums |
| Checks | Purple | Symmetry/error booleans |
| Seal Signals | Red | Cryptographic boundary |

These colors are used in all reference waveform captures.

---

# **4. Annotation Conventions**

### **4.1 Rising‑Edge Markers**
Use vertical dashed lines to indicate rising edges where state transitions occur.

```
|‾‾‾‾‾‾‾‾‾
|
|‾‾‾‾‾‾‾‾‾
```

### **4.2 State Labels**
Place state names above the `state` signal:

```
state:  IDLE | COLLECT | ACCUMULATE | APPLY | CHECK | SEAL | DONE
```

### **4.3 Warp Vector Annotation**
Warp vectors are annotated as:

```
warp_sum = (0x00010000, 0x00020000, 0x00030000)
```

Always show Q16.16 values in **hex**, not float.

### **4.4 Error Annotation**
Errors are annotated as:

```
error_sum = 0x00000001
```

### **4.5 Boolean Transitions**
Boolean transitions use arrows:

```
symmetry_ok: 0 → 1
error_ok:    1 → 1
```

### **4.6 Seal Boundary**
The moment canonical serialization begins is marked with a red vertical bar:

```
| SEAL START |
```

The moment the SHA3‑256 digest is valid is marked with:

```
| SEAL READY |
```

---

# **5. Example Annotated Waveform (ASCII)**  
*(Informative — real PNGs appear in V0001–V0003 docs)*

```
clk:        ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
           ─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─

state:     IDLE     COLLECT     ACCUMULATE     APPLY     CHECK     SEAL     DONE
           |---------|-----------|--------------|---------|---------|--------|

plugin_valid[alpha]:
           0         1           1              0         0         0        0

plugin_warp[alpha]: (0x00010000, 0x00020000, 0x00030000)

warp_sum:  (0,0,0) → (0x00010000, 0x00020000, 0x00030000)

error_sum: 0 → 1

symmetry_ok: 1
error_ok:    1
true_delivery: 1

seal_start:                     |───────────────|
seal_ready:                                   |────|
seal[255:0]: d8e4f0c4e9b6a5b8...
```

This ASCII diagram is intentionally simple — the PNG versions in the vector‑specific docs will be more detailed.

---

# **6. How to Capture Waveforms for ISO‑16 Documentation**

### **6.1 Required Export Settings**
- Timebase: **rising‑edge aligned**
- Value format: **hex for Q16.16**, **binary for seal**
- Font: monospace
- Background: white (for printability)
- Minimum resolution: 1600×900

### **6.2 Required Signals**
Every waveform capture MUST include:

- `clk`, `rst_n`, `state`
- all plugin signals
- warp/error accumulators
- symmetry/error booleans
- seal_start, seal_ready, seal

### **6.3 Optional Signals**
- cycle counter  
- internal FSM debug signals  

---

# **7. How Waveforms Map to Conformance Vectors**

Each conformance vector (V0001–V0003) includes:

- a reference waveform PNG  
- annotations following this guide  
- expected transitions  
- expected accumulator values  
- expected seal boundary timing  

This ensures that:

- software implementations  
- hardware implementations  
- FPGA/ASIC designs  
- vendor SDKs  

…all match the same observable behavior.

---

# **8. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial release of waveform annotation guide |