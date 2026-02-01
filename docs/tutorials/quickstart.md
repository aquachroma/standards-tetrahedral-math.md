# **ISO‑16 Quickstart Guide**
### Run the Reference Implementation, Generate Seals, and View Waveforms  
*(Informative)*

This quickstart walks you through running the ISO‑16 reference implementation, executing the conformance vectors, and inspecting the resulting seals and waveforms. It is intended for engineers, vendors, and auditors who want to see ISO‑16 operating end‑to‑end with minimal setup.

---

## **1. Prerequisites**

You will need:

- A Verilog/SystemVerilog simulator  
  - Icarus Verilog, Verilator, Questa, or Vivado Simulator  
- GNU Make  
- Python 3.10+ (for vector and seal tooling)  
- GTKWave (optional, for waveform viewing)

All commands in this guide assume a Unix‑like shell.

---

## **2. Run the Conformance Suite**

The ISO‑16 repository includes five canonical conformance vectors:

```
V0001 – TRUE  (perfect symmetry, zero error)
V0002 – TRUE  (error == epsilon)
V0003 – FALSE (error > epsilon)
V0004 – FALSE (asymmetry)
V0005 – TRUE  (symmetry restored)
```

To run all vectors:

```
make conformance
```

This produces:

- `conformance/output/V000X_expected.json`  
- `conformance/output/V000X_actual.json`  
- `conformance/output/V000X_seal.txt`  

Each `actual.json` MUST match the corresponding `expected.json`.

---

## **3. Inspect a Seal**

Each vector produces a canonical 256‑bit Tetra‑Seal.

Example:

```
cat conformance/output/V0001_seal.txt
```

You should see a 64‑hex‑character SHA3‑256 digest.

The seal is computed during the **SEAL** state using canonical serialization defined in `seal_timing.md`.

---

## **4. Generate Waveforms**

To produce the Golden Waveform PNGs:

```
make waveforms
```

This runs:

- the reference HDL  
- the canonical testbench  
- the waveform logger  
- the headless GTKWave exporter  

Resulting PNGs appear in:

```
docs/waveforms/V0001_waveform.png
docs/waveforms/V0002_waveform.png
...
docs/waveforms/V0005_waveform.png
```

These MUST match the Golden Waveform Templates in:

```
docs/waveforms/V000X_waveform_template.md
```

---

## **5. View Waveforms Manually (Optional)**

If you want to inspect the raw VCD:

```
gtkwave build/waveforms/V0001.vcd
```

Signals of interest include:

- `state`  
- `warp_sum`  
- `error_sum`  
- `symmetry_ok`  
- `error_ok`  
- `true_delivery`  
- `seal_start`  
- `seal_ready`  
- `seal[255:0]`  

Refer to `waveform_annotation_guide.md` for canonical colors and layout.

---

## **6. Understanding the True Delivery Loop**

The True Delivery Loop executes the following canonical sequence:

```
IDLE
  → COLLECT
  → ACCUMULATE
  → APPLY
  → CHECK (symmetry_ok, error_ok, true_delivery)
  → SEAL (seal_start → seal_ready)
  → DONE
```

This sequence is visible in every waveform and is defined in detail in:

- `true_delivery_loop.md`  
- `plugin_timing.md`  
- `symmetry_check.md`  
- `error_check.md`  
- `seal_timing.md`

---

## **7. Next Steps**

After completing this quickstart, you may want to explore:

- **Implementing your own plugin**  
- **Running ISO‑16 on custom vectors**  
- **Submitting waveforms for audit**  
  (see `vendor_waveform_submission_checklist.md`)  
- **Integrating ISO‑16 into a larger system**  

---

## **8. Revision History**

| Version | Date | Notes |
|---------|-------|--------|
| 0.1 | 2026‑01‑31 | Initial quickstart guide |