#!/usr/bin/env python3
import json, os, sys

def linf_metric(phase_state):
    # phase_state is array of face scalars or objects {v:...}
    m = 0.0
    for f in phase_state:
        if isinstance(f, dict):
            v = float(f.get("v", 0.0))
        else:
            v = float(f)
        m = max(m, abs(v))
    return m

def load_json(p):
    with open(p, "r", encoding="utf-8") as f:
        return json.load(f)

def require(obj, keys, ctx=""):
    for k in keys:
        if k not in obj:
            raise ValueError(f"Missing key '{k}' in {ctx}")

def main(vectors_dir, expected_dir):
    ok = True
    for name in sorted(os.listdir(vectors_dir)):
        if not name.endswith(".json"):
            continue
        vp = os.path.join(vectors_dir, name)
        vec = load_json(vp)
        require(vec, ["vector_id","spec_version","constants","face_ordering","plugins","initial_phase_state","expected"], name)
        const = vec["constants"]
        require(const, ["epsilon","resolution","faces"], f"{name}.constants")
        eps = float(const["epsilon"])

        # basic plugin status check
        plugins = vec["plugins"]
        for p in plugins:
            require(p, ["plugin_id","status"], f"{name}.plugins")
            if p["status"] != "OK":
                # must be FALSE unless explicitly permitted
                pass

        metric = linf_metric(vec["initial_phase_state"])
        verdict = "TRUE" if metric < eps and all(p["status"]=="OK" for p in plugins) else "FALSE"

        exp = vec["expected"]
        exp_verdict = exp["verdict"]
        if verdict != exp_verdict:
            ok = False
            print(f"[FAIL] {vec['vector_id']}: expected {exp_verdict}, got {verdict} (metric={metric}, eps={eps})")
        else:
            print(f"[PASS] {vec['vector_id']}: {verdict} (metric={metric}, eps={eps})")

        # optional expected metric bound
        if verdict == "TRUE":
            if "error_metric_max" in exp and metric > float(exp["error_metric_max"]):
                ok = False
                print(f"[FAIL] {vec['vector_id']}: metric {metric} exceeds expected max {exp['error_metric_max']}")

    sys.exit(0 if ok else 2)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("usage: run_vectors.py <vectors_dir> <expected_dir>")
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])
