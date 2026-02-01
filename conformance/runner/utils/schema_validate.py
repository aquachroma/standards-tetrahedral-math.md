"""
Schema Validation Helper for ISO‑16 Conformance
----------------------------------------------

Validates JSON documents against the canonical schemas:

- conformance/schema/vector_schema.json
- conformance/schema/expected_schema.json

This module is INFORMATIVE. It exists to keep the reference runner
honest and to give vendors/auditors a clear validation path.
"""

import json
from pathlib import Path

from jsonschema import Draft7Validator


# Simple in‑memory cache so we don’t re‑parse schemas repeatedly
_SCHEMA_CACHE = {}


def _load_schema(schema_path: str) -> dict:
    """
    Load and cache a JSON Schema from disk.
    """
    global _SCHEMA_CACHE

    p = Path(schema_path).resolve()
    if p in _SCHEMA_CACHE:
        return _SCHEMA_CACHE[p]

    with p.open("r") as f:
        schema = json.load(f)

    _SCHEMA_CACHE[p] = schema
    return schema


def validate_json(data: dict, schema_path: str) -> None:
    """
    Validate `data` against the JSON Schema at `schema_path`.

    Raises jsonschema.ValidationError on failure.
    """
    schema = _load_schema(schema_path)
    validator = Draft7Validator(schema)
    errors = sorted(validator.iter_errors(data), key=lambda e: e.path)

    if errors:
        # Build a concise, high‑signal error message
        msgs = []
        for e in errors:
            loc = ".".join(str(x) for x in e.path) or "<root>"
            msgs.append(f"{loc}: {e.message}")
        msg = "Schema validation failed:\n  " + "\n  ".join(msgs)
        raise ValueError(msg)
