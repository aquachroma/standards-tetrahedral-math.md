# **Vendor Waveform Submission Checklist**  
### ISO‑16 True Delivery Loop — Waveform Verification Requirements  
*(Normative)*

This checklist defines the **minimum required artifacts** and **quality criteria** for vendors submitting waveform captures for ISO‑16 conformance review. All submitted materials MUST adhere to the canonical timing, annotation, and formatting rules defined in the ISO‑16 waveform documentation set.

A submission is considered complete only when **all items in this checklist are satisfied**.

---

# **1. Required Artifacts**

Vendors MUST submit the following for **each** conformance vector (V0001–V0005):

### **1.1 Waveform PNG Files**
- `V0001_waveform.png`
- `V0002_waveform.png`
- `V0003_waveform.png`
- `V0004_waveform.png`
- `V0005_waveform.png`

Each PNG MUST:

- be exported directly from the vendor’s HDL simulator  
- follow the canonical annotation rules  
- show all required signals (see Section 2)  
- include cycle markers and state labels  
- be high‑resolution and legible  

### **1.2 Simulation Logs**
- Raw simulator log (`.log`, `.txt`, or `.out`)  
- Any warnings or notes MUST be included  
- The log MUST show the vector ID being executed  

### **1.3 Metadata File**
A single JSON or YAML file containing:

- toolchain name and version  
- simulator name and version  
- commit hash of vendor implementation  
- date/time of waveform generation  
- platform/OS information  

---

# **2. Required Signals in Waveform Captures**

Each waveform PNG MUST include the following signals:

### **2.1 State Machine**
- `state`  
- cycle markers  
- state labels (`IDLE`, `COLLECT`, `ACCUMULATE`, `APPLY`, `CHECK`, `SEAL`, `DONE`)  

### **2.2 Plugin Interface**
- `plugin_valid[id]`  
- `plugin_warp[id][x/y/z]`  
- `plugin_error[id]`  

### **2.3 Accumulators**
- `warp_sum[x/y/z]`  
- `error_sum`  

### **2.4 Check Signals**
- `symmetry_ok`  
- `error_ok`  
- `true_delivery`  

### **2.5 Seal Boundary**
- `seal_start`  
- `seal_ready`  
- `seal[255:0]` (may be shown as grouped bus)  

---

# **3. Formatting Requirements**

### **3.1 Annotation Rules**
All captures MUST follow:

- signal naming conventions  
- color assignments  
- marker shapes  
- vertical alignment rules  
- label placement rules  

as defined in `waveform_annotation_guide.md`.

### **3.2 Resolution & Clarity**
- Minimum width: **1600 px**  
- Minimum height: **900 px**  
- Text MUST be readable without zoom  
- No cropping of state labels or cycle markers  

### **3.3 Consistency**
All five PNGs MUST:

- use the same color palette  
- use the same font and size  
- use the same vertical ordering of signals  
- use the same time scale  

---

# **4. Behavioral Requirements**

Each waveform MUST match the corresponding **Golden Waveform Template**:

- `V0001_waveform_template.md`  
- `V0002_waveform_template.md`  
- `V0003_waveform_template.md`  
- `V0004_waveform_template.md`  
- `V0005_waveform_template.md`  

This includes:

- correct state progression  
- correct accumulator behavior  
- correct symmetry/error outcomes  
- correct seal boundary timing  
- correct final `true_delivery` value  

---

# **5. Submission Packaging**

Vendors MUST submit a single archive:

```
vendor_name_iso16_waveforms_<date>.zip
```

containing:

```
waveforms/
  V0001_waveform.png
  V0002_waveform.png
  V0003_waveform.png
  V0004_waveform.png
  V0005_waveform.png

logs/
  V0001.log
  V0002.log
  V0003.log
  V0004.log
  V0005.log

metadata.json
```

---

# **6. Validation Checklist (Auditor Use)**

Auditors MUST verify:

- [ ] All five PNGs are present  
- [ ] All required signals are visible  
- [ ] Annotation rules are followed  
- [ ] State progression matches templates  
- [ ] Symmetry and error behavior is correct  
- [ ] Seal boundary timing is correct  
- [ ] `true_delivery` matches expected output  
- [ ] Logs show no simulation errors  
- [ ] Metadata is complete and consistent  

---

# **7. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial release of vendor waveform submission checklist |