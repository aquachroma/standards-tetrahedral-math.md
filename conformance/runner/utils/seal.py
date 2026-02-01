"""
ISO‑16 Canonical Serialization + Tetra‑Seal Generator
-----------------------------------------------------

Implements the canonical serialization rules defined in:

    spec/iso16_seal.md

This module is INFORMATIVE but matches the normative behavior exactly.

Responsibilities:
- Serialize PhaseState (16×3 Q16.16) in big‑endian
- Serialize plugin outputs in lexicographic order of plugin id
- Serialize warp_total, error_total, booleans
- Serialize implementation_id, timestamp, nonce
- Apply domain‑separation prefix
- Compute SHA3‑256 hash
"""

import hashlib
import struct
import json


# ------------------------------------------------------------
# Helpers
# ------------------------------------------------------------

def _q16_to_be_bytes(value: int) -> bytes:
    """
    Encode a signed Q16.16 integer as big‑endian 32‑bit.
    """
    return struct.pack(">i", value)


def _bool_to_byte(flag: bool) -> bytes:
    """
    Encode boolean as 0x00 or 0x01.
    """
    return b"\x01" if flag else b"\x00"


def _encode_string(s: str) -> bytes:
    """
    UTF‑8 encode a string with no terminator.
    """
    return s.encode("utf-8")


def _encode_length_prefixed_string(s: str) -> bytes:
    """
    Encode string as:
        [1‑byte length][UTF‑8 bytes]
    """
    b = s.encode("utf-8")
    if len(b) > 255:
        raise ValueError("String too long for canonical encoding")
    return bytes([len(b)]) + b


# ------------------------------------------------------------
# Canonical Serialization
# ------------------------------------------------------------

def canonical_serialize(vector: dict, actual: dict) -> bytes:
    """
    Produce the canonical byte sequence for seal hashing.
    Matches the field order in iso16_seal.md §3.
    """

    out = bytearray()

    # --------------------------------------------------------
    # 1. phase_state_initial (16×3 Q16.16)
    # --------------------------------------------------------
    for (x, y, z) in vector["initial_phase_state"]:
        out += _q16_to_be_bytes(x)
        out += _q16_to_be_bytes(y)
        out += _q16_to_be_bytes(z)

    # --------------------------------------------------------
    # 2. plugin_outputs (lexicographic by plugin id)
    # --------------------------------------------------------
    plugins = vector["plugins"]
    for pid in sorted(plugins.keys()):
        p = plugins[pid]

        # id_length + id_bytes
        out += _encode_length_prefixed_string(p["id"])

        # domain_code
        domain = p["domain"]
        if domain == "Refraction":
            out += b"\x01"
        elif domain == "FrameDrag":
            out += b"\x02"
        elif domain == "Jitter":
            out += b"\x03"
        else:
            out += b"\xFF"  # Custom

        # warp_vector (3×Q16.16)
        for w in p["warp_vector"]:
            out += _q16_to_be_bytes(w)

        # error (Q16.16)
        out += _q16_to_be_bytes(p["error"])

        # version_length + version_bytes
        out += _encode_length_prefixed_string(p["version"])

    # --------------------------------------------------------
    # 3. warp_total (3×Q16.16)
    # --------------------------------------------------------
    for w in actual["warp_total"]:
        out += _q16_to_be_bytes(w)

    # --------------------------------------------------------
    # 4. error_total (Q16.16)
    # --------------------------------------------------------
    out += _q16_to_be_bytes(actual["error_total"])

    # --------------------------------------------------------
    # 5. phase_state_warped (16×3 Q16.16)
    # --------------------------------------------------------
    for (x, y, z) in actual["phase_state_warped"]:
        out += _q16_to_be_bytes(x)
        out += _q16_to_be_bytes(y)
        out += _q16_to_be_bytes(z)

    # --------------------------------------------------------
    # 6. symmetry_ok (boolean)
    # --------------------------------------------------------
    out += _bool_to_byte(actual["symmetry_ok"])

    # --------------------------------------------------------
    # 7. error_ok (boolean)
    # --------------------------------------------------------
    out += _bool_to_byte(actual["error_ok"])

    # --------------------------------------------------------
    # 8. true_delivery (boolean)
    # --------------------------------------------------------
    out += _bool_to_byte(actual["true_delivery"])

    # --------------------------------------------------------
    # 9. implementation_id (string)
    # --------------------------------------------------------
    impl_id = vector.get("implementation_id", "iso16-ref")
    out += _encode_string(impl_id)

    # --------------------------------------------------------
    # 10. timestamp (uint64, microseconds since epoch)
    # --------------------------------------------------------
    timestamp = vector.get("timestamp", 0)
    out += struct.pack(">Q", timestamp)

    # --------------------------------------------------------
    # 11. nonce (128‑bit random)
    # --------------------------------------------------------
    nonce = vector.get("nonce", bytes(16))
    if isinstance(nonce, str):
        nonce = bytes.fromhex(nonce)
    if len(nonce) != 16:
        raise ValueError("Nonce must be 16 bytes")
    out += nonce

    return bytes(out)


# ------------------------------------------------------------
# Seal Hashing
# ------------------------------------------------------------

def canonical_serialize_and_hash(vector: dict, actual: dict) -> str:
    """
    Serialize fields, prepend domain‑separation prefix,
    compute SHA3‑256, return lowercase hex string.
    """

    prefix = b"ISO16-SEAL-V1:"
    body = canonical_serialize(vector, actual)

    h = hashlib.sha3_256()
    h.update(prefix)
    h.update(body)

    return h.hexdigest()
