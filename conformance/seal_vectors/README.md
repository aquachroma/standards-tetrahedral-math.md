# ISO‑16 Seal Vectors

This directory contains **canonical seal test vectors** used to verify:

- canonical serialization
- plugin ordering
- Q16.16 encoding
- boolean encoding
- domain‑separation prefix
- SHA3‑256 implementation

Files:

- `SEALTEST_input.json` — minimal deterministic input
- `SEALTEST_expected.json` — canonical bytes (hex) + expected SHA3‑256 seal

Any implementation of the ISO‑16 Tetra‑Seal MUST reproduce the expected seal exactly.
