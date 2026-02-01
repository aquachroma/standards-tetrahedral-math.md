"""
Q16.16 Deterministic Fixed‑Point Arithmetic Utilities
-----------------------------------------------------

All ISO‑16 normative arithmetic uses signed 32‑bit Q16.16 integers:

    - upper 16 bits: integer part
    - lower 16 bits: fractional part

This module provides minimal, deterministic helpers for:
    - addition
    - subtraction
    - absolute value
    - comparison (<=)
    - conversion helpers (optional, informative)

No floating‑point operations appear anywhere in this file.
"""

# Mask for 32‑bit signed integer wraparound
INT32_MASK = 0xFFFFFFFF
INT32_MAX  = 0x7FFFFFFF
INT32_MIN  = -0x80000000


def _to_int32(value: int) -> int:
    """
    Force wraparound to signed 32‑bit integer.
    """
    value &= INT32_MASK
    if value & 0x80000000:
        return value - 0x100000000
    return value


# ------------------------------------------------------------
# Core Q16.16 operations
# ------------------------------------------------------------

def q16_add(a: int, b: int) -> int:
    """
    Deterministic 32‑bit signed addition.
    """
    return _to_int32(a + b)


def q16_sub(a: int, b: int) -> int:
    """
    Deterministic 32‑bit signed subtraction.
    """
    return _to_int32(a - b)


def q16_abs(a: int) -> int:
    """
    Deterministic absolute value in Q16.16.
    """
    if a == INT32_MIN:
        # Absolute value of INT32_MIN cannot be represented in int32.
        # ISO‑16 forbids values anywhere near this range, but we clamp.
        return INT32_MAX
    return -a if a < 0 else a


def q16_leq(a: int, b: int) -> bool:
    """
    Deterministic <= comparison.
    """
    return a <= b


# ------------------------------------------------------------
# Optional helpers (informative)
# ------------------------------------------------------------

def from_float(f: float) -> int:
    """
    Convert Python float to Q16.16.
    Informative only — not used in normative paths.
    """
    raw = int(round(f * (1 << 16)))
    return _to_int32(raw)


def to_float(q: int) -> float:
    """
    Convert Q16.16 to Python float.
    Informative only — not used in normative paths.
    """
    return float(q) / float(1 << 16)
